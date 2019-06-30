#pragma  once
#include <cmath>
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

const float UNIT_SIZE = 1.0f;

class Ball
{
public:
    Ball(const std::string vertexfile = "vertex1.sh", const std::string fragfile = "frag1.sh")
    {
        vCount = 0;
        yAngle = xAngle = zAngle = 0;
        r = 0.8f;

        initVertexData();
        initShader(vertexfile, fragfile);
    }
    ~Ball()
    {

    }

    void initVertexData() {
        // �����������ݵĳ�ʼ��================begin============================
        //ArrayList<Float> alVertix = new ArrayList<Float>();// ��Ŷ��������ArrayList
        const int angleSpan = 2;// ������е�λ�зֵĽǶ�
        for (int vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan)// ��ֱ����angleSpan��һ��
        {
            for (int hAngle = 0; hAngle <= 360; hAngle = hAngle + angleSpan)// ˮƽ����angleSpan��һ��
            {// ����������һ���ǶȺ�����Ӧ�Ĵ˵��������ϵ�����
                float x0 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle)) * cos(toRadians(hAngle)));
                float y0 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle)) * sin(toRadians(hAngle)));
                float z0 = (float)(r * UNIT_SIZE * sin(toRadians(vAngle)));

                float x1 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle)) * cos(toRadians(hAngle + angleSpan)));
                float y1 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle)) * sin(toRadians(hAngle + angleSpan)));
                float z1 = (float)(r * UNIT_SIZE * sin(toRadians(vAngle)));

                float x2 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle + angleSpan)));
                float y2 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle + angleSpan)));
                float z2 = (float)(r * UNIT_SIZE * sin(toRadians(vAngle + angleSpan)));

                float x3 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle)));
                float y3 = (float)(r * UNIT_SIZE * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle)));
                float z3 = (float)(r * UNIT_SIZE * sin(toRadians(vAngle + angleSpan)));

                // �����������XYZ��������Ŷ��������ArrayList
                mVertices.push_back(x1);
                mVertices.push_back(y1);
                mVertices.push_back(z1);
                mVertices.push_back(x3);
                mVertices.push_back(y3);
                mVertices.push_back(z3);
                mVertices.push_back(x0);
                mVertices.push_back(y0);
                mVertices.push_back(z0);

                mVertices.push_back(x1);
                mVertices.push_back(y1);
                mVertices.push_back(z1);
                mVertices.push_back(x2);
                mVertices.push_back(y2);
                mVertices.push_back(z2);
                mVertices.push_back(x3);
                mVertices.push_back(y3);
                mVertices.push_back(z3);
            }
        }

        mNormals = mVertices;
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