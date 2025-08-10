#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

// функция выбора устройства
VkPhysicalDevice selectDevice(VkInstance* instance) {
    setlocale(LC_ALL, "RU");

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);
    // Если устройств с поддержкой Vulkan 0, то нет смысла что-то еще делать
    if (!deviceCount) {
        std::cout << "No available devices" << std::endl;
        return VK_NULL_HANDLE;
    }
    // определяем вектор для хранения всех дескрипторов VkPhysicalDevice.
    std::vector<VkPhysicalDevice> devices(deviceCount);
    // получаем устройства
    vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

    VkPhysicalDeviceProperties deviceProperties;
    // перебираем полученные устройства
    for (const auto& device : devices) {
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        // определяем дискретную видеокарту
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            return device;
    }
    return VK_NULL_HANDLE;
}

int main() {

    VkInstance instance;
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cout << "Unable to create VKInstance" << std::endl;
        return 1;
    }

    // получаем устройство
    VkPhysicalDevice physicalDevice = selectDevice(&instance);

    // проверяем, что устройство успешно получено
    if (physicalDevice == VK_NULL_HANDLE) {
        std::cout << "Failed to find a suitable GPU" << std::endl;
    }
    else {
        // если устройство найдено, выводим его название
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        std::cout << "Selected device: " << deviceProperties.deviceName << std::endl;
        std::cout << "Vendor ID: " << deviceProperties.vendorID << std::endl;
        std::cout << "API version: " << deviceProperties.apiVersion << std::endl;
    }

    vkDestroyInstance(instance, nullptr);
    return 0;
}