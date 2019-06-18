#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

static const float UNIT_SIZE = 1.f;

class Cube
{
public:
    Cube()
    {
        initVertexData();
        initShader();
    }

    ~Cube()
    {

    }

    void initVertexData()
    {
        //顶点坐标数据的初始化================begin============================
        

    }

    void initShader()
    {
        //加载顶点着色器的脚本内容
        mVertexShader = ShaderUtil::loadFromAssetsFile("vertex.sh");
        //加载片元着色器的脚本内容
        mFragmentShader = ShaderUtil::loadFromAssetsFile("frag.sh");
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用id  
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点颜色属性引用id  
        maColorHandle = glGetAttribLocation(mProgram, "aColor");
        //获取程序中总变换矩阵引用id
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

    void drawSelf()
    {
        //制定使用某套shader程序
        glUseProgram(mProgram);
        //将最终变换矩阵传入shader程序
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //为画笔指定顶点位置数据
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            vertices
        );
        //为画笔指定顶点着色数据
        glVertexAttribPointer
        (
            maColorHandle,
            4,
            GL_FLOAT,
            false,
            4 * 4,
            colors
        );
        //允许顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);
        glEnableVertexAttribArray(maColorHandle);
        //绘制立方体         
        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }
protected:
private:
    //
    int vCount = 12 * 6;
    float vertices[12 * 6 * 3] =
    {
            //前面
            0, 0, UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            0, 0, UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            0, 0, UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            0, 0, UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            //后面
            0, 0, -UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            0, 0, -UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            0, 0, -UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            0, 0, -UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            //左面
            -UNIT_SIZE, 0, 0,
            -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, 0, 0,
            -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, 0, 0,
            -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            -UNIT_SIZE, 0, 0,
            -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            //右面
            UNIT_SIZE, 0, 0,
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            UNIT_SIZE, 0, 0,
            UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, 0, 0,
            UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, 0, 0,
            UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            //上面
            0, UNIT_SIZE, 0,
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            0, UNIT_SIZE, 0,
            UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            0, UNIT_SIZE, 0,
            -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
            -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            0, UNIT_SIZE, 0,
            -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
            //下面
            0, -UNIT_SIZE, 0,
            UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            0, -UNIT_SIZE, 0,
            -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
            -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            0, -UNIT_SIZE, 0,
            -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            0, -UNIT_SIZE, 0,
            UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
            UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
    };

    float colors[12 * 6 * 4] = {
        //前面        
        1, 1, 1, 0,//中间为白色
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 1, 1, 0,//中间为白色
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 1, 1, 0,//中间为白色
            1, 0, 0, 0,
            1, 0, 0, 0,
            1, 1, 1, 0,//中间为白色
            1, 0, 0, 0,
            1, 0, 0, 0,
            //后面
            1, 1, 1, 0,//中间为白色
            0, 0, 1, 0,
            0, 0, 1, 0,
            1, 1, 1, 0,//中间为白色
            0, 0, 1, 0,
            0, 0, 1, 0,
            1, 1, 1, 0,//中间为白色
            0, 0, 1, 0,
            0, 0, 1, 0,
            1, 1, 1, 0,//中间为白色
            0, 0, 1, 0,
            0, 0, 1, 0,
            //左面
            1, 1, 1, 0,//中间为白色
            1, 0, 1, 0,
            1, 0, 1, 0,
            1, 1, 1, 0,//中间为白色
            1, 0, 1, 0,
            1, 0, 1, 0,
            1, 1, 1, 0,//中间为白色
            1, 0, 1, 0,
            1, 0, 1, 0,
            1, 1, 1, 0,//中间为白色
            1, 0, 1, 0,
            1, 0, 1, 0,
            //右面
            1, 1, 1, 0,//中间为白色
            1, 1, 0, 0,
            1, 1, 0, 0,
            1, 1, 1, 0,//中间为白色
            1, 1, 0, 0,
            1, 1, 0, 0,
            1, 1, 1, 0,//中间为白色
            1, 1, 0, 0,
            1, 1, 0, 0,
            1, 1, 1, 0,//中间为白色
            1, 1, 0, 0,
            1, 1, 0, 0,
            //上面
            1, 1, 1, 0,//中间为白色
            0, 1, 0, 0,
            0, 1, 0, 0,
            1, 1, 1, 0,//中间为白色
            0, 1, 0, 0,
            0, 1, 0, 0,
            1, 1, 1, 0,//中间为白色
            0, 1, 0, 0,
            0, 1, 0, 0,
            1, 1, 1, 0,//中间为白色
            0, 1, 0, 0,
            0, 1, 0, 0,
            //下面
            1, 1, 1, 0,//中间为白色
            0, 1, 1, 0,
            0, 1, 1, 0,
            1, 1, 1, 0,//中间为白色
            0, 1, 1, 0,
            0, 1, 1, 0,
            1, 1, 1, 0,//中间为白色
            0, 1, 1, 0,
            0, 1, 1, 0,
            1, 1, 1, 0,//中间为白色
            0, 1, 1, 0,
            0, 1, 1, 0,
    };
    //
    int mProgram;//自定义渲染管线着色器程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用  
    int maColorHandle; //顶点颜色属性引用 
    std::string mVertexShader;//顶点着色器代码脚本  
    std::string mFragmentShader;//片元着色器代码脚本
};
