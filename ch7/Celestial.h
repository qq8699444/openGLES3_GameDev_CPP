#pragma  once
#include <string>
#include <cmath>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"

class Celestial
{
public:
    Celestial(float scale, float yAngle, int vCount)
    {
        UNIT_SIZE = 10.0f;
        this->scale = scale;
        this->yAngle = yAngle;
        this->vCount = vCount;

        initVertexData();//���ó�ʼ���������ݵ�initVertexData����
        initShader();
    }
    ~Celestial() {}

    void drawSelf()
    {
        //ָ��ʹ��ĳ��shader����
        glUseProgram(mProgram);

     
        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        glUniform1f(uPointSizeHandle, scale);
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
        
        glEnableVertexAttribArray(maPositionHandle);//���ö���λ������  

        //�������εķ�ʽ���
        glDrawArrays(GL_POINTS, 0, vCount);
        checkGLError("glDrawArrays");
    }
protected:
    void initVertexData() { 	
        //�����������ݵĳ�ʼ��       
        
        for (int i = 0; i < vCount; i++) {
            //�������ÿ�����ǵ�xyz����
            float rand1 = (rand() % 256) / 256.0f;
            float rand2 = (rand() % 256) / 256.0f;
            double angleTempJD = glm::pi<float>() * 2 * rand1;
            double angleTempWD = glm::pi<float>()*(rand2 - 0.5f);
            mVertexs.push_back( (float)(UNIT_SIZE*cos(angleTempWD)*sin(angleTempJD)) );
            mVertexs.push_back( (float)(UNIT_SIZE*sin(angleTempWD)));
            mVertexs.push_back( (float)(UNIT_SIZE*cos(angleTempWD)*cos(angleTempJD)));
        }
        
    }

    void initShader()
    {
        //���ض�����ɫ���Ľű�����
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex_xk.sh");
        //����ƬԪ��ɫ���Ľű�����
        std::string  mFragmentShader = ShaderUtil::loadFromAssetsFile("frag_xk.sh");
        //���ڶ�����ɫ����ƬԪ��ɫ����������
        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //��ȡ�����ж���λ����������
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");

        //��ȡ�������ܱ任��������
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");

        uPointSizeHandle = glGetUniformLocation(mProgram, "uPointSize");
        
    }
private:
    float UNIT_SIZE;//����뾶

    int vCount = 0;//��������
    float yAngle;//������Y����ת�ĽǶ�
    float scale;//���ǳߴ�  
    std::vector<float>   mVertexs;

    int mProgram;//�Զ�����Ⱦ���߳���id
    int muMVPMatrixHandle;//�ܱ任��������
    int maPositionHandle; //����λ����������
    int uPointSizeHandle;
};