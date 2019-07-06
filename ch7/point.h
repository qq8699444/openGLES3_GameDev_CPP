#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

class Point
{
public:
    Point()
    {
        initVertexData();
        initShader();
    }
    ~Point() {}

    void drawSelf(int texId)
    {
        //ָ��ʹ��ĳ��shader����
        glUseProgram(mProgram);
        
        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        checkGLError("glUniformMatrix4fv muMVPMatrixHandle");
        //������λ�����ݴ��ͽ���Ⱦ����
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertexs.data()
        );
        
        //������λ����������
        glEnableVertexAttribArray(maPositionHandle);//���ö���λ������  
        checkGLError("glEnableVertexAttribArray");
        //������
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);//����ʹ�õ�������
        glBindTexture(GL_TEXTURE_2D, texId);//��ָ��������id
        checkGLError("glBindTexture");

        //�������εķ�ʽ���
        glDrawArrays(GL_POINTS, 0, vCount);
        checkGLError("glDrawArrays");
    }
protected:
    void initVertexData()
    {
        //�����������ݵĳ�ʼ��================begin============================
        vCount = 9;
        const float UNIT_SIZE = 1.0f;
        float vertices[] = 
        {
            0, 0, 0,
            0, UNIT_SIZE * 2, 0,
            UNIT_SIZE, UNIT_SIZE / 2, 0,
            -UNIT_SIZE / 3, UNIT_SIZE, 0,
            -UNIT_SIZE*0.4f, -UNIT_SIZE*0.4f, 0,
            -UNIT_SIZE, -UNIT_SIZE, 0,
            UNIT_SIZE*0.2f, -UNIT_SIZE*0.7f, 0,
            UNIT_SIZE / 2, -UNIT_SIZE * 3 / 2, 0,
            -UNIT_SIZE * 4 / 5, -UNIT_SIZE * 3 / 2, 0,
        };
        mVertexs = std::vector<float>(vertices, vertices + sizeof(vertices)/sizeof(vertices[0]));

              

    }

    void initShader()
    {
        //���ض�����ɫ���Ľű�����
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex7.sh");
        //����ƬԪ��ɫ���Ľű�����
        std::string  mFragmentShader = ShaderUtil::loadFromAssetsFile("frag7.sh");
        //���ڶ�����ɫ����ƬԪ��ɫ����������
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //��ȡ�����ж���λ����������
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //��ȡ�������ܱ任��������
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

private:

    int mProgram;//�Զ�����Ⱦ���߳���id
    int muMVPMatrixHandle;//�ܱ任��������
    int maPositionHandle; //����λ����������

    std::vector<float> mVertexs;

    int vCount = 0;
};