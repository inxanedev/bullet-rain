cmake_minimum_required(VERSION 3.10)
project(bullet-rain)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "Bullet_Rain")

find_package(SFML COMPONENTS graphics REQUIRED)

add_executable(bullet-rain
    "src/main.cc"
)

target_link_libraries(bullet-rain sfml-graphics)
target_include_directories(bullet-rain PUBLIC "src")
target_include_directories(bullet-rain PUBLIC "libs")

file(GLOB ASSETS "assets/*")
file(COPY ${ASSETS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

add_custom_command(TARGET bullet-rain POST_BUILD COMMAND cp ${CMAKE_SOURCE_DIR}/default_config.json "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json")