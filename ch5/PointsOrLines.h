#pragma  once
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"


static const float UNIT_SIZE = 1.0f;
static const int CURR_DRAW_MODE = GL_LINE_LOOP;

class PointsOrLines
{
public:
    PointsOrLines()
    {
        initVertexData();
        initShader();
    }

    void initVertexData() {
        // �����������ݵĳ�ʼ��================begin============================
        vCount = 5;

        float _vertices[] = {
            0, 0, 0, 
            UNIT_SIZE, UNIT_SIZE, 0,
            -UNIT_SIZE, UNIT_SIZE, 0,
            -UNIT_SIZE, -UNIT_SIZE, 0,
            UNIT_SIZE, -UNIT_SIZE, 0, };

        for (auto v: _vertices)
        {
            vertices.push_back(v);
        }
        //vertices = _vertices;

        // ������ɫֵ���飬ÿ������4��ɫ��ֵRGBA
        float _colors[]  {
            1, 1, 0, 0,// ��
                1, 1, 1, 0,// ��
                0, 1, 0, 0,// ��
                1, 1, 1, 0,// ��
                1, 1, 0, 0,// ��
        };
        for (auto c:_colors)
        {
            colors.push_back(c);
        }
    }

    void initShader() {
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
    void drawSelf() {
        //ָ��ʹ��ĳ����ɫ������
        glUseProgram(mProgram);
        //�����ձ任��������Ⱦ����
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false,
            MatrixState::getFinalMatrix());
        //������λ������������Ⱦ����
        glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT,
            false, 3 * 4, vertices.data());
        //��������ɫ����������Ⱦ����
        glVertexAttribPointer(maColorHandle, 4, GL_FLOAT, false,
            4 * 4, colors.data());
        //���ö���λ����������
        glEnableVertexAttribArray(maPositionHandle);
        //���ö�����ɫ��������
        glEnableVertexAttribArray(maColorHandle);

        glLineWidth(10);//�����ߵĿ��
        //���Ƶ����
        switch (CURR_DRAW_MODE) {
        case GL_POINTS:// GL_POINTS��ʽ
            glDrawArrays(GL_POINTS, 0, vCount);
            break;
        case GL_LINES:// GL_LINES��ʽ			
            glDrawArrays(GL_LINES, 0, vCount);
            break;
        case GL_LINE_STRIP:// GL_LINE_STRIP��ʽ
            glDrawArrays(GL_LINE_STRIP, 0, vCount);
            break;
        case GL_LINE_LOOP:// GL_LINE_LOOP��ʽ
            glDrawArrays(GL_LINE_LOOP, 0, vCount);
            break;
        }
    }
protected:
private:
    int mProgram;// �Զ�����Ⱦ������ɫ������id
    int muMVPMatrixHandle;// �ܱ任��������
    int maPositionHandle; // ����λ����������
    int maColorHandle; // ������ɫ��������
    std::string mVertexShader;// ������ɫ������ű�
    std::string mFragmentShader;// ƬԪ��ɫ������ű�

    std::vector<float>  vertices;
    std::vector<float>  colors;
    int vCount;
};