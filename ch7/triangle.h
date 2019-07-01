#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

class Triangle
{
public:
    Triangle()
    {
        initVertexData();
        initShader();
    }
    ~Triangle() {}

    void drawSelf(int texId)
    {
        //指定使用某套shader程序
        glUseProgram(mProgram);

        //设置沿Z轴正向位移1
        MatrixState::translate(0, 0, -1);

        //设置绕y轴旋转
        MatrixState::rotate(yAngle, 0, 1, 0);
        //设置绕z轴旋转
        MatrixState::rotate(zAngle, 0, 0, 1);
        //设置绕x轴旋转
        MatrixState::rotate(xAngle, 1, 0, 0);
        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //将顶点位置数据传送进渲染管线
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertexs.data()
        );
        //将顶点纹理坐标数据传送进渲染管线
        glVertexAttribPointer
        (
            maTexCoorHandle,
            2,
            GL_FLOAT,
            false,
            2 * 4,
            mTexCoors.data()
        );
        //允许顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);//启用顶点位置数据  
        glEnableVertexAttribArray(maTexCoorHandle);//启用顶点纹理坐标数据
        //绑定纹理
        glActiveTexture(GL_TEXTURE0);//设置使用的纹理编号
        glBindTexture(GL_TEXTURE_2D, texId);//绑定指定的纹理id
        //以三角形的方式填充
        glDrawArrays(GL_TRIANGLES, 0, vCount);
        checkGLError("glDrawArrays");
    }
protected:
    void initVertexData()
    {
        //顶点坐标数据的初始化================begin============================
        vCount = 3;
        const float UNIT_SIZE = 1.0f;
        float vertices[] = 
        {
            0 * UNIT_SIZE,  1 * UNIT_SIZE,  0,
            -1 * UNIT_SIZE, -1 * UNIT_SIZE, 0,
            1 * UNIT_SIZE,  -1 * UNIT_SIZE, 0,
        };
        mVertexs = std::vector<float>(vertices, vertices + 3 * 3);


        //顶点纹理坐标数据的初始化================begin============================
        float texCoor[] = 
        {
            0.5f, 0,
                0, 1,
                1, 1
        };
        mTexCoors  = std::vector<float>(texCoor, texCoor + 3 * 2);

    }

    void initShader()
    {
        //加载顶点着色器的脚本内容
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex1.sh");
        //加载片元着色器的脚本内容
        std::string  mFragmentShader = ShaderUtil::loadFromAssetsFile("frag1.sh");
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点纹理坐标属性引用  
        maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

private:

    int mProgram;//自定义渲染管线程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用
    int maTexCoorHandle; //顶点纹理坐标属性引用

    std::vector<float> mVertexs;
    std::vector<float> mTexCoors;

    int vCount = 0;
    float xAngle = 0;//绕x轴旋转的角度
    float yAngle = 0;//绕y轴旋转的角度
    float zAngle = 0;//绕z轴旋转的角度

};