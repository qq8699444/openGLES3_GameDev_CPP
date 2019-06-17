cmake_minimum_required( VERSION 2.6 )
project( opengles3_note )

include_directories( ../Mali_OpenGL_ES_Emulator/include/)
include_directories( Common/Include )
include_directories(D:/vulkan_dev/glm/)
#find_library( OPENGLES3_LIBRARY GLESv2 "OpenGL ES v3.0 library")
find_library( OPENGLES3_LIBRARY libGLESv2.lib ../Mali_OpenGL_ES_Emulator/ )
find_library( EGL_LIBRARY libEGL.lib ${PROJECT_SOURCE_DIR}/../Mali_OpenGL_ES_Emulator/ )
 
SUBDIRS( Common ch2 ch6 ch8 ch9)		