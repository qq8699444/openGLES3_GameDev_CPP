#pragma once
class ShaderUtil
{
public:
    ShaderUtil();
    ~ShaderUtil();
    static int loadShader(int shaderType, std::string source );
    static int createProgram(std::string vertexSource, std::string fragmentSource);
    static void checkGlError(std::string op);
    static std::string loadFromAssetsFile(std::string filename);
};

