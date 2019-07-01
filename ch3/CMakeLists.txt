#指定生成目标
add_executable(Sample3_1 Sample3_1.cpp)

#链接共享库
target_link_libraries(Sample3_1 Common )
set_target_properties(Sample3_1 PROPERTIES FOLDER "chapter3") 