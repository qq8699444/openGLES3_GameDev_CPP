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
#include "textureUtil.h"
#include "point9.h"
#include "MatrixState.h"

const int WIDTH = 640;
const int HEIGHT = 480 ;
const int STAR_CNT = 6;
typedef struct
{
   // Handle to a program object
   //GLuint programObject;
    //SixPointedStar *star = nullptr;
    std::shared_ptr<Point> t;
    int textureId;

} UserData;



///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
    //check is ETC1 Supported 
    {
        float    pointSizeRange[2];
        glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, pointSizeRange);
        std::cout << "POINT_SIZE_RANGE : " << pointSizeRange[0] << " , " << pointSizeRange[1] << std::endl;
    }

   UserData *userData = (UserData *)esContext->userData;

   //
   MatrixState::setInitStack();
   //onSurfaceCreated
   glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
   
   std::vector<std::string> imgfileArr;
   for (int i = 1; i <= 4; i++)
   {
       char filename[16];
       sprintf_s(filename, "tex%d.png", i);
       imgfileArr.push_back(filename);
   }
   userData->textureId = TextureUtil::loadArray(imgfileArr,128,128);
   userData->t = std::make_shared<Point>();
   


   //打开深度检测
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

   //onSurfaceChanged
   //设置视口的大小及位置 
   glViewport(0, 0, WIDTH, HEIGHT);
   float  r = (float)WIDTH / HEIGHT;
   MatrixState::setperspective(45, r, 1, 100);

   MatrixState::setCamera(
       0, 0, 5.f,
       0, 0, 0.f,
       0.f, 1.0f, 0.0f
   );

   glViewport(0, 0, WIDTH, HEIGHT);
   

   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   

   MatrixState::pushMatrix();
   userData->t->drawSelf(userData->textureId);
   MatrixState::popMatrix();
}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   //delete userData->star;
   userData->t.reset();
   delete userData;
   esContext->userData = nullptr;
}


extern "C" int esMain ( ESContext *esContext )
{
   esContext->userData = new UserData;

   esCreateWindow ( esContext, "Hello SixPointedStar", WIDTH, HEIGHT, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
