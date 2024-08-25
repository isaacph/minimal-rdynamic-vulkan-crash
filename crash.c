#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#define VK_NO_PROTOTYPES
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

// this symbol overrides vkGetInstanceProcAddr's lookup. notice how if you comment out this line then the program doesn't crash
PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion = NULL;

int main() {
    void* vulkan_handle = dlopen("libvulkan.so", RTLD_LAZY);
    assert(vulkan_handle != NULL);
    PFN_vkGetInstanceProcAddr fp_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr) dlsym(vulkan_handle, "vkGetInstanceProcAddr");
    assert(fp_vkGetInstanceProcAddr != NULL);
    PFN_vkEnumerateInstanceVersion fp_vkEnumerateInstanceVersion =
        (PFN_vkEnumerateInstanceVersion) fp_vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
    assert(fp_vkEnumerateInstanceVersion != NULL);
    uint32_t version;

    // this line causes the seg fault because vkEnumerateInstanceVersion ends up being a pointer to the global variable above
    VkResult res = fp_vkEnumerateInstanceVersion(&version);

    assert(res == VK_SUCCESS);
    printf("Vulkan instance version %d\n", version);
}
