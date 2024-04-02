#include "AppGLFW.h"
#include "ClipboardGLFW.h"
#include "FileLogger.h"
#include "WindowGLFW.h"
#include "gl/GPUContextGL.h"
#include "gl/GPUDriverGL.h"
#include <AppCore/Platform.h>
#include <GLFW/glfw3.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/private/PlatformFileSystem.h>
#include <Ultralight/private/util/Debug.h>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <libgen.h>
#include <pwd.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>

extern "C" {

static void GLFW_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

std::filesystem::path GetCacheDirectory()
{
    const char* xdgCacheHome = std::getenv("XDG_CACHE_HOME");
    if (xdgCacheHome) {
        return std::filesystem::path(xdgCacheHome);
    } else {
        const char* home = std::getenv("HOME");
        if (home) {
            return std::filesystem::path(home) / ".cache";
        }
    }
    // Handle error or use a default path
    return std::filesystem::path("/tmp");
}

static std::filesystem::path GetExecutableDirectory()
{
    static char exe_path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", exe_path, PATH_MAX);
    const char* result = "";
    if (count != -1)
        result = dirname(exe_path);
    return std::filesystem::path(result);
}

} // extern "C"

namespace ultralight {

AppGLFW::AppGLFW(Settings settings, Config config)
    : settings_(settings)
{
    // Generate cache path
    std::filesystem::path cache_path = GetCacheDirectory();
    String cache_dirname = "com." + settings_.developer_name + "." + settings_.app_name;
    cache_path /= std::string(cache_dirname.utf8().data());
    if (!std::filesystem::exists(cache_path)) {
        if (!std::filesystem::create_directories(cache_path)) {
            std::cout << "Failed to create cache path: " << cache_path.string() << std::endl;
        }
    }

    if (!Platform::instance().logger()) {
        std::filesystem::path log_path = cache_path / std::filesystem::path("ultralight.log");
        logger_.reset(new FileLogger(log_path.string().c_str()));
        Platform::instance().set_logger(logger_.get());
    }

    // Determine resources path
    std::filesystem::path executable_path = GetExecutableDirectory();

    config.cache_path = cache_path.string().c_str();

    glfwSetErrorCallback(GLFW_error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    main_monitor_.reset(new MonitorGLFW());

    config.face_winding = FaceWinding::Clockwise;
    Platform::instance().set_config(config);

    if (!Platform::instance().file_system()) {
        // Determine file system path
        std::string fs_str = settings.file_system_path.utf8().data();
        std::filesystem::path file_system_path = executable_path / std::filesystem::path(fs_str);

        Platform::instance().set_file_system(GetPlatformFileSystem(file_system_path.string().c_str()));
    }

    if (!Platform::instance().font_loader()) {
        Platform::instance().set_font_loader(GetPlatformFontLoader());
    }

    clipboard_.reset(new ClipboardGLFW());
    Platform::instance().set_clipboard(clipboard_.get());

    gpu_context_.reset(new GPUContextGL(true, true));
    Platform::instance().set_gpu_driver(gpu_context_->driver());

    // We use the GPUContext's global offscreen window to maintain
    // clipboard state for the duration of the app lifetime.
    clipboard_->set_window(gpu_context_->window());

    surface_factory_.reset(new ULTextureSurfaceFactory());
    Platform::instance().set_surface_factory(surface_factory_.get());

    renderer_ = Renderer::Create();
}

AppGLFW::~AppGLFW()
{
    windows_.clear();
    gpu_context_.reset();
    Platform::instance().set_gpu_driver(nullptr);
    glfwTerminate();
}

Monitor* AppGLFW::main_monitor()
{
    return main_monitor_.get();
}

RefPtr<Renderer> AppGLFW::renderer()
{
    return renderer_;
}

void AppGLFW::Run()
{
    if (is_running_)
        return;

    is_running_ = true;

    const double update_interval_ms = 2.0; // 1000ms / 500Hz
    const double repaint_interval_ms = 8.0; // 1000ms / 125Hz

    // Create timer and signal file descriptors
    int update_timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    int repaint_timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);

    // Set up timer intervals
    struct itimerspec update_timer_spec = {};
    update_timer_spec.it_interval.tv_sec = 0;
    update_timer_spec.it_interval.tv_nsec = static_cast<long>(update_interval_ms * 1000000);
    update_timer_spec.it_value = update_timer_spec.it_interval;

    struct itimerspec repaint_timer_spec = {};
    repaint_timer_spec.it_interval.tv_sec = 0;
    repaint_timer_spec.it_interval.tv_nsec = static_cast<long>(repaint_interval_ms * 1000000);
    repaint_timer_spec.it_value = repaint_timer_spec.it_interval;

    // Arm the timers
    timerfd_settime(update_timer_fd, 0, &update_timer_spec, nullptr);
    timerfd_settime(repaint_timer_fd, 0, &repaint_timer_spec, nullptr);

    while (is_running_) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(update_timer_fd, &rfds);
        FD_SET(repaint_timer_fd, &rfds);

        // Wait for timer events or input events
        int max_fd = std::max(update_timer_fd, repaint_timer_fd) + 1;
        int result = select(max_fd, &rfds, nullptr, nullptr, nullptr);

        if (result == -1) {
            // Error occurred
            break;
        }

        if (FD_ISSET(update_timer_fd, &rfds)) {
            // Update timer event
            uint64_t expirations;
            read(update_timer_fd, &expirations, sizeof(expirations));
            Update();
        }

        if (FD_ISSET(repaint_timer_fd, &rfds)) {
            // Repaint timer event
            uint64_t expirations;
            read(repaint_timer_fd, &expirations, sizeof(expirations));
            Repaint();
        }

        glfwPollEvents();
    }

    // Clean up timer file descriptors
    close(update_timer_fd);
    close(repaint_timer_fd);
}

void AppGLFW::Quit()
{
    is_running_ = false;
}

void AppGLFW::Update()
{
    if (listener_)
        listener_->OnUpdate();

    renderer()->Update();
}

void AppGLFW::Repaint()
{
    App::instance()->renderer()->RefreshDisplay(0);

    for (auto window : windows_) {
        if (window->NeedsRepaint())
            window->Repaint();
    }
}

static App* g_app_instance = nullptr;

RefPtr<App> App::Create(Settings settings, Config config)
{
    g_app_instance = (App*)new AppGLFW(settings, config);
    return AdoptRef(*g_app_instance);
}

App::~App()
{
    g_app_instance = nullptr;
}

App* App::instance()
{
    return g_app_instance;
}

} // namespace ultralight
