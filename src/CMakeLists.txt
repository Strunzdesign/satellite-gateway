set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS system signals program_options)
include_directories(${Boost_INCLUDE_DIR})

find_package(Threads)

add_executable(satellite-gateway
main-satellite-gateway.cpp
HdlcdClient/HdlcdClientHandler.cpp
HdlcdClient/HdlcdClientHandlerCollection.cpp
)

if(WIN32)
    set(ADDITIONAL_LIBRARIES wsock32 ws2_32)
else()
    set(ADDITIONAL_LIBRARIES "")
endif()

target_link_libraries(satellite-gateway
${Boost_LIBRARIES}
${CMAKE_THREAD_LIBS_INIT}
${ADDITIONAL_LIBRARIES}
)

install(TARGETS satellite-gateway RUNTIME DESTINATION bin)
