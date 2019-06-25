#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "MtlLoaderUtil.h"



struct ObjData
{
public:
    std::string name;
    std::string materialname;

    std::vector<int> aVertices;
    std::vector<int> aTexCoords;
    std::vector<int> aNormals;

    void reset()
    {
        name = "";
        //mtlData.reset();
        aVertices.clear();
        aTexCoords.clear();
        aNormals.clear();
    }
};

class ObjLoaderUtil
{
public:
    static std::vector<ObjData> load(std::string objfile, 
                        std::vector<float>& vertices,
                        std::vector<float>& texCoords,
                        std::vector<float>& normals,
                        std::map<std::string, std::shared_ptr<MtlData>>& materials
    )
    {
        bool first = true;
        std::fstream f(objfile.c_str());
      
        ObjData currObjData;
        std::string currMaterialName = "";
        bool currObjHasFaces = false;

        std::vector<ObjData> objs;
        if (f)
        {
            while (f.eof() != true)
            {
                std::string tmp;
                f >> tmp;

                if (tmp == V)
                    readVertices(f, vertices);
                else if (tmp == VT)
                    readTexC(f, texCoords);
                else if (tmp == VN)
                    readNormal(f, normals);
                else if (tmp == FACE)
                {
                    readFaces(f,currObjData);
                    currObjHasFaces = true;
                }
                else if (tmp == MTLLIB) 
                {
                    materials = readMtl(f);
                }
                else if (tmp == USEMTL)
                {
                    readMaterialname(f, currObjData.materialname);
                    if (materials.find(currObjData.materialname) == materials.end())
                    {
                        std::cerr << "warning: " << currObjData.materialname << " not exist!" << std::endl;
                    }
                }
                else if (tmp == G || tmp == O)
                {
                    std::string objname;
                    f >> objname;

                    if (objname == "")
                    {
                        objname = "def";
                    }

                    if (currObjHasFaces)
                    {
                        objs.push_back(currObjData);
                        currObjData.reset();
                        currObjHasFaces = false;
                    }

                    currObjData.name = objname;

                }
                if (f.eof())
                    break;
            }
        }
        else
        {
            std::cerr << "fail to open " << objfile << std::endl;
        }

        //last obj
        if (currObjHasFaces)
        {
            objs.push_back(currObjData);
            currObjData.reset();
            currObjHasFaces = false;
        }
        f.close();
        return objs;
    }
protected:
private:

    static void readVertices(std::fstream &f, std::vector<float>& vertices)
    {
        float x, y, z;
        f >> x >> y >> z;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    static void readTexC(std::fstream& f, std::vector<float>& texCoords)
    {
        float u, v;
        f >> u >> v;
        texCoords.push_back(u);
        texCoords.push_back(v);
    }

    static void readNormal(std::fstream& f, std::vector<float>& normals)
    {
        float nx, ny, nz;
        f >> nx >> ny >> nz;
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
    }

    static void readFaces(std::fstream& f,ObjData& currobj)
    {
        for (int i = 0; i < 3; i++)
        {
            char s;
            int lPos, lTexC, lNormal;
            f >> lPos >> s >> lTexC >> s >> lNormal;

            currobj.aVertices.push_back(lPos-1);
            currobj.aTexCoords.push_back(lTexC-1);
            currobj.aNormals.push_back(lNormal-1);
        }
    }
    static std::map<std::string, std::shared_ptr<MtlData>> readMtl(std::fstream& f)
    {
        std::string materialPath;
        f >> materialPath;

        return MtlLoaderUtil::load(materialPath);
    }

    static void readMaterialname(std::fstream& f,std::string& materialname)
    {
        f >> materialname;
    }

    static void reset()
    {
        
    }

    static const std::string MTLLIB;
    static const std::string G;
    static const std::string O;
    static const std::string V;
    static const std::string VT;
    static const std::string VN;
    static const std::string USEMTL;
    static const std::string FACE;
};