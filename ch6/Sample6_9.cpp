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
#include <string>
#include <memory>
#include <iostream>
#include "esUtil.h"
#include "ShaderUtil.h"
#include "ball9.h"
#include "MatrixState.h"

const int WIDTH = 640;
const int HEIGHT = 480;

typedef struct
{
   // Handle to a program object
   //GLuint programObject;
    //SixPointedStar *star = nullptr;
    std::shared_ptr<Ball> ball;


} UserData;

///
// Create a shader object, load the shader source, and
// compile the shader.
//


///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
    
   UserData *userData = (UserData *)esContext->userData;

   //
   MatrixState::setInitStack();
   //onSurfaceCreated
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   
   userData->ball = std::make_shared<Ball>("vertex9.sh","frag9.sh");
 

   //打开深度检测
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

   //onSurfaceChanged
   //设置视口的大小及位置 
   glViewport(0, 0, WIDTH, HEIGHT);
   float  r = (float)WIDTH / HEIGHT;
   //MatrixState::setProjectOrtho(-r, r, -1, 1, 1, 10);
   MatrixState::setProjectFrustum(-r , r, -1 , 1 , 15, 100);
   MatrixState::setCamera(
       0, 0, 16.f,
       0, 0, 0.f,
       0.f, 1.0f, 0.0f
   );
   
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
    float lightOffset = 4;// -4;

   UserData *userData = (UserData *)esContext->userData;
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

   //设置光源位置
   MatrixState::setLightLocation(lightOffset, 0, 1.5f);
   
   {
       MatrixState::pushMatrix();
       //MatrixState::translate(-1.2f, 0, 0);//沿x轴负方向平移
       userData->ball->drawSelf();
       MatrixState::popMatrix();
   }   

}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   //delete userData->star;
   userData->ball.reset();
   delete userData;
   esContext->userData = nullptr;
}


extern "C" int esMain ( ESContext *esContext )
{
   esContext->userData = new UserData;

   esCreateWindow ( esContext, "6-9", WIDTH, HEIGHT, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
