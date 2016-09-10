cmake_minimum_required(VERSION 2.6)

include_directories(
    "shared/ConfigFrames"
    "shared/GatewayFrames"
)

add_subdirectory(satellite-gateway)
add_subdirectory(tests)
