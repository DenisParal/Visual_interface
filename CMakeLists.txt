cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -g")

set(header_path "${${PROJECT_NAME}_SOURCE_DIR}/include")
set(src_path "${${PROJECT_NAME}_SOURCE_DIR}/src")
set(test_path "${${PROJECT_NAME}_SOURCE_DIR}/test")

add_library(${PROJECT_NAME} SHARED ${header_path}/core.hpp ${header_path}/visualizer.hpp ${header_path}/actions.hpp ${src_path}/core.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${header_path})
target_link_options(${PROJECT_NAME} PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)
target_compile_options(${PROJECT_NAME} PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)

add_executable(main ${test_path}/basic_drawing.cpp)
target_link_options(main PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)
target_compile_options(main PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)
target_link_libraries(main ${PROJECT_NAME})

add_executable(functional_test ${test_path}/functional_object_test.cpp)
target_link_options(functional_test PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)
target_compile_options(functional_test PUBLIC -lsfml-graphics -lsfml-window -lsfml-system)
target_link_libraries(functional_test ${PROJECT_NAME})

