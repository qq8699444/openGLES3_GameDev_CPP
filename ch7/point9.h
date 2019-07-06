#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

class Point
{
public:
    Point()
    {
        initVertexData();
        initShader();
    }
    ~Point() {}

    void drawSelf(int texId)
    {
        //指定使用某套shader程序
        glUseProgram(mProgram);
        
        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        checkGLError("glUniformMatrix4fv muMVPMatrixHandle");
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
        
        //允许顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);//启用顶点位置数据  
        checkGLError("glEnableVertexAttribArray");
        //绑定纹理
        //glEnable(GL_TEXTURE_2D);
        //checkGLError("glEnable GL_TEXTURE_2D_ARRAY");
        glActiveTexture(GL_TEXTURE0);//设置使用的纹理编号
        checkGLError("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D_ARRAY, texId);//绑定指定的纹理id
        checkGLError("glBindTexture");
        glUniform1i(sampler_loc, 0);
        //checkGLError("glUniform1i sampler_loc");

        //以三角形的方式填充
        glDrawArrays(GL_POINTS, 0, vCount);
        //glDrawElements(GL_POINTS, vCount, GL_BYTE, mindices.data());
        checkGLError("glDrawElements");
    }
protected:
    void initVertexData()
    {
        //顶点坐标数据的初始化================begin============================
        vCount = 9;
        const float UNIT_SIZE = 1.0f;
        float vertices[] = 
        {
            0, 0, 0,
            0, UNIT_SIZE * 2, 0,
            UNIT_SIZE, UNIT_SIZE / 2, 0,
            -UNIT_SIZE / 3, UNIT_SIZE, 0,
            -UNIT_SIZE*0.4f, -UNIT_SIZE*0.4f, 0,
            -UNIT_SIZE, -UNIT_SIZE, 0,
            UNIT_SIZE*0.2f, -UNIT_SIZE*0.7f, 0,
            UNIT_SIZE / 2, -UNIT_SIZE * 3 / 2, 0,
            -UNIT_SIZE * 4 / 5, -UNIT_SIZE * 3 / 2, 0,
        };
        mVertexs = std::vector<float>(vertices, vertices + sizeof(vertices)/sizeof(vertices[0]));

        for (int  i = 0; i < vCount;i++)
        {
            mindices.push_back(i);
        }
              

    }

    void initShader()
    {
        //加载顶点着色器的脚本内容
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex9.sh");
        //加载片元着色器的脚本内容
        std::string  mFragmentShader = ShaderUtil::loadFromAssetsFile("frag9.sh");
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");

        sampler_loc = glGetUniformLocation(mProgram, "sTexture");
    }

private:

    int mProgram;//自定义渲染管线程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用
    GLuint sampler_loc;

    std::vector<float> mVertexs;
    std::vector<BYTE> mindices;
    int vCount = 0;
};