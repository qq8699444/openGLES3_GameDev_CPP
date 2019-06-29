#指定生成目标
add_executable(Sample5_1 Sample5_1.cpp SixPointedStar.cpp)
add_executable(Sample5_2 Sample5_2.cpp SixPointedStar.cpp)
add_executable(Sample5_3 Sample5_3.cpp)
add_executable(Sample5_4 Sample5_4.cpp)
add_executable(Sample5_5 Sample5_5.cpp)
add_executable(Sample5_6 Sample5_6.cpp)
add_executable(Sample5_7 Sample5_7.cpp)
add_executable(Sample5_8 Sample5_8.cpp)
add_executable(Sample5_9 Sample5_9.cpp)
add_executable(Sample5_10 Sample5_10.cpp)
add_executable(Sample5_12 Sample5_12.cpp SixPointedStar12.cpp)
#链接共享库
target_link_libraries(Sample5_1 Common )
target_link_libraries(Sample5_2 Common )
target_link_libraries(Sample5_3 Common )
target_link_libraries(Sample5_4 Common )
target_link_libraries(Sample5_5 Common )
target_link_libraries(Sample5_6 Common )
target_link_libraries(Sample5_7 Common )
target_link_libraries(Sample5_8 Common )
target_link_libraries(Sample5_9 Common )
target_link_libraries(Sample5_10 Common )
target_link_libraries(Sample5_12 Common )
set(CMAKE_MSVCIDE_RUN_PATH ${PROJECT_SOURCE_DIR}/../Mali_OpenGL_ES_Emulator/)




