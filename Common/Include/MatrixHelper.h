#ifndef matrix_helper_H 
#define matrix_helper_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include <math.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


struct GLMatrix
{
    GLfloat m[16];
};

void scaleM(GLMatrix& result, int offset, GLfloat sx, GLfloat sy, GLfloat sz);

void translateM(GLMatrix& result, int offset, GLfloat x, GLfloat y, GLfloat z);

void rotateM(GLMatrix& result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

int orthoM(GLMatrix& result, int mOffset, float left, float right, float bottom, float top, float near, float far);

int frustumM(GLMatrix& result, int offset, float left, float right, float bottom, float top, float near, float far);

int perspectiveM(GLMatrix &result, int offset, float fovy, float aspect, float zNear, float zFar);

void setIdentityM(GLMatrix &result);

void multiplyMM(GLMatrix &result, GLMatrix &lhs, GLMatrix & rhs);

int setLookAtM(GLMatrix& result, int rmOffset,
    float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ, float upX, float upY,
    float upZ);



#endif