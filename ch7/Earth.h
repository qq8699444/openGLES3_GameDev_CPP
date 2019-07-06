#pragma  once
#include <string>
#include <cmath>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


class Earth
{
public:
    Earth(float r)
    {
        initVertexData(r);
        initShader();
        
    }
    ~Earth()
    {

    }

    void drawSelf(int texId, int texIdNight) {
        //ָ��ʹ��ĳ����ɫ������
        glUseProgram(mProgram);
        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //��λ�á���ת�任��������Ⱦ����
        glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        //�������λ�ô�����Ⱦ����
        glUniform3fv(maCameraHandle, 1, MatrixState::getCameraLocation());
        //����Դλ�ô�����Ⱦ���� 
        glUniform3fv(maSunLightLocationHandle, 1, MatrixState::getLightLocation());
        glVertexAttribPointer(//������λ������������Ⱦ����
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertexs.data()
        );
        glVertexAttribPointer(  //��������������������Ⱦ����
            maTexCoorHandle,
            2,
            GL_FLOAT,
            false,
            2 * 4,
            mTexCoors.data()
        );
        glVertexAttribPointer   //�����㷨��������������Ⱦ����
        (
            maNormalHandle,
            4,
            GL_FLOAT,
            false,
            3 * 4,
            mVertexs.data()
        );
        //���ö���λ����������
        glEnableVertexAttribArray(maPositionHandle);
        //���ö���������������
        glEnableVertexAttribArray(maTexCoorHandle);
        //���ö��㷨������������
        glEnableVertexAttribArray(maNormalHandle);
        //������
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);   //��������  
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texIdNight);  //��ҹ����          
        glUniform1i(uDayTexHandle, 0);//ͨ������ָ����������
        glUniform1i(uNightTexHandle, 1);  //ͨ������ָ����ҹ����        
        //�������η�ʽִ�л���
        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }
protected:
    void initVertexData(float r) {
        //�����������ݵĳ�ʼ��================begin============================    	
        const float UNIT_SIZE = 0.5f;
        
        const float angleSpan = 5;//������е�λ�зֵĽǶ�
        for (float vAngle = 90; vAngle > -90; vAngle = vAngle - angleSpan) {//��ֱ����angleSpan��һ��
            for (float hAngle = 360; hAngle > 0; hAngle = hAngle - angleSpan) {//ˮƽ����angleSpan��һ��
                //����������һ���ǶȺ�����Ӧ�Ĵ˵��������ϵ�����
                double xozLength = r * UNIT_SIZE*cos(toRadians(vAngle));
                float x1 = (float)(xozLength*cos(toRadians(hAngle)));
                float z1 = (float)(xozLength*sin(toRadians(hAngle)));
                float y1 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle)));
                xozLength = r * UNIT_SIZE*cos(toRadians(vAngle - angleSpan));
                float x2 = (float)(xozLength*cos(toRadians(hAngle)));
                float z2 = (float)(xozLength*sin(toRadians(hAngle)));
                float y2 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle - angleSpan)));
                xozLength = r * UNIT_SIZE*cos(toRadians(vAngle - angleSpan));
                float x3 = (float)(xozLength*cos(toRadians(hAngle - angleSpan)));
                float z3 = (float)(xozLength*sin(toRadians(hAngle - angleSpan)));
                float y3 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle - angleSpan)));
                xozLength = r * UNIT_SIZE*cos(toRadians(vAngle));
                float x4 = (float)(xozLength*cos(toRadians(hAngle - angleSpan)));
                float z4 = (float)(xozLength*sin(toRadians(hAngle - angleSpan)));
                float y4 = (float)(r*UNIT_SIZE*sin(toRadians(vAngle)));
                //������һ������
                mVertexs.push_back(x1); mVertexs.push_back(y1); mVertexs.push_back(z1);
                mVertexs.push_back(x2); mVertexs.push_back(y2); mVertexs.push_back(z2);
                mVertexs.push_back(x4); mVertexs.push_back(y4); mVertexs.push_back(z4);
                //�����ڶ�������
                mVertexs.push_back(x4); mVertexs.push_back(y4); mVertexs.push_back(z4);
                mVertexs.push_back(x2); mVertexs.push_back(y2); mVertexs.push_back(z2);
                mVertexs.push_back(x3); mVertexs.push_back(y3); mVertexs.push_back(z3);
            }
        }
        vCount = mVertexs.size() / 3;//���������Ϊ����ֵ������1/3����Ϊһ��������3������
        //��alVertix�е�����ֵת�浽һ��float������
        
        generateTexCoor(
            (int)(360 / angleSpan), //����ͼ�зֵ�����
            (int)(180 / angleSpan)  //����ͼ�зֵ�����
        );
    }

    void initShader() { //��ʼ����ɫ��
        //���ض�����ɫ���Ľű�����       
        std::string mVertexShader = ShaderUtil::loadFromAssetsFile("vertex_earth.sh");
        ShaderUtil::checkGlError("==ss==");
        //����ƬԪ��ɫ���Ľű�����
        std::string mFragmentShader = ShaderUtil::loadFromAssetsFile("frag_earth.sh");
        //���ڶ�����ɫ����ƬԪ��ɫ����������
        ShaderUtil::checkGlError("==ss==");

        mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);
        //��ȡ�����ж���λ����������
        maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        //��ȡ�����ж���������������   
        maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
        //��ȡ�����ж��㷨������������  
        maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
        //��ȡ�������ܱ任��������
        muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
        //��ȡ�����������λ������
        maCameraHandle = glGetUniformLocation(mProgram, "uCamera");
        //��ȡ�����й�Դλ������
        maSunLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocationSun");
        //��ȡ���졢��ҹ������������
        uDayTexHandle = glGetUniformLocation(mProgram, "sTextureDay");
        uNightTexHandle = glGetUniformLocation(mProgram, "sTextureNight");
        //��ȡλ�á���ת�任��������
        muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    }

    void generateTexCoor(int bw, int bh) {
        mTexCoors.resize(bw*bh * 6 * 2);
        float sizew = 1.0f / bw;//����
        float sizeh = 1.0f / bh;//����
        int c = 0;
        for (int i = 0; i < bh; i++) {
            for (int j = 0; j < bw; j++) {
                //ÿ����һ�����Σ������������ι��ɣ��������㣬12����������
                float s = j * sizew;
                float t = i * sizeh;//�õ�i��j��С���ε����ϵ����������ֵ
                mTexCoors[c++] = s;
                mTexCoors[c++] = t;//�þ������ϵ���������ֵ
                mTexCoors[c++] = s;
                mTexCoors[c++] = t + sizeh;//�þ������µ���������ֵ
                mTexCoors[c++] = s + sizew;
                mTexCoors[c++] = t;    		//�þ������ϵ���������ֵ	
                mTexCoors[c++] = s + sizew;
                mTexCoors[c++] = t;//�þ������ϵ���������ֵ
                mTexCoors[c++] = s;
                mTexCoors[c++] = t + sizeh;//�þ������µ���������ֵ
                mTexCoors[c++] = s + sizew;
                mTexCoors[c++] = t + sizeh;    //�þ������µ���������ֵ			
            }
        }

        
    }
private:
    int mProgram;//�Զ�����Ⱦ���߳���id 
    int muMVPMatrixHandle;//�ܱ任��������   
    int muMMatrixHandle;//λ�á���ת�任����
    int maCameraHandle; //�����λ����������  
    int maPositionHandle; //����λ����������  
    int maNormalHandle; //���㷨������������ 
    int maTexCoorHandle; //�������������������� 
    int maSunLightLocationHandle;//��Դλ����������     
    int uDayTexHandle;//����������������
    int uNightTexHandle;//��ҹ������������ 

    std::vector<float>    mVertexs;//�����������ݻ���
    std::vector<float>    mTexCoors;//���������������ݻ���
    int vCount = 0;
};