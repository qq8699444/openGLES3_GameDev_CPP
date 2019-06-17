#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

using namespace std;

class loadUtil
{
public:
    loadUtil() {};
    ~loadUtil() {};

    static vector<string> split(string strtem, char a)
    {
        vector<string> strvec;

        string::size_type pos1, pos2;
        pos2 = strtem.find(a);
        pos1 = 0;
        while (string::npos != pos2)
        {
            if (pos2 != pos1)
                strvec.push_back(strtem.substr(pos1, pos2 - pos1));

            pos1 = pos2 + 1;
            pos2 = strtem.find(a, pos1);
        }
        strvec.push_back(strtem.substr(pos1));
        return strvec;
    }

    static LoadedObjectVertexOnly* loadFromFile(std::string filename)
    {
        //LoadedObjectVertexOnly lo = null;//加载后物体的引用

        std::vector<float> alv; //原始顶点坐标列表
        std::vector<float> vXYZ; //结果顶点坐标列表
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cerr << "failed to open " << filename.c_str() << endl;
            return false;
        }

        const int LINE_LENGTH = 1024;
        char line[LINE_LENGTH];

        while (fin.getline(line, LINE_LENGTH))
        {
            if (line[0] == '#')
            {
                continue;
            }
            auto strings = split(line, ' ');
            if (strings.empty())
            {
                continue;
            }

            if (strings[0] == "v")
            {
                float x = atof(strings[1].c_str()); alv.push_back(x);
                float y = atof(strings[2].c_str()); alv.push_back(y);
                float z = atof(strings[3].c_str()); alv.push_back(z);
            }
            else if (strings[0] == "f")
            {
                
                {
                    auto items = split(strings[1], '/');
                    int index = atoi(items[0].c_str()) - 1;

                    vXYZ.push_back(alv[3 * index + 0]);
                    vXYZ.push_back(alv[3 * index + 1]);
                    vXYZ.push_back(alv[3 * index + 2]);
                }

                {
                    auto items = split(strings[2], '/');
                    int index = atoi(items[0].c_str()) - 1;

                    vXYZ.push_back(alv[3 * index + 0]);
                    vXYZ.push_back(alv[3 * index + 1]);
                    vXYZ.push_back(alv[3 * index + 2]);
                }

                {
                    auto items = split(strings[3], '/');
                    int index = atoi(items[0].c_str()) - 1;

                    vXYZ.push_back(alv[3 * index + 0]);
                    vXYZ.push_back(alv[3 * index + 1]);
                    vXYZ.push_back(alv[3 * index + 2]);
                }
            }
        }
        
        auto lo = new LoadedObjectVertexOnly(vXYZ);//创建物体对象
        fin.close();
        return lo;
    }
private:

};
