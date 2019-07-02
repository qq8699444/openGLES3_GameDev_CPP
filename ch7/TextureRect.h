#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

class TextureRect
{
public:
    TextureRect(float sRange, float tRange)
    {
        this->sRange = sRange;
        this->tRange = tRange;
        initVertexData();
        initShader();
    }
    ~TextureRect() {}

    void drawSelf(int texId)
    {
        //ָ��ʹ��ĳ��shader����
        glUseProgram(mProgram);

        //������Z������λ��1
        MatrixState::translate(0, 0, -1);

        //������y����ת
        MatrixState::rotate(yAngle, 0, 1, 0);
        //������z����ת
        MatrixState::rotate(zAngle, 0, 0, 1);
        //������x����ת
        MatrixState::rotate(xAngle, 1, 0, 0);
        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
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
        //�����������������ݴ��ͽ���Ⱦ����
        glVertexAttribPointer
        (
            maTexCoorHandle,
            2,
            GL_FLOAT,
            false,
            2 * 4,
            mTexCoors.data()
        );
        //������λ����������
        glEnableVertexAttribArray(maPositionHandle);//���ö���λ������  
        glEnableVertexAttribArray(maTexCoorHandle);//���ö���������������
        //������
        glActiveTexture(GL_TEXTURE0);//����ʹ�õ�������
        glBindTexture(GL_TEXTURE_2D, texId);//��ָ��������id
        //�������εķ�ʽ���
        glDrawArrays(GL_TRIANGLES, 0, vCount);
        checkGLError("glDrawArrays");
    }
protected:
    void initVertexData()
    {
        //�����������ݵĳ�ʼ��================begin============================
        
        const float UNIT_SIZE = 1.0f;
        float vertices[] =
        {
            -4 * UNIT_SIZE,4 * UNIT_SIZE,0,
            -4 * UNIT_SIZE,-4 * UNIT_SIZE,0,
            4 * UNIT_SIZE,-4 * UNIT_SIZE,0,

            4 * UNIT_SIZE,-4 * UNIT_SIZE,0,
            4 * UNIT_SIZE,4 * UNIT_SIZE,0,
            -4 * UNIT_SIZE,4 * UNIT_SIZE,0
        };
        vCount = sizeof(vertices) / sizeof(vertices[0]);
        mVertexs = std::vector<float>(vertices, vertices + sizeof(vertices)/sizeof(vertices[0]));


        //���������������ݵĳ�ʼ��================begin============================
        float texCoor[] =
        {
            0,0, 0,tRange, sRange,tRange,
                sRange,tRange, sRange,0, 0,0
        };
        mTexCoors = std::vector<float>(texCoor, texCoor +  sizeof(texCoor) / sizeof(texCoor[0]));

    }

    void initShader()
    {
        //���ض�����ɫ���Ľű�����
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex1.sh");
        //����ƬԪ��ɫ���Ľű�����
        std::string  mFragmentShader = ShaderUtil::loadFromAssetsFile("frag1.sh");
        //���ڶ�����ɫ����ƬԪ��ɫ����������
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //��ȡ�����ж���λ����������
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //��ȡ�����ж�������������������  
        maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
        //��ȡ�������ܱ任��������
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

private:

    int mProgram;//�Զ�����Ⱦ���߳���id
    int muMVPMatrixHandle;//�ܱ任��������
    int maPositionHandle; //����λ����������
    int maTexCoorHandle; //��������������������

    std::vector<float> mVertexs;
    std::vector<float> mTexCoors;

    int vCount = 0;
    float xAngle = 0;//��x����ת�ĽǶ�
    float yAngle = 0;//��y����ת�ĽǶ�
    float zAngle = 0;//��z����ת�ĽǶ�

    float sRange;//s�������귶Χ
    float tRange;//t�������귶Χ


};