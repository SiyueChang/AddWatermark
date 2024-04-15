// trans.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <vector>
#define CHARLENGTH 1024
#define SYMBOL ";"
using namespace std;

struct polarPoint
{
    float deep, radius, angle;
};
struct rectangularPoint
{
    float x, y, z, angle, radius;
    int id;
};

struct pointFace
{
    int one, two, three, four;
};
template <class Type>
Type stringToNum(const std::string& str)
{
    std::istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

int findGroupElementByAngle(rectangularPoint point)
{
    int id = point.id - 72;
    //for (auto i : zGroupPrevious)
    //{
    //    if (i.angle == angle)
    //    {
    //        id = i.id;
    //    }
    //}
    return id;
}
//void fullTopOrBottomHole(vector<rectangularPoint> zGroup, vector<pointFace>& faceGroup,vector<rectangularPoint> vecPoint, float z)
//{
//    int idPlus;
//    if (zGroup.size() < 5)
//    {
//        faceGroup += "f";
//        for (auto i : zGroup)
//        {
//            faceGroup += " " + i.id;
//        }
//        faceGroup += "\n";
//    }
//    else
//    {
//        rectangularPoint rectangular_point;
//        rectangular_point.x = 0;
//        rectangular_point.y = 0;
//        rectangular_point.z = z;
//        rectangular_point.id = vecPoint.size() + 1;
//        vecPoint.push_back(rectangular_point);
//        for (auto i : zGroup)
//        {
//            faceGroup += "f";
//            faceGroup += " " + rectangular_point.id;
//            faceGroup += " " + i.id;
//            if ((i.id + 1) < zGroup.size())
//            {
//                idPlus = i.id + 1;
//            }
//            else
//            {
//                idPlus = zGroup.front().id;
//            }
//            faceGroup += " " + idPlus;
//            faceGroup += "\n";
//        }
//    }
//}
void setGroup(vector<rectangularPoint>& vecPoint, vector<pointFace>& faceGroup)
{
    pointFace vecFace;
    vector<rectangularPoint> zGroup, zGroupPrevious;
    float zId = vecPoint.front().z;
    for(int i = 0;i < vecPoint.size(); ++i)
    {
        if (zId == vecPoint[i].z)
        {
            zGroup.push_back(vecPoint[i]);
        }
        else
        {
            //if (zGroupPrevious.empty() && zGroup.size() > 2)//
            //{
            //    fullTopOrBottomHole(zGroup, faceGroup, vecPoint, vecPoint[i].z);
            //}
            zId = vecPoint[i].z;
            zGroupPrevious.clear();
            zGroupPrevious.swap(zGroup); // copy and clean zGroup
            zGroup.clear();
            zGroup.push_back(vecPoint[i]);
        }
        if (!zGroupPrevious.empty() && zGroup.size() > 1)
        {
            vecFace.one = vecPoint[i - 1].id;
            vecFace.two = vecPoint[i].id;
            vecFace.three = findGroupElementByAngle( vecPoint[i] );
            vecFace.four = findGroupElementByAngle( vecPoint[i - 1] );
            faceGroup.push_back(vecFace);
            if (vecPoint[i].id % 72 == 0)
            {
                vecFace.one = vecPoint[i].id;
                vecFace.two = zGroup.front().id;
                vecFace.three = findGroupElementByAngle(zGroup.front());
                vecFace.four = findGroupElementByAngle((vecPoint[i]));
                
                faceGroup.push_back(vecFace);
            }
        }
    }
    //if (!zGroupPrevious.empty() && !zGroup.empty()  && zGroup.size() > 2)
    //{
    //    fullTopOrBottomHole(zGroup, faceGroup, vecPoint, zGroup.back()->z);
    //}
}
vector<rectangularPoint> openFile(std::string filePath)
{
    std::ifstream inFile;
    std::string buf;
    int index = 0,id = 1;
    bool isData = false;
    polarPoint polar_point;
    rectangularPoint rectangular_point;
    vector<rectangularPoint> vecPoint;
    inFile.open(filePath, std::ios::in);
    if (inFile) 
    { //条件成立，则说明文件打开成功
        char buff[CHARLENGTH];
        while (inFile.getline(buff, CHARLENGTH))
        {
            string str = buff;
            if (str == "#")
            {
                isData = true;
                continue;
            }
            if (str == ">")
            {
                isData = false;
                break;
            }
            if (isData)
            {
                int i = index % 3;
                if (i == 0)
                {
                    polar_point.deep = stringToNum<float>(str);
                }
                else if (i == 1)
                {
                    polar_point.radius = stringToNum<float>(str);
                }
                else
                {
                    polar_point.angle = stringToNum<float>(str);
                    rectangular_point.x = polar_point.radius * cos(polar_point.angle*(3.1415926/180));
                    rectangular_point.y = polar_point.radius * sin(polar_point.angle*(3.1415926/180));
                    rectangular_point.z = polar_point.deep;
                    rectangular_point.angle = polar_point.angle;
                    rectangular_point.radius = polar_point.radius;
                    rectangular_point.id = id;
                    vecPoint.push_back(rectangular_point);
                    id++;
                }
                index++;
            }
        }
        inFile.close();
    }
    else
    {
        std::cerr << "Error: " << filePath << std::endl;
    }
    return vecPoint;
}
void outFile(std::string filePath, vector<rectangularPoint> vectorOutPoint)
{
    ofstream fout;           //创建ofstream
    fout << fixed;
    fout.open(filePath, ios::out);
    float zeroX = 0, zeroY = 0, zeroZ = 0;
    for (int i = 0; i < vectorOutPoint.size(); i++)
    {
   //     cout << vectorOutPoint[i].x << SYMBOL << vectorOutPoint[i].y << SYMBOL << vectorOutPoint[i].z << SYMBOL << zeroX - vectorOutPoint[i].x << SYMBOL << zeroY - vectorOutPoint[i].y << SYMBOL << zeroZ - vectorOutPoint[i].z << SYMBOL << vectorOutPoint[i].id << SYMBOL << vectorOutPoint[i].angle << endl;
        fout << vectorOutPoint[i].x << SYMBOL << vectorOutPoint[i].y << SYMBOL << vectorOutPoint[i].z << SYMBOL << vectorOutPoint[i].angle << SYMBOL << vectorOutPoint[i].radius << SYMBOL << vectorOutPoint[i].id << endl;   //写入
    }
    fout.close();            //关闭
}

void outFile(std::string filePath, vector<rectangularPoint> vectorOutPoint, vector<pointFace> faceGroup)
{
    float zeroX = 0, zeroY = 0, zeroZ = 0;
    ofstream fout;           //创建ofstream
    fout << fixed;
    fout.open(filePath, ios::out);
    fout << "#" << "This file uses mile As the unit of nonparametric coordinates and derived by fout." << endl;
    fout << "g default" << endl;
    for (int i = 0; i < vectorOutPoint.size(); i++)
    {
        fout << "v" << " " << vectorOutPoint[i].x << " " << vectorOutPoint[i].y << " " << vectorOutPoint[i].z << endl; //写入
    }
    for (int j = 0; j < vectorOutPoint.size(); j++)
    {
        zeroZ = vectorOutPoint[j].z;
        fout << "vn" << " " << vectorOutPoint[j].x - zeroX << " " << vectorOutPoint[j].y - zeroY << " " << vectorOutPoint[j].z - zeroZ << endl; //写入
    }
    fout << "s on" << endl;
    for (int k = 0; k < faceGroup.size(); k++)
    {
        fout << "f" << " " << faceGroup[k].four << "//"<< faceGroup[k].four << " " << faceGroup[k].three << "//" << faceGroup[k].three << " " << faceGroup[k].two << "//" << faceGroup[k].two << " " << faceGroup[k].one << "//" << faceGroup[k].one << endl; //写入
    }
    fout.close();            //关闭
}
string readName(string filePath)
{
    char at;
    string name = "null";
    ifstream fin;
    fin.open(filePath, ios::in);
    if (!fin.is_open())
    {
        cout << "无法找到这个文件！" << endl;
        return name;
    }
    fin >> at >> name;
    cout << name;
    fin.close();
    return name;
}
int main()
{
    vector<rectangularPoint> vecPoint;
    vector<pointFace> faceGroup;
    string fileName = "180111SV";
    std::string readPath = "D:\\CPPWorkSpace\\trans\\"+ fileName + "." + "dat";
    string wellName = readName(readPath);
    vecPoint = openFile(readPath);
    setGroup(vecPoint, faceGroup);
    std::string format = "obj";
    std::string writePath = wellName+"_"+fileName + "." + format;
    outFile(writePath, vecPoint,faceGroup);
    return 0;
}