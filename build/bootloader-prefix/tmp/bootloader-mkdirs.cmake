# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Projects/expressif/v5.3/esp-idf/components/bootloader/subproject"
  "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader"
  "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader-prefix"
  "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader-prefix/tmp"
  "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader-prefix/src"
  "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Projects/expressif/projects/espidf-wireguard_benchmark/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
