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

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0
#define VERTEX_COLOR_INDX     1

#define VERTEX_STRIDE	(sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE))
typedef struct
{
   // Handle to a program object
   GLuint programObject;

   GLuint vboIds[2];

   GLuint vaoId;
} UserData;

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   {
      return 0;
   }

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled )
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char *infoLog = new char[infoLen];

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );

		 delete[] infoLog;
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   const char vShaderStr[] =
	   "#version 300 es                            \n"
	   "layout(location = 0) in vec4 a_position;   \n"
	   "layout(location = 1) in vec4 a_color;      \n"
	   "out vec4 v_color;                          \n"
	   "void main()                                \n"
	   "{                                          \n"
	   "    v_color = a_color;                     \n"
	   "    gl_Position = a_position;              \n"
	   "}";


   const char fShaderStr[] =
	   "#version 300 es            \n"
	   "precision mediump float;   \n"
	   "in vec4 v_color;           \n"
	   "out vec4 o_fragColor;      \n"
	   "void main()                \n"
	   "{                          \n"
	   "    o_fragColor = v_color; \n"
	   "}";
	
   GLfloat vertices[3*(VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] = 
   {
	  0.0f,  0.5f, 0.0f,        // v0
	  1.0f, 0.0f, 0.0f, 1.0f,   // c0

	  -0.5f, -0.5f, 0.0f,        // v1
	  0.0f, 1.0f, 0.0f, 1.0f,   // c1

	  0.5f, -0.5f, 0.0f    ,     // v2
	  0.0f, 0.0f, 1.0f, 1.0f    // c2
   };
   
   GLushort indices[3] = { 0, 1, 2 };

   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   programObject = esLoadProgram(vShaderStr, fShaderStr);
   if (programObject == 0)
   {
	   return GL_FALSE;
   }

   // Store the program object
   userData->programObject = programObject;

   glGenBuffers(2, userData->vboIds);

   glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glGenVertexArrays(1, &userData->vaoId);
   glBindVertexArray(userData->vaoId);
   glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1]);


   glEnableVertexAttribArray(VERTEX_POS_INDX);
   glEnableVertexAttribArray(VERTEX_COLOR_INDX);

   glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (const void*)0);
   glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (const void*)(VERTEX_POS_SIZE * sizeof(GLfloat)));
   glBindVertexArray(0);

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

void DrawPrimitiveWithVBOs(ESContext *esContext,
	GLint numVertices, GLfloat **vtxBuf,
	GLint *vtxStrides, GLint numIndices,
	GLushort *indices)
{
	UserData *userData = (UserData *)esContext->userData;

	if (userData->vboIds[0] == 0 && userData->vboIds[1] == 0 && userData->vboIds[2] == 0)
	{
		

		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, indices, GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
	glEnableVertexAttribArray(VERTEX_POS_INDX);
	

	glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[1]);
	glEnableVertexAttribArray(VERTEX_COLOR_INDX);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[2]);
	glDrawElements(GL_TRIANGLES, numIndices,GL_UNSIGNED_SHORT, 0);


	glDisableVertexAttribArray(VERTEX_POS_INDX);
	glDisableVertexAttribArray(VERTEX_COLOR_INDX);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   GLfloat vertexPos[3 * VERTEX_POS_SIZE] =
   {
	  0.0f,  0.5f, 0.0f,        // v0
	  -0.5f, -0.5f, 0.0f,        // v1
	  0.5f, -0.5f, 0.0f         // v2
   };
   GLfloat color[4 * VERTEX_COLOR_SIZE] =
   {
	  1.0f, 0.0f, 0.0f, 1.0f,   // c0
	  0.0f, 1.0f, 0.0f, 1.0f,   // c1
	  0.0f, 0.0f, 1.0f, 1.0f    // c2
   };
   GLint vtxStrides[2] =
   {
	  VERTEX_POS_SIZE * sizeof(GLfloat),
	  VERTEX_COLOR_SIZE * sizeof(GLfloat)
   };

   // Index buffer data
   GLushort indices[3] = { 0, 1, 2 };
   GLfloat *vtxBuf[2] = { vertexPos, color };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex data
   glBindVertexArray(userData->vaoId);
   glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   glDeleteProgram ( userData->programObject );
}
extern "C" int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );
   memset(esContext->userData, 0, sizeof(UserData));

   esCreateWindow ( esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
