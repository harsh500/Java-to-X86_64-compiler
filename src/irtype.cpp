#include "irtype.h"


Quadruple::Quadruple(std::string op, std::string arg1, std::string arg2, std::string result)
{
    this->op = op;
    type = 0;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->result = result;
    pos++;
}

Quadruple::Quadruple(std::string op, std::string arg1, std::string result)
{
    type = 0;
    this->op = op;
    this->arg1 = arg1;
    this->result = result;
    pos++;
}

Quadruple::Quadruple(int type, std::string op, std::string arg1, std::string arg2, std::string result)
{
    this->op = op;
    this->type = type;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->result = result;
    pos++;
}

Quadruple::Quadruple(int type, std::string op, std::string arg1, std::string result)
{
    this->op = op;
    this->type = type;
    this->arg1 = arg1;
    this->result = result;
    pos++;
}

Quadruple::Quadruple(int type, std::string arg1, std::string arg2)
{
    this->type = type;
    this->arg1 = arg1;
    this->arg2 = arg2;
}
Quadruple::Quadruple(int type, std::string arg1)
{
    this->type = type;
    this->arg1 = arg1;
}

Quadruple::Quadruple(int type, std::string arg1, std::vector<std::string> params)
{
    this->type = type;
    this->arg1 = arg1;
    this->params = params;
}

Quadruple::Quadruple(std::string label)
{
    this->label = label;
}
Quadruple::Quadruple(int type)
{
    this->type = type;
}

int Quadruple::pos = 0;

void Quadruple::print()
{
    std::cout << arg1 << " " << arg2 << " " << result << "\n";
    std::cout<<"Scope level is "<<scope_level<<std::endl;
    int last_dig;
    std::cout << arg1.length() << " len\n";
    last_dig = arg1[arg1.length()-1];
    std::cout << "not reaching here\n";
    if(last_dig<'0' || last_dig>'9')
    {
        arg1 = arg1 + "`" + std::to_string(scope_level);
    }
    std::cout << "returning1\n";
    last_dig = arg2[arg2.length()-1];
    if(last_dig<'0' || last_dig>'9')
    {
        arg2 = arg2 + "`" + std::to_string(scope_level);
    }
    std::cout << "returning2\n";
    std::cout << result<< " len\n";
    if(result != "") {
        std::cout << result.length() << " leng\n";
        last_dig = result[result.length()-1];
        std::cout << last_dig << " l\n";
        if(last_dig<'0' || last_dig>'9')
        {
            result = result + "`" + std::to_string(scope_level);
        }
    }
    std::cout << "returning\n";
    if (type == 1)
    {
        std::cout << "if" << arg1 << "then ";
        std::cout << "returning\n";
        return;
    }
    if (type == 2)
    {
        std::cout << "if " << arg1 << op << arg2 << " goto " << result << "\n";
        std::cout << "returning\n";
        return;
    }
    if (type == 3)
    {
        std::cout << "goto " << result << "\n";
        std::cout << "returning\n";
        return;
    }
    else if (type == 4)
    {
        if (result == "")
            std::cout << "call " << arg1 << ", " << arg2 << "\n";
        else
            std::cout << result << "=call " << arg1 << ", " << arg2 << "\n";
        std::cout << "returning\n";
        return;
    }
    else if (type == 5)
    {
        std::cout << "pushparam " << arg1 << "\n";
        std::cout << "returning\n";
        return;
    }
    else if (type == 6)
    {
        std::cout << "beginfunc " << arg1 << "\n";
        std::cout << "returning\n";
        return;
    }
    else if (type == 7)
    {
        std::cout << arg1 << " " << arg2 << "\n";
        std::cout << "returning\n";
        return;
    }

    if (label != "")
    {
        std::cout << this->label << ": ";
    }
    if (arg2 != "")
        std::cout << this->result << "=" << this->arg1 << this->op << this->arg2 << "\n";
    else
        std::cout << this->result << "=" << this->arg1 << "\n";
    std::cout << "returning\n";
    
}

std::string Quadruple::get_op()
{
    return this->op;
}

std::string Quadruple::get_arg1()
{
    return this->arg1;
}

std::string Quadruple::get_arg2()
{
    return this->arg2;
}

IRcode::IRcode(Quadruple *q)
{
    this->q = q;
    this->next = NULL;
}

IRcode::IRcode(Quadruple *q, IRcode *next)
{
    this->q = q;
    this->next = next;
}

void Quadruple::set_result(std::string res)
{
    this->result = res;
}

int Quadruple::get_type()
{
    return this->type;
}