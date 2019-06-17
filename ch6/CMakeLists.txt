#指定生成目标
add_executable(ch6 ch6.cpp)
add_executable(vao vao.cpp)

#链接共享库
target_link_libraries(ch6 Common )
target_link_libraries(vao Common )

