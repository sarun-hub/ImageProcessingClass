add_library(histogram histogram.cpp)
target_link_libraries(histogram PRIVATE imageIO)

target_include_directories(histogram PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sample3 sample3.cpp)

target_link_libraries(sample3 PRIVATE imageIO histogram)

add_executable(histogram_equalization histogram_equalization.cpp)

target_link_libraries(histogram_equalization PRIVATE imageIO histogram)
