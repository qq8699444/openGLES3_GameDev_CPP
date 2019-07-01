#pragma once

#include <atlimage.h>
class TextureUtil
{
public:
    static GLuint load(const std::string imgfile)
    {
        HRESULT hResult;
        CImage img;
        hResult = img.Load(imgfile.c_str());
        if (!FAILED(hResult))
        {
            GLuint textureId;
            glGenTextures
            (
                1,          //产生的纹理id的数量
                &textureId   //纹理id的数组

                );


            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            GLubyte* data = new GLubyte[img.GetWidth() * img.GetHeight() * 3];
            for (int y = 0; y < img.GetHeight(); y++)
            {
                for (int x = 0; x < img.GetWidth(); x++)
                {
                    auto pixel = img.GetPixel(x, y);
                    auto r = GetRValue(pixel);
                    auto g = GetGValue(pixel);
                    auto b = GetBValue(pixel);
                    data[(y * img.GetHeight() + x) * 3] = (GLubyte)r;
                    data[(y * img.GetHeight() + x) * 3 + 1] = (GLubyte)g;
                    data[(y * img.GetHeight() + x) * 3 + 2] = (GLubyte)b;
                }
            }
            glTexImage2D
            (
                GL_TEXTURE_2D, //纹理类型
                0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
                GL_RGB,
                img.GetWidth(),
                img.GetHeight(),
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data 			  //纹理图像
            );

            img.Detach();
            delete[] data;
            return textureId;
        }
        return -1;
    }
protected:
private:
};