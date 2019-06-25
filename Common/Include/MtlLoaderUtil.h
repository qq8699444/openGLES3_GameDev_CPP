#pragma once
#include <string>
#include <map>
#include <memory>
#include <fstream>
struct Number3D
{

    float x, y, z;

};
struct MtlData {
    std::string name;

    Number3D Ka_Color;
    Number3D Kd_Color;
    Number3D Ks_Color;
    float ns;
    float alpha = 1.f;

    std::string Ka_Texture;
    std::string Kd_Texture;
    std::string Ks_ColorTexture;
    std::string Ns_Texture;
    std::string alphaTexture;
    std::string bumpTexture;
};

class MtlLoaderUtil
{
public:
    static std::map<std::string, std::shared_ptr<MtlData>> load(std::string mtlfname)
    {
        std::map<std::string, std::shared_ptr<MtlData>> mtlDatas;
        // If the file is not a material file return false
        if (mtlfname.substr(mtlfname.size() - 4, mtlfname.size()) != ".mtl")
            return mtlDatas;
        std::fstream f(mtlfname.c_str());

        // If the file is not found return false
        if (!f.is_open()) {
            std::cerr << "fail to open " << mtlfname << std::endl;
            return mtlDatas;
        }
            

        std::shared_ptr<MtlData> currMtlData;
        std::string curline;
        
        while (f.eof() != true)
        {
            std::string type;
            f >> type;

            if (type == NEWMTL)
            {
                std::string mtlname;
                f >> mtlname;

                if (mtlname == "")
                {
                    mtlname = "def";
                }

                currMtlData = std::make_shared<MtlData>();
                currMtlData->name = mtlname;
                mtlDatas.insert(std::pair<std::string, std::shared_ptr<MtlData>>(mtlname, currMtlData));
            }
            else if (type == KA)
            {
                currMtlData->Ka_Color = readColor(f);
            }
            else if (type == KD)
            {
                currMtlData->Kd_Color = readColor(f);
            }
            else if (type == KS)
            {
                currMtlData->Ks_Color = readColor(f);
            }
            else if (type == NS)
            {
                f >> currMtlData->ns;
            }
            else if (type == D)
            {
                f >> currMtlData->alpha;
            }
            else if (type == MAP_KA)
            {
                f >> currMtlData->Ka_Texture;
            }
            else if (type == MAP_KD)
            {
                f >> currMtlData->Kd_Texture;
            }
            else if (type == MAP_KS)
            {
                f >> currMtlData->Ks_ColorTexture;
            }
            else if (type == MAP_NS)
            {
                f >> currMtlData->Ns_Texture;
            }
            else if (type == MAP_D || type == MAP_TR)
            {
                f >> currMtlData->alphaTexture;
            }
            else if (type == MAP_BUMP)
            {
                f >> currMtlData->bumpTexture;
            }
        }
        f.close();
        return mtlDatas;
    }
protected:
private:
    static const std::string NEWMTL;    
    static const std::string KA;
    static const std::string KD;
    static const std::string KS;
    static const std::string NS;
    static const std::string D;
    static const std::string TR;
    static const std::string MAP_KA;
    static const std::string MAP_KD;
    static const std::string MAP_KS;
    static const std::string MAP_NS;
    static const std::string MAP_D;
    static const std::string MAP_TR;
    static const std::string MAP_BUMP;

    
    static Number3D readColor(std::fstream& f) {
        Number3D r;
        f >> r.x >> r.y >> r.z;
        return r;
    }
};
