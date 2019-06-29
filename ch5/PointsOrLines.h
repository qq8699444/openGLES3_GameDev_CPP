#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


static const float UNIT_SIZE = 1.0f;
static const int CURR_DRAW_MODE = GL_LINE_LOOP;

class PointsOrLines
{
public:
    PointsOrLines()
    {
        initVertexData();
        initShader();
    }

    void initVertexData() {
        // 顶点坐标数据的初始化================begin============================
        vCount = 5;

        float _vertices[] = {
            0, 0, 0, 
            UNIT_SIZE, UNIT_SIZE, 0,
            -UNIT_SIZE, UNIT_SIZE, 0,
            -UNIT_SIZE, -UNIT_SIZE, 0,
            UNIT_SIZE, -UNIT_SIZE, 0, };

        for (auto v: _vertices)
        {
            vertices.push_back(v);
        }
        //vertices = _vertices;

        // 顶点颜色值数组，每个顶点4个色彩值RGBA
        float _colors[]  {
            1, 1, 0, 0,// 黄
                1, 1, 1, 0,// 白
                0, 1, 0, 0,// 绿
                1, 1, 1, 0,// 白
                1, 1, 0, 0,// 黄
        };
        for (auto c:_colors)
        {
            colors.push_back(c);
        }
    }

    void initShader() {
        // 加载顶点着色器的脚本内容
        mVertexShader = ShaderUtil::loadFromAssetsFile("vertex.sh");
        // 加载片元着色器的脚本内容
        mFragmentShader = ShaderUtil::loadFromAssetsFile("frag.sh");
        // 基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        // 获取程序中顶点位置属性引用id
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        // 获取程序中顶点颜色属性引用id
        maColorHandle = glGetAttribLocation(mProgram, "aColor");
        // 获取程序中总变换矩阵引用id
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }
    void drawSelf() {
        //指定使用某套着色器程序
        glUseProgram(mProgram);
        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false,
            MatrixState::getFinalMatrix());
        //将顶点位置数据送入渲染管线
        glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT,
            false, 3 * 4, vertices.data());
        //将顶点颜色数据送入渲染管线
        glVertexAttribPointer(maColorHandle, 4, GL_FLOAT, false,
            4 * 4, colors.data());
        //启用顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);
        //启用顶点颜色数据数组
        glEnableVertexAttribArray(maColorHandle);

        glLineWidth(10);//设置线的宽度
        //绘制点或线
        switch (CURR_DRAW_MODE) {
        case GL_POINTS:// GL_POINTS方式
            glDrawArrays(GL_POINTS, 0, vCount);
            break;
        case GL_LINES:// GL_LINES方式			
            glDrawArrays(GL_LINES, 0, vCount);
            break;
        case GL_LINE_STRIP:// GL_LINE_STRIP方式
            glDrawArrays(GL_LINE_STRIP, 0, vCount);
            break;
        case GL_LINE_LOOP:// GL_LINE_LOOP方式
            glDrawArrays(GL_LINE_LOOP, 0, vCount);
            break;
        }
    }
protected:
private:
    int mProgram;// 自定义渲染管线着色器程序id
    int muMVPMatrixHandle;// 总变换矩阵引用
    int maPositionHandle; // 顶点位置属性引用
    int maColorHandle; // 顶点颜色属性引用
    std::string mVertexShader;// 顶点着色器代码脚本
    std::string mFragmentShader;// 片元着色器代码脚本

    std::vector<float>  vertices;
    std::vector<float>  colors;
    int vCount;
};