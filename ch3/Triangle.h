#pragma  once
#include <string>
#include "ShaderUtil.h"
#include "MatrixState.h"

class Triangle
{
public:
    Triangle()
    {
        //���ó�ʼ���������ݵ�initVertexData����
        initVertexData();
        //���ó�ʼ����ɫ����intShader����
        initShader();
    }

    void initVertexData()
    {
        vCount = 3;

    }


    void initShader()
    {
        //���ض�����ɫ���Ľű�����
        mVertexShader = ShaderUtil::loadFromAssetsFile("vertex.sh");
        //����ƬԪ��ɫ���Ľű�����
        mFragmentShader = ShaderUtil::loadFromAssetsFile("frag.sh");
        //���ڶ�����ɫ����ƬԪ��ɫ����������
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //��ȡ�����ж���λ����������  
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //��ȡ�����ж�����ɫ��������
        maColorHandle = glGetAttribLocation(mProgram, "aColor");
        //��ȡ�������ܱ任��������
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

    void drawSelf()
    {
        //ָ��ʹ��ĳ��shader����
        glUseProgram(mProgram);
        //��ʼ���任����
        //Matrix.setRotateM(mMMatrix, 0, 0, 0, 1, 0);
        //������Z������λ��1
        //Matrix.translateM(mMMatrix, 0, 0, 0, 1);
        mMMatrix = glm::translate(glm::vec3(0.f, 0.f, 1.f));
        //������x����ת
        //glm::rotate(mMMatrix, 0, xAngle, 1, 0, 0);

        //���任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, getFianlMatrix(mMMatrix));

        //
        testVert();

        //������λ�����ݴ��ͽ���Ⱦ����
        glVertexAttribPointer(
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            vertices
        );
        //��������ɫ���ݴ��ͽ���Ⱦ����
        glVertexAttribPointer
        (
            maColorHandle,
            4,
            GL_FLOAT,
            false,
            4 * 4,
            colors
        );
        glEnableVertexAttribArray(maPositionHandle);//���ö���λ������
        glEnableVertexAttribArray(maColorHandle);//���ö�����ɫ����  
        //����������
        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }

    float* getFianlMatrix(glm::mat4 spec)
    {
        glm::mat4 tmp;
        //Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, spec, 0);
        tmp = mVMatrix * spec;

        //Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
        mMVPMatrix = mProjMatrix * tmp;
        return glm::value_ptr(mMVPMatrix);
    }

    void testVert()
    {
        for (int i = 0; i < vCount; i++)
        {
            glm::vec4   vert(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2], 1.0f);
            glm::vec4   ret = mMVPMatrix * vert;
            float* pt = glm::value_ptr(ret);
            printf("%f %f %f %f\n", pt[0], pt[1], pt[2], pt[3]);
        }
    }
protected:
private:
    int vCount = 3;

    float UNIT_SIZE = 0.2f;
    float vertices[9] = //������������
    {
        -4 * UNIT_SIZE, 0, 0,
        0, -4 * UNIT_SIZE, 0,
        4 * UNIT_SIZE, 4 * UNIT_SIZE, 0,
    };

    float colors[12] =//������ɫ����
    {
        1, 1, 1, 0,//��ɫ	
        0, 0, 1, 0,//��
        0, 1, 0, 0//��
    };

    //
    int mProgram;//�Զ�����Ⱦ���߳���id
    int muMVPMatrixHandle;//�ܱ任��������
    int maPositionHandle; //����λ����������
    int maColorHandle; //������ɫ��������
    std::string mVertexShader;//������ɫ������ű�
    std::string mFragmentShader;//ƬԪ��ɫ������ű�

    //
public:
    glm::mat4 mProjMatrix;//4x4 ͶӰ����
    glm::mat4 mVMatrix;//�����λ�ó���Ĳ�������
    glm::mat4 mMVPMatrix;//��������õ��ܱ任����
    glm::mat4  mMMatrix;
};