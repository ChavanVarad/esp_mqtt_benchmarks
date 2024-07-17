# The following four lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Define variables
set(chip "esp32") #Set to "esp32" or "esp8266"
set(clientid "1") #Set to "1", "2", or "3".

# Include the IDF project CMake file
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp_mtls_mqtt)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/${chip}-certs/${chip}-${clientid}.key" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/${chip}-certs/${chip}-${clientid}.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/ca.crt" TEXT)