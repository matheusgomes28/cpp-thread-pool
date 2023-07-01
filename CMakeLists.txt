cmake_minimum_required(VERSION 3.21)
project(CppThreadPool LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)


add_subdirectory(src/thread_safe_queue)
add_subdirectory(src/thread_pool)
add_subdirectory(tests/thread_pool_app)
