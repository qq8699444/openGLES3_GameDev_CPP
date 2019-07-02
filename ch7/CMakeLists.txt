#指定生成目标
add_executable(Sample7_1 Sample7_1.cpp)
target_link_libraries(Sample7_1 Common )
set_target_properties(Sample7_1 PROPERTIES FOLDER "chapter7") 

add_executable(Sample7_3 Sample7_3.cpp)
target_link_libraries(Sample7_3 Common )
set_target_properties(Sample7_3 PROPERTIES FOLDER "chapter7") 

