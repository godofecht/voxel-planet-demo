cmake_minimum_required(VERSION 3.15)
project(VoxelPlanet VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenVDB CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenVDB::openvdb
    TBB::tbb
    Boost::boost
)