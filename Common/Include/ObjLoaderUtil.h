#pragma  once
#include <string>
#include <vector>
#include <fstream>
#include "MtlLoaderUtil.h"


struct ObjData
{
public:
    std::string name;
    MtlData mtlData;

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
    static std::vector<ObjData>  load(std::string objfile)
    {
        bool first = true;
        std::fstream f(objfile.c_str());
        std::string tmp;

        std::vector<float>  vertices;
        std::vector<float>  texCoords;
        std::vector<float>  normals;

        ObjData currObjData;
        std::string currMaterialName = "";
        bool currObjHasFaces = false;

        std::vector<ObjData>    objs;
        if (f)
        {
            while (f.eof() != true)
            {
                f >> tmp;

                if (tmp == V)
                    readVertices(f, vertices);
                if (tmp == VT)
                    readTexC(f, texCoords);
                if (tmp == VN)
                    readNormal(f, normals);
                if (tmp == FACE)
                {
                    readFaces(f,currObjData);
                    currObjHasFaces = true;
                }
                else if (tmp == MTLLIB)
                    readMtl(f);
                if (tmp == "g")
                    readGroup(f);
                else if (tmp == O)
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

    static void readTexC(std::fstream& f, std::vector<float>&  texCoords)
    {
        float u, v;
        f >> u >> v;
        texCoords.push_back(u);
        texCoords.push_back(v);
    }

    static void readNormal(std::fstream& f, std::vector<float>&  normals)
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
    static void readMtl(std::fstream& f)
    {
        std::string materialPath;
        f >> materialPath;

        
    }

    static void readGroup(std::fstream& f)
    {
        std::string group;
        f >> group;
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
