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

#define  toRadians(x)   (x*3.14159/180)

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
    static void scale(float x, float y, float z);
    //
    static void pushMatrix();
    static void popMatrix();
    //
    static void testglm();
    //
    static const float* getFinalMatrix();
    static const float* getMMatrix();
    //
    

    static void setperspective
    (
        float fovy,
        float aspect,
        float znear,	//near面距离
        float zfar //far面距离
    );
    
    static void setProjectFrustum(
        float left, // near面的left
        float right, // near面的right
        float bottom, // near面的bottom
        float top, // near面的top
        float znear, // near面与视点的距离
        float zfar // far面与视点的距离
    );
    static void setProjectOrtho(
        float left,
        float right,
        float bottom,
        float top,
        float znear,
        float zfar
    );
    static void setCamera
    (
        float cx,	//摄像机位置x
        float cy, //摄像机位置y
        float cz, //摄像机位置z
        float tx, //摄像机目标点x
        float ty, //摄像机目标点y
        float tz, //摄像机目标点z
        float upx, //摄像机UP向量X分量
        float upy, //摄像机UP向量Y分量
        float upz //摄像机UP向量Z分量  
    );
    
    static void setLightLocation(float x, float y, float z);
    static float * getLightLocation() {
        return glm::value_ptr(lightLocation);
    }

    static float * getCameraLocation() {
        return glm::value_ptr(cameraLocation);
    }

    static void setLightDirection(float x, float y, float z);
    static float * getLightDirection() {
        return glm::value_ptr(lightDirection);
    }

public:
#ifdef USE_GLM
    static glm::mat4 mProjMatrix;
    static glm::mat4 mVMatrix;
    static glm::mat4 currMatrix;

    //stack
    static glm::mat4 mStack[16];
    static int stackTop;

    //
    static glm::mat4 mMVPMatrix;

    static glm::vec3   lightLocation;
    static glm::vec3   cameraLocation;
    static glm::vec3   lightDirection;
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
