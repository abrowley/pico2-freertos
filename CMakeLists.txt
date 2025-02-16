cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)
set(project_name pico2_freertos)
include(lib/pico-sdk/pico_sdk_init.cmake)

project(${project_name} C CXX ASM)
pico_sdk_init()
include(lib/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)

add_executable(${project_name} src/main.cpp)

target_link_libraries(${project_name}
        pico_stdlib
        pico_lwip_mqtt
        hardware_i2c
        hardware_adc
        hardware_pwm
        FreeRTOS-Kernel-Heap4
        pico_cyw43_arch_lwip_sys_freertos
)


target_include_directories(${project_name} PRIVATE port/FreeRTOS-Kernel)
target_include_directories(${project_name} PRIVATE port/lwip)

if((DEFINED WIFI_SSID) AND (DEFINED WIFI_PASSWORD))
    target_compile_definitions(${project_name} PRIVATE
            WIFI_SSID=\"${WIFI_SSID}\"
            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
    )
else()
    message(FATAL_ERROR
            "WIFI Credentials not defined. Add -DWIFI_SSID=<YOUR_SSID> and -DWIFI_PASSWORD=<YOUR_PASSWORD> to CMAKE Args")
endif ()

pico_enable_stdio_uart(${project_name} 1)
pico_add_extra_outputs(${project_name})