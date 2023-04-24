#include "types.h"
using namespace std;

TypeHandler ::TypeHandler()
{
    widths[BYTE_NUM] = 1;
    widths[SHORT_NUM] = 2;
    widths[INT_NUM] = 4;
    widths[LONG_NUM] = 8;
    widths[FLOAT_NUM] = 4;
    widths[DOUBLE_NUM] = 8;
    widths[BOOL_NUM] = 1;
    widths[CHAR_NUM] = 2;
    widths[VOID_TYPE] = 0;
    widths[TYPE_ERROR] = 0;
    // widthsAswidths a 32-bit processor
    widths[BYTE_PTR] = 4;
    widths[SHORT_PTR] = 4;
    widths[INT_PTR] = 4;
    widths[LONG_PTR] = 4;
    widths[FLOAT_PTR] = 4;
    widths[DOUBLE_PTR] = 4;
    widths[CHAR_PTR] = 4;
    widths[VOID_PTR] = 4;

    typewidth["byte"] = {BYTE_NUM, 1};
    typewidth["short"] = {SHORT_NUM, 2};
    typewidth["int"] = {INT_NUM, 4};
    typewidth["long"] = {LONG_NUM, 8};
    typewidth["float"] = {FLOAT_NUM, 4};
    typewidth["double"] = {DOUBLE_NUM, 8};
    typewidth["boolean"] = {BOOL_NUM, 1};
    typewidth["char"] = {CHAR_NUM, 2};
    typewidth["void"] = {VOID_TYPE, 0};

    typewidth["type_error"] = {TYPE_ERROR, 0};

    // Assuming a 32-bit processor

    typewidth["byte*"] = {BYTE_PTR, 4};
    typewidth["short*"] = {SHORT_PTR, 4};
    typewidth["int*"] = {INT_PTR, 4};
    typewidth["long*"] = {LONG_PTR, 4};
    typewidth["float*"] = {FLOAT_PTR, 4};
    typewidth["double*"] = {DOUBLE_PTR, 4};
    typewidth["char*"] = {CHAR_PTR, 4};
    typewidth["void*"] = {VOID_PTR, 4};

    typewidth["String"] = {STRING_NUM, 8};

    for (auto it : typewidth)
    {
        inv_types.insert({it.second.first, it.first});
    }

    for (int i = 0; i < 8; i++)
    {
        inv_types.insert({100 + i, inv_types[i] + "[]"});
        inv_types.insert({200 + i, inv_types[i] + "[][]"});
        inv_types.insert({300 + i, inv_types[i] + "[][][]"});
    }
}

int TypeHandler::maxtype(string lex1, string lex2)
{
    if (typewidth[lex1].first == INT_NUM && typewidth[lex2].first == FLOAT_NUM)
        return FLOAT_NUM;
    if (typewidth[lex1].first == LONG_NUM && typewidth[lex2].first == DOUBLE_NUM)
        return DOUBLE_NUM;
    int res = (typewidth[lex1].second > typewidth[lex2].second) ? typewidth[lex1].first : typewidth[lex2].first;
    return res;
}

int TypeHandler::maxtype(int type1, int type2)
{
    if (type1 == INT_NUM && type2 == FLOAT_NUM)
        return FLOAT_NUM;
    if (type1 == LONG_NUM && type2 == DOUBLE_NUM)
        return DOUBLE_NUM;
    int res = (widths[type1] > widths[type2]) ? type1 : type2;
    return res;
}

int TypeHandler::categorize(int type)
{
    if (type == BYTE_NUM || type == SHORT_NUM || type == INT_NUM || type == LONG_NUM)
        return INTEGER_TYPE;
    if (type == FLOAT_NUM || type == DOUBLE_NUM)
        return FLOATING_TYPE;
    return NONNUMERIC_TYPE;
}

int TypeHandler::categorize(string typ)
{
    int type = typewidth[typ].first;
    if (type == BYTE_NUM || type == SHORT_NUM || type == INT_NUM || type == LONG_NUM)
        return INTEGER_TYPE;
    if (type == FLOAT_NUM || type == DOUBLE_NUM)
        return FLOATING_TYPE;
    return NONNUMERIC_TYPE;
}

int TypeHandler::addNewClassType()
{
    max_type++;
    return max_type;
}

int TypeHandler::addNewClassType(string nm)
{
    max_type++;
    this->typewidth[nm] = {max_type, -1}; // for a class, width = unknown
    this->inv_types[max_type] = nm;
    return max_type;
}
