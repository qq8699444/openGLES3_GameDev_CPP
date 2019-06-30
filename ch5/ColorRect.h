#pragma  once
#include <cmath>
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


class ColorRect
{
public:
    ColorRect()
    {
        initVertexData();
        initShader();
    }

    void initVertexData()
    {
        
        //顶点坐标数据的初始化================begin============================
        vCount = 6;
        float vertices[] = 
        {
            0, 0, 0,
                UNIT_SIZE, UNIT_SIZE, 0,
                -UNIT_SIZE, UNIT_SIZE, 0,
                -UNIT_SIZE, -UNIT_SIZE, 0,
                UNIT_SIZE, -UNIT_SIZE, 0,
                UNIT_SIZE, UNIT_SIZE, 0
        };

        mVertices = std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0]));
        

        //顶点着色数据的初始化================begin============================
        float colors[] = 
        {
            1, 1, 1, 0,
                0, 0, 1, 0,
                0, 0, 1, 0,
                0, 0, 1, 0,
                0, 0, 1, 0,
                0, 0, 1, 0,
        };
        mColors = std::vector<float>(colors, colors + sizeof(colors) / sizeof(colors[0]));

    }
    void initShader()
    {
        // 加载顶点着色器的脚本内容
        mVertexShader = ShaderUtil::loadFromAssetsFile("vertex13.sh");
        // 加载片元着色器的脚本内容
        mFragmentShader = ShaderUtil::loadFromAssetsFile("frag13.sh");
        // 基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        // 获取程序中顶点位置属性引用id
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        // 获取程序中顶点颜色属性引用id
        maColorHandle = glGetAttribLocation(mProgram, "aColor");
        // 获取程序中总变换矩阵引用id
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");

        muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    }

    void drawSelf()
    {
        //指定使用某套着色器程序
        glUseProgram(mProgram);
        checkGLError("glUseProgram");


        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //将平移、旋转变换矩阵传入渲染管线
        glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        checkGLError("glUniformMatrix4fv");
        //将顶点位置数据传入渲染管线
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertices.data()
        );
        //将顶点颜色数据传入渲染管线
        glVertexAttribPointer
        (
            maColorHandle,
            4,
            GL_FLOAT,
            false,
            4 * 4,
            mColors.data()
        );
        checkGLError("glVertexAttribPointer");


        //启用顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);
        //启用顶点颜色数据数组
        glEnableVertexAttribArray(maColorHandle);
        checkGLError("glEnableVertexAttribArray");


        //绘制颜色矩形
        glDrawArrays(GL_TRIANGLE_FAN, 0, vCount);
        checkGLError("glDrawArrays");
    }

    static const float UNIT_SIZE;
protected:
private:
    int mProgram;//自定义渲染管线着色器程序id
    int muMMatrixHandle;//位置、旋转变换矩阵引用
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用  
    int maColorHandle; //顶点颜色属性引用 
    std::string mVertexShader;//顶点着色器代码脚本  
    std::string mFragmentShader;//片元着色器代码脚本

    std::vector<float>   mVertices;//顶点坐标数据缓冲
    std::vector<float>   mColors;//顶点着色数据缓冲
    int vCount = 0; //顶点数量 
    
};

const float ColorRect::UNIT_SIZE = 1.0f;