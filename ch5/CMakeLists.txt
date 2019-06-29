#指定生成目标
add_executable(Sample5_1 Sample5_1.cpp SixPointedStar.cpp)
add_executable(Sample5_2 Sample5_2.cpp SixPointedStar.cpp)
add_executable(Sample5_3 Sample5_3.cpp)
add_executable(Sample5_4 Sample5_4.cpp)

#链接共享库
target_link_libraries(Sample5_1 Common )
target_link_libraries(Sample5_2 Common )
target_link_libraries(Sample5_3 Common )
target_link_libraries(Sample5_4 Common )

set(CMAKE_MSVCIDE_RUN_PATH ${PROJECT_SOURCE_DIR}/../Mali_OpenGL_ES_Emulator/)




