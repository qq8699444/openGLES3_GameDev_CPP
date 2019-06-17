#pragma once
#include <stack>
#include "MatrixHelper.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext.hpp> // glm::pi

#define USE_GLM
//#undef USE_GLM

class MatrixState
{
public:
    MatrixState();
    ~MatrixState();

    static void setInitStack();
    static void translate(float x, float y, float z);
    static void rotate(float angle, float x, float y, float z);

    //
    static void pushMatrix();
    static void popMatrix();
    //
    static void testglm();
    //
    static const float* getFinalMatrix();

    //
    

    static void setperspective
    (
        float fovy,
        float aspect,
        float znear,	//near�����
        float zfar //far�����
    );
    

    static void setCamera
    (
        float cx,	//�����λ��x
        float cy, //�����λ��y
        float cz, //�����λ��z
        float tx, //�����Ŀ���x
        float ty, //�����Ŀ���y
        float tz, //�����Ŀ���z
        float upx, //�����UP����X����
        float upy, //�����UP����Y����
        float upz //�����UP����Z����  
    );
    

private:
#ifdef USE_GLM
    static glm::mat4 mProjMatrix;
    static glm::mat4 mVMatrix;
    static glm::mat4 currMatrix;

    //stack
    static glm::mat4 mStack[16];
    static int stackTop;

    //
    static glm::mat4 mMVPMatrix;
#else
    static GLMatrix mProjMatrix;
    static GLMatrix mVMatrix;
    static GLMatrix currMatrix;

    //stack
    static GLMatrix mStack[16];
    static int stackTop;

    //
    static GLMatrix mMVPMatrix;
#endif
};
