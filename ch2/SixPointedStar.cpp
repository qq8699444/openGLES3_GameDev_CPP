#include <cmath>
#include "esUtil.h"
#include "SixPointedStar.h"
#include "MatrixState.h"

using namespace std;

const float UNIT_SIZE = 1;
const float Pi = 3.1415926;

static float toRadians(float angle)
{
    return Pi * angle / 180;
}

SixPointedStar::SixPointedStar(float r, float R, float z)
{
    esMatrixLoadIdentity(&matrix);
    initVertexData(R, r, z);
    initShader();
}

void SixPointedStar::initVertexData(float R, float r, float z)
{
    float tempAngle = 360.0 / 6;
    VertexList.clear();
    for (float angle = 0; angle < 360; angle += tempAngle)
    {
        //1.0
        VertexList.push_back(0);
        VertexList.push_back(0);
        VertexList.push_back(z);

        //1.1
        VertexList.push_back((float)(R*UNIT_SIZE*cos(toRadians(angle))));
        VertexList.push_back((float)(R*UNIT_SIZE*sin(toRadians(angle))));
        VertexList.push_back(z);

        ////1.2
        VertexList.push_back((float)(r*UNIT_SIZE*cos(toRadians(angle + tempAngle / 2))));
        VertexList.push_back((float)(r*UNIT_SIZE*sin(toRadians(angle + tempAngle / 2))));
        VertexList.push_back(z);

        //2.0
        VertexList.push_back(0);
        VertexList.push_back(0);
        VertexList.push_back(z);

        //2.1
        VertexList.push_back((float)(r*UNIT_SIZE*cos(toRadians(angle + tempAngle / 2))));
        VertexList.push_back((float)(r*UNIT_SIZE*sin(toRadians(angle + tempAngle / 2))));
        VertexList.push_back(z);

        //2.2
        VertexList.push_back((float)(R*UNIT_SIZE*cos(toRadians(angle + tempAngle))));
        VertexList.push_back((float)(R*UNIT_SIZE*sin(toRadians(angle + tempAngle))));
        VertexList.push_back(z);
    }

    vCount = VertexList.size()/3;
    for (int i = 0;i < vCount; i++)
    {
        if (i % 3 == 0) {//
            colorList.push_back(1);
            colorList.push_back(1);
            colorList.push_back(1);
            colorList.push_back(0);
        }
        else {//
            colorList.push_back(0.45f);
            colorList.push_back(0.75f);
            colorList.push_back(0.75f);
            colorList.push_back(0);
        }
    }

}

void SixPointedStar::initShader()
{
    mVertexShader = ShaderUtil::loadFromAssetsFile("vertex.sh");
    mFragmentShader = ShaderUtil::loadFromAssetsFile("frag.sh");

    mProgram = ShaderUtil::createProgram(mVertexShader, mFragmentShader);

    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    maColorHandle = glGetAttribLocation(mProgram, "aColor");
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
}

void SixPointedStar::drawSelf()
{
    glUseProgram(mProgram);

    //Matrix.setRotateM(mMMatrix, 0, 0, 0, 1, 0);
    esMatrixLoadIdentity(&matrix);

    //Matrix.translateM(mMMatrix, 0, 0, 0, 1);
    //esTranslate(&matrix, 0, 0, 0.9);

    //Matrix.rotateM(mMMatrix, 0, yAngle, 0, 1, 0);
    esRotate(&matrix, yAngle, 0, 1, 0);

    //Matrix.rotateM(mMMatrix, 0, xAngle, 1, 0, 0);
    esRotate(&matrix, xAngle, 1, 0, 0);

    auto retMat = MatrixState::getFinalMatrix();
    glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, false, 3 * sizeof(float), VertexList.data());
    glVertexAttribPointer(maColorHandle, 3, GL_FLOAT, false, 4 * sizeof(float), colorList.data());

    glUniformMatrix4fv(muMVPMatrixHandle, 1, false, (const GLfloat*)retMat);

    glEnableVertexAttribArray(maPositionHandle);
    glEnableVertexAttribArray(maColorHandle);

    glDrawArrays(GL_TRIANGLES, 0, vCount);
}