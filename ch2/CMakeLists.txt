#指定生成目标
add_executable(ch2 ch2.cpp SixPointedStar.cpp)


#链接共享库
target_link_libraries(ch2 Common )
set(CMAKE_MSVCIDE_RUN_PATH ${PROJECT_SOURCE_DIR}/../Mali_OpenGL_ES_Emulator/)

