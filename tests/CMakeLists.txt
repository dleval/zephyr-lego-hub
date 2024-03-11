cmake_minimum_required(VERSION 3.22)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(esp32_lego_hub)

include_directories(src
                    ../src/lwp_hub
                    test_doubles)

FILE(GLOB test_sources src/*.c)
FILE(GLOB doubles_sources test_doubles/*.c)
FILE(GLOB lwp_hub_sources ../src/lwp_hub/*.c)
target_sources(app PRIVATE
  ${test_sources}
  ${doubles_sources}
  ${lwp_hub_sources}
  )