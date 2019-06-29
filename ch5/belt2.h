#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


class Belt
{
public:
    Belt()
    {
        initVertexData();
        initShader();
    }
    void initVertexData()
    {
        float UNIT_SIZE = 1.0f;
        //顶点坐标数据的初始化================begin============================
        int n1 = 3;
        int n2 = 5;
        vCount = 2 * (n1 + n2 + 2) + 2;	//计算总顶点数 
        float angdegBegin1 = 0;
        float angdegEnd1 = 90;
        float angdegSpan1 = (angdegEnd1 - angdegBegin1) / n1;

        float angdegBegin2 = 180;
        float angdegEnd2 = 270;
        float angdegSpan2 = (angdegEnd2 - angdegBegin2) / n2;

        mVertices = new float[vCount * 3];//顶点坐标数据数组
        //坐标数据初始化
        int count = 0;
        for (float angdeg = angdegBegin1; angdeg <= angdegEnd1; angdeg += angdegSpan1) {
            double angrad = toRadians(angdeg);//当前弧度	
            //大圆上的点
            mVertices[count++] = (float)(-0.6f*UNIT_SIZE*sin(angrad));//顶点坐标
            mVertices[count++] = (float)(0.6f*UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
            //小圆上的点
            mVertices[count++] = (float)(-UNIT_SIZE*sin(angrad));//顶点坐标
            mVertices[count++] = (float)(UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
        }

        //重复第一批三角形的最后一个顶点
        mVertices[count++] = mVertices[count - 3];
        mVertices[count++] = mVertices[count - 3];
        mVertices[count++] = 0;

        for (float angdeg = angdegBegin2; angdeg <= angdegEnd2; angdeg += angdegSpan2) {
            double angrad = toRadians(angdeg);//当前弧度
            //重复第二批三角形的第一个顶点
            if (angdeg == angdegBegin2) {
                mVertices[count++] = (float)(-0.6f*UNIT_SIZE*sin(angrad));//顶点坐标
                mVertices[count++] = (float)(0.6f*UNIT_SIZE*cos(angrad));
                mVertices[count++] = 0;
            }
            //大圆上的点
            mVertices[count++] = (float)(-0.6f*UNIT_SIZE*sin(angrad));//顶点坐标
            mVertices[count++] = (float)(0.6f*UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
            //小圆上的点
            mVertices[count++] = (float)(-UNIT_SIZE*sin(angrad));//顶点坐标
            mVertices[count++] = (float)(UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
        }
        

        //顶点颜色值数组，每个顶点4个色彩值RGBA
        count = 0;
        mColors = new float[vCount * 4];
        for (int i = 0; i < vCount * 4; i += 8) {
            mColors[count++] = 1;
            mColors[count++] = 1;
            mColors[count++] = 1;
            mColors[count++] = 0;

            mColors[count++] = 0;
            mColors[count++] = 1;
            mColors[count++] = 1;
            mColors[count++] = 0;
        }

       
    }

    void initShader()
    {
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

    void drawSelf()
    {
        //指定使用某套着色器程序
        glUseProgram(mProgram);
        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //将顶点位置数据送入渲染管线
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertices
        );
        //将顶点颜色数据送入渲染管线
        glVertexAttribPointer
        (
            maColorHandle,
            4,
            GL_FLOAT,
            false,
            4 * 4,
            mColors
        );
        //启用顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);
        //启用顶点颜色数据数组
        glEnableVertexAttribArray(maColorHandle);
        //采用三角形条带方式绘制
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vCount);
    }
protected:
private:
    int mProgram;//自定义渲染管线着色器程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用  
    int maColorHandle; //顶点颜色属性引用 
    std::string mVertexShader;//顶点着色器代码脚本  
    std::string mFragmentShader;//片元着色器代码脚本

    float*   mVertices;//顶点坐标数据缓冲
    float*   mColors;//顶点着色数据缓冲
    int vCount = 0; //顶点数量 
};