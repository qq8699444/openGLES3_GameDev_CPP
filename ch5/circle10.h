#pragma  once
#include <cmath>
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


class  Circle
{
public:
    Circle()
    {
        initVertexData();
        initShader();
    }

    void initVertexData()
    {
        //顶点坐标数据的初始化================begin============================
        int n = 8;
        vCount = n + 2;
        float UNIT_SIZE = 1.0f;

        float angdegSpan = 360.0f / n;
        mVertices = new float[vCount * 3];//顶点坐标数据数组
        //坐标数据初始化
        int count = 0;
        //第一个顶点的坐标
        mVertices[count++] = 0;
        mVertices[count++] = 0;
        mVertices[count++] = 0;
        for (float angdeg = 0; ceil(angdeg) <= 360; angdeg += angdegSpan) {//循环生成其他顶点的坐标
            double angrad = toRadians(angdeg);//当前弧度			
            //当前点
            mVertices[count++] = (float)(-UNIT_SIZE*sin(angrad));//顶点x坐标
            mVertices[count++] = (float)(UNIT_SIZE*cos(angrad));//顶点y坐标
            mVertices[count++] = 0;//顶点z坐标
        }
        
        BYTE _mIndex[] = {
                0,1,2,
                0,2,3,
                0,3,4,
                0,4,5,
                0,5,6,
                0,6,7,
                0,7,8,
                0,8,1
        };

        mIndex = std::vector<BYTE>(_mIndex, _mIndex + sizeof(_mIndex) / sizeof(_mIndex[0]));
        //顶点颜色值数组，每个顶点4个色彩值RGBA
        count = 0;
        mColors = new float[vCount * 4];
        //第一个顶点的颜色:白色
        mColors[count++] = 1;
        mColors[count++] = 1;
        mColors[count++] = 1;
        mColors[count++] = 0;
        //剩余顶点的颜色:绿色
        for (int i = 4; i < vCount * 4; i += 4) {
            mColors[count++] = 0;
            mColors[count++] = 1;
            mColors[count++] = 0;
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

    void drawSelf(int start,int count)
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
        //glDrawArrays(GL_TRIANGLE_FAN, 0, vCount);
        //glDrawElements(GL_TRIANGLE_FAN, vCount, GL_UNSIGNED_BYTE, mIndex.data());
        glDrawRangeElements
        (
            GL_TRIANGLES,	//绘制方式	
            0, 						//最小顶点索引值
            8, 						//最大顶点索引值
            count, 					//索引数量
            GL_UNSIGNED_BYTE, //数据类型
            mIndex.data() + start
        );
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
    std::vector<BYTE>    mIndex;
    int vCount = 0; //顶点数量 

    
};