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
        float UNIT_SIZE = 1.0f;
        //�����������ݵĳ�ʼ��================begin============================
        int n1 = 3;
        int n2 = 5;
        vCount = 2 * (n1 + n2 + 2) + 2;	//�����ܶ����� 
        float angdegBegin1 = 0;
        float angdegEnd1 = 90;
        float angdegSpan1 = (angdegEnd1 - angdegBegin1) / n1;

        float angdegBegin2 = 180;
        float angdegEnd2 = 270;
        float angdegSpan2 = (angdegEnd2 - angdegBegin2) / n2;

        mVertices = new float[vCount * 3];//����������������
        //�������ݳ�ʼ��
        int count = 0;
        for (float angdeg = angdegBegin1; angdeg <= angdegEnd1; angdeg += angdegSpan1) {
            double angrad = toRadians(angdeg);//��ǰ����	
            //��Բ�ϵĵ�
            mVertices[count++] = (float)(-0.6f*UNIT_SIZE*sin(angrad));//��������
            mVertices[count++] = (float)(0.6f*UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
            //СԲ�ϵĵ�
            mVertices[count++] = (float)(-UNIT_SIZE*sin(angrad));//��������
            mVertices[count++] = (float)(UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
        }

        //�ظ���һ�������ε����һ������
        mVertices[count++] = mVertices[count - 3];
        mVertices[count++] = mVertices[count - 3];
        mVertices[count++] = 0;

        for (float angdeg = angdegBegin2; angdeg <= angdegEnd2; angdeg += angdegSpan2) {
            double angrad = toRadians(angdeg);//��ǰ����
            //�ظ��ڶ��������εĵ�һ������
            if (angdeg == angdegBegin2) {
                mVertices[count++] = (float)(-0.6f*UNIT_SIZE*sin(angrad));//��������
                mVertices[count++] = (float)(0.6f*UNIT_SIZE*cos(angrad));
                mVertices[count++] = 0;
            }
            //��Բ�ϵĵ�
            mVertices[count++] = (float)(-0.6f*UNIT_SIZE*sin(angrad));//��������
            mVertices[count++] = (float)(0.6f*UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
            //СԲ�ϵĵ�
            mVertices[count++] = (float)(-UNIT_SIZE*sin(angrad));//��������
            mVertices[count++] = (float)(UNIT_SIZE*cos(angrad));
            mVertices[count++] = 0;
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
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vCount);
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
    int vCount = 0; //�������� 
};