# Физические устройства в Vulkan

Физические устройства (GPU) представляются в Vulkan через `VkPhysicalDevice` — непрозрачный указатель, который позволяет получать информацию о возможностях и характеристиках оборудования.

---

## Получение списка устройств

```cpp
uint32_t deviceCount = 0;
vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

std::vector<VkPhysicalDevice> devices(deviceCount);
vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
```

**Важно:** Сначала вызывается функция для получения количества устройств, затем — для получения самих дескрипторов.

---

## Свойства устройства

```cpp
VkPhysicalDeviceProperties properties;
vkGetPhysicalDeviceProperties(physicalDevice, &properties);

// Основные поля:
properties.deviceName;      // Название GPU (например, "NVIDIA GeForce RTX 3090")
properties.deviceType;      // Тип устройства (дискретная/встроенная видеокарта и др.)
properties.apiVersion;      // Поддерживаемая версия Vulkan
```

**Типы устройств:**
```cpp
VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU   // Дискретная видеокарта (наилучший выбор)
VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU // Встроенная графика
VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU    // Виртуальная видеокарта
VK_PHYSICAL_DEVICE_TYPE_CPU           // Эмуляция на CPU
```

---

## Функциональные возможности

```cpp
VkPhysicalDeviceFeatures features;
vkGetPhysicalDeviceFeatures(physicalDevice, &features);

// Проверка поддержки возможностей:
if (features.geometryShader) {
    // Геометрические шейдеры поддерживаются
}
```

**Ключевые возможности:**
- `geometryShader` — поддержка геометрических шейдеров
- `tessellationShader` — поддержка тесселяционных шейдеров
- `samplerAnisotropy` — анизотропная фильтрация
- `wideLines` — широкие линии

---

## Выбор устройства (пример)

```cpp
VkPhysicalDevice SelectSuitableDevice(VkInstance instance) {
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        
        // Выбираем дискретную видеокарту
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return device;
        }
    }
    
    // Если дискретной нет, возвращаем первую доступную
    return devices.empty() ? VK_NULL_HANDLE : devices[0];
}
```

---

## Критические моменты

1. **Проверяйте наличие устройств** перед их использованием:
   ```cpp
   if (deviceCount == 0) {
       // Обработка отсутствия Vulkan-совместимых устройств
   }
   ```

2. **Приоритет выбора устройств:**
   - Дискретная видеокарта (`VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU`)
   - Встроенная графика (`VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU`)
   - Последнее средство — эмуляция на CPU

3. **Не забывайте проверять функциональные возможности** перед использованием специфических функций Vulkan

4. Физические устройства **не требуют явного удаления** — они уничтожаются автоматически при уничтожении `VkInstance`