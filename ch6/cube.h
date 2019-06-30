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
        // �����������ݵĳ�ʼ��================begin============================
        //ArrayList<Float> alVertix = new ArrayList<Float>();// ��Ŷ��������ArrayList
        float vertices[] = 
        {
            //������ǰ��
            UNIT_SIZE, UNIT_SIZE, UNIT_SIZE, //0       	
                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//1 
                -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE, //2 

                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE, //0 
                -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE, //2        	
                UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//3 
                //����  	
                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//0  
                UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//1 
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2

                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//0
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//3
                //����	
                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0 
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//1
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2

                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0 
                -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2
                -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//3
                //����
                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0  
                UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,//1
                UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2

                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0  
                UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,//2
                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//3
                //����     
                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0  
                UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//1
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//2

                UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//0 
                -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,//2
                -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,//3
                //����  	
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
            //ǰ��
            0, 0, 1, 0, 0, 1, 0, 0, 1,
                0, 0, 1, 0, 0, 1, 0, 0, 1,
                //����
                0, 0, -1, 0, 0, -1, 0, 0, -1,
                0, 0, -1, 0, 0, -1, 0, 0, -1,
                //����
                -1, 0, 0, -1, 0, 0, -1, 0, 0,
                -1, 0, 0, -1, 0, 0, -1, 0, 0,
                //����
                1, 0, 0, 1, 0, 0, 1, 0, 0,
                1, 0, 0, 1, 0, 0, 1, 0, 0,
                //����
                0, 1, 0, 0, 1, 0, 0, 1, 0,
                0, 1, 0, 0, 1, 0, 0, 1, 0,
                //����
                0, -1, 0, 0, -1, 0, 0, -1, 0,
                0, -1, 0, 0, -1, 0, 0, -1, 0,
        };
        mNormals = std::vector<float>(normals, normals + sizeof(normals) / sizeof(normals[0]));
        vCount = mVertices.size() / 3;// ���������Ϊ����ֵ������1/3����Ϊһ��������3������        
    }

    void initShader(const std::string vertexfile, const std::string fragfile)
    {
        // ���ض�����ɫ���Ľű�����
        mVertexShader = ShaderUtil::loadFromAssetsFile(vertexfile);
        // ����ƬԪ��ɫ���Ľű�����
        mFragmentShader = ShaderUtil::loadFromAssetsFile(fragfile);
        // ���ڶ�����ɫ����ƬԪ��ɫ����������
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        // ��ȡ�����ж���λ����������id
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
       
        
        // ��ȡ�������ܱ任��������id
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
        muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");

        muRHandle = glGetUniformLocation(mProgram, "uR");
        maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
        //��ȡ�����й�Դλ������
        maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");
        maCameraLocationHandle = glGetUniformLocation(mProgram, "uCamera");
    }

    void drawSelf()
    {
        MatrixState::rotate(xAngle, 1, 0, 0);//��X��ת��
        MatrixState::rotate(yAngle, 0, 1, 0);//��Y��ת��
        MatrixState::rotate(zAngle, 0, 0, 1);//��Z��ת��

        //ָ��ʹ��ĳ����ɫ������
        glUseProgram(mProgram);
        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        //������λ������������Ⱦ����
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

        //���ö���λ����������
        glEnableVertexAttribArray(maPositionHandle);
        glEnableVertexAttribArray(maNormalHandle);
        
        //����������������ʽ����
        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }
protected:
   
private:
    int mProgram;//�Զ�����Ⱦ������ɫ������id
    int muMVPMatrixHandle;//�ܱ任��������
    int muMMatrixHandle;//λ�á���ת�任��������
    int maPositionHandle; //����λ����������  
    int muRHandle;//��İ뾶��������
    int maNormalHandle; //���㷨������������
    int maLightLocationHandle;//��Դλ����������
    int maCameraLocationHandle; //camera λ����������

    std::string mVertexShader;//������ɫ������ű�  
    std::string mFragmentShader;//ƬԪ��ɫ������ű�

    std::vector<float>   mVertices;//�����������ݻ���
    std::vector<float>   mNormals;//�����������ݻ���

    int vCount ;
    float yAngle ;// ��y����ת�ĽǶ�
    float xAngle ;// ��x����ת�ĽǶ�
    float zAngle ;// ��z����ת�ĽǶ�
    float r;
};