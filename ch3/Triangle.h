#pragma  once
#include <string>
#include "ShaderUtil.h"
#include "MatrixState.h"

class Triangle
{
public:
    Triangle()
    {
        //调用初始化顶点数据的initVertexData方法
        initVertexData();
        //调用初始化着色器的intShader方法
        initShader();
    }

    void initVertexData()
    {
        vCount = 3;

    }


    void initShader()
    {
        //加载顶点着色器的脚本内容
        mVertexShader = ShaderUtil::loadFromAssetsFile("vertex.sh");
        //加载片元着色器的脚本内容
        mFragmentShader = ShaderUtil::loadFromAssetsFile("frag.sh");
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用  
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点颜色属性引用
        maColorHandle = glGetAttribLocation(mProgram, "aColor");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

    void drawSelf()
    {
        //指定使用某套shader程序
        glUseProgram(mProgram);
        //初始化变换矩阵
        //Matrix.setRotateM(mMMatrix, 0, 0, 0, 1, 0);
        //设置沿Z轴正向位移1
        //Matrix.translateM(mMMatrix, 0, 0, 0, 1);
        mMMatrix = glm::translate(glm::vec3(0.f, 0.f, 1.f));
        //设置绕x轴旋转
        //glm::rotate(mMMatrix, 0, xAngle, 1, 0, 0);

        //将变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, getFianlMatrix(mMMatrix));

        //
        testVert();

        //将顶点位置数据传送进渲染管线
        glVertexAttribPointer(
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            vertices
        );
        //将顶点颜色数据传送进渲染管线
        glVertexAttribPointer
        (
            maColorHandle,
            4,
            GL_FLOAT,
            false,
            4 * 4,
            colors
        );
        glEnableVertexAttribArray(maPositionHandle);//启用顶点位置数据
        glEnableVertexAttribArray(maColorHandle);//启用顶点着色数据  
        //绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }

    float* getFianlMatrix(glm::mat4 spec)
    {
        glm::mat4 tmp;
        //Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, spec, 0);
        tmp = mVMatrix * spec;

        //Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
        mMVPMatrix = mProjMatrix * tmp;
        return glm::value_ptr(mMVPMatrix);
    }

    void testVert()
    {
        for (int i = 0; i < vCount; i++)
        {
            glm::vec4   vert(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2], 1.0f);
            glm::vec4   ret = mMVPMatrix * vert;
            float* pt = glm::value_ptr(ret);
            printf("%f %f %f %f\n", pt[0], pt[1], pt[2], pt[3]);
        }
    }
protected:
private:
    int vCount = 3;

    float UNIT_SIZE = 0.2f;
    float vertices[9] = //顶点坐标数组
    {
        -4 * UNIT_SIZE, 0, 0,
        0, -4 * UNIT_SIZE, 0,
        4 * UNIT_SIZE, 4 * UNIT_SIZE, 0,
    };

    float colors[12] =//顶点颜色数组
    {
        1, 1, 1, 0,//白色	
        0, 0, 1, 0,//蓝
        0, 1, 0, 0//绿
    };

    //
    int mProgram;//自定义渲染管线程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用
    int maColorHandle; //顶点颜色属性引用
    std::string mVertexShader;//顶点着色器代码脚本
    std::string mFragmentShader;//片元着色器代码脚本

    //
public:
    glm::mat4 mProjMatrix;//4x4 投影矩阵
    glm::mat4 mVMatrix;//摄像机位置朝向的参数矩阵
    glm::mat4 mMVPMatrix;//最后起作用的总变换矩阵
    glm::mat4  mMMatrix;
};