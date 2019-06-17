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

    {
        MatrixState::pushMatrix();
        //Matrix.setRotateM(mMMatrix, 0, 0, 0, 1, 0);
        //esMatrixLoadIdentity(&matrix);
    

        //Matrix.translateM(mMMatrix, 0, 0, 0, 1);
        MatrixState::translate(0, 0, 1);

        //Matrix.rotateM(mMMatrix, 0, yAngle, 0, 1, 0);
        MatrixState::rotate(yAngle, 0, 1, 0);

        //Matrix.rotateM(mMMatrix, 0, xAngle, 1, 0, 0);
        MatrixState::rotate(xAngle, 1, 0, 0);

        auto retMat = MatrixState::getFinalMatrix();
        //testVert();
        glUniformMatrix4fv(muMVPMatrixHandle, 1, false, (const GLfloat*)retMat);

        MatrixState::popMatrix();
    }
    

    glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, false, 3 * sizeof(float), VertexList.data());
    glVertexAttribPointer(maColorHandle, 3, GL_FLOAT, false, 4 * sizeof(float), colorList.data());

    

    glEnableVertexAttribArray(maPositionHandle);
    glEnableVertexAttribArray(maColorHandle);

    glDrawArrays(GL_TRIANGLES, 0, vCount);
}

void SixPointedStar::testVert()
{
    for (int i = 0; i < vCount; i++)
    {
        glm::vec4   vert(VertexList[3 * i], VertexList[3 * i + 1], VertexList[3 * i + 2], 1.0f);
        glm::vec4   ret = MatrixState::mMVPMatrix * vert;
        float* pt = glm::value_ptr(ret);
        printf("%f %f %f %f\n", pt[0], pt[1], pt[2], pt[3]);
    }
}