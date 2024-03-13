// DisplayLinkWin.h
#pragma once
#include <Windows.h>
#include <atomic>
#include <d3d11.h>
#include <dxgi.h>
#include <functional>

/*
    The DisplayLinkWin class is designed to approximate the functionality of macOS's CVDisplayLink on
    Windows platforms.

    This class invokes a callback from a child thread whenever the target HMONITOR is refreshed.

    Direct3D is used to detect hardware display refresh if it is available, otherwise a high-precision
    timer is used to approximate the refresh rate.
*/
class DisplayLinkWin {
public:
    DisplayLinkWin(HMONITOR hMonitor, std::function<void()> callback);
    ~DisplayLinkWin();

private:
    HMONITOR m_hMonitor;
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    int m_refreshRate;
    std::function<void()> m_callback;
    std::atomic<bool> m_running;
    bool m_useD3D;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    IDXGISwapChain* m_swapChain;
    HANDLE m_threadHandle;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool createHiddenWindowOnMonitor();
    bool checkD3DAvailability();
    bool setupD3DForVSync();
    void d3dRenderLoop();
    void timerLoop();
    int queryRefreshRate();
    void startD3DRenderLoop();
    void startTimerLoop();
};
