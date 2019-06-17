// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this
//    example is to demonstrate the basic concepts of
//    OpenGL ES 3.0 rendering.
#include "esUtil.h"
#include "LoadedObjectVertexOnly.h"
#include "LoadUtil.h"
#include "MatrixState.h"

typedef struct
{
   // Handle to a program object
    LoadedObjectVertexOnly* obj;
} UserData;

const int winw = 640;
const int winh = 480;
///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
    UserData *userData = (UserData *)esContext->userData;

    //onSurfaceCreated
   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   //打开深度检测
   glEnable(GL_DEPTH_TEST);
   //打开背面剪裁   
   glEnable(GL_CULL_FACE);
   //初始化变换矩阵
   MatrixState::setInitStack();
   //加载要绘制的物体
   userData->obj = loadUtil::loadFromFile("ch.obj");



   //onSurfaceChanged
   glViewport(0, 0, esContext->width, esContext->height);
   float ratio = (float)esContext->width / esContext->height;
   //调用此方法计算产生透视投影矩阵
   MatrixState::setperspective(45.0f, ratio,  2, 100);
   //调用此方法产生摄像机9参数位置矩阵
   MatrixState::setCamera(0, 0, 0, 0.f, 0.f, -1.f, 0.f, 1.0f, 0.0f);

   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   

   //清除深度缓冲与颜色缓冲
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

   MatrixState::pushMatrix();//保护现场
   MatrixState::translate(0, -2.f, -25.f);//平移坐标系
   //绕Y轴、X轴旋转
   //MatrixState.rotate(yAngle, 0, 1, 0);//绕Y轴旋转
   //MatrixState.rotate(xAngle, 1, 0, 0);//绕X轴旋转

   //若加载的物体不为空则绘制物体
   if (userData->obj != nullptr)
   {
       userData->obj->drawSelf();
   }
   MatrixState::popMatrix(); //恢复现场   
   
}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   delete userData->obj;
   free(userData);   
}
extern "C" int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );
   memset(esContext->userData, 0, sizeof(UserData));

   esCreateWindow ( esContext, "Hello Triangle", winw, winh, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
