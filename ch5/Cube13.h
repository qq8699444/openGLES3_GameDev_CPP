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
        //总绘制思想：通过把一个颜色矩形旋转移位到立方体每个面的位置
        //绘制立方体的每个面

        //保护现场
        MatrixState::pushMatrix();

        //绘制前小面
        MatrixState::pushMatrix();
        MatrixState::translate(0, 0, ColorRect::UNIT_SIZE);//移到相应位置
        cr->drawSelf();//绘制矩形面		
        MatrixState::popMatrix();

        //绘制后小面
        MatrixState::pushMatrix();
        MatrixState::translate(0, 0, -ColorRect::UNIT_SIZE);
        MatrixState::rotate(180, 0, 1, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //绘制上大面
        MatrixState::pushMatrix();
        MatrixState::translate(0, ColorRect::UNIT_SIZE, 0);
        MatrixState::rotate(-90, 1, 0, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //绘制下大面
        MatrixState::pushMatrix();
        MatrixState::translate(0, -ColorRect::UNIT_SIZE, 0);
        MatrixState::rotate(90, 1, 0, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //绘制左大面
        MatrixState::pushMatrix();
        MatrixState::translate(ColorRect::UNIT_SIZE, 0, 0);
        MatrixState::rotate(-90, 1, 0, 0);
        MatrixState::rotate(90, 0, 1, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //绘制右大面
        MatrixState::pushMatrix();
        MatrixState::translate(-ColorRect::UNIT_SIZE, 0, 0);
        MatrixState::rotate(90, 1, 0, 0);
        MatrixState::rotate(-90, 0, 1, 0);
        cr->drawSelf();
        MatrixState::popMatrix();

        //恢复现场
        MatrixState::popMatrix();
    }
protected:
private:
    std::shared_ptr<ColorRect>  cr;
};