#include <bits/stdc++.h>
using namespace std;

#define BYTE_NUM 0
#define SHORT_NUM 1
#define INT_NUM 2
#define LONG_NUM 3
#define FLOAT_NUM 4
#define DOUBLE_NUM 5
#define BOOL_NUM 6
#define CHAR_NUM 7
#define VOID_TYPE 8
// #define BYTE_NUM 9

#define BYTE_PTR 10
#define SHORT_PTR 11
#define INT_PTR 12
#define LONG_PTR 13
#define FLOAT_PTR 14
#define DOUBLE_PTR 15
#define CHAR_PTR 16
#define VOID_PTR 17

#define TYPE_ERROR 18

#define CLASS_LITERAL 19
#define TEXT_BLOCK 20
#define STRING_LIT 21
#define NULL_LIT 22
#define KWORD 23

#define NUMERIC_TYPE 24
#define INTEGER_TYPE 25
#define FLOATING_TYPE 26
#define NONNUMERIC_TYPE 27

// for arrays add 100 to the basic types

#define PACKAGE_TYPE 28
#define IMPORT_TYPE 29

#define ARRAY_ONE 30
#define ARRAY_TWO 31
#define ARRAY_THREE 32
#define STRING_NUM 33

#define UNKNOWN_TYPE 34

#ifndef ARRAYTYPE
#define ARRAYTYPE
class Arraytype
{
public:
    //     int base_type;
    //     int num_elems;
    //     string class_type;

    //     Arraytype(int base_type, int num_elems);
    //     Arraytype(string class_type, int num_elems);

    int get_size();
    int get_basetype();
    string get_classtype();
};
#endif

#ifndef TYPEHANDLER
#define TYPEHANDLER
class TypeHandler
{

public:
    map<string, pair<int, int>> typewidth;
    map<int, int> widths;
    map<int, string> inv_types;
    int max_type = UNKNOWN_TYPE; // always keep the unknown_type to be the last number so that adding more basic types doesn't change this line

    TypeHandler();

    int maxtype(string lex1, string lex2); // returns the type having max precision between the types of lex1 and lex2
    int maxtype(int type1, int type2);
    int categorize(int type);
    int categorize(string type);
    int addNewClassType();          // returns the code of the newly added type
    int addNewClassType(string nm); // returns the code of the newly added type
};
#endif
