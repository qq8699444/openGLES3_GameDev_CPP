#pragma  once
#include <cmath>
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

const float UNIT_SIZE = 1.0f;

class Cube
{
public:
    Cube(const std::string vertexfile = "vertex1.sh", const std::string fragfile = "frag1.sh")
    {
        vCount = 0;
        yAngle = xAngle = zAngle = 0;
        r = 0.8f;
        yAngle = 30;

        initVertexData();
        initShader(vertexfile, fragfile);
    }
    ~Cube()
    {

    }

    void initVertexData() {
        const float UNIT_SIZE = 1.0f;
        // 顶点坐标数据的初始化================begin============================
        //ArrayList<Float> alVertix = new ArrayList<Float>();// 存放顶点坐标的ArrayList
        float vertices[] = 
        {
            //立方体前面
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE, //0       	
                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//1 
                -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE, //2 

                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE, //0 
                -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE, //2        	
                UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//3 
                //后面  	
                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//0  
                UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//1 
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2

                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//0
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//3
                //左面	
                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0 
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//1
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2

                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0 
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2
                -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//3
                //右面
                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0  
                UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//1
                UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2

                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0  
                UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2
                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//3
                //上面     
                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0  
                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//1
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//2

                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0 
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//2
                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//3
                //下面  	
                UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//0  
                -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//1
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2

                UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//0 
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2
                UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//3
        };
        mVertices = std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0]));


        float normals[] = 
        {
            //前面
            0, 0, 1, 0, 0, 1, 0, 0, 1,
                0, 0, 1, 0, 0, 1, 0, 0, 1,
                //后面
                0, 0, -1, 0, 0, -1, 0, 0, -1,
                0, 0, -1, 0, 0, -1, 0, 0, -1,
                //左面
                -1, 0, 0, -1, 0, 0, -1, 0, 0,
                -1, 0, 0, -1, 0, 0, -1, 0, 0,
                //右面
                1, 0, 0, 1, 0, 0, 1, 0, 0,
                1, 0, 0, 1, 0, 0, 1, 0, 0,
                //上面
                0, 1, 0, 0, 1, 0, 0, 1, 0,
                0, 1, 0, 0, 1, 0, 0, 1, 0,
                //下面
                0, -1, 0, 0, -1, 0, 0, -1, 0,
                0, -1, 0, 0, -1, 0, 0, -1, 0,
        };
        mNormals = std::vector<float>(normals, normals + sizeof(normals) / sizeof(normals[0]));
        vCount = mVertices.size() / 3;// 顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标        
    }

    void initShader(const std::string vertexfile, const std::string fragfile)
    {
        // 加载顶点着色器的脚本内容
        mVertexShader = ShaderUtil::loadFromAssetsFile(vertexfile);
        // 加载片元着色器的脚本内容
        mFragmentShader = ShaderUtil::loadFromAssetsFile(fragfile);
        // 基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        // 获取程序中顶点位置属性引用id
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
       
        
        // 获取程序中总变换矩阵引用id
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
        muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");

        muRHandle = glGetUniformLocation(mProgram, "uR");
        maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
        //获取程序中光源位置引用
        maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");
        maCameraLocationHandle = glGetUniformLocation(mProgram, "uCamera");
    }

    void drawSelf()
    {
        MatrixState::rotate(xAngle, 1, 0, 0);//绕X轴转动
        MatrixState::rotate(yAngle, 0, 1, 0);//绕Y轴转动
        MatrixState::rotate(zAngle, 0, 0, 1);//绕Z轴转动

        //指定使用某套着色器程序
        glUseProgram(mProgram);
        //将最终变换矩阵传入渲染管线
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        //将顶点位置数据送入渲染管线
        glUniform1f(muRHandle, r * UNIT_SIZE);
        glUniform3fv(maLightLocationHandle, 1, MatrixState::getLightLocation());
        glUniform3fv(maCameraLocationHandle, 1, MatrixState::getCameraLocation());

        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertices.data()
        );
        
        glVertexAttribPointer
        (
            maNormalHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mNormals.data()
        );

        //启用顶点位置数据数组
        glEnableVertexAttribArray(maPositionHandle);
        glEnableVertexAttribArray(maNormalHandle);
        
        //采用三角形条带方式绘制
        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }
protected:
   
private:
    int mProgram;//自定义渲染管线着色器程序id
    int muMVPMatrixHandle;//总变换矩阵引用
    int muMMatrixHandle;//位置、旋转变换矩阵引用
    int maPositionHandle; //顶点位置属性引用  
    int muRHandle;//球的半径属性引用
    int maNormalHandle; //顶点法向量属性引用
    int maLightLocationHandle;//光源位置属性引用
    int maCameraLocationHandle; //camera 位置属性引用

    std::string mVertexShader;//顶点着色器代码脚本  
    std::string mFragmentShader;//片元着色器代码脚本

    std::vector<float>   mVertices;//顶点坐标数据缓冲
    std::vector<float>   mNormals;//顶点坐标数据缓冲

    int vCount ;
    float yAngle ;// 绕y轴旋转的角度
    float xAngle ;// 绕x轴旋转的角度
    float zAngle ;// 绕z轴旋转的角度
    float r;
};