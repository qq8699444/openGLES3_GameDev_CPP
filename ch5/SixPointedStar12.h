#include <string>
#include <vector>

#include "ShaderUtil.h"

class SixPointedStar
{
public:
    SixPointedStar(float r, float R, float z,float* c);
    void initVertexData(float R, float r, float z);
    void initShader();
    void drawSelf();
    void incxAngle() {
        xAngle += 10.f;
    }
    void decxAngle() {
        xAngle -= 10.f;
    }
    void incyAngle() {
        yAngle += 10.f;
    }
    void decyAngle() {
        yAngle -= 10.f;
    }

    void testVert();
protected:
private:
    std::string mVertexShader;
    std::string mFragmentShader;

    int mProgram;
    
    int maPositionHandle;
    int maColorHandle;
    int muMVPMatrixHandle;

    int vCount = 0;//Vertex count
    std::vector<float>  VertexList;
    std::vector<float>  colorList;

    ESMatrix matrix;

    float yAngle = 0;
    float xAngle = 0;
    float* color;
};