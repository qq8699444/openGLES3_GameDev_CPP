#pragma once
#include <string>
#include <vector>
#include "esUtil.h"
#include "ShaderUtil.h"
#include "MatrixState.h"

class LoadedObjectVertexOnly
{
public:
    LoadedObjectVertexOnly(std::vector<float>& vlist)
    {
        initVertexData(vlist);
        initShader();
    }

    void initVertexData(std::vector<float>& vlist)
    {
        //顶点坐标数据的初始化================begin============================
        vertices = vlist;
        vCount = vertices.size() / 3;

       
        //顶点坐标数据的初始化================end============================
    }

    //初始化shader
    void initShader()
    {
        //加载顶点着色器的脚本内容
        mVertexShader = ShaderUtil::loadFromAssetsFile("vertex_color.sh");
        //加载片元着色器的脚本内容
        mFragmentShader = ShaderUtil::loadFromAssetsFile("frag_color.sh");
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用  
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

    void drawSelf()
    {
        //制定使用某套着色器程序
        glUseProgram(mProgram);
        checkGLError("glUseProgram");

        //将最终变换矩阵传入着色器程序
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        checkGLError("glUniformMatrix4fv");

        // 将顶点位置数据传入渲染管线
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            vertices.data()
        );
        checkGLError("glVertexAttribPointer");

        //启用顶点位置数据
        glEnableVertexAttribArray(maPositionHandle);
        checkGLError("glDrawArrays");

        //绘制加载的物体
        glDrawArrays(GL_TRIANGLES, 0, vCount);
        checkGLError("glDrawArrays");
    }
protected:
private:
    int mProgram;//自定义渲染管线着色器程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用  
    std::string mVertexShader;//顶点着色器代码脚本      
    std::string mFragmentShader;//片元着色器代码脚本

    std::vector<float> vertices;
    int vCount = 0; //顶点数量
};
