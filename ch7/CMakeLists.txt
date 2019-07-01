#指定生成目标
add_executable(Sample7_1 Sample7_1.cpp)

#链接共享库
target_link_libraries(Sample7_1 Common )
set_target_properties(Sample7_1 PROPERTIES FOLDER "chapter7") 
