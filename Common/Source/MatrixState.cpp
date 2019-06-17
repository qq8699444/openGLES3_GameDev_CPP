#include "MatrixState.h"

#ifdef USE_GLM
glm::mat4 MatrixState::mProjMatrix;
glm::mat4 MatrixState::mVMatrix;
glm::mat4 MatrixState::currMatrix;
glm::mat4 MatrixState::mMVPMatrix;
glm::mat4 MatrixState::mStack[16];
#else
GLMatrix MatrixState::mProjMatrix;
GLMatrix MatrixState::mVMatrix;
GLMatrix MatrixState::currMatrix;
GLMatrix MatrixState::mMVPMatrix;
GLMatrix MatrixState::mStack[16];
#endif
int MatrixState::stackTop;
MatrixState::MatrixState()
{

    stackTop = -1;
}

MatrixState::~MatrixState()
{
}

void MatrixState::setInitStack()
{
#ifdef USE_GLM
    currMatrix = glm::mat4(1.0f);
    mProjMatrix = glm::mat4(1.0f);
    mVMatrix = glm::mat4(1.0f);
    mMVPMatrix = glm::mat4(1.0f);
#else
    setIdentityM(currMatrix);
    setIdentityM(mProjMatrix);
    setIdentityM(mVMatrix);
    setIdentityM(mMVPMatrix);

    rotateM(currMatrix, 0, 1, 0, 0);
#endif // USE_GLM

    
}

void MatrixState::translate(float x, float y, float z)
{
#ifdef USE_GLM
    glm::translate(currMatrix, glm::vec3(x, y, z));
#else
    translateM(currMatrix, 0, x, y, z);
    #endif
}

void MatrixState::rotate(float angle, float x, float y, float z)
{
#ifdef USE_GLM
    glm::rotate(currMatrix, angle, glm::vec3(x, y, z));
#else
    rotateM(currMatrix, angle, x, y, z);
#endif
}

void MatrixState::pushMatrix()
{
    stackTop++;
    for (int i = 0; i < 16; i++) {
#ifdef USE_GLM
        mStack[stackTop] = currMatrix;
#else
        mStack[stackTop].m[i] = currMatrix.m[i];
#endif
    }
}


void MatrixState::popMatrix()
{
    for (int i = 0; i < 16; i++) {
#ifdef USE_GLM
        currMatrix = mStack[stackTop];
#else
        currMatrix.m[i] = mStack[stackTop].m[i];
#endif
    }
    stackTop--;
}

const float* MatrixState::getFinalMatrix()
{
#ifdef USE_GLM
    auto tmp = mVMatrix * currMatrix;
    mMVPMatrix = mProjMatrix * tmp;
    float* ptr = glm::value_ptr(mMVPMatrix);
    for (int i = 0; i < 4; i++)
    {
        printf("%f %f %f %f\n", ptr[i], ptr[i + 1], ptr[i + 2], ptr[i + 3]);
    }
    
    return glm::value_ptr(mMVPMatrix);
#else
    multiplyMM(mMVPMatrix, mVMatrix, currMatrix);
    multiplyMM(mMVPMatrix, mProjMatrix, mMVPMatrix);
    float* ptr = mMVPMatrix.m;
    for (int i = 0; i < 4; i++)
    {
        printf("%f %f %f %f\n", ptr[i], ptr[i+1], ptr[i+2], ptr[i+3]);
    }
    
    return &mMVPMatrix.m[0];
#endif
    
}

void MatrixState::setCamera
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
)
{
#ifdef USE_GLM
    mVMatrix = glm::lookAt(glm::vec3(cx, cy, cz),
            glm::vec3(tx, ty, tz),
            glm::vec3(upx, upy, upz));
#else
    setLookAtM
    (
        mVMatrix,
        0,
        cx,        cy,        cz,
        tx,        ty,        tz,
        upx,        upy,      upz
    );
#endif
}

void MatrixState::setperspective
(
    float fovy,
    float aspect,
    float znear,	//near面距离
    float zfar //far面距离
)
{
#ifdef USE_GLM
    mProjMatrix = glm::perspective(fovy*glm::pi<float>() / 180.f, aspect, znear, zfar);
#else
    //frustumM(mProjMatrix, 0, left, right, bottom, top, znear, zfar);
    perspectiveM(mProjMatrix, 0, fovy, aspect, znear, zfar);
#endif
}
void MatrixState::testglm()
{
    //glm::mat4 m1 = glm::ortho(3.0f, 5.0f, 1.0f, 3.0f, 3.0f, 7.0f);
    //glm::mat4 m1 = glm::frustum(3.0f, 5.0f, 1.0f, 3.0f, 3.0f, 7.0f);
    //glm::mat4 m1 = glm::lookAt(
    //    glm::vec3(3.f, 3.0f, 3.0f),
    //    glm::vec3(0.f, 0.f, 0.f),
    //    glm::vec3(0.f, 1.f, 0.f)
    //    );
    glm::mat4 m1 = glm::perspective(45.f*glm::pi<float>()/180.f, 0.8f, 3.f, 10.f);
    float* ptr1 = glm::value_ptr(m1);
    
    GLMatrix m2;
    //orthoM(m2, 0, 3.0f, 5.0f, 1.0f, 3.0f, 3.0f, 7.0f);
    //frustumM(m2, 0, 3.0f, 5.0f, 1.0f, 3.0f, 3.0f, 7.0f);
    //setLookAtM(m2, 0, 
    //    3.f,3.0f,3.0f,
    //    0.f,0.f,0.f,
    //    0.f,1.f,0.f);
    perspectiveM(m2, 0, 45.f, 0.8f, 3.f, 10.f);
    float* ptr2 = m2.m;

    for (int i = 0; i < 16; i++)
    {
        printf("%f %f \n", ptr1[i], ptr2[i]);
    }

    printf("test glm done\n");
}