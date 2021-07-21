#include "GPUContextVK.h"
#include "GPUDriverVK.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace ultralight {

GPUContextVK::GPUContextVK(bool enable_vsync, bool enable_msaa) : msaa_enabled_(enable_msaa) {

  std::vector<const char*> instance_layers;
  std::vector<const char*> instance_extensions;
  std::vector<const char*> device_extensions;

  // SUPINE for debug
  instance_layers.push_back("VK_LAYER_LUNARG_standard_validation");
  instance_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

  device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME); // make sure we can display anything

  uint32_t instance_extension_count = 0;
  const char** instance_extensions_buffer = glfwGetRequiredInstanceExtensions(&instance_extension_count);
  for (uint32_t i = 0; i < instance_extension_count; ++i) {
    instance_extensions.push_back(instance_extensions_buffer[i]);
  }

  VkApplicationInfo application_info {};
  application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  application_info.apiVersion = VK_MAKE_VERSION(1, 0, 2);
  application_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
  application_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  application_info.pApplicationName = "GLFW with Vulkan";
  application_info.pEngineName = "GLFW with Vulkan";

  VkInstanceCreateInfo instance_create_info {};
  instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_create_info.pApplicationInfo = &application_info;
  instance_create_info.enabledLayerCount = instance_layers.size();
  instance_create_info.ppEnabledLayerNames = instance_layers.data();
  instance_create_info.enabledExtensionCount = instance_extensions.size();
  instance_create_info.ppEnabledExtensionNames = instance_extensions.data();

  VkInstance instance = VK_NULL_HANDLE;
  vkCreateInstance(&instance_create_info, nullptr, &instance);
  this->instance = instance;

  // Get GPUs
  uint32_t GPU_count;
  vkEnumeratePhysicalDevices(instance, &GPU_count, nullptr);
  std::vector<VkPhysicalDevice> GPUs(GPU_count);
  vkEnumeratePhysicalDevices(instance, &GPU_count, GPUs.data());

  uint32_t queue_family_count;
  vkGetPhysicalDeviceQueueFamilyProperties(GPUs[0], &queue_family_count, nullptr);
  std::vector<VkQueueFamilyProperties> family_properties(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(GPUs[0], &queue_family_count, family_properties.data());

  uint32_t graphics_queue_family = UINT32_MAX;
  for (uint32_t i = 0; i < queue_family_count; ++i) {
    if (family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      graphics_queue_family = i;
    }
  }
  if (graphics_queue_family == UINT32_MAX) {
    // queue family not found
    glfwTerminate();
  }
  // graphics_queue_family now contains queue family ID which supports graphics

  // Create Vulkan device
  const float priorities[] { 1.0f };
  VkDeviceQueueCreateInfo queue_create_info {};
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueCount = 1;
  queue_create_info.queueFamilyIndex = graphics_queue_family;
  queue_create_info.pQueuePriorities = priorities;

  VkDeviceCreateInfo device_create_info {};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.queueCreateInfoCount = 1;
  device_create_info.pQueueCreateInfos = &queue_create_info;
  device_create_info.enabledLayerCount = device_extensions.size();
  device_create_info.ppEnabledExtensionNames = device_extensions.data();

  VkDevice device = VK_NULL_HANDLE;
  vkCreateDevice(GPUs[0], &device_create_info, nullptr, &device);

  this->device = device;
}

GPUContextVK::~GPUContextVK(){
  vkDestroyDevice( device, nullptr );
	vkDestroyInstance( instance, nullptr );
}

} // namespace ultralight