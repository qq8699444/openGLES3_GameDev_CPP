// firstTriangle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <stdio.h>
#include "EGL/egl.h"
#include "GLES3/gl3.h"

GLuint LoadShader(GLenum type, const char *shaderSrc) {
	GLuint	shaderid;
	GLint compileStatus;

	shaderid = glCreateShader(type);
	if (shaderid == 0)
	{

	}

	glShaderSource(shaderid, 1, shaderSrc, NULL);
	glCompileShader(shaderid);
	glGetShaderiv(shaderid, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != 0)
	{
		GLint  loglen = 0;
		glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &loglen);
		if (loglen > 0)
		{
			char*log = new char[loglen];
			glGetShaderInfoLog(shaderid, loglen, NULL, log);
			fprintf(stderr, "compile erroe: %s\n", log);
			delete[]log;
			 
		}
		glDeleteShader(shaderid);
		return 0;
	}

	return shaderid;
}
int main()
{
    std::cout << "Hello World!\n"; 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
