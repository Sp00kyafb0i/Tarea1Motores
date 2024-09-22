cmake_minimum_required(VERSION 3.20)

project(tarea1_sln CXX)

add_subdirectory("external/rusty_audio")

add_executable(Tarea1
    "main.cpp"
)
set_property(TARGET Tarea1 PROPERTY CXX_STANDARD 20)
target_include_directories(Tarea1 PRIVATE ${RUSTY_AUDIO_INCLUDE})
target_link_libraries(Tarea1 PRIVATE rusty_audio)