#pragma  once
#include <cmath>
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


class ColorRect
{
public:
    ColorRect(float unitSize, float* color)
    {
        initVertexData(unitSize,color);
        initShader();
    }

    void initVertexData(float unitSize, float* color)
    {
        
        //�����������ݵĳ�ʼ��================begin============================
        vCount = 6;
        float vertices[] = 
        {
            0, 0, 0,
                unitSize, unitSize, 0,
                -unitSize, unitSize, 0,
                -unitSize, -unitSize, 0,
                unitSize, -unitSize, 0,
                unitSize, unitSize, 0
        };

        mVertices = std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0]));
        

        //������ɫ���ݵĳ�ʼ��================begin============================
        float colors[] = 
        {
                color[0], color[1], color[2], color[3],
                color[0], color[1], color[2], color[3],
                color[0], color[1], color[2], color[3],
                color[0], color[1], color[2], color[3],
                color[0], color[1], color[2], color[3],
                color[0], color[1], color[2], color[3],
        };
        mColors = std::vector<float>(colors, colors + sizeof(colors) / sizeof(colors[0]));

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

        //muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    }

    void drawSelf()
    {
        //ָ��ʹ��ĳ����ɫ������
        glUseProgram(mProgram);
        checkGLError("glUseProgram");


        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState::getFinalMatrix());
        //��ƽ�ơ���ת�任��������Ⱦ����
        //glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState::getMMatrix());
        checkGLError("glUniformMatrix4fv");
        //������λ�����ݴ�����Ⱦ����
        glVertexAttribPointer
        (
            maPositionHandle,
            3,
            GL_FLOAT,
            false,
            3 * 4,
            mVertices.data()
        );
        //��������ɫ���ݴ�����Ⱦ����
        glVertexAttribPointer
        (
            maColorHandle,
            4,
            GL_FLOAT,
            false,
            4 * 4,
            mColors.data()
        );
        checkGLError("glVertexAttribPointer");


        //���ö���λ����������
        glEnableVertexAttribArray(maPositionHandle);
        //���ö�����ɫ��������
        glEnableVertexAttribArray(maColorHandle);
        checkGLError("glEnableVertexAttribArray");


        //������ɫ����
        glDrawArrays(GL_TRIANGLE_FAN, 0, vCount);
        checkGLError("glDrawArrays");
    }

    static const float UNIT_SIZE;
protected:
private:
    int mProgram;//�Զ�����Ⱦ������ɫ������id
    //int muMMatrixHandle;//λ�á���ת�任��������
    int muMVPMatrixHandle;//�ܱ任��������
    int maPositionHandle; //����λ����������  
    int maColorHandle; //������ɫ�������� 
    std::string mVertexShader;//������ɫ������ű�  
    std::string mFragmentShader;//ƬԪ��ɫ������ű�

    std::vector<float>   mVertices;//�����������ݻ���
    std::vector<float>   mColors;//������ɫ���ݻ���
    int vCount = 0; //�������� 
    
};

const float ColorRect::UNIT_SIZE = 1.0f;