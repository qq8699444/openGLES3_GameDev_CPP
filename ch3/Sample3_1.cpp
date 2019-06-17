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
#include "Triangle.h"
#include "MatrixState.h"

static const int WIDTH = 640;
static const int HEIGHT = 480;

typedef struct
{
    std::shared_ptr<Triangle> triangle;

} UserData;

///

///
// Initialize the shader and program object
//
int Init(ESContext *esContext)
{
    {
        GLint maxVertexAttribs;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
        std::cout << "maxVertexAttribs:" << maxVertexAttribs << std::endl;
    }

    UserData *userData = (UserData *)esContext->userData;
    
    {
        userData->triangle = std::make_shared<Triangle>();
    }   


    //onSurfaceCreated    
    glClearColor(0.5f, 0.5f, 0.5f, 0.7f);
    glEnable(GL_DEPTH_TEST);

    //onSurfaceChanged
    glViewport(0, 0, WIDTH, HEIGHT);
    float  r = (float)WIDTH / HEIGHT;
    //调用此方法计算产生透视投影矩阵
    //Matrix.frustumM(Triangle.mProjMatrix, 0, -ratio, ratio, -1, 1, 1, 10);
    userData->triangle->mProjMatrix = glm::frustum(-r, r, -1.0f, 1.0f, 1.f, 10.f);
    //调用此方法产生摄像机9参数位置矩阵
    //Matrix.setLookAtM(Triangle.mVMatrix, 0, 0, 0, 3, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
    userData->triangle->mVMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 3.f),
                                glm::vec3(0.f, 0.f, 0.f), 
                                glm::vec3(0.f, 1.0f, 0.0f));
    return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    {
        userData->triangle->drawSelf();
    }
    //userData->star->drawSelf();
}

void Shutdown(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    //delete userData->star;
    userData->triangle.reset();
    delete userData;
}

extern "C" int esMain(ESContext *esContext)
{
    esContext->userData = new UserData;

    esCreateWindow(esContext, "Hello SixPointedStar", WIDTH, HEIGHT, ES_WINDOW_RGB);

    if (!Init(esContext))
    {
        return GL_FALSE;
    }

    //esRegisterKeyFunc(esContext, KeyProc);
    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterDrawFunc(esContext, Draw);

    return GL_TRUE;
}
