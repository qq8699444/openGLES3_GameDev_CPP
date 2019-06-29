#pragma  once
#include <cmath>
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


class  Circle
{
public:
    Circle()
    {
        initVertexData();
        initShader();
    }

    void initVertexData()
    {
        //�����������ݵĳ�ʼ��================begin============================
        int n = 8;
        vCount = n + 2;
        float UNIT_SIZE = 1.0f;

        float angdegSpan = 360.0f / n;
        mVertices = new float[vCount * 3];//����������������
        //�������ݳ�ʼ��
        int count = 0;
        //��һ�����������
        mVertices[count++] = 0;
        mVertices[count++] = 0;
        mVertices[count++] = 0;
        for (float angdeg = 0; ceil(angdeg) <= 360; angdeg += angdegSpan) {//ѭ�������������������
            double angrad = toRadians(angdeg);//��ǰ����			
            //��ǰ��
            mVertices[count++] = (float)(-UNIT_SIZE*sin(angrad));//����x����
            mVertices[count++] = (float)(UNIT_SIZE*cos(angrad));//����y����
            mVertices[count++] = 0;//����z����
        }
        
        BYTE _mIndex[] = {
                0,1,2,
                0,2,3,
                0,3,4,
                0,4,5,
                0,5,6,
                0,6,7,
                0,7,8,
                0,8,1
        };

        mIndex = std::vector<BYTE>(_mIndex, _mIndex + sizeof(_mIndex) / sizeof(_mIndex[0]));
        //������ɫֵ���飬ÿ������4��ɫ��ֵRGBA
        count = 0;
        mColors = new float[vCount * 4];
        //��һ���������ɫ:��ɫ
        mColors[count++] = 1;
        mColors[count++] = 1;
        mColors[count++] = 1;
        mColors[count++] = 0;
        //ʣ�ඥ�����ɫ:��ɫ
        for (int i = 4; i < vCount * 4; i += 4) {
            mColors[count++] = 0;
            mColors[count++] = 1;
            mColors[count++] = 0;
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

    void drawSelf(int start,int count)
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
        //glDrawArrays(GL_TRIANGLE_FAN, 0, vCount);
        //glDrawElements(GL_TRIANGLE_FAN, vCount, GL_UNSIGNED_BYTE, mIndex.data());
        glDrawRangeElements
        (
            GL_TRIANGLES,	//���Ʒ�ʽ	
            0, 						//��С��������ֵ
            8, 						//��󶥵�����ֵ
            count, 					//��������
            GL_UNSIGNED_BYTE, //��������
            mIndex.data() + start
        );
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
    std::vector<BYTE>    mIndex;
    int vCount = 0; //�������� 

    
};