#pragma once

#include <atlimage.h>
class TextureUtil
{
public:
    enum 
    {
        Stretch_Mode_None,
        Stretch_Mode_Repeat,
        Stretch_Mode_Clamp_to_Edge,
        Stretch_Mode_Mirror_Repeat,
    };
    static GLuint load(const std::string imgfile, int strtchmode = Stretch_Mode_None)
    {
        HRESULT hResult;
        CImage img;
        hResult = img.Load(imgfile.c_str());
        if (!FAILED(hResult))
        {
            GLuint textureId;
            glGenTextures
            (
                1,          //����������id������
                &textureId   //����id������

                );


            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if (strtchmode == Stretch_Mode_Repeat)
            {
                glTexParameterf(GL_TEXTURE_2D, //S��Ϊ�ظ����췽ʽ
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D, //T��Ϊ�ظ����췽ʽ
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else if (strtchmode == Stretch_Mode_Clamp_to_Edge)
            {
                glTexParameterf(GL_TEXTURE_2D, //S��Ϊ��ȡ���췽ʽ
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameterf(GL_TEXTURE_2D, //T��Ϊ��ȡ���췽ʽ
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            else if (strtchmode == Stretch_Mode_Mirror_Repeat)
            {
                glTexParameterf(GL_TEXTURE_2D, //S��Ϊ�����ظ����췽ʽ
                    GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameterf(GL_TEXTURE_2D, //T��Ϊ�����ظ����췽ʽ
                    GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            }
            else
            {

            }
            //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_GREEN);

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
                GL_TEXTURE_2D, //��������
                0, 					  //����Ĳ�Σ�0��ʾ����ͼ��㣬�������Ϊֱ����ͼ
                GL_RGB,
                img.GetWidth(),
                img.GetHeight(),
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data 			  //����ͼ��
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