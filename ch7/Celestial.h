#pragma  once
#include <string>
#include <cmath>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

class Celestial
{
public:
    Celestial(float scale, float yAngle, int vCount)
    {
        UNIT_SIZE = 10.0f;
        this->scale = scale;
        this->yAngle = yAngle;
        this->vCount = vCount;

        initVertexData();//调用初始化顶点数据的initVertexData方法
        initShader();
    }
    ~Celestial() {}

    void drawSelf()
    {
        //指定使用某套shader程序
        glUseProgram(mProgram);

     
        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        glUniform1f(uPointSizeHandle, scale);
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
        
        glEnableVertexAttribArray(maPositionHandle);//启用顶点位置数据  

        //以三角形的方式填充
        glDrawArrays(GL_POINTS, 0, vCount);
        checkGLError("glDrawArrays");
    }
protected:
    void initVertexData() { 	
        //顶点坐标数据的初始化       
        
        for (int i = 0; i < vCount; i++) {
            //随机产生每个星星的xyz坐标
            float rand1 = (rand() % 256) / 256.0f;
            float rand2 = (rand() % 256) / 256.0f;
            double angleTempJD = glm::pi<float>() * 2 * rand1;
            double angleTempWD = glm::pi<float>()*(rand2 - 0.5f);
            mVertexs.push_back( (float)(UNIT_SIZE*cos(angleTempWD)*sin(angleTempJD)) );
            mVertexs.push_back( (float)(UNIT_SIZE*sin(angleTempWD)));
            mVertexs.push_back( (float)(UNIT_SIZE*cos(angleTempWD)*cos(angleTempJD)));
        }
        
    }

    void initShader()
    {
        //加载顶点着色器的脚本内容
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex_xk.sh");
        //加载片元着色器的脚本内容
        std::string  mFragmentShader = ShaderUtil::loadFromAssetsFile("frag_xk.sh");
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");

        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");

        uPointSizeHandle = glGetUniformLocation(mProgram, "uPointSize");
        
    }
private:
    float UNIT_SIZE;//天球半径

    int vCount = 0;//星星数量
    float yAngle;//天球绕Y轴旋转的角度
    float scale;//星星尺寸  
    std::vector<float>   mVertexs;

    int mProgram;//自定义渲染管线程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle; //顶点位置属性引用
    int uPointSizeHandle;
};