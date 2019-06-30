#pragma  once
#include <cmath>
#include <string>
#include <vector>
#include "MatrixState.h"
#include "ShaderUtil.h"
#include "ColorRect.h"

class Cube
{
public:
    Cube()
    {
        cr = std::make_shared<ColorRect>();
    }

    void drawSelf()
    {
        //�ܻ���˼�룺ͨ����һ����ɫ������ת��λ��������ÿ�����λ��
        //�����������ÿ����

        //�����ֳ�
        MatrixState::pushMatrix();

        //����ǰС��
        MatrixState::pushMatrix();
        MatrixState::translate(0, 0, ColorRect::UNIT_SIZE);//�Ƶ���Ӧλ��
        cr->drawSelf();//���ƾ�����		
        MatrixState::popMatrix();

        //���ƺ�С��
        MatrixState::pushMatrix();
        MatrixState::translate(0, 0, -ColorRect::UNIT_SIZE);
        MatrixState::rotate(180, 0, 1, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //�����ϴ���
        MatrixState::pushMatrix();
        MatrixState::translate(0, ColorRect::UNIT_SIZE, 0);
        MatrixState::rotate(-90, 1, 0, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //�����´���
        MatrixState::pushMatrix();
        MatrixState::translate(0, -ColorRect::UNIT_SIZE, 0);
        MatrixState::rotate(90, 1, 0, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //���������
        MatrixState::pushMatrix();
        MatrixState::translate(ColorRect::UNIT_SIZE, 0, 0);
        MatrixState::rotate(-90, 1, 0, 0);
        MatrixState::rotate(90, 0, 1, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //�����Ҵ���
        MatrixState::pushMatrix();
        MatrixState::translate(-ColorRect::UNIT_SIZE, 0, 0);
        MatrixState::rotate(90, 1, 0, 0);
        MatrixState::rotate(-90, 0, 1, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //�ָ��ֳ�
        MatrixState::popMatrix();
    }
protected:
private:
    std::shared_ptr<ColorRect>  cr;
};