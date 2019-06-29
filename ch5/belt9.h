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
        //�����������ݵĳ�ʼ��================begin============================
        int n = 6;
        vCount = 2 * (n + 1);
        float UNIT_SIZE = 1.0f;
        float angdegBegin = -90;
        float angdegEnd = 90;
        float angdegSpan = (angdegEnd - angdegBegin) / n;

        mVertices = new float[vCount * 3];//����������������
        //�������ݳ�ʼ��
        int count = 0;
        for (float angdeg = angdegBegin; angdeg <= angdegEnd; angdeg += angdegSpan) {
            double angrad = toRadians(angdeg);//��ǰ����	
            //��ǰ��
            mVertices[count++] = (float)(-0.6f*UNIT_SIZE*sin(angrad));//����x����
            mVertices[count++] = (float)(0.6f*UNIT_SIZE*cos(angrad));//����y����
            mVertices[count++] = 0;//����z����		
            //��ǰ��
            mVertices[count++] = (float)(-UNIT_SIZE*sin(angrad));//����x����
            mVertices[count++] = (float)(UNIT_SIZE*cos(angrad));//����y����
            mVertices[count++] = 0;//����z����
        }
        
        mIndex = new BYTE[vCount];
        for (int i = 0; i < vCount; i++) {
            mIndex[i] = (BYTE)i;
        }

        //������ɫֵ���飬ÿ������4��ɫ��ֵRGBA
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
        // ���ض�����ɫ���Ľű�����
        mVertexShader = ShaderUtil::loadFromAssetsFile("vertex.sh");
        // ����ƬԪ��ɫ���Ľű�����
        mFragmentShader = ShaderUtil::loadFromAssetsFile("frag.sh");
        // ���ڶ�����ɫ����ƬԪ��ɫ����������
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        // ��ȡ�����ж���λ����������id
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        // ��ȡ�����ж�����ɫ��������id
        maColorHandle = glGetAttribLocation(mProgram, "aColor");
        // ��ȡ�������ܱ任��������id
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    }

    void drawSelf()
    {
        //ָ��ʹ��ĳ����ɫ������
        glUseProgram(mProgram);
        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //������λ������������Ⱦ����
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertices
        );
        //��������ɫ����������Ⱦ����
        glVertexAttribPointer
        (
            maColorHandle,
            4,
            GL_FLOAT,
            false,
            4 * 4,
            mColors
        );
        //���ö���λ����������
        glEnableVertexAttribArray(maPositionHandle);
        //���ö�����ɫ��������
        glEnableVertexAttribArray(maColorHandle);
        //����������������ʽ����
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, vCount);
        glDrawElements(GL_TRIANGLE_STRIP, vCount, GL_UNSIGNED_BYTE,mIndex);
    }
protected:
private:
    int mProgram;//�Զ�����Ⱦ������ɫ������id
    int muMVPMatrixHandle;//�ܱ任��������
    int maPositionHandle; //����λ����������  
    int maColorHandle; //������ɫ�������� 
    std::string mVertexShader;//������ɫ������ű�  
    std::string mFragmentShader;//ƬԪ��ɫ������ű�

    float*   mVertices;//�����������ݻ���
    float*   mColors;//������ɫ���ݻ���
    BYTE*    mIndex;
    int vCount = 0; //�������� 
};