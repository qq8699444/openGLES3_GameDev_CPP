#pragma  once
#include <string>
#include <cmath>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


class Earth
{
public:
    Earth(float r)
    {
        initVertexData(r);
        initShader();
        
    }
    ~Earth()
    {

    }

    void drawSelf(int texId, int texIdNight) {
        //指定使用某套着色器程序
        glUseProgram(mProgram);
        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //将位置、旋转变换矩阵传入渲染管线
        glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        //将摄像机位置传入渲染管线
        glUniform3fv(maCameraHandle, 1, MatrixState::getCameraLocation());
        //将光源位置传入渲染管线 
        glUniform3fv(maSunLightLocationHandle, 1, MatrixState::getLightLocation());
        glVertexAttribPointer(//将顶点位置数据送入渲染管线
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertexs.data()
        );
        glVertexAttribPointer(  //将顶点纹理数据送入渲染管线
            maTexCoorHandle,
            2,
            GL_FLOAT,
            false,
            2 * 4,
            mTexCoors.data()
        );
        glVertexAttribPointer   //将顶点法向量数据送入渲染管线
        (
            maNormalHandle,
            4,
            GL_FLOAT,
            false,
            3 * 4,
            mVertexs.data()
        );
        //启用顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);
        //启用顶点纹理数据数组
        glEnableVertexAttribArray(maTexCoorHandle);
        //启用顶点法向量数据数组
        glEnableVertexAttribArray(maNormalHandle);
        //绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);   //白天纹理  
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texIdNight);  //黑夜纹理          
        glUniform1i(uDayTexHandle, 0);//通过引用指定白天纹理
        glUniform1i(uNightTexHandle, 1);  //通过引用指定黑夜纹理        
        //以三角形方式执行绘制
        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }
protected:
    void initVertexData(float r) {
        //顶点坐标数据的初始化================begin============================    	
        const float UNIT_SIZE = 0.5f;
        
        const float angleSpan = 5;//将球进行单位切分的角度
        for (float vAngle = 90; vAngle > -90; vAngle = vAngle - angleSpan) {//垂直方向angleSpan度一份
            for (float hAngle = 360; hAngle > 0; hAngle = hAngle - angleSpan) {//水平方向angleSpan度一份
                //纵向横向各到一个角度后计算对应的此点在球面上的坐标
                double xozLength = r * UNIT_SIZE*cos(toRadians(vAngle));
                float x1 = (float)(xozLength*cos(toRadians(hAngle)));
                float z1 = (float)(xozLength*sin(toRadians(hAngle)));
                float y1 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle)));
                xozLength = r * UNIT_SIZE*cos(toRadians(vAngle - angleSpan));
                float x2 = (float)(xozLength*cos(toRadians(hAngle)));
                float z2 = (float)(xozLength*sin(toRadians(hAngle)));
                float y2 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle - angleSpan)));
                xozLength = r * UNIT_SIZE*cos(toRadians(vAngle - angleSpan));
                float x3 = (float)(xozLength*cos(toRadians(hAngle - angleSpan)));
                float z3 = (float)(xozLength*sin(toRadians(hAngle - angleSpan)));
                float y3 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle - angleSpan)));
                xozLength = r * UNIT_SIZE*cos(toRadians(vAngle));
                float x4 = (float)(xozLength*cos(toRadians(hAngle - angleSpan)));
                float z4 = (float)(xozLength*sin(toRadians(hAngle - angleSpan)));
                float y4 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle)));
                //构建第一三角形
                mVertexs.push_back(x1); mVertexs.push_back(y1); mVertexs.push_back(z1);
                mVertexs.push_back(x2); mVertexs.push_back(y2); mVertexs.push_back(z2);
                mVertexs.push_back(x4); mVertexs.push_back(y4); mVertexs.push_back(z4);
                //构建第二三角形
                mVertexs.push_back(x4); mVertexs.push_back(y4); mVertexs.push_back(z4);
                mVertexs.push_back(x2); mVertexs.push_back(y2); mVertexs.push_back(z2);
                mVertexs.push_back(x3); mVertexs.push_back(y3); mVertexs.push_back(z3);
            }
        }
        vCount = mVertexs.size() / 3;//顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
        //将alVertix中的坐标值转存到一个float数组中
        
        generateTexCoor(
            (int)(360 / angleSpan), //纹理图切分的列数
            (int)(180 / angleSpan)  //纹理图切分的行数
        );
    }

    void initShader() { //初始化着色器
        //加载顶点着色器的脚本内容       
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex_earth.sh");
        ShaderUtil::checkGlError("==ss==");
        //加载片元着色器的脚本内容
        std::string mFragmentShader = ShaderUtil::loadFromAssetsFile("frag_earth.sh");
        //基于顶点着色器与片元着色器创建程序
        ShaderUtil::checkGlError("==ss==");

        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点纹理属性引用   
        maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
        //获取程序中顶点法向量属性引用  
        maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
        //获取程序中摄像机位置引用
        maCameraHandle = glGetUniformLocation(mProgram, "uCamera");
        //获取程序中光源位置引用
        maSunLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocationSun");
        //获取白天、黑夜两个纹理引用
        uDayTexHandle = glGetUniformLocation(mProgram, "sTextureDay");
        uNightTexHandle = glGetUniformLocation(mProgram, "sTextureNight");
        //获取位置、旋转变换矩阵引用
        muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    }

    void generateTexCoor(int bw, int bh) {
        mTexCoors.resize(bw*bh * 6 * 2);
        float sizew = 1.0f / bw;//列数
        float sizeh = 1.0f / bh;//行数
        int c = 0;
        for (int i = 0; i < bh; i++) {
            for (int j = 0; j < bw; j++) {
                //每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
                float s = j * sizew;
                float t = i * sizeh;//得到i行j列小矩形的左上点的纹理坐标值
                mTexCoors[c++] = s;
                mTexCoors[c++] = t;//该矩形左上点纹理坐标值
                mTexCoors[c++] = s;
                mTexCoors[c++] = t + sizeh;//该矩形左下点纹理坐标值
                mTexCoors[c++] = s + sizew;
                mTexCoors[c++] = t;    		//该矩形右上点纹理坐标值	
                mTexCoors[c++] = s + sizew;
                mTexCoors[c++] = t;//该矩形右上点纹理坐标值
                mTexCoors[c++] = s;
                mTexCoors[c++] = t + sizeh;//该矩形左下点纹理坐标值
                mTexCoors[c++] = s + sizew;
                mTexCoors[c++] = t + sizeh;    //该矩形右下点纹理坐标值			
            }
        }

        
    }
private:
    int mProgram;//自定义渲染管线程序id 
    int muMVPMatrixHandle;//总变换矩阵引用   
    int muMMatrixHandle;//位置、旋转变换矩阵
    int maCameraHandle; //摄像机位置属性引用  
    int maPositionHandle; //顶点位置属性引用  
    int maNormalHandle; //顶点法向量属性引用 
    int maTexCoorHandle; //顶点纹理坐标属性引用 
    int maSunLightLocationHandle;//光源位置属性引用     
    int uDayTexHandle;//白天纹理属性引用
    int uNightTexHandle;//黑夜纹理属性引用 

    std::vector<float>    mVertexs;//顶点坐标数据缓冲
    std::vector<float>    mTexCoors;//顶点纹理坐标数据缓冲
    int vCount = 0;
};