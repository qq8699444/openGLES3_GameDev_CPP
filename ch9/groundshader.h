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

struct groundRenderObj
{
public:
    struct vertexData
    {
        Number3d v;
        uv texCoord;
        Number3d n;
    };
    groundRenderObj() :
        name("ground"),
        alpha(1.0)
    {
        const float Unit = 6.0f;
        const Number3d normal(0, 1, 0);

        {
            vertexData a1;
            a1.v = Number3d(-1.0*Unit, 0, -4.0* Unit);
            a1.texCoord = uv(0, 0);
            a1.n = normal; //vertical
            aVertexData.push_back(a1);
        }

        {
            vertexData a2;
            a2.v = Number3d(-1.0*Unit, 0, 4.0*Unit);
            a2.texCoord = uv(0, 1);
            a2.n = normal; //vertical
            aVertexData.push_back(a2);
        }
        {
            vertexData a3;
            a3.v = Number3d(1.0*Unit, 0, 4.0*Unit);
            a3.texCoord = uv(1, 1);
            a3.n = normal; //vertical
            aVertexData.push_back(a3);
        }
        {
            vertexData a4;
            a4.v = Number3d(1.0*Unit, 0, -4.0*Unit);
            a4.texCoord = uv(1, 0);
            a4.n = normal; //vertical
            aVertexData.push_back(a4);
        }

        Indices = {
            0,1,2,
            2,3,0
        };

        {
            glGenBuffers(1, &VB);
            glBindBuffer(GL_ARRAY_BUFFER, VB);
            glBufferData(GL_ARRAY_BUFFER, sizeof(groundRenderObj::vertexData) * aVertexData.size(), aVertexData.data(), GL_STATIC_DRAW);
            checkGLError("VB");

            glGenBuffers(1, &IB);
            glBindBuffer(GL_ARRAY_BUFFER, IB);
            glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), Indices.data(), GL_STATIC_DRAW);
            checkGLError("IB");
        }

        this->textureId = initTexture("road.bmp");
    }

    GLuint initTexture(const char* rgbfilename)//textureId
    {
        //生成纹理ID
        GLuint textures = 0;
        glGenTextures
        (
            1, //产生的纹理id的数量
            &textures //纹理id的数组
        );

        GLuint textureId = textures;
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //通过输入流加载图片===============begin===================
        const int imgWidth = 512;
        const int imgHeight = 512;
        //unsigned char* pixels1 = (unsigned char*)malloc(3 * imgWidth*imgHeight);
        unsigned char* pixels = (unsigned char*)malloc(3 * imgWidth*imgHeight);

        {
            //bitmapTmp = BitmapFactory.decodeStream(is);
            FILE* bmpfile = fopen(rgbfilename, "rb");
            int readed = fread(pixels, 3 * imgWidth, imgHeight, bmpfile);


            //swap B and R
            for (int h = 0; h < imgHeight; h++)
            {
                int offset = 3 * h * imgWidth;
                for (int w = 0; w < imgWidth; w++)
                {
                    unsigned char tmp = pixels[offset + 3 * w];
                    pixels[offset + 3 * w] = pixels[offset + 3 * w + 2];
                    pixels[offset + 3 * w + 2] = tmp;
                }

            }
            fclose(bmpfile);
        }

        //通过输入流加载图片===============end===================== 
        glTexImage2D
        (
            GL_TEXTURE_2D, //纹理类型
            0,
            GL_RGB,
            imgWidth,
            imgHeight,
            0, //broder
            GL_RGB,
            GL_UNSIGNED_BYTE,
            pixels
        );
        checkGLError("glTexImage2D");
        free(pixels); //纹理加载成功后释放图片
        //free(pixels1);
        return textureId;
    }
public:
    std::string name;
    std::vector<vertexData> aVertexData;
    std::vector<unsigned int> Indices;
    
    float alpha = 1.0;

    //
    GLuint VB;
    GLuint IB;
    // Texture handle
    GLuint textureId;
};

class groundShaderObj
{
public:
    groundShaderObj()
    {
        initShader();
    }

    void initShader()
    {
        mVertexShader = ShaderUtil::loadFromAssetsFile("texture_vertex.sh");
        mFragmentShader = ShaderUtil::loadFromAssetsFile("texture_frag.sh");

        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        checkGLError("compile mProgram");

        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
        maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
        checkGLError("glGetAttribLocation");


        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
        muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
        maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");
        maCameraHandle = glGetUniformLocation(mProgram, "uCamera");
        checkGLError("glGetUniformLocation");

        mOpacityHandle = glGetUniformLocation(mProgram, "uOpacity");
    }

    void drawSelf(const groundRenderObj* obj)
    {
        glUseProgram(mProgram);
        checkGLError("glUseProgram");

        glBindBuffer(GL_ARRAY_BUFFER, obj->VB);
        glEnableVertexAttribArray(maPositionHandle);
        glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, GL_FALSE, sizeof(groundRenderObj::vertexData), 0);
        glEnableVertexAttribArray(maTexCoorHandle);
        glVertexAttribPointer(maTexCoorHandle, 2, GL_FLOAT, GL_FALSE, sizeof(groundRenderObj::vertexData), (const GLvoid*)offsetof(groundRenderObj::vertexData, texCoord));
        glEnableVertexAttribArray(maNormalHandle);
        glVertexAttribPointer(maNormalHandle, 3, GL_FLOAT, GL_FALSE, sizeof(groundRenderObj::vertexData), (const GLvoid*)offsetof(groundRenderObj::vertexData, n));

        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        checkGLError("glUniformMatrix4fv");

        glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        checkGLError("glUniformMatrix4fv");

        glUniform3fv(maLightLocationHandle, 1, MatrixState::lightLocation);
        glUniform3fv(maCameraHandle, 1, MatrixState::cameraLocation);
        checkGLError("location");

        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->textureId);

        //
        glUniform1f(mOpacityHandle, obj->alpha);
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
    int maTexCoorHandle; //顶点法向量属性引用  
    int maLightLocationHandle;//光源位置属性引用  
    int maCameraHandle; //摄像机位置属性引用 
    int mOpacityHandle;

    std::string mVertexShader;//顶点着色器代码脚本      
    std::string mFragmentShader;//片元着色器代码脚本
};