add_executable(sample1 sample1.cpp)

add_executable(rgb_to_grey rgb_to_grey.cpp)

add_executable(color_swap color_swap.cpp)

set(group_lec1 
    sample1
    rgb_to_grey
    color_swap
)

foreach(target ${group_lec1})
    target_link_libraries(${target} 
    PRIVATE 
    imageIO
)
endforeach()