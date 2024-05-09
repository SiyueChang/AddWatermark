#include<bits/stdc++.h>
#include <cmath>

#define PI 3.1415926535898
#define MYZERO 0.0000000000001
#define longlong double long double
int nCount = 13;
struct TPathDat {
    long double Incl; //井斜
    long double Azim; //方位角
    long double L; //井深
    long double KI;
    long double KA;
    long double X;
    long double Y;
    long double Z;
    long double S;
    long double V;
    long double Fv;
    long double K;
    long double T;
};
std::vector<TPathDat> PathDat;
/*
struct TDataSet {
    std::map<std::string, std::string> AsString;

    void FieldByName(const std::string& fieldName, std::string& value) {
        if (AsString.find(fieldName) != AsString.end()) {
            value = AsString[fieldName];
        } else {
            value = "Field not found";
        }
    }
};*/
/*int FieldByName(const std::string& fieldName) {
    for(const auto& field : fields) {
        if(field.name == fieldName) {
            return field.value;
        }
    }
    return -1; // 如果未找到指定字段，则返回-1
}*/
long double DegToRad(long double deg) {
    return deg * PI / 180;
}
long double RadToDeg(long double rad) {
    return rad * 180 / PI;
}
long double decimation(long double num)
{
    int intNum = static_cast<int>(num); // 将long double类型转换为int类型，小数部分会被去掉
    return (long double)intNum;
}
long double frac(long double num)
{
    if(num >= 0)
    {
        return num - floor(num);
    }
    return num - ceil(num);
}
//方位角标准化
long double AzimStandard(long double Azim)
{
    while (Azim >= 2 * PI)
    {
        Azim = Azim - 2 * PI;
    }
    while (Azim < 0)
    {
        Azim = Azim + 2 * PI;
    }
    return Azim;
}
//方位角增量标准化
long double AzimIncStandard(long double AzimInc)
{
    while(abs(AzimInc) > PI)
    {
        if(AzimInc > 0)
        {
            AzimInc = AzimInc - 2 * PI;
        }
        if(AzimInc == 0)
        {
            AzimInc = AzimInc;
        }
        if(AzimInc < 0)
        {
            AzimInc = AzimInc + 2 * PI;
        }
    }
    return AzimInc;
}
//根据北坐标X、东坐标Y，计算方位
//值域Cta：[0, 2*pi)
long double Cta(long double X,long double Y)
{
    long double Cta;
    if(X == 0)
    {
        if(Y > 0)
        {
            Cta = PI / 2;
        }else
        {
            if (Y == 0)
            {
                Cta = 0;
            }else
            {
                Cta = 3 * PI / 2;
            }
        }
    }else
    {
        if(X > 0)
        {
            if(Y > 0)
            {
                Cta = atan(Y / X) + 2 * PI;
            }
            else
            {
                if(Y == 0)
                {
                    Cta = 0;
                } else
                {
                    Cta = atan(Y / X) + 2 * PI;
                }
            }
        } else
        {
            if(Y == 0)
            {
                Cta = PI;
            }
            else
            {
                Cta = atan(Y / X) + PI;
            }
        }
    }
    return Cta;
}
//******************  自然曲线模型  ******************
//井斜函数
long double Nature_Incl(long double Io,long double KI,long double dL)
{
    return Io + KI * dL;
}
//方位函数
long double Nature_Azim(long double Ao,long double KA,long double dL)
{
    return Ao + KA * dL;
}
//相关正弦函数
long double FsFunc(long double A,long double K,long double dL)
{
    return 1 / K * (sin(A + K * dL) - sin(A));
}
//相关余弦函数
long double FcFunc(long double A,long double K,long double dL)
{
    return 1 / K * (cos(A) - cos(A + K * dL));
}
//北坐标函数
long double Nature_dX(long double Io,long double Ao,long double KI,long double KA,long double dL)
{
    long double Nature_dX;
    if(abs(KI) < MYZERO)
    {
        Nature_dX = dL * sin(Io) * cos(Ao);
    }else if (abs(KI - KA) < MYZERO)
    {
        Nature_dX = 0.5 * (dL * sin(Io - Ao) + FcFunc(Io + Ao, KI + KA, dL));
    }else if(abs(KI + KA) < MYZERO)
    {
        Nature_dX =  0.5 * (dL * sin(Io + Ao) + FcFunc(Io - Ao, KI - KA, dL));
    }else
    {
        Nature_dX =  0.5 * (FcFunc(Io + Ao, KI + KA, dL) + FcFunc(Io - Ao, KI - KA, dL));
    }
    return Nature_dX;
}
//东坐标函数
long double Nature_dY(long double Io,long double Ao,long double KI,long double KA,long double dL)
{
    long double Nature_dY;
    if((abs(KI) < MYZERO) && (abs(KA) < MYZERO))
    {
        Nature_dY = dL * sin(Io) * sin(Ao);
    }else if(abs(KI - KA) < MYZERO)
    {
        Nature_dY = 0.5 * (dL * cos(Io - Ao) - FsFunc(Io + Ao, KI + KA, dL));
    }else if(abs(KI + KA) < MYZERO)
    {
        Nature_dY = 0.5 * (FsFunc(Io - Ao, KI - KA, dL) - dL * cos(Io + Ao));
    }else
    {
        Nature_dY = 0.5 * (FsFunc(Io - Ao, KI - KA, dL) - FsFunc(Io + Ao, KI + KA, dL));
    }
    return Nature_dY;
}
//垂深坐标函数
long double Nature_dZ(long double Io,long double KI,long double dL)
{
    long double Nature_dZ;
    if(abs(KI) < MYZERO)
    {
        Nature_dZ = dL * cos(Io);
    }
    else
    {
        Nature_dZ = FsFunc(Io,KI,dL);
    }
    return Nature_dZ;
}
//平长坐标函数
long double Nature_dS(long double Io,long double KI,long double dL)
{
    if(abs(KI) < MYZERO)
    {
        return dL * sin(Io);
    }
    return FcFunc(Io,KI,dL);
}

//******************  计算相关过程  ******************
//规整后的计算井深
long double GetGoodL(long double EndL,long double StepL,long double L)
{
    L = L + StepL;
    if(L > EndL)
    {
        L = EndL;
    } else
    {
        if(frac(StepL / 10) == 0)
        {
            if(decimation(L / StepL) != L / StepL)
            {
                L = round(L / StepL) * StepL;
            }
            if(L > EndL)
            {
                L = EndL;
            }
        } else if(frac(StepL) == 0)
        {
            L = decimation(L);
        }
        if((abs(StepL) > 1) && (abs(EndL - L) < 1))
        {
            L = EndL;
        }
    }
    return L;
}
//井眼轨迹插值
void Get_L_Path(int i, long double aL, TPathDat& aPoint) {
    long double dL = aL - PathDat[i - 1].L;
    if ((PathDat[i - 1].Incl == 0) && (PathDat[i].Incl == 0)) {
        PathDat[i - 1].Azim = 0;
        PathDat[i].Azim = 0;
    }
    else {
        if (PathDat[i - 1].Incl == 0) PathDat[i - 1].Azim = PathDat[i].Azim;
        if (PathDat[i].Incl == 0) PathDat[i].Azim = PathDat[i - 1].Azim;
    }

    aPoint.KI = PathDat[i].KI;
    aPoint.KA = PathDat[i].KA;

    long double Incl = Nature_Incl(PathDat[i - 1].Incl, aPoint.KI, dL);
    long double Azim = Nature_Azim(PathDat[i - 1].Azim, aPoint.KA, dL);

    aPoint.Incl = Incl;
    aPoint.Azim = Azim;
    aPoint.X = PathDat[i - 1].X + Nature_dX(PathDat[i - 1].Incl, PathDat[i - 1].Azim, aPoint.KI, aPoint.KA, dL);
    aPoint.Y = PathDat[i - 1].Y + Nature_dY(PathDat[i - 1].Incl, PathDat[i - 1].Azim, aPoint.KI, aPoint.KA, dL);
    aPoint.Z = PathDat[i - 1].Z + Nature_dZ(PathDat[i - 1].Incl, aPoint.KI, dL);
    aPoint.S = PathDat[i - 1].S + Nature_dS(PathDat[i - 1].Incl, aPoint.KI, dL);

    aPoint.V = sqrt(pow(aPoint.X - 0, 2) + pow(aPoint.Y - 0, 2));
    aPoint.Fv = Cta(aPoint.X - 0, aPoint.Y - 0);
    aPoint.K = sqrt(pow(aPoint.KI, 2) + pow(aPoint.KA * sin(aPoint.Incl), 2));
    aPoint.T = (aPoint.K == 0) ? 0 : aPoint.KA * (1 + pow(aPoint.KI / aPoint.K, 2)) * cos(aPoint.Incl);

    aPoint.Azim = AzimStandard(aPoint.Azim);
    aPoint.Fv = AzimStandard(aPoint.Fv);
}
//获取对应于井深的井斜角(°)
long double Get_Incl(long double aL) {
    int i;
    if (aL <= 0) {
        return 0;
    }
    else {
        i = 0;
        while ((aL > PathDat[i].L) && (i < nCount - 1)) {
            i++;
        }
        return RadToDeg(Nature_Incl(PathDat[i - 1].Incl, PathDat[i].KI, aL - PathDat[i - 1].L));
    }
}
//获取对应于井深的垂深值
long double Get_L_Z(long double aL) {
    int i;
    if (aL <= 0) {
        return aL;
    }
    else {
        i = 0;
        while ((aL > PathDat[i].L) && (i < nCount - 1)) {
            i++;
        }
        return PathDat[i - 1].Z + Nature_dZ(PathDat[i - 1].Incl, PathDat[i].KI, aL - PathDat[i - 1].L);
    }
}
//获取对应于井深的垂深和平长值
void Get_L_ZS(long double a_L, long double a_h, long double& a_Z, long double& a_S) {
    long double Incl;
    int i;
    if (a_L <= 0) {
        Incl = 0;
        a_Z = 0;
        a_S = 0;
    }
    else {
        i = 0;
        while ((a_L > PathDat[i].L) && (i < nCount - 1)) {
            i++;
        }
        Incl = Nature_Incl(PathDat[i - 1].Incl, PathDat[i].KI, a_L - PathDat[i - 1].L);
        a_Z = PathDat[i - 1].Z + Nature_dZ(PathDat[i - 1].Incl, PathDat[i].KI, a_L - PathDat[i - 1].L);
        a_S = PathDat[i - 1].S + Nature_dS(PathDat[i - 1].Incl, PathDat[i].KI, a_L - PathDat[i - 1].L);
    }
    a_Z -= a_h * sin(Incl);
    a_S += a_h * cos(Incl);
}
//井眼轨迹计算
void aPointOnPath(TPathDat& FromPoint, TPathDat& ToPoint)
{
    long double dL;
    if (FromPoint.Incl == 0 && ToPoint.Incl == 0) {
        FromPoint.Azim = 0;
        ToPoint.Azim = 0;
    }
    else {
        if (FromPoint.Incl == 0) FromPoint.Azim = ToPoint.Azim;
        if (ToPoint.Incl == 0) ToPoint.Azim = FromPoint.Azim;
    }

    dL = ToPoint.L - FromPoint.L;
    ToPoint.KI = (ToPoint.Incl - FromPoint.Incl) / dL;
    ToPoint.KA = AzimIncStandard(ToPoint.Azim - FromPoint.Azim) / dL;
    if (FromPoint.Incl == 0 && ToPoint.KI == 0)
    {
        ToPoint.KA = 0;
    }
    ToPoint.X = FromPoint.X + Nature_dX(FromPoint.Incl, FromPoint.Azim, ToPoint.KI, ToPoint.KA, dL);
    ToPoint.Y = FromPoint.Y + Nature_dY(FromPoint.Incl, FromPoint.Azim, ToPoint.KI, ToPoint.KA, dL);
    ToPoint.Z = FromPoint.Z + Nature_dZ(FromPoint.Incl, ToPoint.KI, dL);
    ToPoint.S = FromPoint.S + Nature_dS(FromPoint.Incl, ToPoint.KI, dL);

    ToPoint.V = sqrt(pow(ToPoint.X - 0, 2) + pow(ToPoint.Y - 0, 2));
    ToPoint.Fv = Cta(ToPoint.X - 0, ToPoint.Y - 0);
    ToPoint.K = sqrt(pow(ToPoint.KI, 2) + pow(ToPoint.KA * sin(ToPoint.Incl), 2));
    if (ToPoint.K == 0) ToPoint.T = 0;
    else ToPoint.T = ToPoint.KA * (1 + pow(ToPoint.KI / ToPoint.K, 2)) * cos(ToPoint.Incl);
    ToPoint.Azim = AzimStandard(ToPoint.Azim);
    ToPoint.Fv = AzimStandard(ToPoint.Fv);
}
//井眼轨道数据Read准备
void PathDataReadReady(long double KUnit, TPathDat& aPoint)
{
    aPoint.Incl = DegToRad(aPoint.Incl);
    aPoint.Azim = DegToRad(aPoint.Azim);
    aPoint.Fv = DegToRad(aPoint.Fv);
    aPoint.KI = DegToRad(aPoint.KI / KUnit);
    aPoint.KA = DegToRad(aPoint.KA / KUnit);
    aPoint.K = DegToRad(aPoint.K / KUnit);
    aPoint.T = DegToRad(aPoint.T / KUnit);
}
//井眼轨道数据写入准备
void PathDataPrepare(long double KUnit, TPathDat& aPoint) {
    aPoint.Incl = RadToDeg(abs(aPoint.Incl));
    if(aPoint.Incl < 0)
    {
        aPoint.Azim = RadToDeg(AzimStandard(aPoint.Azim + PI));
    } else
    {
        aPoint.Azim = RadToDeg(AzimStandard(aPoint.Azim));
    }
    aPoint.Fv = RadToDeg(AzimStandard(aPoint.Fv));
    aPoint.KI = RadToDeg(aPoint.KI * KUnit);
    aPoint.KA = RadToDeg(aPoint.KA * KUnit);
    aPoint.K = RadToDeg(aPoint.K * KUnit);
    aPoint.T = RadToDeg(aPoint.T * KUnit);
}
//读取井眼轨道数据
void ReadMsdrRes(long double KUnit) {
    if (KUnit <= 0)
    {
        KUnit = 30;
    }
    std::ifstream csv_data("201-3.csv", std::ios::in);
    std::string line;
    if (!csv_data.is_open())
    {
        std::cout << "Error: opening file fail" << std::endl;
        std::exit(1);
    }
    std::istringstream sin;         //将整行字符串line读入到字符串istringstream中
    std::string word;
    // 读取标题行
    std::getline(csv_data, line);
    //计数
    int i = 0;
    TPathDat Dat = TPathDat(.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0);
    TPathDat Dat2 = TPathDat(.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0);
    // 读取数据
    while (std::getline(csv_data, line))
    {
        sin.clear();
        sin.str(line);
        if(i >= 2)
        {
            i = 0;
            Dat2 = Dat;
            Dat = TPathDat(.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0,.0);
        }
        while (std::getline(sin, word, ',')) //将字符串流sin中的字符读到field字符串中，以逗号为分隔符
        {
            if(i == 0)
            {
                Dat.L = std::stof(word);
            }
            if(i == 1)
            {
                Dat.Incl =std::stof(word) ;
            }
            if(i == 2) {
                Dat.Azim =std::stof(word);
            }
            i++;
        }
        PathDataReadReady(KUnit,Dat2);
        PathDataReadReady(KUnit,Dat);
        aPointOnPath(Dat2,Dat);
        PathDataPrepare(KUnit,Dat);
        PathDat.push_back(Dat);
    }
}
int main() {
    std::cout.setf(std::ios::fixed,std::ios::floatfield);//十进制计数法，不是科学计数法
    ReadMsdrRes(30);
    for(auto i : PathDat)
    {
        std::cout <<"L:"<< i.L <<"   Incl:"<< i.Incl <<"   Azim:"<< i.Azim <<"   Z:"<< i.Z <<"   X:"<< i.X + 3623657.410<<"   Y:"<< i.Y + 40519547.720 <<"   S:"<< i.S <<"   V:"<< i.V <<"   Fv:"<< i.Fv<<"   KI:" << i.KI <<"   KA:"<< i.KA <<"   K:"<< i.K <<"   T:"<< i.T<<std::endl;
    }
    return 0;
}
