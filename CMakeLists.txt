cmake_minimum_required(VERSION 3.22)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(esp32_lego_hub)

include_directories(src
                    src/lwp_hub
                    src/lwp_hub/devices)

FILE(GLOB app_sources src/*.c)
FILE(GLOB lwp_hub_sources src/lwp_hub/*c)
target_sources(app PRIVATE
  ${app_sources}
  ${lwp_hub_sources}
  )