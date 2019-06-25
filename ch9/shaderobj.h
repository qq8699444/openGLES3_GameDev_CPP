#pragma once
#include <string>
#include <vector>
#ifndef ANDROID
#include "esUtil.h"
#else
#include "EGLUtils.h"
#endif
#include "ShaderUtil.h"
#include "MatrixState.h"
#include "objtypes.h"

#define DIFFUSE_COLOR

struct renderObj
{
    
public:
    struct vertexData
    {
        Number3d v;
        Number3d n;
    };

    renderObj(std::vector<float> vertices,
        std::vector<float> normals,
        std::vector<int> aVertices,
        std::vector<int> aNormals,
        std::shared_ptr<MtlData> _material)
        :material(_material)
    {
        if (aVertices.size() != aNormals.size())
        {
            std::cerr << "size mismatch" << std::endl;
        }

        for (size_t i=0; i < aVertices.size();i++)
        {
            vertexData data;
            int vi = aVertices[i];
            int ni = aNormals[i];
            data.v = Number3d(vertices[3 * vi], vertices[3 * vi + 1], vertices[3 * vi + 2]);
            data.n = Number3d(normals[3 * ni], normals[3 * ni + 1], normals[3 * ni + 2]);
            aVertexData.push_back(data);
            Indices.push_back(i);
        }

        {
            glGenBuffers(1, &VB);
            glBindBuffer(GL_ARRAY_BUFFER, VB);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData) * aVertexData.size(), aVertexData.data(), GL_STATIC_DRAW);
            checkGLError("VB");

            glGenBuffers(1, &IB);
            glBindBuffer(GL_ARRAY_BUFFER, IB);
            glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), Indices.data(), GL_STATIC_DRAW);
            checkGLError("IB");
        }
    }
protected:
public:
    std::string name;
    std::vector<vertexData> aVertexData;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    std::shared_ptr<MtlData> material;
};

class shaderObj
{
public:
    shaderObj()
    {
        initShader();
    }

    void initShader()    
    {
#ifdef DIFFUSE_COLOR
        mVertexShader = ShaderUtil::loadFromAssetsFile("color_vertex.sh");        
        mFragmentShader = ShaderUtil::loadFromAssetsFile("color_frag.sh"); 
#else 
        mVertexShader = ShaderUtil::loadFromAssetsFile("color_vertex.sh2");
        mFragmentShader = ShaderUtil::loadFromAssetsFile("color_frag.sh2");
#endif
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        checkGLError("compile mProgram");

        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
        checkGLError("glGetAttribLocation");


        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
        muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
        maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");
        maCameraHandle = glGetUniformLocation(mProgram, "uCamera");
        checkGLError("glGetUniformLocation");

#ifdef DIFFUSE_COLOR
        mColorHandle = glGetUniformLocation(mProgram, "uColor");
#else
        mKaHandle = glGetUniformLocation(mProgram, "ka");
        mKdHandle = glGetUniformLocation(mProgram, "kd");
        mKsHandle = glGetUniformLocation(mProgram, "ks");
#endif
        mOpacityHandle = glGetUniformLocation(mProgram, "uOpacity");
    }

    void drawSelf(const renderObj* obj)
    {
        glUseProgram(mProgram);
        checkGLError("glUseProgram");
                
        glBindBuffer(GL_ARRAY_BUFFER, obj->VB);
        glEnableVertexAttribArray(maPositionHandle);
        glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, GL_FALSE, sizeof(renderObj::vertexData), 0);
        glEnableVertexAttribArray(maNormalHandle);
        glVertexAttribPointer(maNormalHandle, 3, GL_FLOAT, GL_FALSE, sizeof(renderObj::vertexData), (const GLvoid*)offsetof(renderObj::vertexData,n));

        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        checkGLError("glUniformMatrix4fv");

        glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        checkGLError("glUniformMatrix4fv");

        glUniform3fv(maLightLocationHandle, 1, MatrixState::lightLocation);
        glUniform3fv(maCameraHandle, 1, MatrixState::cameraLocation);
        checkGLError("location");
#ifdef DIFFUSE_COLOR
        glUniform3fv(mColorHandle, 1, &obj->material->Kd_Color.x);
#else
        glUniform3fv(mKaHandle, 1, &obj->material->Ka_Color.x);
        glUniform3fv(mKdHandle, 1, &obj->material->Kd_Color.x);
        glUniform3fv(mKsHandle, 1, &obj->material->Ks_Color.x);
#endif
        glUniform1f(mOpacityHandle,obj->material->alpha);
        checkGLError("colors");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->IB);
        checkGLError("bind iB");

        glDrawElements(GL_TRIANGLES, obj->Indices.size(), GL_UNSIGNED_INT, 0);
        checkGLError("glDrawElements");

        glDisableVertexAttribArray(maPositionHandle);
        glDisableVertexAttribArray(maNormalHandle);
    }
protected:
private:
    int mProgram;//自定义渲染管线着色器程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int muMMatrixHandle;//位置、旋转变换矩阵
    int maPositionHandle; //顶点位置属性引用  
    int maNormalHandle; //顶点法向量属性引用  
    int maLightLocationHandle;//光源位置属性引用  
    int maCameraHandle; //摄像机位置属性引用 
#ifdef DIFFUSE_COLOR
    int mColorHandle;
#else 
    int mKaHandle; //Ambient 
    int mKdHandle; //Diffuse 
    int mKsHandle; //Specular 
#endif
    int mOpacityHandle;
    std::string mVertexShader;//顶点着色器代码脚本      
    std::string mFragmentShader;//片元着色器代码脚本
};
