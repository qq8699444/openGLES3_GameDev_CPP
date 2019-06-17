#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "esUtil.h"
#include "ShaderUtil.h"

using namespace std;

ShaderUtil::ShaderUtil()
{
}


ShaderUtil::~ShaderUtil()
{
}


int ShaderUtil::loadShader(int shaderType, std::string source)
{
    GLuint shader;
    GLint compiled;
    shader = glCreateShader(shaderType);
    if (shader == 0)
    {
        return 0;
    }

    const GLchar* srccode = source.c_str();
    glShaderSource(shader, 1, &srccode, NULL);
    checkGlError("glShaderSource");

    glCompileShader(shader);
    checkGlError("glCompileShader");

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (compiled == 0)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 0)
        {
            char *infoLog = new char[infoLen+1];
            glGetShaderInfoLog(shader, infoLen+1, NULL, infoLog);

            std::cerr << "Could not compile shader " << shaderType << std::endl;
            std::cerr << "error: " << infoLog << std::endl;

            delete[]infoLog;

        }       
        
        glDeleteShader(shader);
        return 0;
    }


    return shader;
}


int ShaderUtil::createProgram(std::string vertexSource, std::string fragmentSource)
{
    int vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0)
    {
        return 0;
    }

    int pixelShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (pixelShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }

    int program = glCreateProgram();
    if (program != 0)
    {
        glAttachShader(program,vertexShader);
        checkGlError("glAttachShader");

        glAttachShader(program,pixelShader);
        checkGlError("glAttachShader");

        glLinkProgram(program);

        int linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == 0)
        {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 0)
            {
                char *infoLog = new char[infoLen + 1];
                glGetProgramInfoLog(program, infoLen + 1, NULL, infoLog);

                std::cerr << "Could not link program " <<  std::endl;
                std::cerr << "error: " << infoLog << std::endl;

                delete[]infoLog;

            }

            glDeleteProgram(program);
            return 0;
        }
    }
    return program;
}


void ShaderUtil::checkGlError(std::string op)
{
    int error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "ES30_ERROR " << op + ": glError " << error << std::endl;
        //throw new RuntimeException(op + ": glError " + error);
    }
}


std::string ShaderUtil::loadFromAssetsFile(std::string filename)
{
    std::ifstream in(filename);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    std::string str = tmp.str();
    return str;
}
