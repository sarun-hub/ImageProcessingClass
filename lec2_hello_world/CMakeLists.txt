add_executable(sample1 sample1.cpp)

add_executable(rgb_to_grey sample1_rgb_to_grey.cpp)

set(group_lec1 
    sample1
    rgb_to_grey
)

foreach(target ${group_lec1})
    target_link_libraries(${target} 
    PRIVATE 
    imageIO
)
endforeach()