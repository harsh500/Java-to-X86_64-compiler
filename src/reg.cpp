#include "reg.h"

std::string reg::getname()
{
    return regName;
}

int reg::getRegDesSize()
{
    int cnt = 0;
    for (auto it : regDes)
    {
        if (it.second == true)
            cnt++;
    }
    return cnt;
}

reg::reg(int id, string regName)
{
    this->id = id;
    this->regName = regName;
}
