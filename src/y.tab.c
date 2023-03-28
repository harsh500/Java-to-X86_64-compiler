/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.yacc"

#include "dotgen.h"

#include "types.h"
#include "utils.h"


using namespace std;
int yylex();
void yyerror(const char* sp);
extern FILE* yyin;
extern int yylineno;
extern char* yytext;
extern int fin;
extern int v;
extern int cnt;
extern int isarrayinit;
// map<string, pair<int, int>> typeroot->typewidth;
extern map<string,SymNode*> list_class;
extern string otpt;

extern int startPos;

extern int temp ;

extern int varCnt;
extern int labelCnt ;
extern int importflag ;

extern string condvar;
extern int isCond ;
extern TypeHandler* typeroot ;

extern vector<Quadruple*> residualCode;

extern vector<string> castName;


extern int whilepos ;
extern SymGlob* root ;
extern SymGlob* orig_root ;
extern SymNode* magic_ptr;

/*
void ir_gen(vector<Quadruple*> ircode, string fln)
{
    ofstream myFile;
    myFile.open(fln);
    int cnt = 0;
    for(auto it: ircode) {
        myFile << cnt++ << "\t:\t";

    if(it->type == 1) {
        myFile << "if" << it->arg1 << "then ";
        continue;
    }
    if(it->type == 2)
    {
        myFile<<"if "<<it->arg1<< it->op << it->arg2<<" goto "<<it->result<<"\n";
        continue;
    }
    if(it->type == 3)
    {
        myFile<<"goto "<<it->result<<"\n";
        continue;
    }
    else if(it->type == 4) {
        if(it->result == "")
            myFile << "call " << it->arg1 << ", " << it->arg2 << "\n";
        else 
            myFile << it->result << "=call " << it->arg1 << ", " << it->arg2 << "\n";
        continue;
    }
    else if(it->type == 5) {
        myFile << "param " << it->arg1 << "\n"; continue;
    }
    else if(it->type == 6) {
        myFile << "beginfunc " << it->arg1 << "\n"; continue;
    }
    else if(it->type == 7) {
        myFile << it->arg1 << " " << it->arg2 <<"\n"; continue;
    }
    
    if(it->label != "")
    {
        myFile<<it->label<<": ";
    }
    if(it->arg2 != "")
        myFile << it->result << "=" << it->arg1 << it->op << it->arg2 << "\n";
    else 
        myFile << it->result << "=" << it->arg1<< "\n";

    }
    myFile.close();
}

void backpatch(vector<int>& lst, int n)
{
    for(auto it: lst) {
        ircode[it]->print();
        ircode[it]->set_result(to_string(n));
        
    }
}


void processFieldDec(Node* n, Node* n1, int type)
{
    if(n1->isCond == 1) return;
    string resName = n1->children[1]->varName;
    if(type != n1->children[1]->type) {
        resName = string("t") + to_string(varCnt++);
        Quadruple* q = new Quadruple("", "cast_to_" + castName[type] + " ", n1->children[1]->varName, resName);
        n->code.push_back(q);
        ircode.push_back(q);
    }
    
    
    Quadruple* q = new Quadruple("=", resName, n1->children[0]->varName); 
    n->code.push_back(q);
    ircode.push_back(q);
    n->last = ircode.size() - 1;
}

int getmethodtype(SymNode* n)
{
    SymNode* temp = n;
    while(temp->name != "method")
    {
        temp = temp->parent;
    }
    return temp->returntype;
}

// n is the VariableDeclarator node, 
void init1DArray(Node* n, string type)
{
    Node* arrayName = n->children[0];
    Node* arrayInit = n->children[1];   //ArrayInitializer node 
    int sizeArray = arrayInit->children.size();
    for(int i = 0; i < sizeArray; i++) {
        Quadruple* q = new Quadruple("",arrayInit->children[i]->varName, "", arrayName->varName + "[" + to_string((i*(typeroot->typewidth[type].second))) + "]");
        ircode.push_back(q);
        n->code.push_back( q );
    }

    n->last = ircode.size() - 1;
}

void init2DArray(Node* n,string type)
{
    Node* arrayName = n->children[0];
    Node* arrayInit = n->children[1];   //ArrayInitializer node 
    int sizeArray = arrayInit->children.size();
    int n2= arrayInit->children[0]->children.size();
    for(int i = 0; i < sizeArray; i++) {
        for(int j = 0; j < n2; j++) {
            Quadruple* q = new Quadruple("",arrayInit->children[i]->children[j]->varName, "", arrayName->varName + "[" + to_string(((n2*i + j)*typeroot->typewidth[type].second)) + "]");
            ircode.push_back(q);
            n->code.push_back( q );
        }
    }

    n->last = ircode.size() - 1;
}

void init3DArray(Node* n, string type)
{
    Node* arrayName = n->children[0];
    Node* arrayInit = n->children[1];   //ArrayInitializer node 
    int sizeArray = arrayInit->children.size();
    int n2= arrayInit->children[0]->children.size();
    int n3 = arrayInit->children[0]->children[0]->children.size();
    for(int i = 0; i < sizeArray; i++) {
        for(int j = 0; j < n2; j++) {
            for(int k = 0; k < n3; k++) {
                Quadruple* q = new Quadruple("",arrayInit->children[i]->children[j]->children[k]->varName, "", arrayName->varName + "[" + to_string(((i*n2*n3 + j*n3 + k)*typeroot->typewidth[type].second)) + "]");
                ircode.push_back(q);
                n->code.push_back( q );
            }
        }
    }

    n->last = ircode.size() - 1;
}

void processPostIncre(Node* n)
{
    if(residualCode.size() == 0 ) return;
    if(n == NULL) return;
    ircode.insert(ircode.end(), residualCode.begin(), residualCode.end());
    n->code.insert(n->code.end(), residualCode.begin(), residualCode.end());
    residualCode.clear();
    n->last = ircode.size() - 1;
}

void processAssignment(Node* n1, Node* n2, Node* n3, Node* n)
{
    char oper = n2->attr[0];
    cout << oper<< " dsad\n";
    Quadruple* q = new Quadruple(string(1, oper), n1->varName, n3->varName, n1->varName);
    n->code.push_back(q);
    ircode.push_back(  q );
    n->last = ircode.size() - 1;
    n->varName = n1->varName;
    return;
}

int spacestripind(string s)
{
    int n = s.length();
    int i = n-1;
    while(i>=0 && s[i]!='.')
        i--;
    return i;
}

string spacestrip(string s)
{
    int n = s.length();
    int i = n-1;
    while(i>=0 && s[i]!='.')
        i--;
    return s.substr(i+1, n-i-1);
}

void processRelational(vector<Node*> nodes, string op)
{
        nodes[0]->truelist.push_back(ircode.size());
        nodes[0]->falselist.push_back(ircode.size() + 1);
        
        Quadruple* q = new Quadruple(2, op, nodes[1]->varName, nodes[2]->varName, "");
        nodes[0]->code.push_back(q);
        ircode.push_back(q);
        q = new Quadruple(3, "", "", "", "");
        nodes[0]->code.push_back(q);
        ircode.push_back(q);
        
        nodes[0]->last = ircode.size() - 1;
        nodes[0]->last = ircode.size() - 1;
        
}

void processArithmetic(vector<Node*> nodes, string op)
{
    string resName = string("t") + to_string(varCnt);
    varCnt++;        
    int type1 = typeroot->categorize(nodes[1]->type);
    int type2 = typeroot->categorize(nodes[2]->type);
    int flag = 1;
    if(type1 == FLOATING_TYPE && type2 == INTEGER_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        Quadruple* q = new Quadruple("", "cast_to_float ", nodes[2]->varName, resName);
        nodes[0]->code.push_back(q);
        ircode.push_back(q);
        flag = 2;
    }
    else if(type1 == INTEGER_TYPE && type2 == FLOATING_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        Quadruple* q = new Quadruple("", "cast_to_float", nodes[1]->varName, resName);
        nodes[0]->code.push_back(q);
        ircode.push_back(q);
        flag = 2;
    }
    if(type1 == FLOATING_TYPE || type2 == FLOATING_TYPE) flag=2;
    Quadruple* q = new Quadruple(op + (flag == 1?string("int "): string("float ")), nodes[1]->varName, nodes[2]->varName, resName);
    nodes[0]->varName = resName;
    nodes[0]->code.push_back(q );
    ircode.push_back(q);
    nodes[0]->last = ircode.size() - 1;
}

void processWhile(Node* n, Node* n1, Node* n2)
{
    backpatch(n2->nextlist, n1->last + 1 - n1->code.size());
    backpatch(n1->truelist, n1->last + 1);
    n->nextlist = n1->falselist;
    Quadruple* q = new Quadruple(3, "", "", "", to_string( n1->last + 1 - n1->code.size() ));
    ircode.push_back(q);
    n->code.push_back(q);
    n->last = ircode.size() - 1;
}

void generateArgumentList(vector<Node*> nodes, Node* n)
{
    for(auto node: nodes) {
        Quadruple* q = new Quadruple(5, node->varName);
        n->code.push_back(q);
        ircode.push_back(q);
    }
    n->last = ircode.size() - 1;
}


void verbose(int v,string h){
    if(v==1){
        cout<<cnt<<") "<<h<<"\n";
        cnt++;
    }
}
*/





#line 378 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BOOLEAN = 258,                 /* BOOLEAN  */
    CHAR = 259,                    /* CHAR  */
    BYTE = 260,                    /* BYTE  */
    SHORT = 261,                   /* SHORT  */
    INT = 262,                     /* INT  */
    LONG = 263,                    /* LONG  */
    FLOAT = 264,                   /* FLOAT  */
    DOUBLE = 265,                  /* DOUBLE  */
    VOID = 266,                    /* VOID  */
    EXTENDS = 267,                 /* EXTENDS  */
    SUPER = 268,                   /* SUPER  */
    STRING = 269,                  /* STRING  */
    THIS = 270,                    /* THIS  */
    VAR = 271,                     /* VAR  */
    INSTANCEOF = 272,              /* INSTANCEOF  */
    FINAL = 273,                   /* FINAL  */
    NEW = 274,                     /* NEW  */
    CHARACTERLITERAL = 275,        /* CHARACTERLITERAL  */
    STRINGLITERAL = 276,           /* STRINGLITERAL  */
    TEXTBLOCK = 277,               /* TEXTBLOCK  */
    NULLLITERAL = 278,             /* NULLLITERAL  */
    CLASS = 279,                   /* CLASS  */
    PACKAGE = 280,                 /* PACKAGE  */
    IMPORT = 281,                  /* IMPORT  */
    STATIC = 282,                  /* STATIC  */
    DO = 283,                      /* DO  */
    INTEGERLITERAL = 284,          /* INTEGERLITERAL  */
    FLOATINGPOINTLITERAL = 285,    /* FLOATINGPOINTLITERAL  */
    BOOLEANLITERAL = 286,          /* BOOLEANLITERAL  */
    JAVALETTER = 287,              /* JAVALETTER  */
    JAVALETTERORDIGIT = 288,       /* JAVALETTERORDIGIT  */
    OPEN = 289,                    /* OPEN  */
    MODULE = 290,                  /* MODULE  */
    REQUIRES = 291,                /* REQUIRES  */
    EXPORTS = 292,                 /* EXPORTS  */
    OPENS = 293,                   /* OPENS  */
    USES = 294,                    /* USES  */
    PROVIDES = 295,                /* PROVIDES  */
    TO = 296,                      /* TO  */
    WITH = 297,                    /* WITH  */
    TRANSITIVE = 298,              /* TRANSITIVE  */
    LEFTSQUAREBRACKET = 299,       /* LEFTSQUAREBRACKET  */
    RIGHTSQUAREBRACKET = 300,      /* RIGHTSQUAREBRACKET  */
    LEFTCURLYBRACKET = 301,        /* LEFTCURLYBRACKET  */
    RIGHTCURLYBRACKET = 302,       /* RIGHTCURLYBRACKET  */
    LEFTPARENTHESIS = 303,         /* LEFTPARENTHESIS  */
    RIGHTPARENTHESIS = 304,        /* RIGHTPARENTHESIS  */
    SEMICOLON = 305,               /* SEMICOLON  */
    COMMA = 306,                   /* COMMA  */
    DOT = 307,                     /* DOT  */
    ELLIPSIS = 308,                /* ELLIPSIS  */
    AT = 309,                      /* AT  */
    DOUBLECOLON = 310,             /* DOUBLECOLON  */
    ASSIGN = 311,                  /* ASSIGN  */
    GRT = 312,                     /* GRT  */
    LSS = 313,                     /* LSS  */
    NOT = 314,                     /* NOT  */
    TIL = 315,                     /* TIL  */
    QUES = 316,                    /* QUES  */
    COL = 317,                     /* COL  */
    ARW = 318,                     /* ARW  */
    EQUAL = 319,                   /* EQUAL  */
    GEQ = 320,                     /* GEQ  */
    IMPLEMENTS = 321,              /* IMPLEMENTS  */
    LEQ = 322,                     /* LEQ  */
    NEQUAL = 323,                  /* NEQUAL  */
    AND = 324,                     /* AND  */
    OR = 325,                      /* OR  */
    INCRE = 326,                   /* INCRE  */
    DECRE = 327,                   /* DECRE  */
    PLUS = 328,                    /* PLUS  */
    MINUS = 329,                   /* MINUS  */
    MULT = 330,                    /* MULT  */
    DIV = 331,                     /* DIV  */
    BAND = 332,                    /* BAND  */
    BOR = 333,                     /* BOR  */
    BXOR = 334,                    /* BXOR  */
    MOD = 335,                     /* MOD  */
    LSHIFT = 336,                  /* LSHIFT  */
    RSHIFT = 337,                  /* RSHIFT  */
    UNRSHIFT = 338,                /* UNRSHIFT  */
    PLUSEQUAL = 339,               /* PLUSEQUAL  */
    MINUSEQUAL = 340,              /* MINUSEQUAL  */
    MULTEQUAL = 341,               /* MULTEQUAL  */
    DIVEQUAL = 342,                /* DIVEQUAL  */
    BANDEQUAL = 343,               /* BANDEQUAL  */
    BOREQUAL = 344,                /* BOREQUAL  */
    BXOREQUAL = 345,               /* BXOREQUAL  */
    MODEQUAL = 346,                /* MODEQUAL  */
    LSHIFTEQUAL = 347,             /* LSHIFTEQUAL  */
    RSHIFTEQUAL = 348,             /* RSHIFTEQUAL  */
    UNRSHIFTEQUAL = 349,           /* UNRSHIFTEQUAL  */
    IF = 350,                      /* IF  */
    ELSE = 351,                    /* ELSE  */
    WHILE = 352,                   /* WHILE  */
    FOR = 353,                     /* FOR  */
    RETURN = 354,                  /* RETURN  */
    CONTINUE = 355,                /* CONTINUE  */
    BREAK = 356,                   /* BREAK  */
    YIELD = 357,                   /* YIELD  */
    SEALED = 358,                  /* SEALED  */
    PROTECTED = 359,               /* PROTECTED  */
    PUBLIC = 360,                  /* PUBLIC  */
    PRIVATE = 361,                 /* PRIVATE  */
    STRICTFP = 362,                /* STRICTFP  */
    ABSTRACT = 363,                /* ABSTRACT  */
    DEFAULT = 364,                 /* DEFAULT  */
    INTERFACE = 365,               /* INTERFACE  */
    PERMITS = 366,                 /* PERMITS  */
    NONSEALED = 367,               /* NONSEALED  */
    TRANSIENT = 368,               /* TRANSIENT  */
    VOLATILE = 369,                /* VOLATILE  */
    NATIVE = 370,                  /* NATIVE  */
    SYNCHRONIZED = 371,            /* SYNCHRONIZED  */
    THROWS = 372,                  /* THROWS  */
    ASSERT = 373,                  /* ASSERT  */
    IDENTIFIER = 374,              /* IDENTIFIER  */
    RECORD = 375,                  /* RECORD  */
    LRSQUAREBRACKET = 376,         /* LRSQUAREBRACKET  */
    SYSTEMOUTPRINTLN = 377         /* SYSTEMOUTPRINTLN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define BOOLEAN 258
#define CHAR 259
#define BYTE 260
#define SHORT 261
#define INT 262
#define LONG 263
#define FLOAT 264
#define DOUBLE 265
#define VOID 266
#define EXTENDS 267
#define SUPER 268
#define STRING 269
#define THIS 270
#define VAR 271
#define INSTANCEOF 272
#define FINAL 273
#define NEW 274
#define CHARACTERLITERAL 275
#define STRINGLITERAL 276
#define TEXTBLOCK 277
#define NULLLITERAL 278
#define CLASS 279
#define PACKAGE 280
#define IMPORT 281
#define STATIC 282
#define DO 283
#define INTEGERLITERAL 284
#define FLOATINGPOINTLITERAL 285
#define BOOLEANLITERAL 286
#define JAVALETTER 287
#define JAVALETTERORDIGIT 288
#define OPEN 289
#define MODULE 290
#define REQUIRES 291
#define EXPORTS 292
#define OPENS 293
#define USES 294
#define PROVIDES 295
#define TO 296
#define WITH 297
#define TRANSITIVE 298
#define LEFTSQUAREBRACKET 299
#define RIGHTSQUAREBRACKET 300
#define LEFTCURLYBRACKET 301
#define RIGHTCURLYBRACKET 302
#define LEFTPARENTHESIS 303
#define RIGHTPARENTHESIS 304
#define SEMICOLON 305
#define COMMA 306
#define DOT 307
#define ELLIPSIS 308
#define AT 309
#define DOUBLECOLON 310
#define ASSIGN 311
#define GRT 312
#define LSS 313
#define NOT 314
#define TIL 315
#define QUES 316
#define COL 317
#define ARW 318
#define EQUAL 319
#define GEQ 320
#define IMPLEMENTS 321
#define LEQ 322
#define NEQUAL 323
#define AND 324
#define OR 325
#define INCRE 326
#define DECRE 327
#define PLUS 328
#define MINUS 329
#define MULT 330
#define DIV 331
#define BAND 332
#define BOR 333
#define BXOR 334
#define MOD 335
#define LSHIFT 336
#define RSHIFT 337
#define UNRSHIFT 338
#define PLUSEQUAL 339
#define MINUSEQUAL 340
#define MULTEQUAL 341
#define DIVEQUAL 342
#define BANDEQUAL 343
#define BOREQUAL 344
#define BXOREQUAL 345
#define MODEQUAL 346
#define LSHIFTEQUAL 347
#define RSHIFTEQUAL 348
#define UNRSHIFTEQUAL 349
#define IF 350
#define ELSE 351
#define WHILE 352
#define FOR 353
#define RETURN 354
#define CONTINUE 355
#define BREAK 356
#define YIELD 357
#define SEALED 358
#define PROTECTED 359
#define PUBLIC 360
#define PRIVATE 361
#define STRICTFP 362
#define ABSTRACT 363
#define DEFAULT 364
#define INTERFACE 365
#define PERMITS 366
#define NONSEALED 367
#define TRANSIENT 368
#define VOLATILE 369
#define NATIVE 370
#define SYNCHRONIZED 371
#define THROWS 372
#define ASSERT 373
#define IDENTIFIER 374
#define RECORD 375
#define LRSQUAREBRACKET 376
#define SYSTEMOUTPRINTLN 377

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 309 "parser.yacc"

    struct Node* node;
	char* lexeme;

#line 680 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_BOOLEAN = 3,                    /* BOOLEAN  */
  YYSYMBOL_CHAR = 4,                       /* CHAR  */
  YYSYMBOL_BYTE = 5,                       /* BYTE  */
  YYSYMBOL_SHORT = 6,                      /* SHORT  */
  YYSYMBOL_INT = 7,                        /* INT  */
  YYSYMBOL_LONG = 8,                       /* LONG  */
  YYSYMBOL_FLOAT = 9,                      /* FLOAT  */
  YYSYMBOL_DOUBLE = 10,                    /* DOUBLE  */
  YYSYMBOL_VOID = 11,                      /* VOID  */
  YYSYMBOL_EXTENDS = 12,                   /* EXTENDS  */
  YYSYMBOL_SUPER = 13,                     /* SUPER  */
  YYSYMBOL_STRING = 14,                    /* STRING  */
  YYSYMBOL_THIS = 15,                      /* THIS  */
  YYSYMBOL_VAR = 16,                       /* VAR  */
  YYSYMBOL_INSTANCEOF = 17,                /* INSTANCEOF  */
  YYSYMBOL_FINAL = 18,                     /* FINAL  */
  YYSYMBOL_NEW = 19,                       /* NEW  */
  YYSYMBOL_CHARACTERLITERAL = 20,          /* CHARACTERLITERAL  */
  YYSYMBOL_STRINGLITERAL = 21,             /* STRINGLITERAL  */
  YYSYMBOL_TEXTBLOCK = 22,                 /* TEXTBLOCK  */
  YYSYMBOL_NULLLITERAL = 23,               /* NULLLITERAL  */
  YYSYMBOL_CLASS = 24,                     /* CLASS  */
  YYSYMBOL_PACKAGE = 25,                   /* PACKAGE  */
  YYSYMBOL_IMPORT = 26,                    /* IMPORT  */
  YYSYMBOL_STATIC = 27,                    /* STATIC  */
  YYSYMBOL_DO = 28,                        /* DO  */
  YYSYMBOL_INTEGERLITERAL = 29,            /* INTEGERLITERAL  */
  YYSYMBOL_FLOATINGPOINTLITERAL = 30,      /* FLOATINGPOINTLITERAL  */
  YYSYMBOL_BOOLEANLITERAL = 31,            /* BOOLEANLITERAL  */
  YYSYMBOL_JAVALETTER = 32,                /* JAVALETTER  */
  YYSYMBOL_JAVALETTERORDIGIT = 33,         /* JAVALETTERORDIGIT  */
  YYSYMBOL_OPEN = 34,                      /* OPEN  */
  YYSYMBOL_MODULE = 35,                    /* MODULE  */
  YYSYMBOL_REQUIRES = 36,                  /* REQUIRES  */
  YYSYMBOL_EXPORTS = 37,                   /* EXPORTS  */
  YYSYMBOL_OPENS = 38,                     /* OPENS  */
  YYSYMBOL_USES = 39,                      /* USES  */
  YYSYMBOL_PROVIDES = 40,                  /* PROVIDES  */
  YYSYMBOL_TO = 41,                        /* TO  */
  YYSYMBOL_WITH = 42,                      /* WITH  */
  YYSYMBOL_TRANSITIVE = 43,                /* TRANSITIVE  */
  YYSYMBOL_LEFTSQUAREBRACKET = 44,         /* LEFTSQUAREBRACKET  */
  YYSYMBOL_RIGHTSQUAREBRACKET = 45,        /* RIGHTSQUAREBRACKET  */
  YYSYMBOL_LEFTCURLYBRACKET = 46,          /* LEFTCURLYBRACKET  */
  YYSYMBOL_RIGHTCURLYBRACKET = 47,         /* RIGHTCURLYBRACKET  */
  YYSYMBOL_LEFTPARENTHESIS = 48,           /* LEFTPARENTHESIS  */
  YYSYMBOL_RIGHTPARENTHESIS = 49,          /* RIGHTPARENTHESIS  */
  YYSYMBOL_SEMICOLON = 50,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 51,                     /* COMMA  */
  YYSYMBOL_DOT = 52,                       /* DOT  */
  YYSYMBOL_ELLIPSIS = 53,                  /* ELLIPSIS  */
  YYSYMBOL_AT = 54,                        /* AT  */
  YYSYMBOL_DOUBLECOLON = 55,               /* DOUBLECOLON  */
  YYSYMBOL_ASSIGN = 56,                    /* ASSIGN  */
  YYSYMBOL_GRT = 57,                       /* GRT  */
  YYSYMBOL_LSS = 58,                       /* LSS  */
  YYSYMBOL_NOT = 59,                       /* NOT  */
  YYSYMBOL_TIL = 60,                       /* TIL  */
  YYSYMBOL_QUES = 61,                      /* QUES  */
  YYSYMBOL_COL = 62,                       /* COL  */
  YYSYMBOL_ARW = 63,                       /* ARW  */
  YYSYMBOL_EQUAL = 64,                     /* EQUAL  */
  YYSYMBOL_GEQ = 65,                       /* GEQ  */
  YYSYMBOL_IMPLEMENTS = 66,                /* IMPLEMENTS  */
  YYSYMBOL_LEQ = 67,                       /* LEQ  */
  YYSYMBOL_NEQUAL = 68,                    /* NEQUAL  */
  YYSYMBOL_AND = 69,                       /* AND  */
  YYSYMBOL_OR = 70,                        /* OR  */
  YYSYMBOL_INCRE = 71,                     /* INCRE  */
  YYSYMBOL_DECRE = 72,                     /* DECRE  */
  YYSYMBOL_PLUS = 73,                      /* PLUS  */
  YYSYMBOL_MINUS = 74,                     /* MINUS  */
  YYSYMBOL_MULT = 75,                      /* MULT  */
  YYSYMBOL_DIV = 76,                       /* DIV  */
  YYSYMBOL_BAND = 77,                      /* BAND  */
  YYSYMBOL_BOR = 78,                       /* BOR  */
  YYSYMBOL_BXOR = 79,                      /* BXOR  */
  YYSYMBOL_MOD = 80,                       /* MOD  */
  YYSYMBOL_LSHIFT = 81,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 82,                    /* RSHIFT  */
  YYSYMBOL_UNRSHIFT = 83,                  /* UNRSHIFT  */
  YYSYMBOL_PLUSEQUAL = 84,                 /* PLUSEQUAL  */
  YYSYMBOL_MINUSEQUAL = 85,                /* MINUSEQUAL  */
  YYSYMBOL_MULTEQUAL = 86,                 /* MULTEQUAL  */
  YYSYMBOL_DIVEQUAL = 87,                  /* DIVEQUAL  */
  YYSYMBOL_BANDEQUAL = 88,                 /* BANDEQUAL  */
  YYSYMBOL_BOREQUAL = 89,                  /* BOREQUAL  */
  YYSYMBOL_BXOREQUAL = 90,                 /* BXOREQUAL  */
  YYSYMBOL_MODEQUAL = 91,                  /* MODEQUAL  */
  YYSYMBOL_LSHIFTEQUAL = 92,               /* LSHIFTEQUAL  */
  YYSYMBOL_RSHIFTEQUAL = 93,               /* RSHIFTEQUAL  */
  YYSYMBOL_UNRSHIFTEQUAL = 94,             /* UNRSHIFTEQUAL  */
  YYSYMBOL_IF = 95,                        /* IF  */
  YYSYMBOL_ELSE = 96,                      /* ELSE  */
  YYSYMBOL_WHILE = 97,                     /* WHILE  */
  YYSYMBOL_FOR = 98,                       /* FOR  */
  YYSYMBOL_RETURN = 99,                    /* RETURN  */
  YYSYMBOL_CONTINUE = 100,                 /* CONTINUE  */
  YYSYMBOL_BREAK = 101,                    /* BREAK  */
  YYSYMBOL_YIELD = 102,                    /* YIELD  */
  YYSYMBOL_SEALED = 103,                   /* SEALED  */
  YYSYMBOL_PROTECTED = 104,                /* PROTECTED  */
  YYSYMBOL_PUBLIC = 105,                   /* PUBLIC  */
  YYSYMBOL_PRIVATE = 106,                  /* PRIVATE  */
  YYSYMBOL_STRICTFP = 107,                 /* STRICTFP  */
  YYSYMBOL_ABSTRACT = 108,                 /* ABSTRACT  */
  YYSYMBOL_DEFAULT = 109,                  /* DEFAULT  */
  YYSYMBOL_INTERFACE = 110,                /* INTERFACE  */
  YYSYMBOL_PERMITS = 111,                  /* PERMITS  */
  YYSYMBOL_NONSEALED = 112,                /* NONSEALED  */
  YYSYMBOL_TRANSIENT = 113,                /* TRANSIENT  */
  YYSYMBOL_VOLATILE = 114,                 /* VOLATILE  */
  YYSYMBOL_NATIVE = 115,                   /* NATIVE  */
  YYSYMBOL_SYNCHRONIZED = 116,             /* SYNCHRONIZED  */
  YYSYMBOL_THROWS = 117,                   /* THROWS  */
  YYSYMBOL_ASSERT = 118,                   /* ASSERT  */
  YYSYMBOL_IDENTIFIER = 119,               /* IDENTIFIER  */
  YYSYMBOL_RECORD = 120,                   /* RECORD  */
  YYSYMBOL_LRSQUAREBRACKET = 121,          /* LRSQUAREBRACKET  */
  YYSYMBOL_SYSTEMOUTPRINTLN = 122,         /* SYSTEMOUTPRINTLN  */
  YYSYMBOL_YYACCEPT = 123,                 /* $accept  */
  YYSYMBOL_CompilationUnit = 124,          /* CompilationUnit  */
  YYSYMBOL_OrdinaryCompilationUnit = 125,  /* OrdinaryCompilationUnit  */
  YYSYMBOL_PackageDeclaration = 126,       /* PackageDeclaration  */
  YYSYMBOL_ModularCompilationUnit = 127,   /* ModularCompilationUnit  */
  YYSYMBOL_ImportDeclaration = 128,        /* ImportDeclaration  */
  YYSYMBOL_SingleTypeImportDeclaration = 129, /* SingleTypeImportDeclaration  */
  YYSYMBOL_TypeImportOnDemandDeclaration = 130, /* TypeImportOnDemandDeclaration  */
  YYSYMBOL_SingleStaticImportDeclaration = 131, /* SingleStaticImportDeclaration  */
  YYSYMBOL_StaticImportOnDemandDeclaration = 132, /* StaticImportOnDemandDeclaration  */
  YYSYMBOL_ModuleDeclaration = 133,        /* ModuleDeclaration  */
  YYSYMBOL_ModuleDirective = 134,          /* ModuleDirective  */
  YYSYMBOL_RequiresModifier = 135,         /* RequiresModifier  */
  YYSYMBOL_TopLevelClassOrInterfaceDeclaration = 136, /* TopLevelClassOrInterfaceDeclaration  */
  YYSYMBOL_TopLevelClassOrInterfaceDeclarations = 137, /* TopLevelClassOrInterfaceDeclarations  */
  YYSYMBOL_Name = 138,                     /* Name  */
  YYSYMBOL_DotIdentifiers = 139,           /* DotIdentifiers  */
  YYSYMBOL_CommaNames = 140,               /* CommaNames  */
  YYSYMBOL_Class = 141,                    /* Class  */
  YYSYMBOL_ClassDeclaration = 142,         /* ClassDeclaration  */
  YYSYMBOL_NormalClassDeclaration = 143,   /* NormalClassDeclaration  */
  YYSYMBOL_Modifier = 144,                 /* Modifier  */
  YYSYMBOL_Modifiers = 145,                /* Modifiers  */
  YYSYMBOL_TypeParameters = 146,           /* TypeParameters  */
  YYSYMBOL_TypeParameterList = 147,        /* TypeParameterList  */
  YYSYMBOL_ClassExtends = 148,             /* ClassExtends  */
  YYSYMBOL_ClassPermits = 149,             /* ClassPermits  */
  YYSYMBOL_ClassBody = 150,                /* ClassBody  */
  YYSYMBOL_ClassBodyDeclaration = 151,     /* ClassBodyDeclaration  */
  YYSYMBOL_ClassBodyDeclarations = 152,    /* ClassBodyDeclarations  */
  YYSYMBOL_ClassMemberDeclaration = 153,   /* ClassMemberDeclaration  */
  YYSYMBOL_FieldDeclaration = 154,         /* FieldDeclaration  */
  YYSYMBOL_VariableDeclaratorList = 155,   /* VariableDeclaratorList  */
  YYSYMBOL_VariableDeclarator = 156,       /* VariableDeclarator  */
  YYSYMBOL_CommaVariableDeclarators = 157, /* CommaVariableDeclarators  */
  YYSYMBOL_VariableDeclaratorId = 158,     /* VariableDeclaratorId  */
  YYSYMBOL_VariableInitializer = 159,      /* VariableInitializer  */
  YYSYMBOL_CommaVariableInitializers = 160, /* CommaVariableInitializers  */
  YYSYMBOL_leftcurl = 161,                 /* leftcurl  */
  YYSYMBOL_ArrayInitializer = 162,         /* ArrayInitializer  */
  YYSYMBOL_MethodDeclaration = 163,        /* MethodDeclaration  */
  YYSYMBOL_MethodHeader = 164,             /* MethodHeader  */
  YYSYMBOL_Result = 165,                   /* Result  */
  YYSYMBOL_MethodDeclarator = 166,         /* MethodDeclarator  */
  YYSYMBOL_ReceiverParameter = 167,        /* ReceiverParameter  */
  YYSYMBOL_FormalParameterList = 168,      /* FormalParameterList  */
  YYSYMBOL_CommaFormalParameters = 169,    /* CommaFormalParameters  */
  YYSYMBOL_FormalParameter = 170,          /* FormalParameter  */
  YYSYMBOL_VariableArityParameter = 171,   /* VariableArityParameter  */
  YYSYMBOL_VariableModifier = 172,         /* VariableModifier  */
  YYSYMBOL_VariableModifiers = 173,        /* VariableModifiers  */
  YYSYMBOL_MethodBody = 174,               /* MethodBody  */
  YYSYMBOL_InstanceInitializer = 175,      /* InstanceInitializer  */
  YYSYMBOL_StaticInitializer = 176,        /* StaticInitializer  */
  YYSYMBOL_ConstructorDeclaration = 177,   /* ConstructorDeclaration  */
  YYSYMBOL_ConstructorDeclarator = 178,    /* ConstructorDeclarator  */
  YYSYMBOL_ConstructorBody = 179,          /* ConstructorBody  */
  YYSYMBOL_ExplicitConstructorInvocation = 180, /* ExplicitConstructorInvocation  */
  YYSYMBOL_ArgumentList = 181,             /* ArgumentList  */
  YYSYMBOL_Block = 182,                    /* Block  */
  YYSYMBOL_BlockStatement = 183,           /* BlockStatement  */
  YYSYMBOL_BlockStatements = 184,          /* BlockStatements  */
  YYSYMBOL_LocalClassOrInterfaceDeclaration = 185, /* LocalClassOrInterfaceDeclaration  */
  YYSYMBOL_LocalVariableDeclarationStatement = 186, /* LocalVariableDeclarationStatement  */
  YYSYMBOL_LocalVariableDeclaration = 187, /* LocalVariableDeclaration  */
  YYSYMBOL_LocalVariableType = 188,        /* LocalVariableType  */
  YYSYMBOL_Statement = 189,                /* Statement  */
  YYSYMBOL_StatementNoShortIf = 190,       /* StatementNoShortIf  */
  YYSYMBOL_StatementWithoutTrailingSubstatement = 191, /* StatementWithoutTrailingSubstatement  */
  YYSYMBOL_DoStatement = 192,              /* DoStatement  */
  YYSYMBOL_EmptyStatement = 193,           /* EmptyStatement  */
  YYSYMBOL_LabeledStatement = 194,         /* LabeledStatement  */
  YYSYMBOL_LabeledStatementNoShortIf = 195, /* LabeledStatementNoShortIf  */
  YYSYMBOL_ExpressionStatement = 196,      /* ExpressionStatement  */
  YYSYMBOL_StatementExpression = 197,      /* StatementExpression  */
  YYSYMBOL_CommaStatementExpressions = 198, /* CommaStatementExpressions  */
  YYSYMBOL_IfThenStatement = 199,          /* IfThenStatement  */
  YYSYMBOL__StatementNoShortIf = 200,      /* _StatementNoShortIf  */
  YYSYMBOL_IfThenElseStatement = 201,      /* IfThenElseStatement  */
  YYSYMBOL_IfThenElseStatementNoShortIf = 202, /* IfThenElseStatementNoShortIf  */
  YYSYMBOL_AssertStatement = 203,          /* AssertStatement  */
  YYSYMBOL_WhileStatement = 204,           /* WhileStatement  */
  YYSYMBOL_WhileStatementNoShortIf = 205,  /* WhileStatementNoShortIf  */
  YYSYMBOL_ForStatement = 206,             /* ForStatement  */
  YYSYMBOL_ForStatementNoShortIf = 207,    /* ForStatementNoShortIf  */
  YYSYMBOL_BasicForStatement = 208,        /* BasicForStatement  */
  YYSYMBOL_BasicForStatementNoShortIf = 209, /* BasicForStatementNoShortIf  */
  YYSYMBOL_ForInit = 210,                  /* ForInit  */
  YYSYMBOL_ForUpdate = 211,                /* ForUpdate  */
  YYSYMBOL_StatementExpressionList = 212,  /* StatementExpressionList  */
  YYSYMBOL_EnhancedForStatement = 213,     /* EnhancedForStatement  */
  YYSYMBOL_EnhancedForStatementNoShortIf = 214, /* EnhancedForStatementNoShortIf  */
  YYSYMBOL_BreakStatement = 215,           /* BreakStatement  */
  YYSYMBOL_ContinueStatement = 216,        /* ContinueStatement  */
  YYSYMBOL_ReturnStatement = 217,          /* ReturnStatement  */
  YYSYMBOL_LeftRightSquareBrackets = 218,  /* LeftRightSquareBrackets  */
  YYSYMBOL_Primary = 219,                  /* Primary  */
  YYSYMBOL_PrimaryNoNewArray = 220,        /* PrimaryNoNewArray  */
  YYSYMBOL_ClassLiteral = 221,             /* ClassLiteral  */
  YYSYMBOL_NumericType = 222,              /* NumericType  */
  YYSYMBOL_IntegralType = 223,             /* IntegralType  */
  YYSYMBOL_FloatingPointType = 224,        /* FloatingPointType  */
  YYSYMBOL_ClassInstanceCreationExpression = 225, /* ClassInstanceCreationExpression  */
  YYSYMBOL_UnqualifiedClassInstanceCreationExpression = 226, /* UnqualifiedClassInstanceCreationExpression  */
  YYSYMBOL_FieldAccess = 227,              /* FieldAccess  */
  YYSYMBOL_ArrayAccess = 228,              /* ArrayAccess  */
  YYSYMBOL_MethodInvocation = 229,         /* MethodInvocation  */
  YYSYMBOL_MethodReference = 230,          /* MethodReference  */
  YYSYMBOL_ArrayCreationExpression = 231,  /* ArrayCreationExpression  */
  YYSYMBOL_DimExpr_ = 232,                 /* DimExpr_  */
  YYSYMBOL_Dims = 233,                     /* Dims  */
  YYSYMBOL_Expression = 234,               /* Expression  */
  YYSYMBOL_CommaExpressions = 235,         /* CommaExpressions  */
  YYSYMBOL_AssignmentExpression = 236,     /* AssignmentExpression  */
  YYSYMBOL_Assignment = 237,               /* Assignment  */
  YYSYMBOL_LeftHandSide = 238,             /* LeftHandSide  */
  YYSYMBOL_AssignmentOperatorEqual = 239,  /* AssignmentOperatorEqual  */
  YYSYMBOL_AssignmentOperator = 240,       /* AssignmentOperator  */
  YYSYMBOL_ColConditional = 241,           /* ColConditional  */
  YYSYMBOL_ConditionalExpression = 242,    /* ConditionalExpression  */
  YYSYMBOL_ConditionalOrExpression = 243,  /* ConditionalOrExpression  */
  YYSYMBOL_ConditionalAndExpression = 244, /* ConditionalAndExpression  */
  YYSYMBOL_InclusiveOrExpression = 245,    /* InclusiveOrExpression  */
  YYSYMBOL_ExclusiveOrExpression = 246,    /* ExclusiveOrExpression  */
  YYSYMBOL_AndExpression = 247,            /* AndExpression  */
  YYSYMBOL_EqualityExpression = 248,       /* EqualityExpression  */
  YYSYMBOL_RelationalExpression = 249,     /* RelationalExpression  */
  YYSYMBOL_InstanceofExpression = 250,     /* InstanceofExpression  */
  YYSYMBOL_ShiftExpression = 251,          /* ShiftExpression  */
  YYSYMBOL_AdditiveExpression = 252,       /* AdditiveExpression  */
  YYSYMBOL_MultiplicativeExpression = 253, /* MultiplicativeExpression  */
  YYSYMBOL_UnaryExpression = 254,          /* UnaryExpression  */
  YYSYMBOL_PreIncrementExpression = 255,   /* PreIncrementExpression  */
  YYSYMBOL_PreDecrementExpression = 256,   /* PreDecrementExpression  */
  YYSYMBOL_UnaryExpressionNotPlusMinus = 257, /* UnaryExpressionNotPlusMinus  */
  YYSYMBOL_CastExpression = 258,           /* CastExpression  */
  YYSYMBOL_PostfixExpression = 259,        /* PostfixExpression  */
  YYSYMBOL_PostIncrementExpression = 260,  /* PostIncrementExpression  */
  YYSYMBOL_PostDecrementExpression = 261,  /* PostDecrementExpression  */
  YYSYMBOL_Type = 262,                     /* Type  */
  YYSYMBOL_PrimitiveType = 263,            /* PrimitiveType  */
  YYSYMBOL_ReferenceType = 264,            /* ReferenceType  */
  YYSYMBOL_ArrayType = 265,                /* ArrayType  */
  YYSYMBOL_TypeParameter = 266,            /* TypeParameter  */
  YYSYMBOL_CommaTypeParameters = 267,      /* CommaTypeParameters  */
  YYSYMBOL_Literal = 268,                  /* Literal  */
  YYSYMBOL_ImportDeclarations = 269,       /* ImportDeclarations  */
  YYSYMBOL_ModuleDirectives = 270,         /* ModuleDirectives  */
  YYSYMBOL_RequiresModifiers = 271         /* RequiresModifiers  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   4654

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  123
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  149
/* YYNRULES -- Number of rules.  */
#define YYNRULES  441
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  864

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   377


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   360,   360,   394,   412,   426,   444,   457,   492,   496,
     528,   533,   538,   543,   551,   569,   590,   609,   632,   641,
     653,   666,   695,   705,   721,   734,   757,   765,   780,   788,
     797,   814,   822,   841,   849,   858,   878,   886,   898,   905,
     906,   926,   948,  1006,  1007,  1027,  1028,  1054,  1064,  1073,
    1094,  1113,  1140,  1158,  1190,  1226,  1260,  1300,  1317,  1335,
    1362,  1380,  1408,  1427,  1455,  1484,  1513,  1531,  1550,  1578,
    1593,  1618,  1638,  1670,  1677,  1683,  1690,  1699,  1700,  1720,
    1732,  1736,  1754,  1781,  1790,  1811,  1816,  1835,  1839,  1843,
    1847,  1854,  1855,  1875,  1879,  1883,  1892,  1953,  2033,  2100,
    2124,  2129,  2177,  2217,  2218,  2238,  2244,  2260,  2264,  2271,
    2274,  2291,  2298,  2305,  2340,  2354,  2370,  2404,  2427,  2488,
    2547,  2599,  2603,  2614,  2624,  2637,  2651,  2666,  2683,  2703,
    2724,  2752,  2764,  2776,  2783,  2810,  2811,  2831,  2838,  2871,
    2878,  2888,  2906,  2917,  2918,  2939,  2961,  2974,  2981,  2995,
    3016,  3060,  3094,  3108,  3167,  3220,  3233,  3248,  3301,  3361,
    3380,  3395,  3419,  3436,  3467,  3504,  3535,  3570,  3590,  3609,
    3633,  3659,  3701,  3741,  3769,  3801,  3830,  3836,  3858,  3862,
    3866,  3873,  3874,  3897,  3906,  3920,  3982,  4053,  4057,  4061,
    4072,  4076,  4080,  4084,  4088,  4092,  4099,  4103,  4107,  4111,
    4115,  4122,  4126,  4130,  4134,  4139,  4143,  4147,  4151,  4163,
    4208,  4222,  4238,  4254,  4271,  4275,  4279,  4283,  4287,  4291,
    4295,  4302,  4303,  4323,  4353,  4369,  4402,  4432,  4451,  4476,
    4503,  4536,  4540,  4547,  4551,  4558,  4599,  4622,  4645,  4678,
    4712,  4745,  4776,  4815,  4843,  4867,  4891,  4918,  4951,  4978,
    5010,  5048,  5052,  5059,  5072,  5076,  5087,  5103,  5117,  5133,
    5151,  5167,  5184,  5207,  5240,  5241,  5264,  5268,  5275,  5279,
    5283,  5288,  5299,  5317,  5321,  5325,  5329,  5333,  5340,  5353,
    5369,  5389,  5412,  5429,  5434,  5441,  5447,  5453,  5459,  5465,
    5474,  5480,  5489,  5493,  5509,  5528,  5564,  5597,  5632,  5666,
    5699,  5734,  5755,  5776,  5803,  5839,  5882,  5942,  5988,  6034,
    6057,  6082,  6123,  6166,  6209,  6256,  6312,  6339,  6384,  6426,
    6458,  6471,  6482,  6493,  6503,  6515,  6530,  6541,  6559,  6575,
    6591,  6616,  6620,  6638,  6642,  6658,  6680,  6684,  6701,  6725,
    6760,  6767,  6777,  6784,  6785,  6806,  6810,  6817,  6878,  6937,
    6941,  6945,  6962,  6970,  6975,  6980,  6985,  6990,  6995,  7000,
    7005,  7010,  7015,  7020,  7028,  7054,  7058,  7119,  7123,  7169,
    7173,  7218,  7222,  7264,  7268,  7310,  7314,  7356,  7360,  7398,
    7440,  7444,  7483,  7521,  7560,  7600,  7607,  7633,  7637,  7676,
    7715,  7758,  7762,  7806,  7850,  7854,  7894,  7933,  7975,  7979,
    7983,  8007,  8031,  8037,  8070,  8102,  8106,  8130,  8155,  8162,
    8210,  8214,  8218,  8222,  8229,  8266,  8300,  8304,  8311,  8315,
    8321,  8330,  8334,  8347,  8371,  8398,  8403,  8411,  8412,  8449,
    8458,  8466,  8474,  8483,  8491,  8499,  8519,  8520,  8539,  8540,
    8559,  8560
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "BOOLEAN", "CHAR",
  "BYTE", "SHORT", "INT", "LONG", "FLOAT", "DOUBLE", "VOID", "EXTENDS",
  "SUPER", "STRING", "THIS", "VAR", "INSTANCEOF", "FINAL", "NEW",
  "CHARACTERLITERAL", "STRINGLITERAL", "TEXTBLOCK", "NULLLITERAL", "CLASS",
  "PACKAGE", "IMPORT", "STATIC", "DO", "INTEGERLITERAL",
  "FLOATINGPOINTLITERAL", "BOOLEANLITERAL", "JAVALETTER",
  "JAVALETTERORDIGIT", "OPEN", "MODULE", "REQUIRES", "EXPORTS", "OPENS",
  "USES", "PROVIDES", "TO", "WITH", "TRANSITIVE", "LEFTSQUAREBRACKET",
  "RIGHTSQUAREBRACKET", "LEFTCURLYBRACKET", "RIGHTCURLYBRACKET",
  "LEFTPARENTHESIS", "RIGHTPARENTHESIS", "SEMICOLON", "COMMA", "DOT",
  "ELLIPSIS", "AT", "DOUBLECOLON", "ASSIGN", "GRT", "LSS", "NOT", "TIL",
  "QUES", "COL", "ARW", "EQUAL", "GEQ", "IMPLEMENTS", "LEQ", "NEQUAL",
  "AND", "OR", "INCRE", "DECRE", "PLUS", "MINUS", "MULT", "DIV", "BAND",
  "BOR", "BXOR", "MOD", "LSHIFT", "RSHIFT", "UNRSHIFT", "PLUSEQUAL",
  "MINUSEQUAL", "MULTEQUAL", "DIVEQUAL", "BANDEQUAL", "BOREQUAL",
  "BXOREQUAL", "MODEQUAL", "LSHIFTEQUAL", "RSHIFTEQUAL", "UNRSHIFTEQUAL",
  "IF", "ELSE", "WHILE", "FOR", "RETURN", "CONTINUE", "BREAK", "YIELD",
  "SEALED", "PROTECTED", "PUBLIC", "PRIVATE", "STRICTFP", "ABSTRACT",
  "DEFAULT", "INTERFACE", "PERMITS", "NONSEALED", "TRANSIENT", "VOLATILE",
  "NATIVE", "SYNCHRONIZED", "THROWS", "ASSERT", "IDENTIFIER", "RECORD",
  "LRSQUAREBRACKET", "SYSTEMOUTPRINTLN", "$accept", "CompilationUnit",
  "OrdinaryCompilationUnit", "PackageDeclaration",
  "ModularCompilationUnit", "ImportDeclaration",
  "SingleTypeImportDeclaration", "TypeImportOnDemandDeclaration",
  "SingleStaticImportDeclaration", "StaticImportOnDemandDeclaration",
  "ModuleDeclaration", "ModuleDirective", "RequiresModifier",
  "TopLevelClassOrInterfaceDeclaration",
  "TopLevelClassOrInterfaceDeclarations", "Name", "DotIdentifiers",
  "CommaNames", "Class", "ClassDeclaration", "NormalClassDeclaration",
  "Modifier", "Modifiers", "TypeParameters", "TypeParameterList",
  "ClassExtends", "ClassPermits", "ClassBody", "ClassBodyDeclaration",
  "ClassBodyDeclarations", "ClassMemberDeclaration", "FieldDeclaration",
  "VariableDeclaratorList", "VariableDeclarator",
  "CommaVariableDeclarators", "VariableDeclaratorId",
  "VariableInitializer", "CommaVariableInitializers", "leftcurl",
  "ArrayInitializer", "MethodDeclaration", "MethodHeader", "Result",
  "MethodDeclarator", "ReceiverParameter", "FormalParameterList",
  "CommaFormalParameters", "FormalParameter", "VariableArityParameter",
  "VariableModifier", "VariableModifiers", "MethodBody",
  "InstanceInitializer", "StaticInitializer", "ConstructorDeclaration",
  "ConstructorDeclarator", "ConstructorBody",
  "ExplicitConstructorInvocation", "ArgumentList", "Block",
  "BlockStatement", "BlockStatements", "LocalClassOrInterfaceDeclaration",
  "LocalVariableDeclarationStatement", "LocalVariableDeclaration",
  "LocalVariableType", "Statement", "StatementNoShortIf",
  "StatementWithoutTrailingSubstatement", "DoStatement", "EmptyStatement",
  "LabeledStatement", "LabeledStatementNoShortIf", "ExpressionStatement",
  "StatementExpression", "CommaStatementExpressions", "IfThenStatement",
  "_StatementNoShortIf", "IfThenElseStatement",
  "IfThenElseStatementNoShortIf", "AssertStatement", "WhileStatement",
  "WhileStatementNoShortIf", "ForStatement", "ForStatementNoShortIf",
  "BasicForStatement", "BasicForStatementNoShortIf", "ForInit",
  "ForUpdate", "StatementExpressionList", "EnhancedForStatement",
  "EnhancedForStatementNoShortIf", "BreakStatement", "ContinueStatement",
  "ReturnStatement", "LeftRightSquareBrackets", "Primary",
  "PrimaryNoNewArray", "ClassLiteral", "NumericType", "IntegralType",
  "FloatingPointType", "ClassInstanceCreationExpression",
  "UnqualifiedClassInstanceCreationExpression", "FieldAccess",
  "ArrayAccess", "MethodInvocation", "MethodReference",
  "ArrayCreationExpression", "DimExpr_", "Dims", "Expression",
  "CommaExpressions", "AssignmentExpression", "Assignment", "LeftHandSide",
  "AssignmentOperatorEqual", "AssignmentOperator", "ColConditional",
  "ConditionalExpression", "ConditionalOrExpression",
  "ConditionalAndExpression", "InclusiveOrExpression",
  "ExclusiveOrExpression", "AndExpression", "EqualityExpression",
  "RelationalExpression", "InstanceofExpression", "ShiftExpression",
  "AdditiveExpression", "MultiplicativeExpression", "UnaryExpression",
  "PreIncrementExpression", "PreDecrementExpression",
  "UnaryExpressionNotPlusMinus", "CastExpression", "PostfixExpression",
  "PostIncrementExpression", "PostDecrementExpression", "Type",
  "PrimitiveType", "ReferenceType", "ArrayType", "TypeParameter",
  "CommaTypeParameters", "Literal", "ImportDeclarations",
  "ModuleDirectives", "RequiresModifiers", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-503)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-422)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     295,   -12,    17,   103,    83,    85,  -503,   198,  -503,   198,
    -503,  -503,  -503,  -503,  -503,   250,   325,   154,  -503,   352,
     172,   316,  -503,   198,   250,   202,    89,   222,  -503,  -503,
    -503,  -503,   250,  -503,    54,  -503,  -503,    89,  -503,   232,
     383,  -503,    99,   321,   409,   272,  -503,  -503,  -503,    89,
     164,  -503,  -503,   154,  2479,   303,   154,    69,    46,   394,
    -503,   164,   377,  -503,   166,   393,  -503,   454,   331,    12,
     154,   154,   154,   154,  -503,   598,   377,  -503,    54,   431,
    -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,   392,  -503,
      89,   449,  2263,  -503,   148,  -503,    89,   780,  2599,  -503,
    -503,  -503,    64,  -503,  -503,  -503,   456,  -503,  -503,  -503,
    -503,   410,   426,  -503,  -503,   551,   514,   526,   407,    46,
     394,  -503,   394,  -503,  -503,    54,   462,   539,   547,  -503,
    -503,   598,   377,  -503,  -503,   201,   412,   274,   290,   421,
     236,   598,   559,   553,    69,    46,   394,  -503,   508,   562,
    -503,   966,  -503,   576,   234,  -503,  -503,    63,  1094,  -503,
    -503,  -503,  -503,  2783,  -503,  -503,  -503,  -503,  3969,  -503,
    3969,  3969,   581,   592,   594,  1602,    73,   100,  3969,   582,
     595,   355,  -503,   630,  -503,  2663,  -503,  -503,   602,   540,
    -503,  -503,  -503,  -503,  -503,  -503,   610,  -503,  -503,  -503,
    -503,  -503,  -503,  -503,  -503,  -503,  -503,   324,  -503,  -503,
     314,  -503,  1084,  1165,   419,  -503,  -503,  -503,  1421,  -503,
    -503,   408,   427,   530,   121,  -503,   606,  -503,   938,   426,
    -503,   966,  -503,   169,   392,  -503,  2599,   615,  -503,  -503,
    -503,  2423,  -503,     8,   613,   616,   608,  -503,  -503,  -503,
    -503,   303,   154,   394,  -503,  -503,  -503,    69,    46,   394,
    -503,   377,  -503,  -503,   619,   622,   201,   154,  -503,   154,
    -503,   154,  -503,  -503,   154,  -503,  -503,   477,    46,   394,
    -503,   394,  -503,  -503,  1066,   160,   780,    64,   410,   645,
     554,   555,    50,    -3,  2132,   573,   121,   606,  3969,  3969,
    3969,  3969,  3969,  -503,  -503,   627,  -503,  -503,  -503,   241,
     612,   599,   600,   607,   242,   279,  -503,   444,   534,   337,
    -503,  -503,  -503,  -503,  -503,   408,  -503,  -503,   161,  -503,
    -503,  -503,  -503,  3969,  3969,  4042,  -503,   633,  -503,   636,
    -503,   637,    78,  2783,  1402,  3969,  2957,    18,    41,   171,
    -503,   630,   378,  2663,   641,  -503,   426,  -503,  -503,    45,
     570,  -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,
    -503,  -503,  -503,  3969,  3969,  -503,  -503,   667,   171,    51,
    -503,   642,   643,   646,  -503,   630,    35,  -503,    64,   456,
     410,  1480,  -503,  -503,  -503,   345,   648,  -503,   561,  2543,
    2663,   346,  -503,  -503,   540,  -503,  3029,   647,   552,  -503,
      46,   394,  -503,   394,  -503,  -503,  -503,  -503,   520,  -503,
     425,   487,   566,   491,  -503,   598,   394,  -503,  -503,  -503,
     426,   650,   653,  -503,   644,  -503,   657,  -503,  3969,  3101,
     588,  -503,    31,  -503,   664,   663,   178,  -503,  -503,  -503,
    -503,  -503,  3969,  3969,  3969,  3969,  3969,  3969,  3969,  3969,
    1094,  3969,  3969,  3969,  3969,  3969,  3969,  3969,  3969,  3969,
    3969,  3969,  3969,   665,   666,  3174,   651,   668,   670,  -503,
    -503,  -503,  -503,  -503,  3969,  -503,  -503,   672,   677,  -503,
     674,   435,   363,  -503,   154,  -503,  -503,  -503,  -503,   171,
     673,  -503,   540,   426,  -503,  -503,   676,  -503,  -503,  -503,
    -503,  -503,   678,  -503,  -503,  1732,  -503,  1224,  1094,  -503,
     623,   177,  -503,  -503,  -503,   679,  -503,   680,   685,  3246,
    3318,    59,  -503,  2663,   697,    15,   616,  -503,  2050,  -503,
    -503,   687,  -503,   154,  -503,   394,  -503,  -503,  -503,  -503,
     598,  -503,  -503,   154,   154,  -503,   154,   699,  -503,  -503,
    1751,   426,  -503,  3390,   702,   394,   700,   471,   631,    62,
    -503,  3969,  3969,   686,  3969,   612,   599,   600,   607,   242,
     279,   279,   426,  -503,   444,   444,   444,   444,   534,   534,
     534,   337,   337,  -503,  -503,  -503,  2903,  2783,  1319,   705,
    3969,  4532,  3463,   707,  -503,   714,  -503,  -503,  3969,   640,
     655,   301,  -503,   736,  -503,  3535,   738,  -503,   717,    58,
     712,   157,  -503,   752,  -503,  1798,  -503,   719,   726,   484,
     728,   730,   495,   376,   733,  -503,   744,  -503,  -503,   746,
     745,  -503,  -503,   647,   552,  -503,   748,   552,   552,   552,
    -503,   426,   754,  -503,  -503,   755,   504,    40,  -503,   394,
     394,  -503,    86,   756,  -503,  -503,  -503,   749,   753,   758,
     747,  -503,   711,   715,  -503,  2783,  -503,  -503,  -503,  -503,
    -503,  -503,  2783,   761,  -503,  4166,   763,   762,  4197,   764,
    -503,  3969,   765,   767,  -503,  -503,  -503,   768,   513,  -503,
    -503,  1224,  -503,   701,  -503,  -503,  -503,   769,  -503,   772,
     774,  -503,   776,   777,  3607,  -503,  3679,  -503,  3029,   781,
    -503,  -503,  -503,   779,   784,   789,  -503,   426,  -503,  -503,
    3969,   709,  -503,  -503,   790,  3969,  3969,  4112,  2903,  -503,
    -503,  -503,  2783,  2783,   782,  2783,  4532,  -503,  2783,   792,
    4251,   797,  3969,  -503,  3751,  -503,  -503,   712,  -503,  -503,
    -503,  -503,  -503,  -503,   794,   796,   521,   798,   800,   533,
     745,  -503,  -503,  -503,  -503,  -503,   806,  -503,  -503,   804,
     805,  3824,   793,   808,  -503,  -503,  -503,  2783,  -503,   762,
    -503,  2783,  2783,   807,   815,   765,  -503,   812,   543,  -503,
    -503,   814,   819,  -503,   821,   822,  -503,    42,  2903,  2903,
    4323,   824,  3969,  3897,  -503,  -503,  -503,  -503,  2783,  3969,
    -503,  -503,  -503,  -503,  -503,  -503,  -503,  3969,  -503,  2903,
    -503,  2903,   816,  4377,   826,  4408,   827,  -503,   831,   833,
    -503,  -503,  2903,  2903,   832,  2903,  2903,   834,  4462,  -503,
    -503,  -503,  -503,  2903,  -503,  -503,  2903,  2903,   835,  -503,
    -503,  -503,  2903,  -503
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     436,     0,     0,     0,     0,     0,     2,   436,     3,   436,
      10,    11,    12,    13,     8,    39,     0,     0,    41,     0,
       0,     0,     1,   436,    39,   437,    77,     0,    76,    74,
      73,    75,    39,     5,     0,    38,    48,    77,     6,     0,
       0,    14,     0,     0,     0,     0,   437,     4,     9,    77,
       0,    47,    40,     0,     0,     0,     0,     0,     0,     0,
      49,     0,    43,    16,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,    18,   438,    43,    78,     0,    82,
     419,   289,   285,   286,   287,   288,   290,   291,     0,   420,
      77,    76,     0,    85,   421,    95,    77,     0,    91,    87,
      93,    94,     0,    88,    89,    90,     0,   147,   418,   283,
     284,     0,   416,   417,   422,   425,     0,    80,    83,     0,
       0,    50,     0,    51,    52,     0,     0,     0,     0,    15,
      22,   438,    43,    37,    36,   440,     0,     0,     0,     0,
       0,   438,     0,     0,     0,     0,     0,    66,     0,     0,
     119,     0,   148,     0,     0,   270,   189,   142,     0,   433,
     432,   435,   434,     0,   429,   430,   431,   176,     0,   210,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
       0,   349,   183,   143,   201,   181,   178,   179,     0,     0,
     180,   190,   205,   202,   191,   203,     0,   192,   193,   204,
     194,   195,   231,   232,   206,   207,   208,   410,   266,   269,
     273,   292,   274,   275,   276,   277,   267,   214,     0,   215,
     216,     0,   217,   218,   187,   188,   422,   268,     0,   340,
     424,     0,   122,   421,     0,   121,    91,     0,   146,   115,
     145,     0,   149,   105,     0,   103,   100,   118,   423,   426,
      79,     0,     0,     0,    53,    54,    55,     0,     0,     0,
      57,    43,     7,    17,     0,     0,   440,     0,    26,     0,
      28,     0,    31,    33,     0,   439,    20,     0,     0,     0,
      67,     0,    68,    69,     0,   421,     0,     0,     0,     0,
       0,     0,     0,     0,   411,     0,     0,     0,     0,     0,
       0,     0,     0,   273,   276,     0,   342,   346,   345,   365,
     367,   369,   371,   373,   375,   377,   385,   380,   387,   391,
     394,   398,   399,   402,   408,   405,   412,   413,   411,   274,
     275,   403,   404,     0,     0,     0,   262,     0,   260,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,   340,
     142,   143,     0,   181,     0,   184,   105,   185,   213,     0,
       0,   352,   356,   357,   353,   354,   361,   362,   363,   355,
     358,   359,   360,     0,     0,   414,   415,     0,   340,     0,
     151,     0,     0,   133,   139,   143,     0,   341,     0,     0,
       0,     0,   120,    92,    86,     0,   270,   159,   349,     0,
     181,   410,   106,    96,     0,    99,     0,   427,    45,    56,
       0,     0,    58,     0,    59,    60,    44,    24,     0,   441,
       0,     0,     0,     0,    19,   438,     0,    70,    71,    72,
     123,     0,     0,   117,     0,   282,   302,   324,     0,     0,
     331,   329,   331,   328,     0,     0,     0,   407,   406,   400,
     401,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   252,   254,     0,   251,
     263,   261,   259,   227,     0,   211,   320,     0,     0,   307,
       0,     0,     0,   271,     0,   278,   293,   326,   321,   340,
       0,   144,     0,   187,   182,   177,   301,   294,   322,   347,
     348,   280,     0,   327,   323,     0,   153,     0,     0,   132,
       0,   105,   137,   116,   150,     0,   155,     0,     0,     0,
       0,     0,   160,   181,     0,     0,   103,   111,     0,   102,
     101,     0,    81,     0,    84,     0,    61,    62,    63,    23,
     438,    27,    29,     0,     0,    34,     0,     0,    65,   127,
       0,   125,    98,     0,     0,   298,     0,     0,   333,   333,
     330,     0,     0,     0,     0,   368,   370,   372,   374,   376,
     378,   379,     0,   386,   382,   381,   384,   383,   388,   389,
     390,   392,   393,   395,   396,   397,     0,     0,     0,     0,
       0,     0,     0,     0,   319,   304,   308,   315,     0,     0,
       0,     0,   265,     0,   186,     0,     0,   152,     0,     0,
     135,     0,   140,     0,    97,     0,   157,     0,     0,     0,
       0,     0,     0,     0,     0,   161,     0,   104,   112,     0,
     109,   108,   107,   427,    45,    64,     0,    45,    45,    45,
      21,   124,     0,   129,   311,     0,     0,   332,   296,   297,
     300,   336,   336,     0,   409,   364,   366,     0,     0,     0,
      41,   223,     0,   190,   197,     0,   198,   199,   200,   233,
     234,   229,     0,     0,   253,     0,     0,   221,     0,     0,
     228,     0,   343,   303,   325,   279,   309,     0,     0,   281,
     154,     0,   134,     0,   138,   131,   156,     0,   165,     0,
       0,   163,     0,     0,     0,   162,     0,   114,     0,     0,
     428,    46,    25,     0,     0,     0,   128,   126,   312,   317,
       0,   334,   295,   299,     0,     0,     0,     0,     0,   224,
     225,   236,     0,     0,     0,     0,     0,   255,     0,     0,
       0,     0,     0,   175,     0,   310,   316,   135,   141,   158,
     166,   172,   164,   171,     0,     0,     0,     0,     0,     0,
     109,   113,    30,    32,    35,   130,     0,   337,   209,     0,
       0,     0,   252,     0,   212,   239,   238,     0,   256,   221,
     237,     0,     0,     0,   305,   343,   313,     0,     0,   136,
     167,     0,     0,   169,     0,     0,   110,   335,     0,     0,
       0,     0,     0,     0,   242,   222,   241,   240,     0,     0,
     344,   314,   318,   168,   173,   170,   174,     0,   338,     0,
     230,     0,     0,     0,     0,     0,     0,   235,     0,     0,
     226,   244,     0,     0,     0,     0,     0,     0,     0,   306,
     339,   247,   246,     0,   257,   245,     0,     0,     0,   250,
     249,   248,     0,   243
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -503,  -503,  -503,  -503,  -503,   879,  -503,  -503,  -503,  -503,
     857,   -38,   817,  -503,   228,  1478,   -57,  -210,   -34,    11,
    -503,    -1,   213,   -29,  -503,    47,     1,   809,   836,   649,
    -503,  -503,  -175,   485,   356,  -364,   170,   123,  -503,  -181,
    -503,  -104,  -503,   -68,  -261,  -267,   134,  -502,  -503,  -165,
    -250,  -249,  -503,  -503,  -503,   675,   505,  -503,  -382,   -22,
     -83,  -346,  -503,  -503,  -332,   544,  1045,  -231,  -277,  -503,
    -503,  -503,  -503,  -503,  -314,   108,  -503,    90,  -503,  -503,
    -503,  -503,  -503,  -503,  -503,  -503,  -503,   167,  -391,  -331,
    -503,  -503,  -503,  -503,  -503,  -348,   662,  -503,  -503,  -503,
    -503,  -503,   -92,  -286,   830,  1674,   -10,  -503,  -503,   614,
     -21,  1636,   113,  -503,  1476,  -503,  -503,  -503,  -503,   335,
    -503,   457,   460,   461,   455,   458,   162,  -503,    -9,    84,
     155,  -158,   175,   304,  -503,  -503,   527,   710,   848,   -46,
     -52,   -82,   -30,   671,   276,  -503,   257,  -130,   658
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     5,     6,     7,     8,    23,    10,    11,    12,    13,
      14,   141,   266,    32,    33,   294,   127,   544,    34,   182,
      36,    37,    50,    97,   116,    58,    59,    60,   236,   237,
      99,   100,   244,   245,   405,   246,   640,   719,   538,   641,
     101,   102,   234,   247,   381,   382,   702,   383,   384,   183,
     352,   239,   103,   104,   105,   106,   242,   399,   490,   184,
     353,   354,   186,   187,   188,   189,   190,   672,   191,   192,
     193,   194,   674,   195,   196,   747,   197,   675,   198,   676,
     199,   200,   677,   201,   678,   202,   679,   478,   683,   684,
     203,   680,   204,   205,   206,   500,   207,   208,   209,   108,
     109,   110,   303,   211,   212,   213,   304,   215,   216,   441,
     230,   305,   753,   306,   217,   218,   373,   374,   574,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   619,
     296,   113,   297,   117,   542,   227,    15,   142,   267
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     210,   264,   112,   476,   479,    57,    75,   504,   111,   185,
     225,   275,   331,   332,   357,   620,    78,   432,   351,   143,
     150,   477,   522,   431,   114,    49,    35,   125,   636,   131,
     512,   492,   107,   493,   494,    35,    49,   494,   433,   133,
     224,   438,   495,    35,    17,   112,   112,   287,    49,   144,
     519,   235,   111,    96,   534,   134,   284,   566,   120,   122,
     497,   496,   226,   385,   494,    95,    53,   114,   114,   152,
     513,   210,   633,   507,   493,   265,   107,  -340,   494,   146,
     240,    53,   214,   495,   730,    22,   827,   -77,   520,    49,
      28,   248,    54,   210,   438,    49,   257,    96,   439,   112,
      54,   501,   148,   225,   119,   288,   293,    16,  -340,    95,
      92,   520,    55,   434,   238,    54,    28,    78,   442,   385,
     253,   114,   286,   338,   528,   145,   259,   388,   483,   229,
     527,    18,  -340,   224,   506,   518,    18,    66,    20,   523,
     484,   447,   448,   449,   450,   279,   281,   628,   631,   210,
     340,   612,   569,   214,   521,   226,    49,    56,   400,   225,
     498,   731,   249,   828,   506,    56,   392,    29,    30,    31,
     514,   440,   258,   377,    65,   214,   112,   356,    66,   112,
      56,   655,   386,   662,   112,   390,   351,   634,    27,   224,
     111,   278,   339,    29,    30,    31,   228,   125,   114,   757,
     148,   114,    21,   248,   416,   345,   114,   229,   387,   346,
     703,   226,   148,   347,   107,   525,   348,   391,    66,   341,
     351,   148,   402,  -264,     2,   539,   385,   572,   133,   623,
     377,   214,   112,   697,   112,    96,     3,     4,   386,   425,
     235,   128,   378,   210,   134,   496,   446,    95,   618,   507,
      61,   210,    47,   225,   114,   522,   114,   704,   411,   413,
      52,   210,    77,   570,    24,   240,    25,   219,    26,   229,
     225,   225,   444,    18,    27,   248,   356,    28,   274,   426,
      46,   229,   349,   224,   477,    66,   290,   687,   148,   291,
     229,    43,   499,   652,   744,   557,   460,   749,   229,   378,
     503,   224,   452,   151,   410,   226,   458,   210,   210,   231,
     459,   453,   593,   594,   595,   269,   533,   225,   225,   673,
       1,     2,   114,   226,   270,   214,   148,   614,   387,     3,
       4,   271,   765,   214,   768,   402,   461,   462,   219,   112,
     272,    51,   148,   214,   463,   386,   464,   224,   224,   439,
     385,    62,   385,   148,    29,    30,    31,   387,   707,   793,
     219,   114,    44,  -220,  -220,  -220,   240,    67,    45,   226,
     226,   477,   797,    68,   477,    38,   359,    39,   583,   360,
     550,    80,    81,    82,    83,    84,    85,    86,    87,   214,
     214,    76,    89,   529,   156,   385,   220,   290,   535,   345,
     291,   360,    41,   346,    42,   782,   479,   347,   582,   559,
     348,   545,   470,   471,   664,   609,   219,   472,   610,   832,
     646,   387,   115,   477,   714,   248,  -411,  -411,   609,   126,
     114,   610,   789,    63,   721,    64,   477,   723,   724,   725,
      54,   210,   844,   129,   847,    69,    70,    71,    72,    73,
     132,   225,   584,   585,   586,   587,    74,   858,   252,   148,
     385,   673,   268,   112,   148,   112,   112,   220,  -219,  -219,
    -219,   273,   621,   148,  -421,   551,   349,   148,   387,   375,
     376,   224,   248,   148,   607,   114,   608,   114,   114,   220,
      69,    70,    71,    72,    73,    92,   477,    18,  -412,  -412,
     402,   130,   241,   226,   210,   210,   210,   784,   112,   210,
     219,   149,   643,    69,    70,    71,    72,    73,   219,   477,
     660,   477,   608,   214,   424,   465,   466,   467,   219,   243,
     114,   673,   673,   710,   477,   608,   385,   552,   553,   148,
     653,   555,   556,   148,   713,   220,   608,   229,   387,   588,
     589,   590,   673,   729,   673,   608,    69,    70,    71,    72,
      73,   248,   756,    53,   608,   673,   673,   549,   673,   673,
     802,   250,   608,   112,   219,   219,   673,   251,   830,   673,
     673,   261,   805,   210,   608,   673,   214,   214,   214,   262,
     210,   214,   822,   210,   608,   114,   210,   263,   840,   277,
     841,  -413,  -413,   543,   148,   345,   276,   468,   469,   346,
     284,   851,   852,   531,   854,   855,   348,   554,   148,   221,
     580,   581,   859,   591,   592,   860,   861,    66,   289,   333,
     726,   863,  -411,  -411,    69,    70,    71,    72,    73,   220,
     334,   387,   335,   344,   343,   210,   210,   220,   350,   112,
     210,   210,   355,   210,   210,   225,   210,   220,   210,   356,
     358,   379,   394,   403,   406,   214,   417,   404,   418,   435,
     445,   114,   214,   436,   437,   214,   451,   455,   214,   456,
    -421,   454,   349,   480,   457,   224,   481,   482,   505,   508,
     221,   511,   516,   515,   562,   210,   530,   517,   541,   210,
     210,   560,   561,   220,   220,   563,   775,   226,   219,   568,
     537,   571,   221,   600,   596,   597,   210,   210,   210,   601,
     602,   604,   605,   606,   615,   613,   210,   214,   214,   624,
     616,   625,   214,   214,   626,   214,   214,   210,   214,   210,
     214,   210,   622,   210,   635,    55,   650,   657,   665,   659,
     210,   210,   661,   210,   210,   685,   210,   690,   691,   693,
     695,   210,   699,   701,   210,   210,   700,   705,   221,   708,
     210,   219,   219,   219,   694,   709,   219,   214,   711,   712,
     715,   214,   214,    80,    81,    82,    83,    84,    85,    86,
      87,   232,   716,   717,    89,   722,   718,   735,   214,   214,
     214,   736,   222,   727,   728,   734,   737,   739,   214,   738,
     742,  -196,   745,   746,   750,   754,   752,   755,   759,   214,
     758,   214,   760,   214,   761,   214,   762,   763,   771,   772,
     777,   787,   214,   214,   773,   214,   214,   220,   214,   774,
     778,   791,   794,   214,   800,   801,   214,   214,   803,   804,
     219,   807,   214,   808,   809,   812,   818,   219,   813,   819,
     219,   821,   221,   219,   823,   842,   121,   123,   124,   824,
     221,   825,   826,   222,   833,   845,   849,   848,   850,     9,
     221,   853,    48,   856,   862,   393,   135,   147,   770,   536,
      98,   799,   637,   806,   524,   222,   502,   815,   829,    18,
     220,   220,   220,   401,   783,   220,   389,   443,   820,   666,
     575,   578,   219,   219,   576,   579,   577,   219,   219,   720,
     219,   219,   407,   219,   419,   219,   221,   221,   254,   255,
       0,   256,     0,     0,   260,     0,     0,     0,     0,     0,
     223,    80,    81,    82,    83,    84,    85,    86,    87,     0,
       0,   222,    89,   280,   282,   283,   350,     0,     0,     0,
       0,     0,   219,     0,     0,     0,   219,   219,     0,    80,
      81,    82,    83,    84,    85,    86,    87,    88,     0,   220,
      89,     0,     0,   219,   219,   219,   220,   380,     0,   220,
      27,     0,   220,   219,     0,     0,     0,     0,     0,     0,
     329,   329,     0,     0,   219,     0,   219,     0,   219,     0,
     219,   223,     0,     0,     0,     0,     0,   219,   219,     0,
     219,   219,     0,   219,    55,     0,     0,     0,   219,     0,
       0,   219,   219,   223,     0,     0,     0,   219,     0,     0,
       0,   220,   220,     0,     0,   222,   220,   220,     0,   220,
     220,     0,   220,   222,   220,     0,     0,    18,     0,     0,
     221,     0,   409,   222,     0,     0,   412,   414,   415,    80,
      81,    82,    83,    84,    85,    86,    87,     0,     0,     0,
      89,     0,     0,     0,   350,    18,     0,   427,   428,   223,
     429,   220,     0,     0,     0,   220,   220,    80,    81,    82,
      83,    84,    85,    86,    87,     0,     0,     0,    89,   222,
     222,     0,   220,   220,   220,   430,     0,     0,     0,     0,
       0,     0,   220,   221,   221,   221,     0,     0,   221,   329,
     329,   329,   329,   220,     0,   220,     0,   220,     0,   220,
    -350,     0,     0,     0,     0,     0,   220,   220,     0,   220,
     220,     0,   220,     0,     0,     0,     0,   220,     0,     0,
     220,   220,     0,     0,     0,     0,   220,     0,  -350,  -350,
    -350,  -350,  -350,  -350,  -350,  -350,  -350,  -350,  -350,     0,
       0,     0,     0,   223,     0,    18,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   223,   221,     0,     0,     0,     0,     0,   295,   221,
       0,     0,   221,    18,     0,   221,     0,     0,     0,   546,
     547,  -351,   548,     0,     0,     0,     0,    80,    81,    82,
      83,    84,    85,    86,    87,   558,     0,     0,    89,     0,
       0,     0,   350,   222,     0,     0,     0,   223,   223,  -351,
    -351,  -351,  -351,  -351,  -351,  -351,  -351,  -351,  -351,  -351,
       0,     0,     0,     0,   221,   221,     0,     0,     0,   221,
     221,     0,   221,   221,     0,   221,     0,   221,     0,     0,
       0,     0,     0,   329,   329,   329,   329,   329,   329,   329,
       0,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,     0,     0,     0,   222,   222,   222,     0,
       0,   222,     0,     0,   221,     0,     0,     0,   221,   221,
       0,     0,    80,    81,    82,    83,    84,    85,    86,    87,
     153,     0,   154,    89,   155,   221,   221,   221,   158,   159,
     160,   161,   162,    18,     0,   221,     0,     0,   164,   165,
     166,     0,     0,     0,   645,     0,   221,     0,   221,     0,
     221,     0,   221,     0,     0,     0,     0,   168,   682,   221,
     221,     0,   221,   221,   658,   221,     0,     0,     0,     0,
     221,   223,     0,   221,   221,   222,     0,     0,   485,   221,
     170,   171,   222,     0,     0,   222,     0,     0,   222,     0,
       0,     0,   329,     0,   329,    80,    81,    82,    83,    84,
      85,    86,    87,   153,     0,   154,    89,   155,     0,     0,
       0,   158,   159,   160,   161,   162,     0,     0,     0,     0,
       0,   164,   165,   166,     0,     0,     0,     0,    18,     0,
       0,   180,     0,     0,   223,   223,   223,   222,   222,   223,
     298,   486,   222,   222,     0,   222,   222,     0,   222,     0,
     222,   299,   300,     0,     0,     0,     0,     0,   732,   733,
       0,     0,     0,   170,   171,   301,   302,   361,     0,     0,
      19,     0,     0,    80,    81,    82,    83,    84,    85,    86,
      87,     0,     0,     0,    89,    40,     0,   222,   350,     0,
       0,   222,   222,     0,     0,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,     0,     0,   222,   222,
     222,    18,     0,   223,   180,     0,     0,     0,   222,   526,
     223,    79,    94,   223,   118,     0,   223,     0,     0,   222,
       0,   222,     0,   222,     0,   222,     0,   136,   137,   138,
     139,   140,   222,   222,     0,   222,   222,     0,   222,     0,
       0,     0,     0,   222,     0,     0,   222,   222,     0,     0,
     181,     0,   222,     0,     0,   233,    94,     0,     0,     0,
       0,     0,     0,     0,     0,   223,   223,     0,     0,     0,
     223,   223,     0,   223,   223,     0,   223,     0,   223,    18,
       0,     0,     0,     0,     0,    80,    81,    82,    83,    84,
      85,    86,    87,   153,     0,   154,    89,   155,     0,     0,
       0,   158,   159,   160,   161,   162,     0,     0,     0,   285,
       0,   164,   165,   166,     0,   223,   292,     0,     0,   223,
     223,   671,   681,     0,   307,     0,     0,     0,   328,   328,
     298,   307,   336,     0,   307,     0,   223,   223,   223,     0,
       0,   299,   300,   181,     0,     0,   223,     0,     0,     0,
       0,     0,     0,   170,   171,   301,   302,   223,     0,   223,
       0,   223,     0,   223,     0,     0,     0,     0,     0,     0,
     223,   223,     0,   223,   223,     0,   223,     0,     0,     0,
       0,   223,     0,     0,   223,   223,   285,     0,     0,    94,
     223,     0,     0,     0,    94,     0,     0,     0,     0,   398,
     740,    18,     0,     0,   180,     0,     0,   741,     0,     0,
     408,     0,     0,     0,     0,    80,    81,    82,    83,    84,
      85,    86,    87,     0,     0,   420,    89,   421,     0,   422,
     350,     0,   423,     0,    80,    81,    82,    83,    84,    85,
      86,    87,   285,     0,   285,    89,     0,     0,     0,   350,
       0,     0,     0,     0,   307,     0,     0,   328,   328,   328,
     328,   617,     0,   485,     0,     0,     0,   785,   786,     0,
     788,     0,     0,   790,     0,     0,     0,     0,     0,     0,
     651,    80,    81,    82,    83,    84,    85,    86,    87,   307,
     307,   337,    89,   181,   342,     0,   350,     0,     0,     0,
     307,   307,   307,     0,     0,     0,     0,     0,     0,     0,
     285,   181,   814,     0,     0,     0,   816,   817,     0,     0,
       0,     0,     0,     0,   330,   330,     0,   706,     0,   307,
     307,    18,     0,   671,   681,     0,     0,     0,     0,     0,
       0,     0,     0,   837,     0,     0,     0,     0,     0,   285,
      18,     0,     0,     0,   740,     0,   741,   181,   181,     0,
       0,     0,   307,     0,     0,     0,     0,   785,   786,     0,
     788,   790,     0,     0,     0,     0,     0,     0,   814,     0,
       0,   816,   817,     0,     0,     0,     0,   837,     0,     0,
       0,     0,     0,     0,   307,   307,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,     0,
       0,   328,   328,   328,   328,   328,   328,   328,   285,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   307,     0,     0,     0,     0,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,   473,
     474,     0,   611,   330,   330,   330,   330,     0,     0,     0,
     487,   488,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   285,     0,   285,   285,     0,     0,     0,
       0,     0,     0,     0,     0,   307,   307,     0,     0,   509,
     510,   181,     0,     0,   307,     0,     0,     0,     0,     0,
       0,   644,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   647,   648,     0,   649,     0,     0,     0,   285,   307,
       0,     0,   540,     0,     0,     0,     0,   307,     0,     0,
     328,     0,   328,    80,    81,    82,    83,    84,    85,    86,
      87,   153,     0,   154,    89,   155,     0,     0,     0,   158,
     159,   160,   161,   162,   564,   567,   307,     0,   307,   164,
     165,   166,     0,     0,   307,     0,     0,     0,   573,     0,
       0,   307,     0,     0,     0,     0,   537,   638,   298,     0,
       0,   639,     0,   285,     0,     0,     0,     0,     0,   299,
     300,   599,     0,     0,     0,     0,     0,     0,     0,     0,
     603,   170,   171,   301,   302,     0,     0,   330,   330,   330,
     330,   330,   330,   330,     0,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   629,   632,   307,     0,    18,
       0,     0,   180,     0,   642,     0,   345,     0,     0,   285,
     346,     0,     0,     0,   347,     0,     0,   348,  -349,     0,
     307,     0,   307,     0,   307,     0,     0,     0,     0,   656,
       0,     0,     0,     0,     0,     0,   307,   663,     0,     0,
       0,   307,   307,     0,     0,   181,  -349,  -349,  -349,  -349,
    -349,  -349,  -349,  -349,  -349,  -349,  -349,     0,   307,     0,
     307,     0,     0,     0,     0,     0,   686,     0,   689,     0,
       0,     0,     0,     0,   692,     0,   330,     0,   330,     0,
       0,   698,     0,   349,     0,     0,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   154,    89,   155,   156,
       0,   157,   158,   159,   160,   161,   162,    27,   307,   307,
      28,   163,   164,   165,   166,   307,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,    92,
     167,   168,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,   170,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,     0,   769,     0,   642,     0,     0,     0,   172,     0,
     173,   174,   175,   176,   177,     0,   776,    29,    30,    31,
       0,   779,   780,     0,     0,     0,     0,     0,     0,     0,
       0,   178,   179,     0,     0,   180,     0,     0,   795,     0,
     798,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   811,     0,     0,
       0,     0,     0,     0,     0,     0,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   395,    89,   396,   156,
       0,   157,   158,   159,   160,   161,   162,    27,   834,   836,
      28,   163,   164,   165,   166,   838,     0,     0,     0,     0,
       0,     0,     0,   839,     0,     0,     0,     0,     0,    92,
     397,   168,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,    80,    81,    82,    83,    84,    85,    86,    87,
      88,     0,     0,    89,   170,   171,     0,    90,     0,     0,
       0,     0,     0,    27,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,     0,
     173,   174,   175,   176,   177,    92,    93,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,   178,   179,     0,     0,   180,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   154,    89,   155,   156,
       0,   157,   158,   159,   160,   161,   162,    27,     0,     0,
      28,   163,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     0,     0,     0,    92,
     532,   168,     0,   169,     0,     0,     0,     0,    18,     0,
       0,     0,    80,    81,    82,    83,    84,    85,    86,    87,
      88,     0,     0,    89,   170,   171,     0,    90,     0,     0,
       0,     0,     0,    27,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,     0,
     173,   174,   175,   176,   177,    92,     0,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,   178,   179,     0,     0,   180,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   154,    89,   155,   156,
       0,   157,   158,   159,   160,   161,   162,    27,     0,     0,
      28,   163,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     0,     0,     0,    92,
       0,   168,     0,   169,     0,     0,     0,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,     0,
     173,   174,   175,   176,   177,     0,     0,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,   179,     0,     0,   180,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   154,    89,   155,     0,
       0,     0,   158,   159,   160,   161,   162,     0,     0,     0,
       0,   163,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,   168,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,     0,
     173,   174,   175,   176,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,   179,     0,     0,   180,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   154,    89,   155,     0,
       0,     0,   158,   159,   160,   161,   162,     0,     0,     0,
       0,   163,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,   168,     0,   169,     0,     0,     0,     0,     0,     0,
      80,    81,    82,    83,    84,    85,    86,    87,   153,     0,
     154,    89,   155,     0,   170,   171,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   667,     0,
     668,   669,   175,   176,   177,   298,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   299,   300,     0,     0,
       0,   178,   670,     0,     0,   180,     0,     0,   170,   171,
     301,   302,    80,    81,    82,    83,    84,    85,    86,    87,
     153,     0,   154,    89,   155,     0,     0,     0,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,   164,   165,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   537,    18,   298,     0,   180,
       0,     0,     0,     0,     0,     0,     0,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,   171,   301,   302,    80,    81,    82,    83,    84,    85,
      86,    87,   153,     0,   154,    89,   155,     0,     0,     0,
     158,   159,   160,   161,   162,     0,     0,     0,     0,     0,
     164,   165,   166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,   298,
     565,   180,     0,     0,     0,     0,     0,     0,     0,     0,
     299,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,   171,   301,   302,     0,    80,    81,    82,
      83,    84,    85,    86,    87,   153,     0,   154,    89,   155,
       0,     0,     0,   158,   159,   160,   161,   162,     0,     0,
       0,     0,     0,   164,   165,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,     0,   298,   180,   598,     0,     0,     0,     0,     0,
       0,     0,     0,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   170,   171,   301,   302,    80,
      81,    82,    83,    84,    85,    86,    87,   153,     0,   154,
      89,   155,     0,     0,     0,   158,   159,   160,   161,   162,
       0,     0,     0,     0,     0,   164,   165,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,   298,   627,   180,     0,     0,     0,
       0,     0,     0,     0,     0,   299,   300,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   171,   301,
     302,    80,    81,    82,    83,    84,    85,    86,    87,   153,
       0,   154,    89,   155,     0,     0,     0,   158,   159,   160,
     161,   162,     0,     0,     0,     0,     0,   164,   165,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,   298,   630,   180,     0,
       0,     0,     0,     0,     0,     0,     0,   299,   300,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     171,   301,   302,    80,    81,    82,    83,    84,    85,    86,
      87,   153,     0,   154,    89,   155,     0,     0,     0,   158,
     159,   160,   161,   162,     0,     0,     0,     0,     0,   164,
     165,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,   298,   654,
     180,     0,     0,     0,     0,     0,     0,     0,     0,   299,
     300,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   170,   171,   301,   302,     0,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   154,    89,   155,     0,
       0,     0,   158,   159,   160,   161,   162,     0,     0,     0,
       0,     0,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
       0,   298,   180,   688,     0,     0,     0,     0,     0,     0,
       0,     0,   299,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   171,   301,   302,    80,    81,
      82,    83,    84,    85,    86,    87,   153,     0,   154,    89,
     155,     0,     0,     0,   158,   159,   160,   161,   162,     0,
       0,     0,     0,     0,   164,   165,   166,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,   298,   696,   180,     0,     0,     0,     0,
       0,     0,     0,     0,   299,   300,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,   171,   301,   302,
      80,    81,    82,    83,    84,    85,    86,    87,   153,     0,
     154,    89,   155,     0,     0,     0,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,   298,   764,   180,     0,     0,
       0,     0,     0,     0,     0,     0,   299,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   171,
     301,   302,    80,    81,    82,    83,    84,    85,    86,    87,
     153,     0,   154,    89,   155,     0,     0,     0,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,   164,   165,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,   298,   767,   180,
       0,     0,     0,     0,     0,     0,     0,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,   171,   301,   302,    80,    81,    82,    83,    84,    85,
      86,    87,   153,     0,   154,    89,   155,     0,     0,     0,
     158,   159,   160,   161,   162,     0,     0,     0,     0,     0,
     164,   165,   166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,   298,
     796,   180,     0,     0,     0,     0,     0,     0,     0,     0,
     299,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,   171,   301,   302,     0,    80,    81,    82,
      83,    84,    85,    86,    87,   153,     0,   154,    89,   155,
       0,     0,     0,   158,   159,   160,   161,   162,     0,     0,
       0,     0,     0,   164,   165,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,     0,   298,   180,   810,     0,     0,     0,     0,     0,
       0,     0,     0,   299,   300,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   170,   171,   301,   302,     0,
      80,    81,    82,    83,    84,    85,    86,    87,   153,     0,
     154,    89,   155,     0,     0,     0,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,     0,   298,   180,   835,     0,     0,
       0,     0,     0,     0,     0,     0,   299,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   171,
     301,   302,    80,    81,    82,    83,    84,    85,    86,    87,
     153,     0,   154,    89,   155,     0,     0,     0,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,   164,   165,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,   298,     0,   180,
       0,     0,     0,     0,     0,     0,     0,     0,   299,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,   171,   301,   302,     0,    80,    81,    82,    83,    84,
      85,    86,    87,   153,     0,   154,    89,   155,   156,     0,
     350,   158,   159,   160,   161,   162,     0,     0,     0,     0,
       0,   164,   165,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
     168,   180,   475,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   170,   171,    80,    81,    82,    83,    84,
      85,    86,    87,   153,     0,   154,    89,   155,   156,     0,
     350,   158,   159,   160,   161,   162,     0,     0,     0,     0,
       0,   164,   165,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,    18,   781,     0,   180,     0,     0,     0,     0,    80,
      81,    82,    83,    84,    85,    86,    87,   153,     0,   154,
      89,   155,     0,   170,   171,   158,   159,   160,   161,   162,
       0,     0,     0,     0,     0,   164,   165,   166,     0,     0,
      80,    81,    82,    83,    84,    85,    86,    87,   153,     0,
     154,    89,   155,     0,   168,   743,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,   164,   165,   166,     0,
       0,    18,     0,     0,   180,     0,     0,   170,   171,     0,
       0,     0,     0,     0,     0,   168,   748,     0,     0,     0,
       0,     0,     0,     0,    80,    81,    82,    83,    84,    85,
      86,    87,   153,     0,   154,    89,   155,     0,   170,   171,
     158,   159,   160,   161,   162,     0,     0,     0,     0,     0,
     164,   165,   166,     0,     0,    18,     0,     0,   180,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   168,
     792,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,   180,
       0,     0,   170,   171,     0,     0,    80,    81,    82,    83,
      84,    85,    86,    87,   153,     0,   154,    89,   155,     0,
       0,     0,   158,   159,   160,   161,   162,     0,     0,     0,
       0,     0,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,   168,   831,   180,     0,     0,     0,     0,     0,     0,
      80,    81,    82,    83,    84,    85,    86,    87,   153,     0,
     154,    89,   155,     0,   170,   171,   158,   159,   160,   161,
     162,     0,     0,     0,     0,     0,   164,   165,   166,     0,
       0,    80,    81,    82,    83,    84,    85,    86,    87,   153,
       0,   154,    89,   155,     0,   168,   843,   158,   159,   160,
     161,   162,     0,     0,     0,     0,     0,   164,   165,   166,
       0,     0,    18,     0,     0,   180,     0,     0,   170,   171,
       0,     0,     0,     0,     0,     0,   168,   846,     0,     0,
       0,     0,     0,     0,     0,    80,    81,    82,    83,    84,
      85,    86,    87,   153,     0,   154,    89,   155,     0,   170,
     171,   158,   159,   160,   161,   162,     0,     0,     0,     0,
       0,   164,   165,   166,     0,     0,    18,     0,     0,   180,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,   857,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
     180,     0,     0,   170,   171,    80,    81,    82,    83,    84,
      85,    86,    87,   153,     0,   154,    89,   155,     0,     0,
       0,   158,   159,   160,   161,   162,     0,     0,     0,     0,
       0,   164,   165,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,    18,     0,     0,   180,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   170,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,     0,   180
};

static const yytype_int16 yycheck[] =
{
      92,   131,    54,   335,   335,    34,    44,   353,    54,    92,
      92,   141,   170,   171,   189,   517,    50,   284,   183,    76,
      88,   335,   386,   284,    54,    26,    15,    61,    13,    67,
     378,    13,    54,    15,    19,    24,    37,    19,   287,    27,
      92,    44,    24,    32,    27,    97,    98,   151,    49,    78,
      15,    97,    98,    54,   400,    43,    48,   439,    57,    58,
      19,   347,    92,   228,    19,    54,    12,    97,    98,    91,
      19,   163,    13,   359,    15,   132,    98,    46,    19,    78,
     102,    12,    92,    24,    44,     0,    44,    24,    53,    90,
      27,   112,    46,   185,    44,    96,   125,    98,    48,   151,
      46,   351,    52,   185,    57,   151,   158,   119,    46,    98,
      46,    53,    58,   288,    50,    46,    27,   151,   121,   284,
     119,   151,   151,    50,   391,    78,   125,   231,    50,   121,
     391,   119,    46,   185,   119,   385,   119,   119,    35,   388,
      62,   299,   300,   301,   302,   144,   145,   529,   530,   241,
      50,   499,   121,   163,   119,   185,   157,   111,   241,   241,
     119,   121,   115,   121,   119,   111,   234,   104,   105,   106,
     119,   121,   125,    52,    75,   185,   228,   119,   119,   231,
     111,   563,   228,   121,   236,   231,   351,   533,    24,   241,
     236,   144,   119,   104,   105,   106,    48,   231,   228,   701,
      52,   231,   119,   224,   261,    44,   236,   121,   229,    48,
      53,   241,    52,    52,   236,   390,    55,    48,   119,   119,
     385,    52,   243,    52,    26,   406,   391,    49,    27,    52,
      52,   241,   284,   615,   286,   236,    34,    35,   284,   277,
     286,    75,   121,   335,    43,   531,   298,   236,   515,   535,
      37,   343,    24,   335,   284,   619,   286,   621,   257,   258,
      32,   353,    49,   444,     7,   287,     9,    92,    18,   121,
     352,   353,   293,   119,    24,   296,   119,    27,    42,   278,
      23,   121,   121,   335,   598,   119,    52,   601,    52,    55,
     121,   119,   121,   560,   685,   425,    17,   688,   121,   121,
     352,   353,    61,    90,   257,   335,    64,   399,   400,    96,
      68,    70,   470,   471,   472,    41,   399,   399,   400,   596,
      25,    26,   352,   353,    50,   335,    52,   502,   349,    34,
      35,    41,   714,   343,   716,   356,    57,    58,   163,   391,
      50,   119,    52,   353,    65,   391,    67,   399,   400,    48,
     515,   119,   517,    52,   104,   105,   106,   378,   625,   750,
     185,   391,    46,    49,    50,    51,   388,    46,    52,   399,
     400,   685,   754,    52,   688,    50,    52,    52,   460,    55,
     418,     3,     4,     5,     6,     7,     8,     9,    10,   399,
     400,   119,    14,    48,    16,   560,    92,    52,    52,    44,
      55,    55,    50,    48,    52,   737,   737,    52,   460,   430,
      55,   410,    75,    76,   572,    52,   241,    80,    55,   810,
     550,   442,   119,   737,    48,   446,    71,    72,    52,    52,
     460,    55,   746,    50,   644,    52,   750,   647,   648,   649,
      46,   533,   833,    50,   835,    36,    37,    38,    39,    40,
     119,   533,   461,   462,   463,   464,    47,   848,    51,    52,
     625,   738,    50,   515,    52,   517,   518,   163,    49,    50,
      51,    50,   518,    52,   119,    50,   121,    52,   499,    71,
      72,   533,   503,    52,    49,   515,    51,   517,   518,   185,
      36,    37,    38,    39,    40,    46,   810,   119,    71,    72,
     521,    47,    46,   533,   596,   597,   598,   738,   560,   601,
     335,   119,   541,    36,    37,    38,    39,    40,   343,   833,
      49,   835,    51,   533,    47,    81,    82,    83,   353,   119,
     560,   808,   809,    49,   848,    51,   701,    50,    51,    52,
     561,    50,    51,    52,    49,   241,    51,   121,   569,   465,
     466,   467,   829,    49,   831,    51,    36,    37,    38,    39,
      40,   582,    49,    12,    51,   842,   843,    47,   845,   846,
      49,    57,    51,   625,   399,   400,   853,    51,   809,   856,
     857,   119,    49,   675,    51,   862,   596,   597,   598,    50,
     682,   601,    49,   685,    51,   625,   688,    50,   829,    46,
     831,    71,    72,    51,    52,    44,    47,    73,    74,    48,
      48,   842,   843,    52,   845,   846,    55,    51,    52,    92,
     458,   459,   853,   468,   469,   856,   857,   119,    52,    48,
     651,   862,    71,    72,    36,    37,    38,    39,    40,   335,
      48,   662,    48,    48,    62,   737,   738,   343,    18,   701,
     742,   743,    50,   745,   746,   737,   748,   353,   750,   119,
      50,    55,    47,    50,    56,   675,    47,    51,    46,    24,
      97,   701,   682,   119,   119,   685,    49,    78,   688,    79,
     119,    69,   121,    50,    77,   737,    50,    50,    47,   119,
     163,    24,    49,    51,    50,   787,    48,    51,    51,   791,
     792,    51,    49,   399,   400,    48,   727,   737,   533,   121,
      46,    48,   185,    62,    49,    49,   808,   809,   810,    51,
      50,    49,    45,    49,    48,    52,   818,   737,   738,    50,
      52,    51,   742,   743,    49,   745,   746,   829,   748,   831,
     750,   833,   119,   835,    47,    58,    47,    45,    62,    49,
     842,   843,   121,   845,   846,    50,   848,    50,    44,   119,
      24,   853,    24,    51,   856,   857,    49,    15,   241,    50,
     862,   596,   597,   598,   119,    49,   601,   787,    50,    49,
      47,   791,   792,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    48,    47,    14,    47,    51,    48,   808,   809,
     810,    48,    92,    49,    49,    49,    48,    96,   818,    62,
      49,    96,    49,    51,    50,    48,    51,    49,    49,   829,
     119,   831,    50,   833,    50,   835,    50,    50,    47,    50,
     121,    49,   842,   843,    50,   845,   846,   533,   848,    50,
      50,    49,    45,   853,    50,    49,   856,   857,    50,    49,
     675,    45,   862,    49,    49,    62,    49,   682,    50,    44,
     685,    49,   335,   688,    50,    49,    57,    58,    59,    50,
     343,    50,    50,   163,    50,    49,    45,    50,    45,     0,
     353,    49,    25,    49,    49,   236,    69,    78,   718,   404,
      54,   757,   536,   770,   389,   185,   352,   789,   808,   119,
     596,   597,   598,   241,   737,   601,   231,   293,   795,   574,
     453,   456,   737,   738,   454,   457,   455,   742,   743,   643,
     745,   746,   251,   748,   266,   750,   399,   400,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      92,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,   241,    14,   144,   145,   146,    18,    -1,    -1,    -1,
      -1,    -1,   787,    -1,    -1,    -1,   791,   792,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,   675,
      14,    -1,    -1,   808,   809,   810,   682,    49,    -1,   685,
      24,    -1,   688,   818,    -1,    -1,    -1,    -1,    -1,    -1,
     170,   171,    -1,    -1,   829,    -1,   831,    -1,   833,    -1,
     835,   163,    -1,    -1,    -1,    -1,    -1,   842,   843,    -1,
     845,   846,    -1,   848,    58,    -1,    -1,    -1,   853,    -1,
      -1,   856,   857,   185,    -1,    -1,    -1,   862,    -1,    -1,
      -1,   737,   738,    -1,    -1,   335,   742,   743,    -1,   745,
     746,    -1,   748,   343,   750,    -1,    -1,   119,    -1,    -1,
     533,    -1,   253,   353,    -1,    -1,   257,   258,   259,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,   119,    -1,   278,   279,   241,
     281,   787,    -1,    -1,    -1,   791,   792,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,   399,
     400,    -1,   808,   809,   810,    49,    -1,    -1,    -1,    -1,
      -1,    -1,   818,   596,   597,   598,    -1,    -1,   601,   299,
     300,   301,   302,   829,    -1,   831,    -1,   833,    -1,   835,
      56,    -1,    -1,    -1,    -1,    -1,   842,   843,    -1,   845,
     846,    -1,   848,    -1,    -1,    -1,    -1,   853,    -1,    -1,
     856,   857,    -1,    -1,    -1,    -1,   862,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,   335,    -1,   119,    -1,    -1,    -1,    -1,
      -1,   343,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   353,   675,    -1,    -1,    -1,    -1,    -1,   163,   682,
      -1,    -1,   685,   119,    -1,   688,    -1,    -1,    -1,   410,
     411,    56,   413,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,   426,    -1,    -1,    14,    -1,
      -1,    -1,    18,   533,    -1,    -1,    -1,   399,   400,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,   737,   738,    -1,    -1,    -1,   742,
     743,    -1,   745,   746,    -1,   748,    -1,   750,    -1,    -1,
      -1,    -1,    -1,   453,   454,   455,   456,   457,   458,   459,
      -1,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,    -1,    -1,    -1,   596,   597,   598,    -1,
      -1,   601,    -1,    -1,   787,    -1,    -1,    -1,   791,   792,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    13,    14,    15,   808,   809,   810,    19,    20,
      21,    22,    23,   119,    -1,   818,    -1,    -1,    29,    30,
      31,    -1,    -1,    -1,   545,    -1,   829,    -1,   831,    -1,
     833,    -1,   835,    -1,    -1,    -1,    -1,    48,    49,   842,
     843,    -1,   845,   846,   565,   848,    -1,    -1,    -1,    -1,
     853,   533,    -1,   856,   857,   675,    -1,    -1,   343,   862,
      71,    72,   682,    -1,    -1,   685,    -1,    -1,   688,    -1,
      -1,    -1,   572,    -1,   574,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    -1,    -1,    -1,   119,    -1,
      -1,   122,    -1,    -1,   596,   597,   598,   737,   738,   601,
      48,    49,   742,   743,    -1,   745,   746,    -1,   748,    -1,
     750,    59,    60,    -1,    -1,    -1,    -1,    -1,   659,   660,
      -1,    -1,    -1,    71,    72,    73,    74,    56,    -1,    -1,
       2,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    17,    -1,   787,    18,    -1,
      -1,   791,   792,    -1,    -1,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    -1,    -1,   808,   809,
     810,   119,    -1,   675,   122,    -1,    -1,    -1,   818,    49,
     682,    53,    54,   685,    56,    -1,   688,    -1,    -1,   829,
      -1,   831,    -1,   833,    -1,   835,    -1,    69,    70,    71,
      72,    73,   842,   843,    -1,   845,   846,    -1,   848,    -1,
      -1,    -1,    -1,   853,    -1,    -1,   856,   857,    -1,    -1,
      92,    -1,   862,    -1,    -1,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   737,   738,    -1,    -1,    -1,
     742,   743,    -1,   745,   746,    -1,   748,    -1,   750,   119,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    -1,    -1,    -1,   151,
      -1,    29,    30,    31,    -1,   787,   158,    -1,    -1,   791,
     792,   596,   597,    -1,   168,    -1,    -1,    -1,   170,   171,
      48,   175,    50,    -1,   178,    -1,   808,   809,   810,    -1,
      -1,    59,    60,   185,    -1,    -1,   818,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,   829,    -1,   831,
      -1,   833,    -1,   835,    -1,    -1,    -1,    -1,    -1,    -1,
     842,   843,    -1,   845,   846,    -1,   848,    -1,    -1,    -1,
      -1,   853,    -1,    -1,   856,   857,   228,    -1,    -1,   231,
     862,    -1,    -1,    -1,   236,    -1,    -1,    -1,    -1,   241,
     675,   119,    -1,    -1,   122,    -1,    -1,   682,    -1,    -1,
     252,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,   267,    14,   269,    -1,   271,
      18,    -1,   274,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,   284,    -1,   286,    14,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   298,    -1,    -1,   299,   300,   301,
     302,    49,    -1,   738,    -1,    -1,    -1,   742,   743,    -1,
     745,    -1,    -1,   748,    -1,    -1,    -1,    -1,    -1,    -1,
      49,     3,     4,     5,     6,     7,     8,     9,    10,   333,
     334,   175,    14,   335,   178,    -1,    18,    -1,    -1,    -1,
     344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     352,   353,   787,    -1,    -1,    -1,   791,   792,    -1,    -1,
      -1,    -1,    -1,    -1,   170,   171,    -1,    49,    -1,   373,
     374,   119,    -1,   808,   809,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   818,    -1,    -1,    -1,    -1,    -1,   391,
     119,    -1,    -1,    -1,   829,    -1,   831,   399,   400,    -1,
      -1,    -1,   406,    -1,    -1,    -1,    -1,   842,   843,    -1,
     845,   846,    -1,    -1,    -1,    -1,    -1,    -1,   853,    -1,
      -1,   856,   857,    -1,    -1,    -1,    -1,   862,    -1,    -1,
      -1,    -1,    -1,    -1,   438,   439,    -1,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   452,    -1,
      -1,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   475,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   333,
     334,    -1,   494,   299,   300,   301,   302,    -1,    -1,    -1,
     344,   345,   346,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   515,    -1,   517,   518,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   529,   530,    -1,    -1,   373,
     374,   533,    -1,    -1,   538,    -1,    -1,    -1,    -1,    -1,
      -1,   543,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   553,   554,    -1,   556,    -1,    -1,    -1,   560,   563,
      -1,    -1,   406,    -1,    -1,    -1,    -1,   571,    -1,    -1,
     572,    -1,   574,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    13,    14,    15,    -1,    -1,    -1,    19,
      20,    21,    22,    23,   438,   439,   600,    -1,   602,    29,
      30,    31,    -1,    -1,   608,    -1,    -1,    -1,   452,    -1,
      -1,   615,    -1,    -1,    -1,    -1,    46,    47,    48,    -1,
      -1,    51,    -1,   625,    -1,    -1,    -1,    -1,    -1,    59,
      60,   475,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     484,    71,    72,    73,    74,    -1,    -1,   453,   454,   455,
     456,   457,   458,   459,    -1,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   471,   472,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   529,   530,   691,    -1,   119,
      -1,    -1,   122,    -1,   538,    -1,    44,    -1,    -1,   701,
      48,    -1,    -1,    -1,    52,    -1,    -1,    55,    56,    -1,
     714,    -1,   716,    -1,   718,    -1,    -1,    -1,    -1,   563,
      -1,    -1,    -1,    -1,    -1,    -1,   730,   571,    -1,    -1,
      -1,   735,   736,    -1,    -1,   737,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,   752,    -1,
     754,    -1,    -1,    -1,    -1,    -1,   600,    -1,   602,    -1,
      -1,    -1,    -1,    -1,   608,    -1,   572,    -1,   574,    -1,
      -1,   615,    -1,   121,    -1,    -1,    -1,   781,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,   812,   813,
      27,    28,    29,    30,    31,   819,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   827,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   691,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     714,    -1,   716,    -1,   718,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,   100,   101,    -1,   730,   104,   105,   106,
      -1,   735,   736,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,   122,    -1,    -1,   752,    -1,
     754,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   781,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,   812,   813,
      27,    28,    29,    30,    31,   819,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   827,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    14,    71,    72,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,   100,   101,    46,    47,   104,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,   118,   119,    -1,    -1,   122,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,    -1,    -1,    -1,    46,
      47,    48,    -1,    50,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    14,    71,    72,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,   100,   101,    46,    -1,   104,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,   118,   119,    -1,    -1,   122,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,    -1,    -1,    -1,    46,
      -1,    48,    -1,    50,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,   122,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    48,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,   122,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    48,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    14,    15,    -1,    71,    72,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,   100,   101,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,   118,   119,    -1,    -1,   122,    -1,    -1,    71,    72,
      73,    74,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,   119,    48,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    13,    14,    15,    -1,    -1,    -1,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    48,
      49,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    13,    14,    15,
      -1,    -1,    -1,    19,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    48,   122,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,    -1,    -1,    -1,    19,    20,    21,    22,    23,
      -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    48,    49,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    13,    14,    15,    -1,    -1,    -1,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    48,    49,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    13,    14,    15,    -1,    -1,    -1,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,    29,
      30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    48,    49,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    48,   122,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    13,    14,
      15,    -1,    -1,    -1,    19,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    48,    49,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    14,    15,    -1,    -1,    -1,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    48,    49,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    48,    49,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    13,    14,    15,    -1,    -1,    -1,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    48,
      49,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    13,    14,    15,
      -1,    -1,    -1,    19,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    48,   122,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    14,    15,    -1,    -1,    -1,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    48,   122,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    48,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
      48,   122,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,   119,    50,    -1,   122,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    13,
      14,    15,    -1,    71,    72,    19,    20,    21,    22,    23,
      -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    14,    15,    -1,    48,    49,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      -1,   119,    -1,    -1,   122,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    13,    14,    15,    -1,    71,    72,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    -1,    -1,   119,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,   122,
      -1,    -1,    71,    72,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    13,    14,    15,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    48,    49,   122,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    14,    15,    -1,    71,    72,    19,    20,    21,    22,
      23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    13,    14,    15,    -1,    48,    49,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      -1,    -1,   119,    -1,    -1,   122,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    -1,    71,
      72,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    -1,   119,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
     122,    -1,    -1,    71,    72,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,   122
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    25,    26,    34,    35,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   269,   119,    27,   119,   138,
      35,   119,     0,   128,   269,   269,    18,    24,    27,   104,
     105,   106,   136,   137,   141,   142,   143,   144,    50,    52,
     138,    50,    52,   119,    46,    52,   269,   137,   133,   144,
     145,   119,   137,    12,    46,    58,   111,   146,   148,   149,
     150,   145,   119,    50,    52,    75,   119,    46,    52,    36,
      37,    38,    39,    40,    47,   134,   119,   145,   141,   138,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    14,
      18,    27,    46,    47,   138,   142,   144,   146,   151,   153,
     154,   163,   164,   175,   176,   177,   178,   182,   222,   223,
     224,   262,   263,   264,   265,   119,   147,   266,   138,   148,
     149,   150,   149,   150,   150,   141,    52,   139,    75,    50,
      47,   134,   119,    27,    43,   135,   138,   138,   138,   138,
     138,   134,   270,   139,   146,   148,   149,   150,    52,   119,
     166,   145,   182,    11,    13,    15,    16,    18,    19,    20,
      21,    22,    23,    28,    29,    30,    31,    47,    48,    50,
      71,    72,    95,    97,    98,    99,   100,   101,   118,   119,
     122,   138,   142,   172,   182,   183,   185,   186,   187,   188,
     189,   191,   192,   193,   194,   196,   197,   199,   201,   203,
     204,   206,   208,   213,   215,   216,   217,   219,   220,   221,
     225,   226,   227,   228,   229,   230,   231,   237,   238,   255,
     256,   259,   260,   261,   263,   264,   265,   268,    48,   121,
     233,   145,    11,   138,   165,   262,   151,   152,    50,   174,
     182,    46,   179,   119,   155,   156,   158,   166,   233,   148,
      57,    51,    51,   149,   150,   150,   150,   146,   148,   149,
     150,   119,    50,    50,   270,   139,   135,   271,    50,    41,
      50,    41,    50,    50,    42,   270,    47,    46,   148,   149,
     150,   149,   150,   150,    48,   138,   146,   164,   262,    52,
      52,    55,   138,   263,   138,   189,   263,   265,    48,    59,
      60,    73,    74,   225,   229,   234,   236,   237,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   138,   227,
     228,   254,   254,    48,    48,    48,    50,   234,    50,   119,
      50,   119,   234,    62,    48,    44,    48,    52,    55,   121,
      18,   172,   173,   183,   184,    50,   119,   155,    50,    52,
      55,    56,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,   239,   240,    71,    72,    52,   121,    55,
      49,   167,   168,   170,   171,   172,   262,   233,   164,   178,
     262,    48,   166,   152,    47,    13,    15,    47,   138,   180,
     183,   219,   233,    50,    51,   157,    56,   266,   138,   150,
     148,   149,   150,   149,   150,   150,   139,    47,    46,   271,
     138,   138,   138,   138,    47,   134,   149,   150,   150,   150,
      49,   167,   168,   174,   155,    24,   119,   119,    44,    48,
     121,   232,   121,   232,   233,    97,   263,   254,   254,   254,
     254,    49,    61,    70,    69,    78,    79,    77,    64,    68,
      17,    57,    58,    65,    67,    81,    82,    83,    73,    74,
      75,    76,    80,   234,   234,    50,   187,   197,   210,   212,
      50,    50,    50,    50,    62,   189,    49,   234,   234,    49,
     181,   234,    13,    15,    19,    24,   226,    19,   119,   121,
     218,   173,   188,   263,   184,    47,   119,   226,   119,   234,
     234,    24,   218,    19,   119,    51,    49,    51,   173,    15,
      53,   119,   158,   174,   179,   155,    49,   167,   168,    48,
      48,    52,    47,   183,   184,    52,   156,    46,   161,   162,
     234,    51,   267,    51,   140,   149,   150,   150,   150,    47,
     134,    50,    50,    51,    51,    50,    51,   270,   150,   233,
      51,    49,    50,    48,   234,    49,   181,   234,   121,   121,
     162,    48,    49,   234,   241,   244,   245,   246,   247,   248,
     249,   249,   263,   264,   251,   251,   251,   251,   252,   252,
     252,   253,   253,   254,   254,   254,    49,    49,    50,   234,
      62,    51,    50,   234,    49,    45,    49,    49,    51,    52,
      55,   138,   218,    52,   155,    48,    52,    49,   168,   262,
     170,   262,   119,    52,    50,    51,    49,    49,   181,   234,
      49,   181,   234,    13,   184,    47,    13,   157,    47,    51,
     159,   162,   234,   146,   138,   150,   270,   138,   138,   138,
      47,    49,   168,   233,    49,   181,   234,    45,   150,    49,
      49,   121,   121,   234,   254,    62,   242,    95,    97,    98,
     119,   189,   190,   191,   195,   200,   202,   205,   207,   209,
     214,   189,    49,   211,   212,    50,   234,   197,    50,   234,
      50,    44,   234,   119,   119,    24,    49,   181,   234,    24,
      49,    51,   169,    53,   158,    15,    49,   168,    50,    49,
      49,    50,    49,    49,    48,    47,    48,    47,    51,   160,
     267,   140,    47,   140,   140,   140,   233,    49,    49,    49,
      44,   121,   150,   150,    49,    48,    48,    48,    62,    96,
     189,   189,    49,    49,   211,    49,    51,   198,    49,   211,
      50,   234,    51,   235,    48,    49,    49,   170,   119,    49,
      50,    50,    50,    50,    49,   181,   234,    49,   181,   234,
     159,    47,    50,    50,    50,   233,   234,   121,    50,   234,
     234,    50,   187,   210,   190,   189,   189,    49,   189,   197,
     189,    49,    49,   211,    45,   234,    49,   181,   234,   169,
      50,    49,    49,    50,    49,    49,   160,    45,    49,    49,
      50,   234,    62,    50,   189,   198,   189,   189,    49,    44,
     235,    49,    49,    50,    50,    50,    50,    44,   121,   200,
     190,    49,   211,    50,   234,    50,   234,   189,   234,   234,
     190,   190,    49,    49,   211,    49,    49,   211,    50,    45,
      45,   190,   190,    49,   190,   190,    49,    49,   211,   190,
     190,   190,    49,   190
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   123,   124,   124,   125,   125,   126,   126,   127,   127,
     128,   128,   128,   128,   129,   130,   131,   132,   133,   133,
     133,   133,   133,   133,   133,   133,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   135,   135,   136,   137,
     137,   138,   138,   139,   139,   140,   140,   141,   142,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   144,   144,   144,   144,   145,   145,   146,
     147,   147,   148,   149,   149,   150,   150,   151,   151,   151,
     151,   152,   152,   153,   153,   153,   154,   154,   154,   155,
     156,   156,   156,   157,   157,   158,   158,   159,   159,   160,
     160,   161,   162,   162,   162,   163,   163,   163,   164,   164,
     164,   165,   165,   166,   166,   166,   166,   166,   166,   166,
     166,   167,   167,   168,   168,   169,   169,   170,   170,   170,
     171,   171,   172,   173,   173,   174,   174,   175,   176,   177,
     177,   178,   178,   178,   178,   178,   178,   178,   178,   179,
     179,   179,   179,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   182,   182,   183,   183,
     183,   184,   184,   185,   186,   187,   187,   188,   188,   188,
     189,   189,   189,   189,   189,   189,   190,   190,   190,   190,
     190,   191,   191,   191,   191,   191,   191,   191,   191,   192,
     193,   194,   195,   196,   197,   197,   197,   197,   197,   197,
     197,   198,   198,   199,   200,   201,   202,   203,   203,   204,
     205,   206,   206,   207,   207,   208,   208,   208,   208,   208,
     208,   208,   208,   209,   209,   209,   209,   209,   209,   209,
     209,   210,   210,   211,   212,   212,   213,   214,   215,   215,
     216,   216,   217,   217,   218,   218,   219,   219,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   221,   221,
     221,   221,   221,   222,   222,   223,   223,   223,   223,   223,
     224,   224,   225,   225,   225,   226,   226,   226,   226,   226,
     226,   227,   227,   227,   228,   228,   228,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   230,   230,   230,   230,   230,   230,   230,   231,   231,
     231,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     233,   233,   234,   235,   235,   236,   236,   237,   237,   238,
     238,   238,   239,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   241,   242,   242,   243,   243,   244,
     244,   245,   245,   246,   246,   247,   247,   248,   248,   248,
     249,   249,   249,   249,   249,   249,   250,   251,   251,   251,
     251,   252,   252,   252,   253,   253,   253,   253,   254,   254,
     254,   254,   254,   255,   256,   257,   257,   257,   257,   258,
     259,   259,   259,   259,   260,   261,   262,   262,   263,   263,
     263,   264,   264,   265,   265,   266,   266,   267,   267,   268,
     268,   268,   268,   268,   268,   268,   269,   269,   270,   270,
     271,   271
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     3,     2,     3,     6,     1,     3,
       1,     1,     1,     1,     3,     5,     4,     6,     4,     7,
       6,     9,     5,     8,     7,    10,     3,     5,     3,     5,
       8,     3,     8,     3,     5,     8,     1,     1,     1,     0,
       2,     1,     3,     0,     3,     0,     3,     2,     1,     2,
       3,     3,     3,     4,     4,     4,     5,     4,     5,     5,
       5,     6,     6,     6,     7,     7,     4,     5,     5,     5,
       6,     6,     6,     1,     1,     1,     1,     0,     2,     3,
       1,     4,     2,     2,     5,     2,     4,     1,     1,     1,
       1,     0,     2,     1,     1,     1,     3,     5,     5,     2,
       1,     3,     3,     0,     3,     1,     2,     1,     1,     0,
       3,     1,     2,     4,     3,     2,     4,     4,     2,     2,
       3,     1,     1,     3,     5,     4,     6,     4,     6,     5,
       7,     4,     2,     1,     4,     0,     3,     2,     4,     1,
       3,     5,     1,     0,     2,     1,     1,     1,     2,     2,
       4,     3,     5,     4,     6,     4,     6,     5,     7,     2,
       3,     4,     5,     4,     5,     4,     5,     6,     7,     6,
       7,     5,     5,     7,     7,     4,     2,     4,     1,     1,
       1,     0,     2,     1,     2,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     7,
       1,     3,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     5,     2,     6,     6,     3,     5,     5,
       5,     1,     1,     1,     1,     9,     6,     7,     7,     7,
       8,     8,     8,     9,     6,     7,     7,     7,     8,     8,
       8,     1,     1,     1,     1,     4,     7,     7,     2,     3,
       2,     3,     2,     3,     0,     2,     1,     1,     1,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     3,     5,
       3,     5,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     6,     5,     5,     4,     6,
       5,     3,     3,     5,     4,     7,    10,     3,     4,     5,
       6,     5,     6,     7,     8,     4,     6,     6,     8,     4,
       3,     3,     3,     3,     3,     5,     3,     3,     3,     3,
       4,     1,     3,     2,     4,     6,     3,     5,     7,     9,
       1,     2,     1,     0,     3,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     4,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     4,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     0,     2,     0,     2,
       0,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* CompilationUnit: OrdinaryCompilationUnit  */
#line 360 "parser.yacc"
                             {
        (yyvsp[0].node)->changeLabel("CompilationUnit");
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"OrdinaryCompilationUnit->CompilationUnit");
        // if(otpt.size()!=0){
        //     call_dotgen($$,otpt);
        // }
        // else{
        //     call_dotgen($$,"graph1.dot");
        // }

        if(otpt.size()!=0){
            ir_gen(ircode,otpt);
        }
        else{
            ir_gen(ircode,"3ac.txt");
        }
        if(v==1){
            root->printTree();
            
            root->printFuncs();
            for(auto it: (yyval.node)->code) {
                it->print();
            }
            cout << "Incremental IR \n\n";
            int cnt = 0;
            for(auto it: ircode) {
                cout << cnt << "\t:\t";
                it->print();
                cnt++;
            }
        }
        root->dumpSymbolTable();
    }
#line 3510 "y.tab.c"
    break;

  case 3: /* CompilationUnit: ModularCompilationUnit  */
#line 395 "parser.yacc"
     {
        (yyvsp[0].node)->changeLabel("CompilationUnit");
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ModularCompilationUnit->CompilationUnit");
        printTree((yyval.node));
       if(otpt.size()!=0){
            call_dotgen((yyval.node),otpt);
        }
        else{
            call_dotgen((yyval.node),"graph1.dot");
        }
        root->printTree();
        root->printFuncs();
     }
#line 3529 "y.tab.c"
    break;

  case 4: /* OrdinaryCompilationUnit: PackageDeclaration ImportDeclarations TopLevelClassOrInterfaceDeclarations  */
#line 412 "parser.yacc"
                                                                              {
        vector<Node*> temp;
        temp.push_back((yyvsp[-2].node));
        if((yyvsp[-1].node)) {
            temp.push_back((yyvsp[-1].node));
        }
        if((yyvsp[0].node)) {
            temp.push_back((yyvsp[0].node));
        }
        struct Node* n = new Node("OrdinaryCompilationUnit", temp);
        n->useless();
        (yyval.node) = n;
        verbose(v,"PackageDeclaration ImportDeclarations TopLevelClassOrInterfaceDeclarations->OrdinaryCompilationUnit");
    }
#line 3548 "y.tab.c"
    break;

  case 5: /* OrdinaryCompilationUnit: ImportDeclarations TopLevelClassOrInterfaceDeclarations  */
#line 426 "parser.yacc"
                                                           {
        vector<Node*> temp;
        //temp.push_back($1);
        if((yyvsp[-1].node)) {
            temp.push_back((yyvsp[-1].node));
        }
        if((yyvsp[0].node)) {
            temp.push_back((yyvsp[0].node));
        }
        struct Node* n = new Node("OrdinaryCompilationUnit", temp);
        n->useless();
        (yyval.node) = n;
        verbose(v,"ImportDeclarations TopLevelClassOrInterfaceDeclarations->OrdinaryCompilationUnit");
    }
#line 3567 "y.tab.c"
    break;

  case 6: /* PackageDeclaration: PACKAGE IDENTIFIER SEMICOLON  */
#line 445 "parser.yacc"
    {
        (yyval.node) = new Node("PackageDeclaration", strcat((yyvsp[-2].lexeme), strcat(" ", (yyvsp[-1].lexeme))));
        verbose(v,"PACKAGE IDENTIFIER SEMICOLON->PackageDeclaration");

        Symbol* res = root->lookup((yyvsp[-1].lexeme));
        if(res)
        {
            yyerror("Package has already been declared");
        }
        res = new Symbol((yyvsp[-1].lexeme), PACKAGE_TYPE, yylineno);
        root->insert(res->lexeme, res);
    }
#line 3584 "y.tab.c"
    break;

  case 7: /* PackageDeclaration: PACKAGE IDENTIFIER DOT IDENTIFIER DotIdentifiers SEMICOLON  */
#line 458 "parser.yacc"
    {
        
        string s = string((yyvsp[-5].lexeme)) + string((yyvsp[-4].lexeme)) + "." + string((yyvsp[-2].lexeme));
        
        if((yyvsp[-1].node)) 
            s += (yyvsp[-1].node)->attr;
        (yyval.node) = new Node("PackageDeclaration", s);
        verbose(v,"PACKAGE IDENTIFIER DOT IDENTIFIER DotIdentifiers SEMICOLON->PackageDeclaration");

        Symbol* res;
        if((yyvsp[-1].node))
        {
            res = root->lookup((yyvsp[-1].node)->children.back()->attr);
            if(!res)
            {
                res = new Symbol((yyvsp[-1].node)->children.back()->attr, PACKAGE_TYPE, yylineno);
                root->insert(res->lexeme, res);

            }
        }
        else
        {
            res = root->lookup((yyvsp[-2].lexeme));
            if(!res)
            {
                res = new Symbol((yyvsp[-1].node)->children.back()->attr, PACKAGE_TYPE, yylineno);
                root->insert(res->lexeme, res);

            }
        }       
    }
#line 3620 "y.tab.c"
    break;

  case 8: /* ModularCompilationUnit: ModuleDeclaration  */
#line 492 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ModuleDeclaration->ModularCompilationUnit");
    }
#line 3629 "y.tab.c"
    break;

  case 9: /* ModularCompilationUnit: ImportDeclaration ImportDeclarations ModuleDeclaration  */
#line 496 "parser.yacc"
                                                              {
        vector<struct Node*> temp;
        if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        if((yyvsp[-1].node)) {
            for(auto it: temp) {
                (yyvsp[-1].node)->addChildToLeft(it);
            }
            temp.clear();
            temp.push_back((yyvsp[-1].node));
        
        }
        
        
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("ModularCompilationUnit", temp);
        n->useless();
        (yyval.node) = n;
      verbose(v,"ImportDeclaration ImportDeclarations ModuleDeclaration->ModularCompilationUnit");
    }
#line 3664 "y.tab.c"
    break;

  case 10: /* ImportDeclaration: SingleTypeImportDeclaration  */
#line 528 "parser.yacc"
                                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"SingleTypeImportDeclaration->ImportDeclaration");
        importflag = 0;
    }
#line 3674 "y.tab.c"
    break;

  case 11: /* ImportDeclaration: TypeImportOnDemandDeclaration  */
#line 533 "parser.yacc"
                                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"TypeImportOnDemandDeclaration->ImportDeclaration");
        importflag = 0;
    }
#line 3684 "y.tab.c"
    break;

  case 12: /* ImportDeclaration: SingleStaticImportDeclaration  */
#line 538 "parser.yacc"
                                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"SingleStaticImportDeclaration->ImportDeclaration");
        importflag = 0;
    }
#line 3694 "y.tab.c"
    break;

  case 13: /* ImportDeclaration: StaticImportOnDemandDeclaration  */
#line 543 "parser.yacc"
                                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"StaticImportOnDemandDeclaration->ImportDeclaration");
        importflag = 0;
    }
#line 3704 "y.tab.c"
    break;

  case 14: /* SingleTypeImportDeclaration: IMPORT Name SEMICOLON  */
#line 551 "parser.yacc"
                                {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-2].lexeme));
        

        vector<Node*> t = {t1, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("SingleTypeImportDeclaration", t);
        verbose(v,"IIMPORT Name SEMICOLON->SingleTypeImportDeclaration");

        Symbol* res = root->lookup((yyvsp[-1].node)->attr);
        if(!res)
        {
            res = new Symbol((yyvsp[-1].node)->attr, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
#line 3724 "y.tab.c"
    break;

  case 15: /* TypeImportOnDemandDeclaration: IMPORT Name DOT MULT SEMICOLON  */
#line 569 "parser.yacc"
                                    {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-4].lexeme));
        
        struct Node* t3 = new struct Node ( "Operator" ,(yyvsp[-1].lexeme));
       

        vector<Node*> t = {t1, (yyvsp[-3].node), t3};
        (yyval.node) = new struct Node("TypeImportOnDemandDeclaration", t);
        verbose(v,"IMPORT Name DOT MULT SEMICOLON->TypeImportOnDemandDeclaration");

        string name = (yyvsp[-3].node)->attr+".*";
        Symbol* res = root->lookup(name);
        if(!res)
        {
            res = new Symbol(name, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
#line 3747 "y.tab.c"
    break;

  case 16: /* SingleStaticImportDeclaration: IMPORT STATIC Name SEMICOLON  */
#line 590 "parser.yacc"
                                 {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-3].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-2].lexeme));
       

        vector<Node*> t = {t1, t2, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("SingleStaticImportDeclaration", t);
        verbose(v,"IMPORT STATIC Name SEMICOLON->SingleStaticImportDeclaration");

        Symbol* res = root->lookup((yyvsp[-1].node)->attr);
        if(!res)
        {
            res = new Symbol((yyvsp[-1].node)->attr, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
#line 3768 "y.tab.c"
    break;

  case 17: /* StaticImportOnDemandDeclaration: IMPORT STATIC Name DOT MULT SEMICOLON  */
#line 609 "parser.yacc"
                                            {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-5].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-4].lexeme));
        
        struct Node* t4 = new struct Node ( "Operator" ,(yyvsp[-1].lexeme));
      

        vector<Node*> t = {t1, t2, (yyvsp[-3].node), t4};
        (yyval.node) = new struct Node("StaticImportOnDemandDeclaration", t);
        verbose(v,"IMPORT STATIC Name DOT MULT SEMICOLON->StaticImportOnDemandDeclaration");


        string name = (yyvsp[-3].node)->attr+".*";
        Symbol* res = root->lookup(name);
        if(!res)
        {
            res = new Symbol(name, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
#line 3793 "y.tab.c"
    break;

  case 18: /* ModuleDeclaration: MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET  */
#line 632 "parser.yacc"
                                                            {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-3].lexeme));
        struct Node* t2 = new struct Node ( "Identifier" ,(yyvsp[-2].lexeme));
       

        vector<Node*> t = {t1, t2};
        (yyval.node) = new struct Node("ModuleDeclaration", t);
        verbose(v,"MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    }
#line 3807 "y.tab.c"
    break;

  case 19: /* ModuleDeclaration: MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET  */
#line 641 "parser.yacc"
                                                                                       {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-6].lexeme));
        struct Node* t2 = new struct Node ( "Identifier" ,(yyvsp[-5].lexeme));
       
        struct Node* t4 = new struct Node ( "Identifier" ,(yyvsp[-3].lexeme));
        

        vector<Node*> t = {t1, t2, t4};
        if((yyvsp[-2].node)) t.push_back((yyvsp[-2].node));
        (yyval.node) = new struct Node("ModuleDeclaration", t);
        verbose(v,"MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    }
#line 3824 "y.tab.c"
    break;

  case 20: /* ModuleDeclaration: MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET  */
#line 653 "parser.yacc"
                                                                                            {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-5].lexeme));
        struct Node* t2 = new struct Node ( "Identifier" ,(yyvsp[-4].lexeme));
        struct Node* t3 = new struct Node ( "Separator" ,(yyvsp[-3].lexeme));
      
        vector<Node*> t;
        if((yyvsp[-1].node))  
            t = {t1, t2, (yyvsp[-2].node), (yyvsp[-1].node)};
        else
            t = {t1, t2, (yyvsp[-2].node)};
        (yyval.node) = new struct Node("ModuleDeclaration", t);
        verbose(v,"MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET->ModuleDeclaration");
    }
#line 3842 "y.tab.c"
    break;

  case 21: /* ModuleDeclaration: MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET  */
#line 666 "parser.yacc"
                                                                                                                           {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-8].lexeme));
        struct Node* t2 = new struct Node ( "Identifier" ,(yyvsp[-7].lexeme));
        
        struct Node* t4 = new struct Node ( "Identifier" ,(yyvsp[-5].lexeme));
        

        vector<Node*> temp; 
        
        if((yyvsp[-4].node) && (yyvsp[-1].node)) {
            vector<Node*> t = {t1, t2, t4, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[-1].node)};
            temp = t;
        }
        else if((yyvsp[-4].node)) {
            vector<Node*> t = {t1, t2, t4, (yyvsp[-4].node), (yyvsp[-2].node)};
            temp = t;
        }
        else if((yyvsp[-1].node)) {
            vector<Node*> t = {t1, t2, t4, (yyvsp[-2].node), (yyvsp[-1].node)};
            temp = t;
        }
        else {
            vector<Node*> t = {t1, t2,t4, (yyvsp[-2].node)};
            temp = t;
        }

        (yyval.node) = new struct Node("ModuleDeclaration", temp);
        verbose(v,"MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET->ModuleDeclaration");
    }
#line 3876 "y.tab.c"
    break;

  case 22: /* ModuleDeclaration: OPEN MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET  */
#line 695 "parser.yacc"
                                                                {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-4].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-3].lexeme));
        struct Node* t3 = new struct Node ( "Identifier" ,(yyvsp[-2].lexeme));
        

        vector<Node*> t = {t1, t2, t3};
        (yyval.node) = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    }
#line 3891 "y.tab.c"
    break;

  case 23: /* ModuleDeclaration: OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET  */
#line 705 "parser.yacc"
                                                                                             {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-7].lexeme));
        struct Node* t2 = new struct Node ("Keyword",  (yyvsp[-6].lexeme));
        struct Node* t3 = new struct Node ( "Identifier" ,(yyvsp[-5].lexeme));
       
        struct Node* t5 = new struct Node ( "Identifier" ,(yyvsp[-3].lexeme));
       
        vector<Node*> t;
        
        if((yyvsp[-2].node))
            t = {t1, t2, t3, t5, (yyvsp[-2].node)};
        else
            t = {t1, t2, t3, t5};
        (yyval.node) = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    }
#line 3912 "y.tab.c"
    break;

  case 24: /* ModuleDeclaration: OPEN MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET  */
#line 721 "parser.yacc"
                                                                                                {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-6].lexeme));
        struct Node* t2 = new struct Node ("Keyword",  (yyvsp[-5].lexeme));
        struct Node* t3 = new struct Node ( "Identifier" ,(yyvsp[-4].lexeme));
        
        vector<Node*> t;
        if((yyvsp[-1].node))  
            t = {t1, t2, t3, (yyvsp[-2].node), (yyvsp[-1].node)};
        else
            t = {t1, t2, t3,(yyvsp[-2].node)};
        (yyval.node) = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET->ModuleDeclaration");
    }
#line 3930 "y.tab.c"
    break;

  case 25: /* ModuleDeclaration: OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET  */
#line 734 "parser.yacc"
                                                                                                                               {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-9].lexeme));
        struct Node* t2 = new struct Node ("Keyword",  (yyvsp[-8].lexeme));
        struct Node* t3 = new struct Node ( "Identifier" ,(yyvsp[-7].lexeme));
        
        struct Node* t5 = new struct Node ( "Identifier" ,(yyvsp[-5].lexeme));
        

        vector<Node*> t; 
        
        if((yyvsp[-4].node) && (yyvsp[-1].node))
            t = {t1, t2, t3, t5, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[-1].node)};
        else if((yyvsp[-4].node))
            t = {t1, t2, t3, t5, (yyvsp[-4].node), (yyvsp[-2].node)};
        else if((yyvsp[-1].node))
            t = {t1, t2, t3, t5 , (yyvsp[-2].node), (yyvsp[-1].node)};
        else
            t = {t1, t2, t3, t5, (yyvsp[-2].node)};
        (yyval.node) = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET>ModuleDeclaration");
    }
#line 3956 "y.tab.c"
    break;

  case 26: /* ModuleDirective: REQUIRES Name SEMICOLON  */
#line 757 "parser.yacc"
                            {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-2].lexeme));
        

        vector<Node*> t = {t1, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"REQUIRES Name SEMICOLON->ModuleDirective");
    }
#line 3969 "y.tab.c"
    break;

  case 27: /* ModuleDirective: REQUIRES RequiresModifier RequiresModifiers Name SEMICOLON  */
#line 765 "parser.yacc"
                                                                 {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-4].lexeme));
        

        vector<Node*> t = {t1};
        
        if((yyvsp[-2].node)) {
            (yyvsp[-2].node)->addChildToLeft((yyvsp[-3].node));
            t.push_back((yyvsp[-2].node));
        }
        else t.push_back((yyvsp[-3].node));
        t.push_back((yyvsp[-1].node));
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"REQUIRES RequiresModifier RequiresModifiers Name SEMICOLON->ModuleDirective");
    }
#line 3989 "y.tab.c"
    break;

  case 28: /* ModuleDirective: EXPORTS Name SEMICOLON  */
#line 780 "parser.yacc"
                            {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-2].lexeme));
        

        vector<Node*> t = {t1, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"EXPORTS Name SEMICOLON->ModuleDirective");
    }
#line 4002 "y.tab.c"
    break;

  case 29: /* ModuleDirective: EXPORTS Name TO Name SEMICOLON  */
#line 788 "parser.yacc"
                                    {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-4].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-2].lexeme));
        

        vector<Node*> t = {t1, (yyvsp[-3].node), t2, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"EXPORTS Name TO Name SEMICOLON->ModuleDirective");
    }
#line 4016 "y.tab.c"
    break;

  case 30: /* ModuleDirective: EXPORTS Name TO Name COMMA Name CommaNames SEMICOLON  */
#line 797 "parser.yacc"
                                                           {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-7].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-5].lexeme));
        
        vector<Node*> t = {t1, (yyvsp[-6].node), t2};
        
        if((yyvsp[-1].node)) {
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-2].node));
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-4].node));
            t.push_back((yyvsp[-1].node));
        }
        else {t.push_back((yyvsp[-4].node)); t.push_back((yyvsp[-2].node));}
        

        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"EXPORTS Name TO Name COMMA Name CommaNames SEMICOLON->ModuleDirective");
    }
#line 4038 "y.tab.c"
    break;

  case 31: /* ModuleDirective: OPENS Name SEMICOLON  */
#line 814 "parser.yacc"
                            {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-2].lexeme));
        

        vector<Node*> t = {t1, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"OPENS Name SEMICOLON->ModuleDirective");
    }
#line 4051 "y.tab.c"
    break;

  case 32: /* ModuleDirective: OPENS Name TO Name COMMA Name CommaNames SEMICOLON  */
#line 822 "parser.yacc"
                                                       {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-7].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-5].lexeme));
     
        vector<Node*> t = {t1, (yyvsp[-6].node)};
        
        if((yyvsp[-1].node)) {
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-2].node));
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-4].node));
            t.push_back((yyvsp[-1].node));
        }
        else { 
            t.push_back((yyvsp[-4].node));
            t.push_back((yyvsp[-2].node));
        }
        
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"OPENS Name TO Name COMMA Name CommaNames SEMICOLON->ModuleDirective");
    }
#line 4075 "y.tab.c"
    break;

  case 33: /* ModuleDirective: USES Name SEMICOLON  */
#line 841 "parser.yacc"
                        {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-2].lexeme));
      

        vector<Node*> t = {t1, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"USES Name SEMICOLON->ModuleDirective");
    }
#line 4088 "y.tab.c"
    break;

  case 34: /* ModuleDirective: PROVIDES Name WITH Name SEMICOLON  */
#line 849 "parser.yacc"
                                        {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-4].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-2].lexeme));
       

        vector<Node*> t = {t1, (yyvsp[-3].node), t2, (yyvsp[-1].node)};
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"PROVIDES Name WITH Name SEMICOLON->ModuleDirective");
    }
#line 4102 "y.tab.c"
    break;

  case 35: /* ModuleDirective: PROVIDES Name WITH Name COMMA Name CommaNames SEMICOLON  */
#line 858 "parser.yacc"
                                                               {
        struct Node* t1 = new struct Node ("Keyword",  (yyvsp[-7].lexeme));
        struct Node* t2 = new struct Node ( "Keyword" ,(yyvsp[-5].lexeme));
        

        vector<Node*> t = {t1, (yyvsp[-6].node), t2};
        
        if((yyvsp[-1].node)) {
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-2].node));
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-4].node));
            t.push_back((yyvsp[-1].node));
        }
        else t.push_back((yyvsp[-2].node));
        
        (yyval.node) = new struct Node("ModuleDirective", t);
        verbose(v,"PROVIDES Name WITH Name COMMA Name CommaNames SEMICOLON->ModuleDirective");
    }
#line 4124 "y.tab.c"
    break;

  case 36: /* RequiresModifier: TRANSITIVE  */
#line 878 "parser.yacc"
                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n1);
    struct Node* n = new struct Node("RequiresModifier", temp);
    (yyval.node) = n;
    verbose(v,"TRANSITIVE->RequiresModifier");
}
#line 4137 "y.tab.c"
    break;

  case 37: /* RequiresModifier: STATIC  */
#line 886 "parser.yacc"
            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n1);
    struct Node* n = new struct Node("RequiresModifier", temp);
    (yyval.node) = n;
    verbose(v,"STATIC->RequiresModifier");
}
#line 4150 "y.tab.c"
    break;

  case 38: /* TopLevelClassOrInterfaceDeclaration: ClassDeclaration  */
#line 898 "parser.yacc"
                         {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ClassDeclaration->TopLevelClassOrInterfaceDeclaration");
    }
#line 4159 "y.tab.c"
    break;

  case 39: /* TopLevelClassOrInterfaceDeclarations: %empty  */
#line 905 "parser.yacc"
    {(yyval.node) = NULL;}
#line 4165 "y.tab.c"
    break;

  case 40: /* TopLevelClassOrInterfaceDeclarations: TopLevelClassOrInterfaceDeclaration TopLevelClassOrInterfaceDeclarations  */
#line 906 "parser.yacc"
                                                                                {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("TopLevelClassOrInterfaceDeclarations", temp);
        verbose(v,"TopLevelClassOrInterfaceDeclaration TopLevelClassOrInterfaceDeclarations->TopLevelClassOrInterfaceDeclarations");
    }
#line 4187 "y.tab.c"
    break;

  case 41: /* Name: IDENTIFIER  */
#line 927 "parser.yacc"
    {
        (yyval.node) = new Node("Identifier", (yyvsp[0].lexeme));
        verbose(v,"IDENTIFIER->Name");
        
        if(importflag==0)
        {
        Symbol* res = root->lookup((yyvsp[0].lexeme));
        if(!res)
        {
            if(!root->flookup((yyvsp[0].lexeme)))
            {
                cout<<"Error! Name "<<(yyvsp[0].lexeme)<<" has not been declared before"<<endl;
                yyerror("Error");
            }
        }
        else
        {
            (yyval.node)->type = res->type;
        }
        }
     }
#line 4213 "y.tab.c"
    break;

  case 42: /* Name: Name DOT IDENTIFIER  */
#line 949 "parser.yacc"
    {
        string s = (yyvsp[-2].node)->attr + (yyvsp[-1].lexeme) + (yyvsp[0].lexeme);
        (yyval.node) = new Node("Name", s);
        verbose(v,"Name DOT IDENTIFIER->Name");

        if(importflag==0)
        {
        int i = spacestripind((yyvsp[-2].node)->attr);
        string sp = ((yyvsp[-2].node)->attr).substr(i+1, (yyvsp[-2].node)->attr.length()-i-1);
        string spf;
        SymNode* tr = nullptr;
        if(i>0)
        {
            int j = i-1;
            while(j>=0 && (yyvsp[-2].node)->attr[j]!='.')
                j--;
            spf = ((yyvsp[-2].node)->attr).substr(j+1, i-j-1);
            tr = list_class[typeroot->inv_types[root->lookup(spf)->type]];
        }
        
        Symbol* r;
        if(!tr)
            r = root->lookup(sp);
        else
        {
            r = tr->scope_lookup(sp);
        }
        if(!r)
        {
            cout<<"Error on line number "<<yylineno<<". Name "<<sp<<" has not been declared before "<<endl;
            yyerror("Error");
        }
        string typ = typeroot->inv_types[r->type];
        // SymNode* t = root->clookup(typ);
        SymNode* t = list_class[typ];
        magic_ptr = t;
        SymNode* res = t->scope_flookup((yyvsp[0].lexeme));
        Symbol* symb = t->scope_lookup((yyvsp[0].lexeme));
        if(typ==r->lexeme && symb && symb->isStatic==false)
        {
            cout<<"Error on line number "<<yylineno<<". Non-static member cannot be accessed in a static way"<<endl;
            yyerror("Error");
        }
        if(!res && !symb)
        {
            cout<<"Error! Name "<<(yyvsp[0].lexeme)<<" has not been declared before"<<endl;
            yyerror("Error");
        }
        if(res)
            (yyval.node)->type = res->returntype;
        else
            (yyval.node)->type = symb->type;
        }
    }
#line 4272 "y.tab.c"
    break;

  case 43: /* DotIdentifiers: %empty  */
#line 1006 "parser.yacc"
    {(yyval.node) = NULL;}
#line 4278 "y.tab.c"
    break;

  case 44: /* DotIdentifiers: DOT IDENTIFIER DotIdentifiers  */
#line 1007 "parser.yacc"
                                       {
    vector<struct Node*> temp;
    //struct Node* n1 = new struct Node("Separator", $1);
    //temp.push_back(n1);
    struct Node* t = new Node("Identifier", (yyvsp[-1].lexeme));
    temp.push_back(t);
    
    if((yyvsp[0].node)) {
        for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);

    }
    else (yyval.node) = t;
    verbose(v,"DOT IDENTIFIER DotIdentifiers->DotIdentifiers");
}
#line 4300 "y.tab.c"
    break;

  case 45: /* CommaNames: %empty  */
#line 1027 "parser.yacc"
    {(yyval.node) = NULL;}
#line 4306 "y.tab.c"
    break;

  case 46: /* CommaNames: COMMA Name CommaNames  */
#line 1028 "parser.yacc"
                            {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("Names", temp);
        verbose(v,"COMMA Name CommaNames ->CommaNames");
}
#line 4328 "y.tab.c"
    break;

  case 47: /* Class: CLASS IDENTIFIER  */
#line 1054 "parser.yacc"
                     {
        (yyval.lexeme) = (yyvsp[0].lexeme);
        Quadruple* q= new Quadruple(7, "", string("beginclass ") + (yyvsp[0].lexeme), "");
        
        ircode.push_back(q);
       
    }
#line 4340 "y.tab.c"
    break;

  case 48: /* ClassDeclaration: NormalClassDeclaration  */
#line 1064 "parser.yacc"
                            {
        (yyval.node)= (yyvsp[0].node);
        verbose(v,"NormalClassDeclaration->ClassDeclaration");
    }
#line 4349 "y.tab.c"
    break;

  case 49: /* NormalClassDeclaration: Class ClassBody  */
#line 1074 "parser.yacc"
    {
        
        vector<Node*> temp ;
        if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        //vector<Node*> temp = {t1, t2, $3};
        (yyval.node) = new Node("NormalClassDeclaration", temp);
        //Quadruple* q = new Quadruple(7, "", "beginclass", $2);
        Quadruple* q1 = new Quadruple(7, "", "endclass", ""); 
        (yyval.node)->code.push_back(q1);
        ircode.push_back(q1);
        (yyval.node)->last = ircode.size() - 1;
        
        verbose(v,"Class ClassBody->NormalClassDeclaration");
        
    }
#line 4374 "y.tab.c"
    break;

  case 50: /* NormalClassDeclaration: Class TypeParameters ClassBody  */
#line 1095 "parser.yacc"
    {
        
        vector<Node*> temp ;
        if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
       
        
        (yyval.node) = new Node("NormalClassDeclaration", temp);
        Quadruple* q1 = new Quadruple(7, "", "endclass", "");
        (yyval.node)->code.push_back(q1);
        ircode.push_back(q1);
        (yyval.node)->last = ircode.size() - 1;
        verbose(v,"Class TypeParameters ClassBody->NormalClassDeclaration");
    }
#line 4397 "y.tab.c"
    break;

  case 51: /* NormalClassDeclaration: Class ClassExtends ClassBody  */
#line 1113 "parser.yacc"
                                  {
    vector<struct Node*> temp;
    
    if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
   Quadruple* q1 = new Quadruple(7, "", "endclass", "");
        (yyval.node)->code.push_back(q1);
        ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
        
    verbose(v,"Class ClassExtends ClassBody->NormalClassDeclaration");
    SymNode* r = root->clookup((yyvsp[-1].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-1].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4429 "y.tab.c"
    break;

  case 52: /* NormalClassDeclaration: Class ClassPermits ClassBody  */
#line 1140 "parser.yacc"
                                  {
    vector<struct Node*> temp;
   
    if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Class ClassPermits ClassBody->NormalClassDeclaration");
}
#line 4452 "y.tab.c"
    break;

  case 53: /* NormalClassDeclaration: Class TypeParameters ClassExtends ClassBody  */
#line 1158 "parser.yacc"
                                                  {
    vector<struct Node*> temp;
    
    if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Class TypeParameters ClassExtends ClassBody->NormalClassDeclaration");
    SymNode* r = root->clookup((yyvsp[-1].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-1].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4489 "y.tab.c"
    break;

  case 54: /* NormalClassDeclaration: Class TypeParameters ClassPermits ClassBody  */
#line 1190 "parser.yacc"
                                                  {
    vector<struct Node*> temp;
    
    if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Class TypeParameters ClassPermits ClassBody->NormalClassDeclaration");
    SymNode* res = root->currNode->scope_clookup((yyvsp[-3].lexeme));
    if(res)
    {
        cout<<"Error! Class "<<(yyvsp[-2].node)<<" has already been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    res = new SymNode(root->currNode, "New class", CLASS_SYM);
    root->cinsert((yyvsp[-3].lexeme), res);
    Symbol* r = new Symbol((yyvsp[-3].lexeme), typeroot->addNewClassType(), yylineno);
    list_class[(yyvsp[-3].lexeme)]=res;
    root->insert(r->lexeme, r);
}
#line 4530 "y.tab.c"
    break;

  case 55: /* NormalClassDeclaration: Class ClassExtends ClassPermits ClassBody  */
#line 1226 "parser.yacc"
                                                  {
    vector<struct Node*> temp;
    
    if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Class ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup((yyvsp[-2].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-2].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    
    root->currNode->parent=r;
}
#line 4569 "y.tab.c"
    break;

  case 56: /* NormalClassDeclaration: Class TypeParameters ClassExtends ClassPermits ClassBody  */
#line 1260 "parser.yacc"
                                                             {
    vector<struct Node*> temp;
    
    if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
    if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
   
        
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Class TypeParameters ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup((yyvsp[-2].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-2].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4614 "y.tab.c"
    break;

  case 57: /* NormalClassDeclaration: Modifier Modifiers Class ClassBody  */
#line 1300 "parser.yacc"
                                         {
    vector<struct Node*> temp;
    if((yyvsp[-2].node)) {
        (yyvsp[-2].node)->addChildToLeft((yyvsp[-3].node));
        temp.push_back((yyvsp[-2].node));
    }
    else temp.push_back((yyvsp[-3].node));
    
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassBody->NormalClassDeclaration");
}
#line 4636 "y.tab.c"
    break;

  case 58: /* NormalClassDeclaration: Modifier Modifiers Class TypeParameters ClassBody  */
#line 1317 "parser.yacc"
                                                         {
    vector<struct Node*> temp;
    if((yyvsp[-3].node)) {
        (yyvsp[-3].node)->addChildToLeft((yyvsp[-4].node));
        temp.push_back((yyvsp[-3].node));
    }
    else temp.push_back((yyvsp[-4].node));
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassBody->NormalClassDeclaration");
}
#line 4659 "y.tab.c"
    break;

  case 59: /* NormalClassDeclaration: Modifier Modifiers Class ClassExtends ClassBody  */
#line 1335 "parser.yacc"
                                                     {
    vector<struct Node*> temp;
    if((yyvsp[-3].node)) {
        (yyvsp[-3].node)->addChildToLeft((yyvsp[-4].node));
        temp.push_back((yyvsp[-3].node));
    }
    else temp.push_back((yyvsp[-4].node));
   
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup((yyvsp[-1].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-1].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4691 "y.tab.c"
    break;

  case 60: /* NormalClassDeclaration: Modifier Modifiers Class ClassPermits ClassBody  */
#line 1362 "parser.yacc"
                                                     {
    vector<struct Node*> temp;
    if((yyvsp[-3].node)) {
        (yyvsp[-3].node)->addChildToLeft((yyvsp[-4].node));
        temp.push_back((yyvsp[-3].node));
    }
    else temp.push_back((yyvsp[-4].node));
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassPermits ClassBody->NormalClassDeclaration");
}
#line 4714 "y.tab.c"
    break;

  case 61: /* NormalClassDeclaration: Modifier Modifiers Class TypeParameters ClassExtends ClassBody  */
#line 1380 "parser.yacc"
                                                                     {
    vector<struct Node*> temp;
    if((yyvsp[-4].node)) {
        (yyvsp[-4].node)->addChildToLeft((yyvsp[-5].node));
        temp.push_back((yyvsp[-4].node));
    }
    else temp.push_back((yyvsp[-5].node));
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup((yyvsp[-1].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-1].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4747 "y.tab.c"
    break;

  case 62: /* NormalClassDeclaration: Modifier Modifiers Class TypeParameters ClassPermits ClassBody  */
#line 1408 "parser.yacc"
                                                                     {
    vector<struct Node*> temp;
    if((yyvsp[-4].node)) {
        (yyvsp[-4].node)->addChildToLeft((yyvsp[-5].node));
        temp.push_back((yyvsp[-4].node));
    }
    else temp.push_back((yyvsp[-5].node));
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassPermits ClassBody->NormalClassDeclaration");
}
#line 4771 "y.tab.c"
    break;

  case 63: /* NormalClassDeclaration: Modifier Modifiers Class ClassExtends ClassPermits ClassBody  */
#line 1427 "parser.yacc"
                                                                 {
    vector<struct Node*> temp;
    if((yyvsp[-4].node)) {
        (yyvsp[-4].node)->addChildToLeft((yyvsp[-5].node));
        temp.push_back((yyvsp[-4].node));
    }
    else temp.push_back((yyvsp[-5].node));
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->currNode->scope_clookup((yyvsp[-2].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-2].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4804 "y.tab.c"
    break;

  case 64: /* NormalClassDeclaration: Modifier Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody  */
#line 1455 "parser.yacc"
                                                                                 {
    vector<struct Node*> temp;
    if((yyvsp[-5].node)) {
        (yyvsp[-5].node)->addChildToLeft((yyvsp[-6].node));
        temp.push_back((yyvsp[-5].node));
    }
    else temp.push_back((yyvsp[-6].node));
    
    temp.push_back((yyvsp[-2].node));
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
   Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup((yyvsp[-2].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-2].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4838 "y.tab.c"
    break;

  case 65: /* NormalClassDeclaration: FINAL Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody  */
#line 1484 "parser.yacc"
                                                                              {
    struct Node * g= new Node("Keyword",(yyvsp[-6].lexeme));
    vector<struct Node*> temp;
    if((yyvsp[-5].node)) {
        (yyvsp[-5].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-5].node));
    }
    else temp.push_back(g);
    
    temp.push_back((yyvsp[-2].node));
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"FINAL Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody->NormalClassDeclaration");
    SymNode* r = root->clookup((yyvsp[-2].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-2].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4872 "y.tab.c"
    break;

  case 66: /* NormalClassDeclaration: FINAL Modifiers Class ClassBody  */
#line 1513 "parser.yacc"
                                      {
    struct Node * g= new Node("Keyword",(yyvsp[-3].lexeme));
    vector<struct Node*> temp;
    if((yyvsp[-2].node)) {
        (yyvsp[-2].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-2].node));
    }
    else temp.push_back(g);
    
  
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassBody->NormalClassDeclaration");
}
#line 4895 "y.tab.c"
    break;

  case 67: /* NormalClassDeclaration: FINAL Modifiers Class TypeParameters ClassBody  */
#line 1531 "parser.yacc"
                                                      {
    struct Node * g= new Node("Keyword",(yyvsp[-4].lexeme));
    vector<struct Node*> temp;
    if((yyvsp[-3].node)) {
        (yyvsp[-3].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-3].node));
    }
    else temp.push_back(g);
    
    temp.push_back((yyvsp[0].node));
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassBody->NormalClassDeclaration");
}
#line 4919 "y.tab.c"
    break;

  case 68: /* NormalClassDeclaration: FINAL Modifiers Class ClassExtends ClassBody  */
#line 1550 "parser.yacc"
                                                  {
    struct Node * g= new Node("Keyword",(yyvsp[-4].lexeme));
    vector<struct Node*> temp;
    if((yyvsp[-3].node)) {
        (yyvsp[-3].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-3].node));
    }
    else temp.push_back(g);
   
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"FINAL Modifiers Class ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup((yyvsp[-1].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-1].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 4952 "y.tab.c"
    break;

  case 69: /* NormalClassDeclaration: FINAL Modifiers Class ClassPermits ClassBody  */
#line 1578 "parser.yacc"
                                                  {
    struct Node * g= new Node("Keyword",(yyvsp[-4].lexeme));
    vector<struct Node*> temp;
    if((yyvsp[-3].node)) {
        (yyvsp[-3].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-3].node));
    }
    else temp.push_back(g);
   
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    verbose(v,"Modifier Modifiers Class ClassPermits ClassBody->NormalClassDeclaration");
}
#line 4972 "y.tab.c"
    break;

  case 70: /* NormalClassDeclaration: FINAL Modifiers Class TypeParameters ClassExtends ClassBody  */
#line 1593 "parser.yacc"
                                                                  {
    vector<struct Node*> temp;
    struct Node * g= new Node("Keyword",(yyvsp[-5].lexeme));
    if((yyvsp[-4].node)) {
        (yyvsp[-4].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-4].node));
    }
    else temp.push_back(g);
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup((yyvsp[-1].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-1].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 5002 "y.tab.c"
    break;

  case 71: /* NormalClassDeclaration: FINAL Modifiers Class TypeParameters ClassPermits ClassBody  */
#line 1618 "parser.yacc"
                                                                  {
    vector<struct Node*> temp;
    struct Node * g= new Node("Keyword",(yyvsp[-5].lexeme));
    if((yyvsp[-4].node)) {
        (yyvsp[-4].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-4].node));
    }
    else temp.push_back(g);
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassPermits ClassBody->NormalClassDeclaration");
}
#line 5027 "y.tab.c"
    break;

  case 72: /* NormalClassDeclaration: FINAL Modifiers Class ClassExtends ClassPermits ClassBody  */
#line 1638 "parser.yacc"
                                                              {
    vector<struct Node*> temp;
    struct Node * g= new Node("Keyword",(yyvsp[-5].lexeme));
    if((yyvsp[-4].node)) {
        (yyvsp[-4].node)->addChildToLeft(g);
        temp.push_back((yyvsp[-4].node));
    }
    else temp.push_back(g);
    
    temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    (yyval.node) = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "");
    (yyval.node)->code.push_back(q1);
    ircode.push_back(q1);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->currNode->scope_clookup((yyvsp[-2].node)->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<(yyvsp[-2].node)->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
#line 5061 "y.tab.c"
    break;

  case 73: /* Modifier: PUBLIC  */
#line 1670 "parser.yacc"
            {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"PUBLIC ->Modifier");
    
}
#line 5073 "y.tab.c"
    break;

  case 74: /* Modifier: PROTECTED  */
#line 1677 "parser.yacc"
                {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"PROTECTED ->Modifier");
}
#line 5084 "y.tab.c"
    break;

  case 75: /* Modifier: PRIVATE  */
#line 1683 "parser.yacc"
            {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"PRIVATE ->Modifier");
}
#line 5095 "y.tab.c"
    break;

  case 76: /* Modifier: STATIC  */
#line 1690 "parser.yacc"
            {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"STATIC ->Modifier");
}
#line 5106 "y.tab.c"
    break;

  case 77: /* Modifiers: %empty  */
#line 1699 "parser.yacc"
    {(yyval.node) = NULL;}
#line 5112 "y.tab.c"
    break;

  case 78: /* Modifiers: Modifier Modifiers  */
#line 1700 "parser.yacc"
                        {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("Modifiers", temp);
        verbose(v,"Modifier Modifiers->Modifiers");
    }
#line 5134 "y.tab.c"
    break;

  case 79: /* TypeParameters: LSS TypeParameterList GRT  */
#line 1720 "parser.yacc"
                                {
    vector<struct Node*> temp;
    
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("TypeParameters", temp);
    (yyval.node) = n;
    verbose(v,"LSS TypeParameterList GRT ->TypeParameters");
}
#line 5148 "y.tab.c"
    break;

  case 80: /* TypeParameterList: TypeParameter  */
#line 1732 "parser.yacc"
                     {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"TypeParameter->TypeParameterList");
    }
#line 5157 "y.tab.c"
    break;

  case 81: /* TypeParameterList: TypeParameter COMMA TypeParameter CommaTypeParameters  */
#line 1736 "parser.yacc"
                                                          {
    vector<struct Node*> temp;
    if((yyvsp[0].node)) {
        (yyvsp[0].node)->addChildToLeft((yyvsp[-1].node));
        (yyvsp[0].node)->addChildToLeft((yyvsp[-3].node));
        temp.push_back((yyvsp[0].node));
    } 
    else {
        temp.push_back((yyvsp[-3].node));
        temp.push_back((yyvsp[-1].node));
    }
    struct Node* n = new struct Node("TypeParameterList", temp);
    (yyval.node) = n;
    verbose(v,"TypeParameter COMMA TypeParameter CommaTypeParameters->TypeParameterList");
}
#line 5177 "y.tab.c"
    break;

  case 82: /* ClassExtends: EXTENDS Name  */
#line 1754 "parser.yacc"
                    {
    // vector<struct Node*> temp;
    // struct Node* n1 = new struct Node("Keyword", $1);
    // temp.push_back(n1);
    // temp.push_back($2);
    // struct Node* n = new struct Node("ClassExtends", temp);
    // $$ = n;
    root->currNode->name="classextends";
    root->currNode->ogparent=root->currNode->parent;
    root->currNode->parent->childscopes.pop_back();
    if(list_class.find((yyvsp[0].node)->attr)==list_class.end()){
        cout<<"No such class exist error "<<(yyvsp[0].node)->attr<<" on line number "<<yylineno<<endl;
        yyerror("error");
    }
    if(list_class[(yyvsp[0].node)->attr]->isFinalClass==true){
        cout<<"Cannot be extended "<<(yyvsp[0].node)->attr<<" on line number "<<yylineno<<endl;
        yyerror("error");
    }
    root->currNode->parent=list_class[(yyvsp[0].node)->attr];
    root->currNode->parent->childscopes.push_back(root->currNode);
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"EXTENDS Name->ClassExtends");
}
#line 5205 "y.tab.c"
    break;

  case 83: /* ClassPermits: PERMITS Name  */
#line 1781 "parser.yacc"
                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-1].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("ClassPermits", temp);
    (yyval.node) = n;
    verbose(v,"PERMITS Name->ClassPermits");
}
#line 5219 "y.tab.c"
    break;

  case 84: /* ClassPermits: PERMITS Name COMMA Name CommaNames  */
#line 1790 "parser.yacc"
                                       {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    if((yyvsp[0].node)) {
        (yyvsp[0].node)->addChildToLeft((yyvsp[-1].node));
        (yyvsp[0].node)->addChildToLeft((yyvsp[-3].node));
        temp.push_back((yyvsp[0].node));
    }
    else{
        temp.push_back((yyvsp[-3].node));
        temp.push_back((yyvsp[-1].node));
    } 
    
    struct Node* n = new struct Node("ClassPermits", temp);
    (yyval.node) = n;
    verbose(v,"PERMITS Name COMMA Name CommaNames->ClassPermits");
}
#line 5242 "y.tab.c"
    break;

  case 85: /* ClassBody: LEFTCURLYBRACKET RIGHTCURLYBRACKET  */
#line 1811 "parser.yacc"
                                           {
        vector<Node*>temp;
        (yyval.node) = new struct Node("ClassBody", temp);
        verbose(v,"LEFTCURLYBRACKET RIGHTCURLYBRACKET->ClassBody");
    }
#line 5252 "y.tab.c"
    break;

  case 86: /* ClassBody: LEFTCURLYBRACKET ClassBodyDeclaration ClassBodyDeclarations RIGHTCURLYBRACKET  */
#line 1817 "parser.yacc"
    {
        //struct Node* t1 = new Node ("Separator",  "{");
        //struct Node* t2 = new Node ( "Separator", "}");
        
        vector<Node*> temp ;
        if((yyvsp[-1].node)) {
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-2].node));
            temp.push_back((yyvsp[-1].node));
        }
        else temp.push_back((yyvsp[-2].node));
        
       
        (yyval.node) = new Node("ClassBody", temp);
        verbose(v,"LEFTCURLYBRACKET ClassBodyDeclaration ClassBodyDeclarations RIGHTCURLYBRACKET->ClassBody");
    }
#line 5272 "y.tab.c"
    break;

  case 87: /* ClassBodyDeclaration: ClassMemberDeclaration  */
#line 1835 "parser.yacc"
                          {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ClassMemberDeclaration->ClassBodyDeclaration");
    }
#line 5281 "y.tab.c"
    break;

  case 88: /* ClassBodyDeclaration: InstanceInitializer  */
#line 1839 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"InstanceInitializer->ClassBodyDeclaration");
    }
#line 5290 "y.tab.c"
    break;

  case 89: /* ClassBodyDeclaration: StaticInitializer  */
#line 1843 "parser.yacc"
                        {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"StaticInitializer->ClassBodyDeclaration");
    }
#line 5299 "y.tab.c"
    break;

  case 90: /* ClassBodyDeclaration: ConstructorDeclaration  */
#line 1847 "parser.yacc"
                             {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"ConstructorDeclaration->ClassBodyDeclaration");
    }
#line 5308 "y.tab.c"
    break;

  case 91: /* ClassBodyDeclarations: %empty  */
#line 1854 "parser.yacc"
    {(yyval.node) =NULL;}
#line 5314 "y.tab.c"
    break;

  case 92: /* ClassBodyDeclarations: ClassBodyDeclaration ClassBodyDeclarations  */
#line 1855 "parser.yacc"
                                                {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("BlockStatements", temp);
        verbose(v,"ClassBodyDeclaration ClassBodyDeclarations->ClassBodyDeclarations");
    }
#line 5336 "y.tab.c"
    break;

  case 93: /* ClassMemberDeclaration: FieldDeclaration  */
#line 1875 "parser.yacc"
                        {
       (yyval.node) = (yyvsp[0].node);
        verbose(v,"FieldDeclaration->ClassMemberDeclaration");
    }
#line 5345 "y.tab.c"
    break;

  case 94: /* ClassMemberDeclaration: MethodDeclaration  */
#line 1879 "parser.yacc"
                        {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"MethodDeclaration->ClassMemberDeclaration");
    }
#line 5354 "y.tab.c"
    break;

  case 95: /* ClassMemberDeclaration: ClassDeclaration  */
#line 1883 "parser.yacc"
                     {
        (yyval.node)=(yyvsp[0].node);
        verbose(v,"ClassDeclaration->ClassMemberDeclaration");
 }
#line 5363 "y.tab.c"
    break;

  case 96: /* FieldDeclaration: Type VariableDeclaratorList SEMICOLON  */
#line 1893 "parser.yacc"
    {
        //struct Node* t = new Node("Separator", ";");
        vector<Node*> temp;
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        if((yyvsp[-1].node)->useful == false) {
            for(auto it : (yyvsp[-1].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        //temp.push_back(t);
        (yyval.node) = new Node("FieldDeclaration", temp);
       
        verbose(v,"Type VariableDeclaratorList SEMICOLON->FieldDeclaration");

        for(auto ch : (yyvsp[-1].node)->children)
        {
            int _type = (yyvsp[-2].node)->type;
            if(ch->arrayType && _type < 100) _type += ch->arrayType*100  ; 
            if(ch->children.size() > 1 && ch->children[1]->arrayType) {ch->children[1]->type = (yyvsp[-2].node)->type%100 + 100 * ch->children[1]->arrayType; 
                 if(ch->children[1]->label == "ArrayInitializer") {
                    if(ch->arrayType == 1 ) init1DArray(ch, (yyvsp[-2].node)->attr);
                    else if(ch->arrayType == 2 ) init2DArray(ch, (yyvsp[-2].node)->attr);
                    else if(ch->arrayType == 3 ) init3DArray(ch, (yyvsp[-2].node)->attr);
                }
            }
            cout << ch->arrayType << "\n";
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[(yyvsp[-2].node)->attr].second);
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                if(!ch->arrayType) processFieldDec((yyval.node), ch, _type);

            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                //sym->type += 100*ch->arrayType;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           }
            
            root->insert(sym->lexeme, sym);
        }
        processPostIncre((yyval.node));
    }
#line 5428 "y.tab.c"
    break;

  case 97: /* FieldDeclaration: Modifier Modifiers Type VariableDeclaratorList SEMICOLON  */
#line 1953 "parser.yacc"
                                                                  {
       // struct Node* t = new struct Node("Separator", $5);
        vector<Node*> temp;
        if((yyvsp[-3].node)) {
            (yyvsp[-3].node)->addChildToLeft((yyvsp[-4].node));
            temp.push_back((yyvsp[-3].node));
        }
        else temp.push_back((yyvsp[-4].node));
        
        temp.push_back((yyvsp[-2].node));
        temp.push_back((yyvsp[-1].node));
        (yyval.node) = new struct Node("FieldDeclaration", temp);
        verbose(v,"Modifier Modifiers Type VariableDeclaratorList SEMICOLON->FieldDeclaration");

        // cout<<"Here, field's accesss type is "<<$1->attr;
        int acc=PUBLIC_ACCESS, isStatic=0;
        if((yyvsp[-4].node)->attr=="private")
            acc = PRIVATE_ACCESS;
        else if((yyvsp[-4].node)->attr=="protected")
            acc = PROTECTED_ACCESS;

        if((yyvsp[-4].node)->attr=="static")
        {
            isStatic=1;
        }

        if((yyvsp[-3].node))
        {
            for(auto ch : (yyvsp[-3].node)->children)
            {
                if(ch->attr=="private")
                    acc = PRIVATE_ACCESS;
                else if(ch->attr=="protected")
                    acc = PROTECTED_ACCESS;

                if(ch->attr=="static")
                    isStatic=1;
            }
        }
        for(auto ch : (yyvsp[-1].node)->children)
        {
            int _type = (yyvsp[-2].node)->type;
            if(ch->arrayType) _type += ch->arrayType*100  ; 
            if(ch->children.size() > 1 && ch->children[1]->arrayType) 
            {
                ch->children[1]->type = (yyvsp[-2].node)->type + 100 * ch->children[1]->arrayType; 
                 if(ch->children[1]->label == "ArrayInitializer") {
                    if(ch->arrayType == 1 ) init1DArray(ch, (yyvsp[-2].node)->attr);
                    else if(ch->arrayType == 2 ) init2DArray(ch, (yyvsp[-2].node)->attr);
                    else if(ch->arrayType == 3 ) init3DArray(ch, (yyvsp[-2].node)->attr);
                }
            }
            cout << ch->arrayType << "\n";
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[(yyvsp[-2].node)->attr].second, acc);
            sym->isStatic = isStatic;
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                if(!ch->arrayType ) processFieldDec((yyval.node), ch, _type);

            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           }
            
            root->insert(sym->lexeme, sym);
            // cout<<"Inserted "<<sym->lexeme<<endl;
        }
        processPostIncre((yyval.node));
    }
#line 5513 "y.tab.c"
    break;

  case 98: /* FieldDeclaration: FINAL Modifiers Type VariableDeclaratorList SEMICOLON  */
#line 2033 "parser.yacc"
                                                               {
       // struct Node* t = new struct Node("Separator", $5);
        vector<Node*> temp;
        struct Node * g= new Node("Keyword",(yyvsp[-4].lexeme));
        if((yyvsp[-3].node)) {
            (yyvsp[-3].node)->addChildToLeft(g);
            temp.push_back((yyvsp[-3].node));
        }
        else temp.push_back(g);
        
        temp.push_back((yyvsp[-2].node));
        temp.push_back((yyvsp[-1].node));
        (yyval.node) = new struct Node("FieldDeclaration", temp);
        verbose(v,"FINAL Modifiers Type VariableDeclaratorList SEMICOLON->FieldDeclaration");

        // cout<<"Here, field's accesss type is "<<$1->attr;
        int acc=PRIVATE_ACCESS;
        if((yyvsp[-3].node)&&(yyvsp[-3].node)->attr=="public")
            acc = PUBLIC_ACCESS;
        else if((yyvsp[-3].node)&&(yyvsp[-3].node)->attr=="protected")
            acc = PROTECTED_ACCESS;
        for(auto ch : (yyvsp[-1].node)->children)
        {
            int _type = (yyvsp[-2].node)->type;
            if(ch->arrayType) _type += ch->arrayType*100  ; 
            if(ch->children.size() > 1 && ch->children[1]->arrayType) {ch->children[1]->type = (yyvsp[-2].node)->type + 100 * ch->children[1]->arrayType; 
                if(ch->arrayType == 1) init1DArray(ch, (yyvsp[-2].node)->attr);
                else if(ch->arrayType == 2) init2DArray(ch, (yyvsp[-2].node)->attr);
                else if(ch->arrayType == 3) init3DArray(ch, (yyvsp[-2].node)->attr);

            }
            cout << ch->arrayType << "\n";
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[(yyvsp[-2].node)->attr].second, acc);
            sym->isFinal=true;
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                
                sym->lexeme = ch->children[0]->attr;
                if(!ch->arrayType) processFieldDec((yyval.node), ch, _type);

            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           }
            processPostIncre((yyval.node));
            root->insert(sym->lexeme, sym);
            // cout<<"Inserted "<<sym->lexeme<<endl;
        }
    }
#line 5577 "y.tab.c"
    break;

  case 99: /* VariableDeclaratorList: VariableDeclarator CommaVariableDeclarators  */
#line 2101 "parser.yacc"
    {
        //struct Node* t = new Node("Separator", ",");
        vector<struct Node*> temp;
        struct Node* n = new Node("VariableDeclaratorList", temp);
        n->addChild((yyvsp[-1].node));
        Node* tt = (yyvsp[-1].node);
        if((yyvsp[0].node)) {
            for(auto it: (yyvsp[0].node)->children) {
                //backpatch(tt->nextlist, tt->last + 1);
                tt = it;

                n->addChild(it);
            }
            //$2->addChildToLeft($1);
            //temp.push_back($2);
        }
        (yyval.node) = n;
        verbose(v,"VariableDeclarator CommaVariableDeclarators->VariableDeclaratorList");
    }
#line 5601 "y.tab.c"
    break;

  case 100: /* VariableDeclarator: VariableDeclaratorId  */
#line 2125 "parser.yacc"
    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"VariableDeclaratorId->VariableDeclarator");
    }
#line 5610 "y.tab.c"
    break;

  case 101: /* VariableDeclarator: VariableDeclaratorId ASSIGN Expression  */
#line 2130 "parser.yacc"
    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        // if($3)
        //     cout<<"exists exists "<<endl;
        if((yyvsp[0].node)->useful == false) {
            cout<<"not 3 useful"<<endl;
            // if(!$3->children[0])
                // cout<<"Child laso exists"<<endl;
            // cout<<$3->children[0]->type<<endl;
            for(auto it : (yyvsp[0].node)->children)
            {
                cout<<"pushed "<<it->label<<endl;
                temp.push_back(it);
            }
            cout<<"Came outside"<<endl;
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        
        (yyval.node) = t;
        
        (yyval.node)->width1 = (yyvsp[0].node)->width1;
        (yyval.node)->width2 = (yyvsp[0].node)->width2;
        (yyval.node)->width3 = (yyvsp[0].node)->width3;
        (yyval.node)->arrayType = (yyvsp[-2].node)->arrayType;
        (yyval.node)->last = ircode.size() - 1;
        verbose(v,"VariableDeclaratorId ASSIGN Expression->VariableDeclarator");
        condvar = (yyvsp[-2].node)->attr;
        (yyval.node)->isCond = isCond;
        isCond = 0;
       
    }
#line 5662 "y.tab.c"
    break;

  case 102: /* VariableDeclarator: VariableDeclaratorId ASSIGN ArrayInitializer  */
#line 2177 "parser.yacc"
                                                     {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        if((yyvsp[-2].node)->arrayType != (yyvsp[0].node)->arrayType) {
            cout<<"Type Mismatch on line number "<<yylineno<<endl;
            yyerror("Error");
        }
        (yyval.node)->arrayType = (yyvsp[0].node)->arrayType;
        (yyval.node)->width1 = (yyvsp[0].node)->width1;
        (yyval.node)->width2 = (yyvsp[0].node)->width2;
        (yyval.node)->width3 = (yyvsp[0].node)->width3;
        verbose(v,"VariableDeclaratorId ASSIGN ArrayInitializer->VariableDeclaratorId");
}
#line 5704 "y.tab.c"
    break;

  case 103: /* CommaVariableDeclarators: %empty  */
#line 2217 "parser.yacc"
    {(yyval.node) = NULL;}
#line 5710 "y.tab.c"
    break;

  case 104: /* CommaVariableDeclarators: COMMA VariableDeclarator CommaVariableDeclarators  */
#line 2218 "parser.yacc"
                                                        {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("VariableDeclarators", temp);
        verbose(v,"COMMA VariableDeclarator CommaVariableDeclarators->CommaVariableDeclarators");
}
#line 5732 "y.tab.c"
    break;

  case 105: /* VariableDeclaratorId: IDENTIFIER  */
#line 2239 "parser.yacc"
    {
        (yyval.node) = new Node("Identifier", (yyvsp[0].lexeme));
        verbose(v,"IDENTIFIER->VariableDeclaratorId");
        condvar = (yyvsp[0].lexeme);
    }
#line 5742 "y.tab.c"
    break;

  case 106: /* VariableDeclaratorId: IDENTIFIER Dims  */
#line 2244 "parser.yacc"
                     {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-1].lexeme));
    temp.push_back(n1);
    //if($2)temp.push_back($2);
    struct Node* n = new struct Node("VariableDeclaratorId", (yyvsp[-1].lexeme), temp);
    (yyval.node) = n;
    (yyval.node)->arrayType= (yyvsp[0].node)->arrayType;
    //$$->type = $1->type + 100*$2->arrayType;
    printf("\n\n%d\n\n", (yyvsp[0].node)->arrayType);
    verbose(v,"IDENTIFIER Dims->VariableDeclaratorId");
}
#line 5759 "y.tab.c"
    break;

  case 107: /* VariableInitializer: Expression  */
#line 2260 "parser.yacc"
                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Expression->VariableInitializer");
    }
#line 5768 "y.tab.c"
    break;

  case 108: /* VariableInitializer: ArrayInitializer  */
#line 2264 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ArrayInitializer->VariableInitializer");
    }
#line 5777 "y.tab.c"
    break;

  case 109: /* CommaVariableInitializers: %empty  */
#line 2271 "parser.yacc"
    {(yyval.node) = NULL;
        cout<< "epsilon -> CommaVariableInitializers\n";
    }
#line 5785 "y.tab.c"
    break;

  case 110: /* CommaVariableInitializers: COMMA VariableInitializer CommaVariableInitializers  */
#line 2274 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("StatementExpressions", temp);
    verbose(v,"COMMA VariableInitializer CommaVariableInitializers->CommaVariableInitializers");
}
#line 5807 "y.tab.c"
    break;

  case 111: /* leftcurl: LEFTCURLYBRACKET  */
#line 2291 "parser.yacc"
                           {
    isarrayinit++;
    struct Node* n = new struct Node("LEFTCURLYBRACKET");
    (yyval.node) = n;
    verbose(v,"LEFTCURLYBRACKET->leftcurl");
}
#line 5818 "y.tab.c"
    break;

  case 112: /* ArrayInitializer: leftcurl RIGHTCURLYBRACKET  */
#line 2298 "parser.yacc"
                                 {
    
    struct Node* n = new struct Node("ArrayInitializer");
    (yyval.node) = n;
    (yyval.node)->arrayType = 1;
    verbose(v,"LEFTCURLYBRACKET RIGHTCURLYBRACKET->ArrayInitializer");
}
#line 5830 "y.tab.c"
    break;

  case 113: /* ArrayInitializer: leftcurl VariableInitializer CommaVariableInitializers RIGHTCURLYBRACKET  */
#line 2305 "parser.yacc"
                                                                                {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    if((yyvsp[-1].node)) {
        for(auto it: (yyvsp[-1].node)->children) {
            temp.push_back(it);
        }
    }
    struct Node* n = new struct Node("ArrayInitializer", temp);
    (yyval.node) = n;

    (yyval.node)->arrayType = 1 + (yyvsp[-2].node)->arrayType;
    if((yyval.node)->arrayType == 1) {
        (yyval.node)->_width1 = 1 + (((yyvsp[-1].node) != NULL) ? (yyvsp[-1].node)->children.size() : 0);
        (yyval.node)->width1 = to_string((yyval.node)->_width1);
    }
    else if((yyval.node)->arrayType == 2) {
        (yyval.node)->_width2 = (yyvsp[-2].node)->_width1;
        (yyval.node)->_width1 = 1 + (((yyvsp[-1].node) != NULL) ? (yyvsp[-1].node)->children.size() : 0);
        (yyval.node)->width2 = to_string((yyval.node)->_width2);
        (yyval.node)->width2 = to_string((yyval.node)->_width2);
    }
    else{
        (yyval.node)->_width3 = (yyvsp[-2].node)->_width2;
        (yyval.node)->_width2 = (yyvsp[-2].node)->_width1;
        (yyval.node)->_width1 = 1 + (((yyvsp[-1].node) != NULL) ? (yyvsp[-1].node)->children.size() : 0);
        (yyval.node)->width1 = to_string((yyval.node)->_width1);
        (yyval.node)->width2 = to_string((yyval.node)->_width2);
        (yyval.node)->width3 = to_string((yyval.node)->_width3);
    }
    
    //if($3)
    verbose(v,"LEFTCURLYBRACKET VariableInitializer CommaVariableInitializers RIGHTCURLYBRACKET->ArrayInitializer");
}
#line 5869 "y.tab.c"
    break;

  case 114: /* ArrayInitializer: leftcurl COMMA RIGHTCURLYBRACKET  */
#line 2340 "parser.yacc"
                                       {
    struct Node* n = new struct Node("ArrayInitializer");
    (yyval.node) = n;
    (yyval.node)->type = 202;
    verbose(v,"LEFTCURLYBRACKET COMMA RIGHTCURLYBRACKET->ArrayInitializer");
}
#line 5880 "y.tab.c"
    break;

  case 115: /* MethodDeclaration: MethodHeader MethodBody  */
#line 2354 "parser.yacc"
                             {
        vector<struct Node*> temp;
        temp = {(yyvsp[-1].node), (yyvsp[0].node)};
        struct Node* n = new struct Node("MethodDeclaration", temp);
        (yyval.node) = n;
        Quadruple* q = new Quadruple(7, string("endfunc"));
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
        //backpatch($2->nextlist, ircode.size() -1);
        
        //($1);
        //($2);
        verbose(v," MethodHeader MethodBody->MethodDeclaration");
        
    }
#line 5901 "y.tab.c"
    break;

  case 116: /* MethodDeclaration: Modifier Modifiers MethodHeader MethodBody  */
#line 2370 "parser.yacc"
                                                 {
        vector<struct Node*> temp;
        if((yyvsp[-2].node)) {
            (yyvsp[-2].node)->addChildToLeft((yyvsp[-3].node));
            temp.push_back((yyvsp[-2].node));
        }
        else temp.push_back((yyvsp[-3].node));
        temp.push_back((yyvsp[-1].node));
        temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("MethodDeclaration", temp);
        (yyval.node) = n;
        Quadruple* q = new Quadruple(7, string("endfunc"));
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        //backpatch($4->nextlist, ircode.size() -1);
        (yyval.node)->last = ircode.size() - 1;
        //Quadruple* q = new Quadruple(7);
        //$$->code.push_back(q);
        verbose(v,"Modifier Modifiers MethodHeader MethodBody->MethodDeclaration");

        int acc=PUBLIC_ACCESS;
        if((yyvsp[-3].node)->attr=="private")
            acc = PRIVATE_ACCESS;
        
        if((yyvsp[-2].node))
        {
            for(auto ch:(yyvsp[-2].node)->children)
            {
                if((yyvsp[-3].node)->attr=="private")
                    acc = PRIVATE_ACCESS;
            }
        }
        root->currNode->childscopes.back()->node_acc_type = acc;
    }
#line 5940 "y.tab.c"
    break;

  case 117: /* MethodDeclaration: FINAL Modifiers MethodHeader MethodBody  */
#line 2404 "parser.yacc"
                                              {
        vector<struct Node*> temp;
        struct Node * g= new Node("Keyword",(yyvsp[-3].lexeme));
        if((yyvsp[-2].node)) {
            (yyvsp[-2].node)->addChildToLeft(g);
            temp.push_back((yyvsp[-2].node));
        }
        else temp.push_back(g);
        temp.push_back((yyvsp[-1].node));
        temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("MethodDeclaration", temp);
        (yyval.node) = n;
        Quadruple* q = new Quadruple(7, string("endfunc"));
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        //backpatch($4->nextlist, ircode.size() -1);
        (yyval.node)->last = ircode.size() - 1;
        verbose(v,"Modifier Modifiers MethodHeader MethodBody->MethodDeclaration");
    }
#line 5964 "y.tab.c"
    break;

  case 118: /* MethodHeader: Type MethodDeclarator  */
#line 2427 "parser.yacc"
                             {
        vector<struct Node*> temp;
        temp = {(yyvsp[-1].node), (yyvsp[0].node)};
       
        struct Node* n = new struct Node("MethodHeader", temp);
        (yyval.node) = n;
        //($1);
        //($2);
        verbose(v,"Type MethodDeclarator->MethodHeader");

        vector<int> args;
        for(int i=1; i<(yyvsp[0].node)->children.size(); i+=2)
        {
            if((yyvsp[0].node)->children[i+1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[(yyvsp[0].node)->children[i]->attr].first+100*(yyvsp[0].node)->children[i+1]->arrayType);
            }
            else
            {
                args.push_back(typeroot->typewidth[(yyvsp[0].node)->children[i]->attr].first);
            }
        }

        SymNode* check = root->currNode->scope_flookup((yyvsp[0].node)->children[0]->attr, args, typeroot->typewidth[(yyvsp[-1].node)->attr].first);
        SymNode* check2 = root->flookup((yyvsp[0].node)->children[0]->attr, args, typeroot->typewidth[(yyvsp[-1].node)->attr].first);
        if(check2&&check2->isFinalId==true){
            cout<<"Error on line number "<<yylineno<<". Method with name "<<(yyvsp[0].node)->children[0]->attr<<" has been declared before and set to final"<<endl;
            yyerror("Error");
        }
        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Method with name "<<(yyvsp[0].node)->children[0]->attr<<" has been declared before"<<endl;
            yyerror("Error");
        }    
        SymNode* newf = new SymNode(root->currNode, "method", FUNC_SYM, args, typeroot->typewidth[(yyvsp[-1].node)->attr].first);
        if(fin==1){
            fin=0;
            newf->isFinalId=true;
        }
        root->currNode->childscopes.push_back(newf);
        root->currNode=newf;
        root->finsert((yyvsp[0].node)->children[0]->attr, newf);
        root->currNode->name="method";
        for(int i=2; i<(yyvsp[0].node)->children.size(); i+=2)
        {
            Symbol* sym = new Symbol((yyvsp[0].node)->children[i]->attr,args[(i-1)/2], yylineno, typeroot->widths[args[(i-1)/2]]);
            cout << typeroot->widths[args[(i-1)/2]] << "######################\n";
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            root->insert(sym->lexeme, sym);
        }
        Quadruple* q = new Quadruple(6, (yyvsp[0].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    }
#line 6030 "y.tab.c"
    break;

  case 119: /* MethodHeader: VOID MethodDeclarator  */
#line 2488 "parser.yacc"
                              {
        vector<struct Node*> temp;
        struct Node* t = new Node("Keyword", "void");
        temp = {t, (yyvsp[0].node)};
       
        struct Node* n = new struct Node("MethodHeader", temp);
        (yyval.node) = n;
        //($1);
        //($2);
        verbose(v,"VOID MethodDeclarator->MethodHeader");
        vector<int> args;
        for(int i=1; i<(yyvsp[0].node)->children.size(); i+=2)
        {
            if((yyvsp[0].node)->children[i+1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[(yyvsp[0].node)->children[i]->attr].first+100);
            }
            else
                args.push_back(typeroot->typewidth[(yyvsp[0].node)->children[i]->attr].first);
        }

        SymNode* check = root->currNode->scope_flookup((yyvsp[0].node)->children[0]->attr, args, typeroot->typewidth[(yyvsp[-1].lexeme)].first);
        SymNode* check2 = root->flookup((yyvsp[0].node)->children[0]->attr, args, typeroot->typewidth[(yyvsp[-1].lexeme)].first);
        if(check2&&check2->isFinalId==true){
            cout<<"Error on line number "<<yylineno<<". Method with name "<<(yyvsp[0].node)->children[0]->attr<<" has been declared before and set to final"<<endl;
            yyerror("Error");
        }
        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Method with name "<<(yyvsp[0].node)->children[0]->attr<<" has been declared before"<<endl;
            yyerror("Error");
        }         

        SymNode* newf = new SymNode(root->currNode, "New Function", FUNC_SYM, args, typeroot->typewidth["void"].first);
        if(fin==1){
            fin=0;
            newf->isFinalId=true;
        }
        root->currNode->childscopes.push_back(newf);
        root->currNode=newf;
        root->finsert((yyvsp[0].node)->children[0]->attr, newf);
        root->currNode->name="method";
        for(int i=2; i<(yyvsp[0].node)->children.size(); i+=2)
        {
            Symbol* sym = new Symbol((yyvsp[0].node)->children[i]->attr,args[(i-1)/2], yylineno, typeroot->widths[args[(i-1)/2]]);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            root->insert(sym->lexeme, sym);
        }
        Quadruple* q = new Quadruple(6, (yyvsp[0].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    }
#line 6094 "y.tab.c"
    break;

  case 120: /* MethodHeader: TypeParameters Result MethodDeclarator  */
#line 2547 "parser.yacc"
                                             {
        vector<struct Node*> temp;
        temp = {(yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node)};
        struct Node* n = new struct Node("MethodHeader", temp);
        (yyval.node) = n;
        verbose(v,"TypeParameters Result MethodDeclarator->MethodHeader");
        vector<int> args;
        for(int i=1; i<(yyvsp[0].node)->children.size(); i+=2)
        {
            if((yyvsp[0].node)->children[i+1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[(yyvsp[0].node)->children[i]->attr].first+100);
            }
            else
                args.push_back(typeroot->typewidth[(yyvsp[0].node)->children[i]->attr].first);
        }

        SymNode* check = root->currNode->scope_flookup((yyvsp[0].node)->children[0]->attr, args, typeroot->typewidth[(yyvsp[-1].node)->attr].first);

        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Method with name "<<(yyvsp[0].node)->children[0]->attr<<" has been declared before"<<endl;
            yyerror("Error");
        }         

        SymNode* newf = new SymNode(root->currNode, "New Function", FUNC_SYM, args, typeroot->typewidth[(yyvsp[-1].node)->attr].first);
        root->currNode->childscopes.push_back(newf);
        root->currNode=newf;
        root->finsert((yyvsp[0].node)->children[0]->attr, newf);

        root->currNode->name="method";
        for(int i=2; i<(yyvsp[0].node)->children.size(); i+=2)
        {
            Symbol* sym = new Symbol((yyvsp[0].node)->children[i]->attr,args[(i-1)/2], yylineno, typeroot->widths[args[(i-1)/2]]);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            root->insert(sym->lexeme, sym);
        }
        Quadruple* q = new Quadruple(6, (yyvsp[0].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    }
#line 6147 "y.tab.c"
    break;

  case 121: /* Result: Type  */
#line 2599 "parser.yacc"
            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Type->Result");
    }
#line 6156 "y.tab.c"
    break;

  case 122: /* Result: VOID  */
#line 2603 "parser.yacc"
            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n1);
    struct Node* n = new struct Node("Result", temp);
    (yyval.node) = n;
    verbose(v,"VOID->Result");
}
#line 6169 "y.tab.c"
    break;

  case 123: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 2614 "parser.yacc"
                                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyval.node)->varName = (yyvsp[-2].lexeme);
    verbose(v,"IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS->MethodDeclarator");
}
#line 6184 "y.tab.c"
    break;

  case 124: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS  */
#line 2624 "parser.yacc"
                                                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-4].lexeme));
    temp.push_back(n1);
   
    // temp.push_back($3);
   
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyval.node)->varName = (yyvsp[-4].lexeme);
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS->MethodDeclarator");

}
#line 6202 "y.tab.c"
    break;

  case 125: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS  */
#line 2637 "parser.yacc"
                                                                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n1);

    for(auto it:(yyvsp[-1].node)->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyval.node)->varName = (yyvsp[-3].lexeme);
    verbose(v,"IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS->MethodDeclarator");
}
#line 6221 "y.tab.c"
    break;

  case 126: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS  */
#line 2651 "parser.yacc"
                                                                                                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-5].lexeme));
    temp.push_back(n1);
    
    // temp.push_back($3);
    for(auto it:(yyvsp[-1].node)->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyval.node)->varName = (yyvsp[-5].lexeme);
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS->MethodDeclarator");
}
#line 6241 "y.tab.c"
    break;

  case 127: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS Dims  */
#line 2666 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n1);
   
    // if($4) temp.push_back($4);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS Dims->MethodDeclarator");

    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
}
#line 6263 "y.tab.c"
    break;

  case 128: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS Dims  */
#line 2683 "parser.yacc"
                                                                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-5].lexeme));
    temp.push_back(n1);
    
    // temp.push_back($3);
    
    // if($6) temp.push_back($6);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyval.node)->varName = (yyvsp[-5].lexeme);
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS Dims->MethodDeclarator");

    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
}
#line 6288 "y.tab.c"
    break;

  case 129: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS Dims  */
#line 2703 "parser.yacc"
                                                                            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    for(auto it:(yyvsp[-2].node)->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    // if($5)
        // temp.push_back($5);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyval.node)->varName = (yyvsp[-4].lexeme);
    verbose(v,"IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS Dims->MethodDeclarator");

    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
}
#line 6314 "y.tab.c"
    break;

  case 130: /* MethodDeclarator: IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS Dims  */
#line 2724 "parser.yacc"
                                                                                                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-6].lexeme));
    temp.push_back(n1);
    
    // temp.push_back($3);

    for(auto it:(yyvsp[-2].node)->children)
        for(auto ch : it->children)
            temp.push_back(ch); 
    // if($5)
    //     temp.push_back($5);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyval.node)->varName = (yyvsp[-6].lexeme);
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS Dims->MethodDeclarator");


    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
}
#line 6344 "y.tab.c"
    break;

  case 131: /* ReceiverParameter: Type IDENTIFIER DOT THIS  */
#line 2752 "parser.yacc"
                                {
    vector<struct Node*> temp;
    // temp.push_back($1);
    // struct Node* n1 = new struct Node("Identifier", $2);
    // temp.push_back(n1);
    
    // struct Node* n3 = new struct Node("Keyword", $4);
    // temp.push_back(n3);
    // struct Node* n = new struct Node("ReceiverParameter", temp);
    (yyval.node) = (yyvsp[-3].node);
    verbose(v,"Type IDENTIFIER DOT THIS->ReceiverParameter");
}
#line 6361 "y.tab.c"
    break;

  case 132: /* ReceiverParameter: Type THIS  */
#line 2764 "parser.yacc"
                {
    // vector<struct Node*> temp;
    // temp.push_back($1);
    // struct Node* n1 = new struct Node("Keyword", $2);
    // temp.push_back(n1);
    // struct Node* n = new struct Node("VariableArityParameter", temp);
    (yyval.node) = (yyvsp[-1].node);
    verbose(v,"Type THIS->ReceiverParameter");
}
#line 6375 "y.tab.c"
    break;

  case 133: /* FormalParameterList: FormalParameter  */
#line 2776 "parser.yacc"
                    {
        vector<struct Node*> temp;
        temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("FormalParameterList", temp);
        (yyval.node) = n;
        verbose(v,"FormalParameter->FormalParameterList");
    }
#line 6387 "y.tab.c"
    break;

  case 134: /* FormalParameterList: FormalParameter COMMA FormalParameter CommaFormalParameters  */
#line 2783 "parser.yacc"
                                                                {
    // vector<struct Node*> temp;
    // temp.push_back($1);
  
    // temp.push_back($3);
    // if($4)
    //     temp.push_back($4);
    // struct Node* n = new struct Node("FormalParameterList", temp);
    // $$ = n;

    vector<struct Node*> temp;
    struct Node* n = new Node("FormalParameterList", temp);
    n->addChild((yyvsp[-3].node));
    n->addChild((yyvsp[-1].node));
    if((yyvsp[0].node)) {
        for(auto it: (yyvsp[0].node)->children) {
            n->addChild(it);
        }
            //$2->addChildToLeft($1);
            //temp.push_back($2);
    }
    (yyval.node) = n;
    verbose(v,"FormalParameter COMMA FormalParameter CommaFormalParameter->FormalParameterList");
}
#line 6416 "y.tab.c"
    break;

  case 135: /* CommaFormalParameters: %empty  */
#line 2810 "parser.yacc"
{(yyval.node) = NULL;}
#line 6422 "y.tab.c"
    break;

  case 136: /* CommaFormalParameters: COMMA FormalParameter CommaFormalParameters  */
#line 2811 "parser.yacc"
                                                {
    vector<struct Node*> temp;
    if((yyvsp[-1].node)->useful == false) {
        for(auto it: (yyvsp[-1].node)->children) {
            temp.push_back(it);
        }
    }
    else temp.push_back((yyvsp[-1].node));
    if((yyvsp[0].node)) {
        for(auto it: temp) {
            (yyvsp[0].node)->addChildToLeft(it);
        }
        (yyval.node) = (yyvsp[0].node);
    }
    else (yyval.node) = new Node("FormalParameters", temp);
    verbose(v,"COMMA FormalParameter CommaFormalParameters->CommaFormalParameters");
}
#line 6444 "y.tab.c"
    break;

  case 137: /* FormalParameter: Type VariableDeclaratorId  */
#line 2831 "parser.yacc"
                                {
        vector<struct Node*> temp;
        temp = {(yyvsp[-1].node), (yyvsp[0].node)};
        struct Node* n = new struct Node("FormalParameter", temp);
        (yyval.node) = n;
        verbose(v,"Type VariableDeclaratorId->FormalParameter");
    }
#line 6456 "y.tab.c"
    break;

  case 138: /* FormalParameter: VariableModifier VariableModifiers Type VariableDeclaratorId  */
#line 2838 "parser.yacc"
                                                                   {
    vector<struct Node*>temp;
        // if($2) {
        //     $2->addChildToLeft($1);
        //     temp.push_back($2);
        // }
        // else temp.push_back($1);
        // temp.push_back($3);
        // temp.push_back($4);
        /*****************************
        for(auto it : $1->children)
        {
            temp.push_back(it);
        }
        for(auto it : $2->children)
        {
            temp.push_back(it);
        }
        for(auto it : $3->children)
        {
            temp.push_back(it);
        }
        for(auto it : $4->children)
        {
            temp.push_back(it);
        }
        *********************************/
        // struct Node* n = new struct Node("FormalParameter", temp);
        temp = {(yyvsp[-1].node), (yyvsp[0].node)};
        struct Node* n = new struct Node("FormalParameter", temp);
        (yyval.node) = n;        
        verbose(v,"VariableModifier VariableModifiers Type VariableDeclaratorId->FormalParameter");
    }
#line 6494 "y.tab.c"
    break;

  case 139: /* FormalParameter: VariableArityParameter  */
#line 2871 "parser.yacc"
                            {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"VariableArityParameter->FormalParameter");
    }
#line 6503 "y.tab.c"
    break;

  case 140: /* VariableArityParameter: Type ELLIPSIS IDENTIFIER  */
#line 2878 "parser.yacc"
                                {
    // vector<struct Node*> temp;
    // temp.push_back($1);
    
    // struct Node* n2 = new struct Node("Identifier", $3);
    // temp.push_back(n2);
    // struct Node* n = new struct Node("VariableArityParameter", temp);
    (yyval.node) = (yyvsp[-2].node);
    verbose(v,"Type ELLIPSIS IDENTIFIER->VariableArityParameter");
}
#line 6518 "y.tab.c"
    break;

  case 141: /* VariableArityParameter: VariableModifier VariableModifiers Type ELLIPSIS IDENTIFIER  */
#line 2888 "parser.yacc"
                                                                {
    vector<struct Node*> temp;
    // if($2) {
    //         $2->addChildToLeft($1);
    //         temp.push_back($2);
    //     }
    //     else temp.push_back($1);
    // temp.push_back($3);
    
    // struct Node* n2 = new struct Node("Identifier", $5);
    // temp.push_back(n2);
    // struct Node* n = new struct Node("VariableArityParameter", temp);
    (yyval.node) = (yyvsp[-2].node);
    verbose(v,"VariableModifier VariableModifiers Type ELLIPSIS IDENTIFIER->VariableArityParameter");
}
#line 6538 "y.tab.c"
    break;

  case 142: /* VariableModifier: FINAL  */
#line 2906 "parser.yacc"
            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n1);
    struct Node* n = new struct Node("VariableModifier", temp);
    (yyval.node) = n;
    verbose(v,"FINAL->VariableModifier");
}
#line 6551 "y.tab.c"
    break;

  case 143: /* VariableModifiers: %empty  */
#line 2917 "parser.yacc"
{(yyval.node) = NULL;}
#line 6557 "y.tab.c"
    break;

  case 144: /* VariableModifiers: VariableModifier VariableModifiers  */
#line 2918 "parser.yacc"
                                        {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("VariableModifiers", temp);
        verbose(v,"VariableModifier VariableModifiers->VariableModifiers");
    }
#line 6579 "y.tab.c"
    break;

  case 145: /* MethodBody: Block  */
#line 2939 "parser.yacc"
            {

        if((yyvsp[0].node)->children.size()==0 && root->currNode->childscopes.back()->returntype!=VOID_TYPE)
        {
            cout<<"Error on line number "<<yylineno<<". This method must return a result of type "<<typeroot->inv_types[root->currNode->childscopes.back()->returntype]<<endl;
            yyerror("Error");           
        }     

        if((yyvsp[0].node)->children.size()>0 && (yyvsp[0].node)->children[0]->label!="BlockStatements" && root->currNode->childscopes.back()->returntype!=VOID_TYPE && (yyvsp[0].node)->children[(yyvsp[0].node)->children.size()-1]->label!="ReturnStatement")
        {
            cout<<"Error on line number "<<yylineno<<". This method must return a result of type "<<typeroot->inv_types[root->currNode->childscopes.back()->returntype]<<endl;
            yyerror("Error");
        }

        else if((yyvsp[0].node)->children.size()>0 && root->currNode->childscopes.back()->returntype!=VOID_TYPE && (yyvsp[0].node)->children[0]->label=="BlockStatements" && (yyvsp[0].node)->children[(yyvsp[0].node)->children.size()-1]->children.back()->label!="ReturnStatement")
        {
            cout<<"Error on line number "<<yylineno<<". This method must return a result of type "<<typeroot->inv_types[root->currNode->childscopes.back()->returntype]<<endl;
            yyerror("Error");
        } 
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Block->MethodBody");
    }
#line 6606 "y.tab.c"
    break;

  case 146: /* MethodBody: SEMICOLON  */
#line 2961 "parser.yacc"
                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("","");
    temp.push_back(n1);
    struct Node* n = new struct Node("MethodBody", temp);
    (yyval.node) = n;
    processPostIncre((yyval.node));
    verbose(v,"SEMICOLON->MethodBody");
}
#line 6620 "y.tab.c"
    break;

  case 147: /* InstanceInitializer: Block  */
#line 2974 "parser.yacc"
            {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"Block->InstanceInitializer");
    }
#line 6629 "y.tab.c"
    break;

  case 148: /* StaticInitializer: STATIC Block  */
#line 2981 "parser.yacc"
                  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-1].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("StaticInitializer", temp);
    (yyval.node) = n;
    verbose(v,"STATIC Block->StaticInitializer");
}
#line 6643 "y.tab.c"
    break;

  case 149: /* ConstructorDeclaration: ConstructorDeclarator ConstructorBody  */
#line 2995 "parser.yacc"
                                            {
        vector<struct Node*> temp;
        temp = {(yyvsp[-1].node), (yyvsp[0].node)};
        /*****************************
        for(auto it : $1->children)
        {
            temp.push_back(it);
        }
        for(auto it : $1->children)
        {
            temp.push_back(it);
        }
        ***************************/
        struct Node* n = new struct Node("ConstructorDeclaration", temp);
        (yyval.node) = n;
        Quadruple* q = new Quadruple(7, string("endfunc"));
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
        verbose(v,"ConstructorDeclarator ConstructorBody->ConstructorDeclaration");
    }
#line 6669 "y.tab.c"
    break;

  case 150: /* ConstructorDeclaration: Modifier Modifiers ConstructorDeclarator ConstructorBody  */
#line 3016 "parser.yacc"
                                                                 {
        vector<struct Node*> temp;
        if((yyvsp[-2].node)) {
            (yyvsp[-2].node)->addChildToLeft((yyvsp[-3].node));
            temp.push_back((yyvsp[-2].node));
        }
        else temp.push_back((yyvsp[-3].node));
        temp.push_back((yyvsp[-1].node));
        temp.push_back((yyvsp[0].node));
        /******************************
        for(auto it : $1->children)
        {
            temp.push_back(it);
        }
        for(auto it : $1->children)
        {
            temp.push_back(it);
        }
        for(auto it : $3->children)
        {
            temp.push_back(it);
        }
        for(auto it : $4->children)
        {
            temp.push_back(it);
        }
        ***************************/
        struct Node* n = new struct Node("ConstructorDeclaration", temp);
        (yyval.node) = n;
        Quadruple* q = new Quadruple(7, string("endfunc"));
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
        verbose(v,"Modifier Modifiers ConstructorDeclarator ConstructorBody ->ConstructorDeclaration");
    }
#line 6709 "y.tab.c"
    break;

  case 151: /* ConstructorDeclarator: Name LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 3060 "parser.yacc"
                                           {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
    verbose(v,"Name LEFTPARENTHESIS RIGHTPARENTHESIS->ConstructorDeclarator");


        vector<int> args;
        args.push_back(-1);
        root->currNode->default_done = true;

        bool check = false;
        
        if(args.size()>0)
            check = root->currNode->scope_constrlookup(args);

        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has been declared before"<<endl;
            yyerror("Error");
        }           

        SymNode* newf = new SymNode(root->currNode, "constructor", FUNC_SYM, args, typeroot->typewidth[(yyvsp[-2].node)->attr].first);
        root->currNode->childscopes.push_back(newf);
        root->currNode->constr_insert(args);
        root->currNode=newf;
        Quadruple* q = new Quadruple(6, (yyvsp[-2].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;

}
#line 6748 "y.tab.c"
    break;

  case 152: /* ConstructorDeclarator: Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS  */
#line 3094 "parser.yacc"
                                                                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-4].node));
    
    // temp.push_back($3);
   
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(6, (yyvsp[-4].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS->ConstructorDeclarator");
}
#line 6767 "y.tab.c"
    break;

  case 153: /* ConstructorDeclarator: Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS  */
#line 3108 "parser.yacc"
                                                                {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-3].node));
    temp.push_back((yyvsp[-1].node));
    
    
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
     Quadruple* q = new Quadruple(6, (yyvsp[-3].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");


        // vector<int> args;
        // for(auto it:$3->children)
        // {
        //     args.push_back(typeroot->typewidth[it->children[0]->attr].first);
        // }

        vector<int> args;
        for(auto it : (yyvsp[-1].node)->children)
        {
            if(it->children[1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[it->children[0]->attr].first+100);
            }
            else
                args.push_back(typeroot->typewidth[it->children[0]->attr].first);
        }

        bool check = root->currNode->scope_constrlookup(args);

        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has been declared before"<<endl;
            yyerror("Error");
        }           

        SymNode* newf = new SymNode(root->currNode, "constructor", FUNC_SYM, args);
        root->currNode->childscopes.push_back(newf);
        root->currNode->constr_insert(args);
        root->currNode=newf;

        // for(aut$3->children)
        for(int i=0; i<(yyvsp[-1].node)->children.size(); i++)
        {
            Symbol* sym = new Symbol((yyvsp[-1].node)->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            root->insert(sym->lexeme, sym);
        }
}
#line 6831 "y.tab.c"
    break;

  case 154: /* ConstructorDeclarator: Name LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS  */
#line 3167 "parser.yacc"
                                                                                        {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-5].node));
    
    temp.push_back((yyvsp[-1].node));
   
   
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
     Quadruple* q = new Quadruple(6, (yyvsp[-5].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");

        vector<int> args;
        for(auto it : (yyvsp[-1].node)->children)
        {
            if(it->children[1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[it->children[0]->attr].first+100);
            }
            else
                args.push_back(typeroot->typewidth[it->children[0]->attr].first);
        }

        bool check = root->currNode->scope_constrlookup(args);

        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has been declared before"<<endl;
            yyerror("Error");
        }           

        SymNode* newf = new SymNode(root->currNode, "constructor", FUNC_SYM, args);
        root->currNode->childscopes.push_back(newf);
        root->currNode->constr_insert(args);
        root->currNode=newf;

        // for(aut$3->children)
        for(int i=0; i<(yyvsp[-1].node)->children.size(); i++)
        {
            Symbol* sym = new Symbol((yyvsp[-1].node)->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            root->insert(sym->lexeme, sym);
        }
}
#line 6889 "y.tab.c"
    break;

  case 155: /* ConstructorDeclarator: TypeParameters Name LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 3220 "parser.yacc"
                                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-3].node));
    temp.push_back((yyvsp[-2].node));
   
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
     Quadruple* q = new Quadruple(6, (yyvsp[-2].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS RIGHTPARENTHESIS->ConstructorDeclarator");
}
#line 6907 "y.tab.c"
    break;

  case 156: /* ConstructorDeclarator: TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS  */
#line 3233 "parser.yacc"
                                                                                    {
    vector<struct Node*> temp;
    // temp.push_back($1);
    temp.push_back((yyvsp[-4].node));
    
    // temp.push_back($4);
    
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(6, (yyvsp[-4].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS->ConstructorDeclarator");
}
#line 6927 "y.tab.c"
    break;

  case 157: /* ConstructorDeclarator: TypeParameters Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS  */
#line 3248 "parser.yacc"
                                                                                {
    vector<struct Node*> temp;
    // temp.push_back($1);
    temp.push_back((yyvsp[-3].node));
    temp.push_back((yyvsp[-1].node));
   
   
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(6, (yyvsp[-3].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");

        vector<int> args;
        for(auto it : (yyvsp[-1].node)->children)
        {
            if(it->children[1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[it->children[0]->attr].first+100);
            }
            else
                args.push_back(typeroot->typewidth[it->children[0]->attr].first);
        }

        bool check = root->currNode->scope_constrlookup(args);

        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has been declared before"<<endl;
            yyerror("Error");
        }           

        SymNode* newf = new SymNode(root->currNode, "constructor", FUNC_SYM, args);
        root->currNode->childscopes.push_back(newf);
        root->currNode->constr_insert(args);
        root->currNode=newf;

        // for(aut$3->children)
        for(int i=0; i<(yyvsp[-1].node)->children.size(); i++)
        {
            Symbol* sym = new Symbol((yyvsp[-1].node)->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            root->insert(sym->lexeme, sym);
        }
}
#line 6985 "y.tab.c"
    break;

  case 158: /* ConstructorDeclarator: TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS  */
#line 3301 "parser.yacc"
                                                                                                         {
    vector<struct Node*> temp;
    // temp.push_back($1);
    temp.push_back((yyvsp[-5].node));
    
    temp.push_back((yyvsp[-1].node));
   
    for(auto it:(yyvsp[-1].node)->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(6, (yyvsp[-5].node)->varName);
        (yyval.node)->code.push_back(q);
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA  FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");

        vector<int> args;
        for(auto it : (yyvsp[-1].node)->children)
        {
            if(it->children[1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[it->children[0]->attr].first+100);
            }
            else
                args.push_back(typeroot->typewidth[it->children[0]->attr].first);
        }

        bool check = root->currNode->scope_constrlookup(args);

        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has been declared before"<<endl;
            yyerror("Error");
        }           

        SymNode* newf = new SymNode(root->currNode, "constructor", FUNC_SYM, args);
        root->currNode->childscopes.push_back(newf);
        root->currNode->constr_insert(args);
        root->currNode=newf;

        // for(aut$3->children)
        for(int i=0; i<(yyvsp[-1].node)->children.size(); i++)
        {
            Symbol* sym = new Symbol((yyvsp[-1].node)->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            root->insert(sym->lexeme, sym);
        }
}
#line 7047 "y.tab.c"
    break;

  case 159: /* ConstructorBody: LEFTCURLYBRACKET RIGHTCURLYBRACKET  */
#line 3361 "parser.yacc"
                                        {
    vector<struct Node*> temp;
    
    struct Node* n = new struct Node("ConstructorBody", temp);
    (yyval.node) = n;
    verbose(v,"LEFTCURLYBRACKET RIGHTCURLYBRACKET->ConstructorBody");

    // cout<<"My name is "<<root->currNode->name<<" and The parent name is "<<root->currNode->parent->name<<endl;
    if(root->currNode->name == "classextends")
    {
        // cout<<"FOund such child"<<endl;
        SymNode* r = root->currNode->parent;
        if(r->default_done==false && r->constr_args.size()>0)
        {
            cout<<"Error on line number "<<yylineno<<". Child class must have an explicit constructor invocation"<<endl;
            yyerror("Error");
        }
    }
}
#line 7071 "y.tab.c"
    break;

  case 160: /* ConstructorBody: LEFTCURLYBRACKET ExplicitConstructorInvocation RIGHTCURLYBRACKET  */
#line 3380 "parser.yacc"
                                                                        {
    vector<struct Node*> temp;
    
    temp.push_back((yyvsp[-1].node));

    // if(root->currNode->name=="classextends" && root->currNode->parent->constr_args.size()==0)
    // {
    //     cout<<"Error on line number "<<yylineno<<". Explicit constructor invocation not required for this class"<<endl;
    //     yyerror("Error");
    // }
   
    struct Node* n = new struct Node("ConstructorBody", temp);
    (yyval.node) = n;
    verbose(v,"LEFTCURLYBRACKET ExplicitConstructorInvocation RIGHTCURLYBRACKET->ConstructorBody");
}
#line 7091 "y.tab.c"
    break;

  case 161: /* ConstructorBody: LEFTCURLYBRACKET BlockStatement BlockStatements RIGHTCURLYBRACKET  */
#line 3395 "parser.yacc"
                                                                        {
    vector<struct Node*> temp;
   
    if((yyvsp[-1].node)) {
        (yyvsp[-1].node)->addChildToLeft((yyvsp[-2].node));
        temp.push_back((yyvsp[-1].node));
    }
    else temp.push_back((yyvsp[-2].node));
    struct Node* n = new struct Node("ConstructorBody", temp);
    (yyval.node) = n;
    verbose(v,"LEFTCURLYBRACKET BlockStatement BlockStatements RIGHTCURLYBRACKET->ConstructorBody");

    // cout<<"My name is "<<root->currNode->name<<" and The parent name is "<<root->currNode->parent->name<<endl;
    if(root->currNode->name == "classextends")
    {
        // cout<<"FOund such child"<<endl;
        SymNode* r = root->currNode->parent;
        if(r->default_done==false && r->constr_args.size()>0)
        {
            cout<<"Error on line number "<<yylineno<<". Child class must have an explicit constructor invocation"<<endl;
            yyerror("Error");
        }
    }
}
#line 7120 "y.tab.c"
    break;

  case 162: /* ConstructorBody: LEFTCURLYBRACKET ExplicitConstructorInvocation BlockStatement BlockStatements RIGHTCURLYBRACKET  */
#line 3419 "parser.yacc"
                                                                                                    {
    vector<struct Node*> temp;
    
    temp.push_back((yyvsp[-3].node));
   if((yyvsp[-1].node)) {
        (yyvsp[-1].node)->addChildToLeft((yyvsp[-2].node));
        temp.push_back((yyvsp[-1].node));
    }
    else temp.push_back((yyvsp[-2].node));

    struct Node* n = new struct Node("ConstructorBody", temp);
    (yyval.node) = n;
    verbose(v,"LEFTCURLYBRACKET ExplicitConstructorInvocation BlockStatement BlockStatements RIGHTCURLYBRACKET>ConstructorBody");
}
#line 7139 "y.tab.c"
    break;

  case 163: /* ExplicitConstructorInvocation: THIS LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON  */
#line 3436 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-3].lexeme));
    temp.push_back(n1);
    
    //struct Node* n4 = new struct Node("Separator", $4);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(4, "this", "0");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"THIS LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON ->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent;
    vector<int> args={-1};

    bool check = r->scope_constrlookup(args);
    if(!check)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }
    else if(args==r->constr_args.back())
    {
        cout<<"Error on line number "<<yylineno<<". Recursive constructor invocation is not allowed"<<endl;
        yyerror("Error");
    }
}
#line 7175 "y.tab.c"
    break;

  case 164: /* ExplicitConstructorInvocation: THIS LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON  */
#line 3467 "parser.yacc"
                                                                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    
    temp.push_back((yyvsp[-2].node));
   
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
   // generateArgumentList($$)
    Quadruple* q = new Quadruple(4, "this", "0");
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"THIS LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON ->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent;
    vector<int> args;

    for(auto it : (yyvsp[-2].node)->children)
        args.push_back(it->type);

    bool check = r->scope_constrlookup(args);
    if(!check)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }
    else if(args==r->constr_args.back())
    {
        cout<<"Error on line number "<<yylineno<<". Recursive constructor invocation is not allowed"<<endl;
        yyerror("Error");
    }
}
#line 7217 "y.tab.c"
    break;

  case 165: /* ExplicitConstructorInvocation: SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON  */
#line 3504 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-3].lexeme));
    temp.push_back(n1);
   
    //struct Node* n4 = new struct Node("Separator", $4);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(4, "super", "0");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent->parent;
    if(root->currNode->parent->name != "classextends")
    {
        cout<<"Error on line number "<<yylineno<<". This class has no parent class"<<endl;
        yyerror("Error");
    }
    vector<int> args={-1};

    if(r->constr_args.size()>0 && !r->default_done)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }
}
#line 7253 "y.tab.c"
    break;

  case 166: /* ExplicitConstructorInvocation: SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON  */
#line 3535 "parser.yacc"
                                                                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    
    temp.push_back((yyvsp[-2].node));
   
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(4, "super", "0");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent->parent;
    if(root->currNode->parent->name != "classextends")
    {
        cout<<"Error on line number "<<yylineno<<". This class has no parent class"<<endl;
        yyerror("Error");
    }
    vector<int> args;
    for(auto ch : (yyvsp[-2].node)->children)
        args.push_back(ch->type);
    bool check = r->scope_constrlookup(args);
    if(!check)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }
}
#line 7293 "y.tab.c"
    break;

  case 167: /* ExplicitConstructorInvocation: Name DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON  */
#line 3570 "parser.yacc"
                                                                {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-5].node));
  
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-3].lexeme));
    temp.push_back(n2);
    
   
    //struct Node* n5 = new struct Node("Separator", $6);
    //temp.push_back(n5); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(4, (yyvsp[-5].node)->varName + "." +"super", "0");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"Name DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
}
#line 7318 "y.tab.c"
    break;

  case 168: /* ExplicitConstructorInvocation: Name DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON  */
#line 3590 "parser.yacc"
                                                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-6].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n2);
  
    temp.push_back((yyvsp[-2].node));
   
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(4, (yyvsp[-6].node)->varName + "." +"super", "0");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"Name DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
}
#line 7342 "y.tab.c"
    break;

  case 169: /* ExplicitConstructorInvocation: Primary DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON  */
#line 3609 "parser.yacc"
                                                                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-5].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-3].lexeme));
    temp.push_back(n2);
    /***********************************************************
    struct Node* n3 = new struct Node("Separator", $4);
    temp.push_back(n3);
    struct Node* n4 = new struct Node("Separator", $5);
    temp.push_back(n4);
    //struct Node* n5 = new struct Node("Separator", $6);
    ************************************************************/
    //temp.push_back(n5); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(4, (yyvsp[-5].node)->varName + "." +"super", "0");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"Primary DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
}
#line 7371 "y.tab.c"
    break;

  case 170: /* ExplicitConstructorInvocation: Primary DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON  */
#line 3633 "parser.yacc"
                                                                                {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-6].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n2);
    /*********************************************************
    struct Node* n3 = new struct Node("Separator", $4);
    temp.push_back(n3);
    
    struct Node* n4 = new struct Node("Separator", $6);
    temp.push_back(n4);
    struct Node* n5 = new struct Node("Separator", $7);
    temp.push_back(n5); 
    *********************************************************/
    temp.push_back((yyvsp[-2].node));
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(4, (yyvsp[-6].node)->varName + "." +"super", "0");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"Primary DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
}
#line 7402 "y.tab.c"
    break;

  case 171: /* ExplicitConstructorInvocation: THIS LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON  */
#line 3659 "parser.yacc"
                                                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-2].node));
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-2].node)->varName);
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    q = new Quadruple(4, "this", "1");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"THIS LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent;
    vector<int> args;

    args.push_back((yyvsp[-2].node)->type);

    bool check = r->scope_constrlookup(args);
    if(!check)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }
    else if(args==r->constr_args.back())
    {
        cout<<"Error on line number "<<yylineno<<". Recursive constructor invocation is not allowed"<<endl;
        yyerror("Error");
    }
}
#line 7449 "y.tab.c"
    break;

  case 172: /* ExplicitConstructorInvocation: SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON  */
#line 3701 "parser.yacc"
                                                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-2].node));
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-2].node)->varName);
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    q = new Quadruple(4, "super", "1");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent->parent;
    if(root->currNode->parent->name != "classextends")
    {
        cout<<"Error on line number "<<yylineno<<". This class has no parent class"<<endl;
        yyerror("Error");
    }
    vector<int> args={(yyvsp[-2].node)->type};
    bool check = r->scope_constrlookup(args);

    if(!check)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }
}
#line 7494 "y.tab.c"
    break;

  case 173: /* ExplicitConstructorInvocation: Name DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON  */
#line 3741 "parser.yacc"
                                                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-6].node));
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n2);
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    temp.push_back((yyvsp[-2].node));
    //struct Node* n4 = new struct Node("Separator", $6);
    //temp.push_back(n4);
    //struct Node* n5 = new struct Node("Separator", $7);
    //temp.push_back(n5); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-2].node)->varName);
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    q = new Quadruple(4, (yyvsp[-6].node)->varName + "." + "super", "1");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"Name DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
}
#line 7527 "y.tab.c"
    break;

  case 174: /* ExplicitConstructorInvocation: Primary DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON  */
#line 3769 "parser.yacc"
                                                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-6].node));
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n2);
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    temp.push_back((yyvsp[-2].node));
    //struct Node* n4 = new struct Node("Separator", $6);
    //temp.push_back(n4);
    //struct Node* n5 = new struct Node("Separator", $7);
    //temp.push_back(n5); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-2].node)->varName);
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    q = new Quadruple(4, (yyvsp[-6].node)->varName + "." + "super", "1");
    (yyval.node)->code.push_back(q);

    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    processPostIncre((yyval.node));
    verbose(v,"Primary DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
}
#line 7560 "y.tab.c"
    break;

  case 175: /* ArgumentList: Expression COMMA Expression CommaExpressions  */
#line 3801 "parser.yacc"
                                                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-3].node));
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    temp.push_back((yyvsp[-1].node));
    if((yyvsp[0].node))
    {
        for(auto ch : (yyvsp[0].node)->children)
            temp.push_back(ch);
    }
    struct Node* n = new struct Node("ArgumentList", temp);
    (yyval.node) = n;
    vector<Node*> nodes = {(yyvsp[-3].node), (yyvsp[-1].node)};
    if((yyvsp[0].node)) nodes.insert(nodes.end(), (yyvsp[0].node)->children.begin(), (yyvsp[0].node)->children.end());
    generateArgumentList(nodes, (yyval.node));
    verbose(v,"Expression COMMA Expression CommaExpressions->ArgumentList");


}
#line 7585 "y.tab.c"
    break;

  case 176: /* Block: LEFTCURLYBRACKET RIGHTCURLYBRACKET  */
#line 3830 "parser.yacc"
                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("<empty>", temp);
    (yyval.node) = n1;
    verbose(v,"LEFTCURLYBRACKET RIGHTCURLYBRACKET->Block");
}
#line 7596 "y.tab.c"
    break;

  case 177: /* Block: LEFTCURLYBRACKET BlockStatement BlockStatements RIGHTCURLYBRACKET  */
#line 3837 "parser.yacc"
    {
         vector<struct Node*> temp;
        //struct Node* n1 = new struct Node("Identifier", $1);
        //temp.push_back(n1);
         
        if((yyvsp[-1].node)) {
            (yyvsp[-1].node)->addChildToLeft((yyvsp[-2].node));
            temp.push_back((yyvsp[-1].node));
        }
        else temp.push_back((yyvsp[-2].node));
        //struct Node* n2 = new struct Node("Separator", $4);
        //temp.push_back(n2);
        struct Node* n = new struct Node("Block", temp);
        (yyval.node) = n;
        backpatch((yyvsp[-2].node)->nextlist, (yyvsp[-2].node)->last + 1);
        if((yyvsp[-1].node)) {(yyval.node)->nextlist = (yyvsp[-1].node)->nextlist; }
        verbose(v,"LEFTCURLYBRACKET BlockStatement BlockStatements RIGHTCURLYBRACKET->Block");
    }
#line 7619 "y.tab.c"
    break;

  case 178: /* BlockStatement: LocalClassOrInterfaceDeclaration  */
#line 3858 "parser.yacc"
                                        {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"LocalClassOrInterfaceDeclaration->BlockStatement");
    }
#line 7628 "y.tab.c"
    break;

  case 179: /* BlockStatement: LocalVariableDeclarationStatement  */
#line 3862 "parser.yacc"
                                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"LocalVariableDeclarationStatement->BlockStatement");
    }
#line 7637 "y.tab.c"
    break;

  case 180: /* BlockStatement: Statement  */
#line 3866 "parser.yacc"
                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Statement->BlockStatement");
    }
#line 7646 "y.tab.c"
    break;

  case 181: /* BlockStatements: %empty  */
#line 3873 "parser.yacc"
    {(yyval.node) = NULL;}
#line 7652 "y.tab.c"
    break;

  case 182: /* BlockStatements: BlockStatement BlockStatements  */
#line 3874 "parser.yacc"
                                    {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("BlockStatements", temp);
        backpatch((yyvsp[-1].node)->nextlist, (yyvsp[-1].node)->last + 1);
        if((yyvsp[0].node)) (yyval.node)->nextlist = (yyvsp[0].node)->nextlist;
        verbose(v,"BlockStatement BlockStatements->BlockStatements");
    }
#line 7677 "y.tab.c"
    break;

  case 183: /* LocalClassOrInterfaceDeclaration: ClassDeclaration  */
#line 3897 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ClassDeclaration->LocalClassOrInterfaceDeclaration");

        (yyval.node)->type = VOID_TYPE;
    }
#line 7688 "y.tab.c"
    break;

  case 184: /* LocalVariableDeclarationStatement: LocalVariableDeclaration SEMICOLON  */
#line 3906 "parser.yacc"
                                        {
        vector<struct Node*> temp;
        temp.push_back((yyvsp[-1].node));
        //struct Node* n1 = new struct Node("Separator", $2);
        //temp.push_back(n1);
        struct Node* n = new struct Node("LocalVariableDeclarationStatement", temp);
        (yyval.node) = n;
        verbose(v,"LocalVariableDeclaration SEMICOLON->LocalVariableDeclarationStatement");
        processPostIncre((yyval.node));
        (yyval.node)->type = VOID_TYPE;
    }
#line 7704 "y.tab.c"
    break;

  case 185: /* LocalVariableDeclaration: LocalVariableType VariableDeclaratorList  */
#line 3920 "parser.yacc"
                                                 {
        vector<struct Node*> temp = {(yyvsp[-1].node), (yyvsp[0].node)};
        /*****************************
        for(auto it : $1->children)
        {
            temp.push_back(it);
        }
        for(auto it : $2->children)
        {
            temp.push_back(it);
        }
        *********************************/
        struct Node* n = new struct Node("LocalVariableDeclaration", temp);
        (yyval.node) = n;
        //($1);
        verbose(v,"LocalVariableType VariableDeclaratorList->LocalVariableDeclaration");
        int cc = 0;
        for(auto ch : (yyvsp[0].node)->children)
        {
            int _type = (yyvsp[-1].node)->type;
            if(ch->arrayType && _type < 100) _type += ch->arrayType*100  ; 
           // cout << ch->children.size() << " dfk " << ch->children[1]->arrayType << endl;
            if(ch->children.size() > 1 && ch->children[1]->arrayType) {ch->children[1]->type = (yyvsp[-1].node)->type%100 + 100 * ch->children[1]->arrayType; 
                if(ch->children[1]->label == "ArrayInitializer") {
                    if(ch->arrayType == 1 ) init1DArray(ch, (yyvsp[-1].node)->attr);
                    else if(ch->arrayType == 2 ) init2DArray(ch, (yyvsp[-1].node)->attr);
                    else if(ch->arrayType == 3 ) init3DArray(ch, (yyvsp[-1].node)->attr);
                }
            
            }
            
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[(yyvsp[-1].node)->attr].second);
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE) || (sym->type==DOUBLE_NUM && ch->children[1]->type==FLOAT_NUM)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                if(!ch->arrayType) processFieldDec((yyval.node), ch, _type);
                

            }
            //sym->type += ch->arrayType * 100;
            if(ch->arrayType > 0) {
                sym->isArray = true;
                
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                
                //sym->calcWidths();
           }
            
            root->insert(sym->lexeme, sym);
            //if(cc) backpatch((ch-1)->nextlist,(ch-1)->last + 1);
            cc++;
        }
        (yyval.node)->last = ircode.size() - 1;
    }
#line 7771 "y.tab.c"
    break;

  case 186: /* LocalVariableDeclaration: VariableModifier VariableModifiers LocalVariableType VariableDeclaratorList  */
#line 3982 "parser.yacc"
                                                                                {
        vector<struct Node*> temp;
         if((yyvsp[-2].node)) {
            (yyvsp[-2].node)->addChildToLeft((yyvsp[-3].node));
            temp.push_back((yyvsp[-2].node));
        }
        else temp.push_back((yyvsp[-3].node));
        temp.push_back((yyvsp[-1].node));
        temp.push_back((yyvsp[0].node));
        /******************************
        for(auto it : $1->children)
        {
            temp.push_back(it);
        }
        for(auto it : $2->children)
        {
            temp.push_back(it);
        }
        for(auto it : $3->children)
        {
            temp.push_back(it);
        }
        for(auto it : $4->children)
        {
            temp.push_back(it);
        }
        *****************************/
        struct Node* n = new struct Node("LocalVariableDeclaration", temp);
        (yyval.node) = n;
        verbose(v,"VariableModifier VariableModifiers LocalVariableType VariableDeclaratorList->LocalVariableDeclaration");
        for(auto ch : (yyvsp[0].node)->children)
        {
            int _type = (yyvsp[-1].node)->type;
            if(ch->arrayType && _type < 100) _type += ch->arrayType*100  ; 
            if(ch->children[1]->arrayType) {ch->children[1]->type = (yyvsp[-1].node)->type%100 + 100 * ch->children[1]->arrayType;
                if(ch->children[1]->label == "ArrayInitializer"){    
                    if(ch->arrayType == 1) init1DArray(ch, (yyvsp[-1].node)->attr);
                    else if(ch->arrayType == 2) init2DArray(ch, (yyvsp[-1].node)->attr);
                    else if(ch->arrayType == 3) init3DArray(ch, (yyvsp[-1].node)->attr);
                }
            
            }
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[(yyvsp[-1].node)->attr].second);
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE) || (sym->type==DOUBLE_NUM && ch->children[1]->type==FLOAT_NUM)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                if(!ch->arrayType ) processFieldDec((yyval.node), ch, _type);

            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           }
            
            root->insert(sym->lexeme, sym);
        }
        (yyval.node)->last = ircode.size() - 1;
        //($1);
    }
#line 7844 "y.tab.c"
    break;

  case 187: /* LocalVariableType: PrimitiveType  */
#line 4053 "parser.yacc"
                    {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"PrimitiveType->LocalVariableType");
    }
#line 7853 "y.tab.c"
    break;

  case 188: /* LocalVariableType: ReferenceType  */
#line 4057 "parser.yacc"
                    {
      (yyval.node) = (yyvsp[0].node);
      verbose(v,"ReferenceType->LocalVariableType");
    }
#line 7862 "y.tab.c"
    break;

  case 189: /* LocalVariableType: VAR  */
#line 4061 "parser.yacc"
        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n1);
    struct Node* n = new struct Node("LocalVariableType", temp);
    (yyval.node) = n;
    verbose(v,"VAR->LocalVariableType");
}
#line 7875 "y.tab.c"
    break;

  case 190: /* Statement: StatementWithoutTrailingSubstatement  */
#line 4072 "parser.yacc"
                                            {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"StatementWithoutTrailingSubstatement->Statement");
    }
#line 7884 "y.tab.c"
    break;

  case 191: /* Statement: LabeledStatement  */
#line 4076 "parser.yacc"
                        {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"LabeledStatement->Statement");
    }
#line 7893 "y.tab.c"
    break;

  case 192: /* Statement: IfThenStatement  */
#line 4080 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"IfThenStatement->Statement");
    }
#line 7902 "y.tab.c"
    break;

  case 193: /* Statement: IfThenElseStatement  */
#line 4084 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"IfThenElseStatement->Statement");
    }
#line 7911 "y.tab.c"
    break;

  case 194: /* Statement: WhileStatement  */
#line 4088 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"WhileStatement->Statement");
    }
#line 7920 "y.tab.c"
    break;

  case 195: /* Statement: ForStatement  */
#line 4092 "parser.yacc"
                    {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"ForStatement->Statement");
    }
#line 7929 "y.tab.c"
    break;

  case 196: /* StatementNoShortIf: StatementWithoutTrailingSubstatement  */
#line 4099 "parser.yacc"
                                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
#line 7938 "y.tab.c"
    break;

  case 197: /* StatementNoShortIf: LabeledStatementNoShortIf  */
#line 4103 "parser.yacc"
                                {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
#line 7947 "y.tab.c"
    break;

  case 198: /* StatementNoShortIf: IfThenElseStatementNoShortIf  */
#line 4107 "parser.yacc"
                                    {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
#line 7956 "y.tab.c"
    break;

  case 199: /* StatementNoShortIf: WhileStatementNoShortIf  */
#line 4111 "parser.yacc"
                            {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
#line 7965 "y.tab.c"
    break;

  case 200: /* StatementNoShortIf: ForStatementNoShortIf  */
#line 4115 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
#line 7974 "y.tab.c"
    break;

  case 201: /* StatementWithoutTrailingSubstatement: Block  */
#line 4122 "parser.yacc"
            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Block->StatementWithoutTrailingSubstatement");
    }
#line 7983 "y.tab.c"
    break;

  case 202: /* StatementWithoutTrailingSubstatement: EmptyStatement  */
#line 4126 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"EmptyStatement->StatementWithoutTrailingSubstatement");
    }
#line 7992 "y.tab.c"
    break;

  case 203: /* StatementWithoutTrailingSubstatement: ExpressionStatement  */
#line 4130 "parser.yacc"
                        {
       (yyval.node) = (yyvsp[0].node);
        verbose(v,"ExpressionStatement->StatementWithoutTrailingSubstatement");
    }
#line 8001 "y.tab.c"
    break;

  case 204: /* StatementWithoutTrailingSubstatement: AssertStatement  */
#line 4134 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"AssertStatement->StatementWithoutTrailingSubstatement");
    }
#line 8010 "y.tab.c"
    break;

  case 205: /* StatementWithoutTrailingSubstatement: DoStatement  */
#line 4139 "parser.yacc"
                   {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"DoStatement->StatementWithoutTrailingSubstatement");
    }
#line 8019 "y.tab.c"
    break;

  case 206: /* StatementWithoutTrailingSubstatement: BreakStatement  */
#line 4143 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"BreakStatement->StatementWithoutTrailingSubstatement");
    }
#line 8028 "y.tab.c"
    break;

  case 207: /* StatementWithoutTrailingSubstatement: ContinueStatement  */
#line 4147 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ContinueStatement->StatementWithoutTrailingSubstatement");
    }
#line 8037 "y.tab.c"
    break;

  case 208: /* StatementWithoutTrailingSubstatement: ReturnStatement  */
#line 4151 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ReturnStatement->StatementWithoutTrailingSubstatement");
    }
#line 8046 "y.tab.c"
    break;

  case 209: /* DoStatement: DO Statement WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON  */
#line 4164 "parser.yacc"
                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-6].lexeme));
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-5].node));
    temp.push_back(n2);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-2].node));
    struct Node* n = new struct Node("LabeledStatement", temp);
    (yyval.node) = n;
    verbose(v,"DO Statement WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON ->DoStatement");

    if((yyvsp[-2].node)->type==BOOL_NUM)
        (yyval.node)->type = VOID_TYPE;
    else
        yyerror("Expression should be of boolean type");
    root->end_all_vulnerable();


    string lab = "L"+to_string(labelCnt++);
    (yyval.node)->ir_label = lab;
    Quadruple* quad = new Quadruple((yyval.node)->ir_label);
    (yyval.node)->code.push_back(quad);
    ircode.push_back(quad);
    for(auto quad : (yyvsp[-5].node)->code)
    {
        (yyval.node)->code.push_back(quad);
    }   
    (yyvsp[-2].node)->True = "L"+to_string(labelCnt++);
    (yyvsp[-2].node)->False = (yyvsp[-5].node)->next = (yyval.node)->next;
    string cod = (yyvsp[-2].node)->varName+" = 0";
    Quadruple* jmp = new Quadruple(2, cod, (yyval.node)->False);
    (yyval.node)->code.push_back(jmp);
    ircode.push_back(jmp);
    jmp = new Quadruple(3, cod, (yyval.node)->ir_label);
    (yyval.node)->last = ircode.size() - 1;
    (yyval.node)->code.push_back(jmp);
    processPostIncre((yyval.node));
}
#line 8092 "y.tab.c"
    break;

  case 210: /* EmptyStatement: SEMICOLON  */
#line 4208 "parser.yacc"
                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("<empty>");
    temp.push_back(n1);
    struct Node* n = new struct Node("EmptyStatement", temp);
    (yyval.node) = n;
    verbose(v,"SEMICOLON->EmptyStatement");

    (yyval.node)->type = VOID_TYPE;
    processPostIncre((yyval.node));
}
#line 8108 "y.tab.c"
    break;

  case 211: /* LabeledStatement: IDENTIFIER COL Statement  */
#line 4222 "parser.yacc"
                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("LabeledStatement", temp);
    (yyval.node) = n;
    verbose(v,"IDENTIFIER COL Statement->LabeledStatement");

    (yyval.node)->type = VOID_TYPE;
}
#line 8126 "y.tab.c"
    break;

  case 212: /* LabeledStatementNoShortIf: IDENTIFIER COL StatementNoShortIf  */
#line 4238 "parser.yacc"
                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("LabeledStatementNoShortIf", temp);
    (yyval.node) = n;
    verbose(v,"IDENTIFIER COL StatementNoShortIf->LabeledStatementNoShortIf");

    (yyval.node)->type = VOID_TYPE;
}
#line 8144 "y.tab.c"
    break;

  case 213: /* ExpressionStatement: StatementExpression SEMICOLON  */
#line 4254 "parser.yacc"
                                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-1].node));
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n = new struct Node("ExpressionStatement", temp);
    (yyval.node) = n;
    (yyval.node)->last = (yyvsp[-1].node)->last;
    verbose(v,"StatementExpression SEMICOLON->ExpressionStatement");

    (yyval.node)->type = VOID_TYPE;
    (yyval.node)->nextlist = (yyvsp[-1].node)->nextlist;
    processPostIncre((yyval.node));
}
#line 8163 "y.tab.c"
    break;

  case 214: /* StatementExpression: Assignment  */
#line 4271 "parser.yacc"
                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Assignment->StatementExpression");
    }
#line 8172 "y.tab.c"
    break;

  case 215: /* StatementExpression: PreIncrementExpression  */
#line 4275 "parser.yacc"
                            {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"PreIncrementExpression->StatementExpression");
    }
#line 8181 "y.tab.c"
    break;

  case 216: /* StatementExpression: PreDecrementExpression  */
#line 4279 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"PreDecrementExpression->StatementExpression");
    }
#line 8190 "y.tab.c"
    break;

  case 217: /* StatementExpression: PostIncrementExpression  */
#line 4283 "parser.yacc"
                            {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"PostIncrementExpression->StatementExpression");
    }
#line 8199 "y.tab.c"
    break;

  case 218: /* StatementExpression: PostDecrementExpression  */
#line 4287 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"PostDecrementExpression->StatementExpression");
    }
#line 8208 "y.tab.c"
    break;

  case 219: /* StatementExpression: MethodInvocation  */
#line 4291 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"MethodInvocation->StatementExpression");
    }
#line 8217 "y.tab.c"
    break;

  case 220: /* StatementExpression: ClassInstanceCreationExpression  */
#line 4295 "parser.yacc"
                                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ClassInstanceCreationExpression->StatementExpression");
    }
#line 8226 "y.tab.c"
    break;

  case 221: /* CommaStatementExpressions: %empty  */
#line 4302 "parser.yacc"
    {(yyval.node) = NULL;}
#line 8232 "y.tab.c"
    break;

  case 222: /* CommaStatementExpressions: COMMA StatementExpression CommaStatementExpressions  */
#line 4303 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("StatementExpressions", temp);
        verbose(v,"COMMA StatementExpression CommaStatementExpressions->CommaStatementExpressions");
}
#line 8254 "y.tab.c"
    break;

  case 223: /* IfThenStatement: IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement  */
#line 4324 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-4].lexeme));
        //struct Node* t2 = new Node("Separator", $2);
        //struct Node* t4 = new Node("Separator", $4);
        vector<Node* > temp = {t1, (yyvsp[-2].node),(yyvsp[0].node)};

        (yyval.node) = new Node("IfThenStatement", temp);
        backpatch((yyvsp[-2].node)->truelist, (yyvsp[-2].node)->last + 1);
        (yyval.node)->nextlist = (yyvsp[-2].node)->falselist;
        (yyval.node)->nextlist.insert((yyval.node)->nextlist.end(), (yyvsp[0].node)->nextlist.begin(), (yyvsp[0].node)->nextlist.end());
        verbose(v,"IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement->IfThenStatement");

        if((yyvsp[-2].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
        else if((yyvsp[-2].node)->type != BOOL_NUM)
        {
            cout<<"Error! Expression inside if is not of boolean type on line number "<<yylineno<<endl;
            yyerror("Error");
        }
        else
        {
            cout<<"Error! Statement after if is not of void type on line number "<<yylineno<<endl;
            yyerror("Error");
        }
            // yyerror("Error in statements");
    }
#line 8285 "y.tab.c"
    break;

  case 224: /* _StatementNoShortIf: StatementNoShortIf ELSE  */
#line 4354 "parser.yacc"
{
    struct Node* t = new Node("Keyword", (yyvsp[0].lexeme));
    vector<Node*> temp = {(yyvsp[-1].node), t};
    (yyval.node) = new Node("_StatementNoShortIf", temp);
    
    (yyval.node)->nextlist = (yyvsp[-1].node)->nextlist;
    (yyval.node)->nextlist.push_back(ircode.size());
    cout << ircode.size()<<"over here\n";
    Quadruple* q = new Quadruple(3, "", "","", "");
    ircode.push_back(q);
    (yyval.node)->code.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
}
#line 8303 "y.tab.c"
    break;

  case 225: /* IfThenElseStatement: IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS _StatementNoShortIf Statement  */
#line 4370 "parser.yacc"
    {
        
        struct Node* t1 = new Node("Keyword", (yyvsp[-5].lexeme));
        vector<Node* > temp = {t1, (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node)};
        (yyval.node) = new Node("IfThenElseStatement", temp);
        int lastpos = (yyvsp[-3].node)->last + 1;
        backpatch((yyvsp[-3].node)->truelist, lastpos);
        backpatch((yyvsp[-3].node)->falselist, (yyvsp[-1].node)->last + 1);
        cout << "Lastpos = " << max(lastpos, ((yyvsp[-1].node)->last) + 1)<< "\n";
        (yyval.node)->nextlist = (yyvsp[-1].node)->nextlist;
        (yyval.node)->nextlist.insert((yyval.node)->nextlist.end(), (yyvsp[0].node)->nextlist.begin(), (yyvsp[0].node)->nextlist.end());
        (yyval.node)->last= ircode.size() - 1;
        verbose(v,"IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf ELSE Statement->IfThenElseStatement");

        if((yyvsp[-3].node)->type==BOOL_NUM && (yyvsp[-1].node)->type==VOID_TYPE && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
        else if((yyvsp[-3].node)->type != BOOL_NUM)
        {
            cout<<"Error! Expression inside if is not of boolean type on line number "<<yylineno<<endl;
            yyerror("Error");
        }
        else
        {
            cout<<"Error! Statement in if-else is not of void type on line number "<<yylineno<<endl;
            yyerror("Error");
        }
    }
#line 8335 "y.tab.c"
    break;

  case 226: /* IfThenElseStatementNoShortIf: IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS _StatementNoShortIf StatementNoShortIf  */
#line 4403 "parser.yacc"
    {struct Node* t1 = new Node("Keyword", (yyvsp[-5].lexeme));
        vector<Node* > temp = {t1, (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node)};
        (yyval.node) = new Node("IfThenElseStatement", temp);
        int lastpos = (yyvsp[-3].node)->last + 1;
        cout << "Lastpos = " << lastpos << "\n";
        backpatch((yyvsp[-3].node)->truelist, lastpos);
        backpatch((yyvsp[-3].node)->falselist, (yyvsp[-1].node)->last + 1);
        cout << "Lastpos = " << max(lastpos, ((yyvsp[-1].node)->last) + 1)<< "\n";
        (yyval.node)->nextlist = (yyvsp[-1].node)->nextlist;
        (yyval.node)->nextlist.insert((yyval.node)->nextlist.end(), (yyvsp[0].node)->nextlist.begin(), (yyvsp[0].node)->nextlist.end());
        (yyval.node)->last= ircode.size() - 1;
        verbose(v,"IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf ELSE StatementNoShortIf->IfThenElseStatementNoShortIf");

        if((yyvsp[-3].node)->type==BOOL_NUM && (yyvsp[-1].node)->type==VOID_TYPE && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
        else if((yyvsp[-3].node)->type != BOOL_NUM)
        {
            cout<<"Error! Expression inside if is not of boolean type on line number "<<yylineno<<endl;
            yyerror("Error");
        }
        else
        {
            cout<<"Error! Statement in if-else is not of void type on line number "<<yylineno<<endl;
            yyerror("Error");
        }
    }
#line 8366 "y.tab.c"
    break;

  case 227: /* AssertStatement: ASSERT Expression SEMICOLON  */
#line 4432 "parser.yacc"
                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-1].node));
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    struct Node* n = new struct Node("AssertStatement", temp);
    (yyval.node) = n;
    verbose(v,"ASSERT Expression SEMICOLON ->AssertStatement");

    if((yyvsp[-1].node)->type==BOOL_NUM)
        (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Expression inside Assert Statement is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
}
#line 8390 "y.tab.c"
    break;

  case 228: /* AssertStatement: ASSERT Expression COL Expression SEMICOLON  */
#line 4451 "parser.yacc"
                                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-3].node));
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-1].node));
    //struct Node* n3 = new struct Node("Separator", $5);
    //temp.push_back(n3);
    struct Node* n = new struct Node("AssertStatement", temp);
    (yyval.node) = n;
    verbose(v,"ASSERT Expression COL Expression SEMICOLON->AssertStatement");

    if((yyvsp[-3].node)->type==BOOL_NUM && (yyvsp[-1].node)->type==BOOL_NUM)
        (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Expression inside Assert Statement is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
}
#line 8417 "y.tab.c"
    break;

  case 229: /* WhileStatement: WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement  */
#line 4476 "parser.yacc"
                                                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-2].node));
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("WhileStatement", temp);
    (yyval.node) = n;
    verbose(v,"WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement->WhileStatement");

    if((yyvsp[-2].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
        (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-2].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    processWhile((yyval.node), (yyvsp[-2].node), (yyvsp[0].node));
}
#line 8446 "y.tab.c"
    break;

  case 230: /* WhileStatementNoShortIf: WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf  */
#line 4503 "parser.yacc"
                                                                            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-2].node));
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("WhileStatementNoShortIf", temp);
    (yyval.node) = n;
    verbose(v,"WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf->WhileStatementNoShortIf");

    if((yyvsp[-2].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
        (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-2].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    
    processWhile((yyval.node), (yyvsp[-2].node), (yyvsp[0].node));
}
#line 8480 "y.tab.c"
    break;

  case 231: /* ForStatement: BasicForStatement  */
#line 4536 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"BasicForStatement->ForStatement");
    }
#line 8489 "y.tab.c"
    break;

  case 232: /* ForStatement: EnhancedForStatement  */
#line 4540 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"EnhancedForStatement->ForStatement");
    }
#line 8498 "y.tab.c"
    break;

  case 233: /* ForStatementNoShortIf: BasicForStatementNoShortIf  */
#line 4547 "parser.yacc"
                                {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"BasicForStatementNoShortIf->ForStatementNoShortIf");
    }
#line 8507 "y.tab.c"
    break;

  case 234: /* ForStatementNoShortIf: EnhancedForStatementNoShortIf  */
#line 4551 "parser.yacc"
                                    {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"EnhancedForStatementNoShortIf->BasicForStatement");
    }
#line 8516 "y.tab.c"
    break;

  case 235: /* BasicForStatement: FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement  */
#line 4559 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-8].lexeme));
        /*************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        struct Node* t8 = new Node("Separator", $8);
        *************************************************/
        vector<Node* > temp = {t1, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);

        int i;
        // pop the code for statement, forUpdate
        
        //ircode.insert(ircode.end(), $9->code.begin(), $9->code.end());
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
        for(auto it: (yyvsp[0].node)->nextlist) cout << it << " " ;
        cout << "\n";
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
        (yyvsp[0].node)->last = ircode.size() - 1;
        //$9->last = $7->last;
        processWhile((yyval.node), (yyvsp[-4].node), (yyvsp[0].node));
          verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if((yyvsp[-4].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-4].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }

    }
#line 8561 "y.tab.c"
    break;

  case 236: /* BasicForStatement: FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement  */
#line 4600 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-5].lexeme));
        /********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        *********************************************/
        vector<Node* > temp = {t1, (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyvsp[0].node)->last - (yyvsp[0].node)->code.size();
        processWhile((yyval.node), n, (yyvsp[0].node));
        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8588 "y.tab.c"
    break;

  case 237: /* BasicForStatement: FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement  */
#line 4623 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        /**********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *******************************************/
        vector<Node* > temp = {t1, (yyvsp[-4].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyvsp[0].node)->last - (yyvsp[0].node)->code.size();
        processWhile((yyval.node), n, (yyvsp[0].node));
        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8615 "y.tab.c"
    break;

  case 238: /* BasicForStatement: FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement  */
#line 4646 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1, (yyvsp[-3].node),  (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
       
        
        //$9->nextlist = $6->nextlist;
        //$8->last = $6->last;
        processWhile((yyval.node), (yyvsp[-3].node), (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");

        if((yyvsp[-3].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-3].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8652 "y.tab.c"
    break;

  case 239: /* BasicForStatement: FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement  */
#line 4679 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1,  (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
         int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
       
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
        (yyvsp[0].node)->last = ircode.size() - 1;
        //$7->last = $5->last;
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyval.node)->last - (yyvsp[0].node)->code.size() - (yyvsp[-2].node)->code.size() ;
        processWhile((yyval.node), n, (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8690 "y.tab.c"
    break;

  case 240: /* BasicForStatement: FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement  */
#line 4713 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-7].lexeme));
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        struct Node* t7 = new Node("Separator", $7);
        **************************************************/
        vector<Node* > temp = {t1, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
       
        
        //$9->nextlist = $7->nextlist;
        //$9->last = $7->last;
        processWhile((yyval.node), (yyvsp[-3].node), (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");

        if((yyvsp[-3].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-3].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8727 "y.tab.c"
    break;

  case 241: /* BasicForStatement: FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement  */
#line 4746 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-7].lexeme));
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        ***********************************************/
        vector<Node* > temp = {t1, (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
        
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyvsp[-5].node)->last;
        (yyvsp[0].node)->last = ircode.size()- 1;
        processWhile((yyval.node), n , (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8762 "y.tab.c"
    break;

  case 242: /* BasicForStatement: FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement  */
#line 4777 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-7].lexeme));
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        **********************************************/
        vector<Node* > temp = {t1, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
        (yyvsp[0].node)->last = ircode.size() - 1;
        //$9->last = $7->last;
        processWhile((yyval.node), (yyvsp[-4].node), (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if((yyvsp[-4].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-4].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8802 "y.tab.c"
    break;

  case 243: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf  */
#line 4816 "parser.yacc"
    {
       struct Node* t1 = new Node("Keyword", (yyvsp[-8].lexeme));
        vector<Node* > temp = {t1, (yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
      
       
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
        for(auto it: (yyvsp[0].node)->nextlist) cout << it << " " ;
        cout << "\n";
        
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
       
        (yyvsp[0].node)->last = ircode.size() - 1;
        processWhile((yyval.node), (yyvsp[-4].node), (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[-4].node)->type==BOOL_NUM)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8834 "y.tab.c"
    break;

  case 244: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf  */
#line 4844 "parser.yacc"
    {
       struct Node* t1 = new Node("Keyword", (yyvsp[-5].lexeme));
        /********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        *********************************************/
        vector<Node* > temp = {t1, (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyvsp[0].node)->last - (yyvsp[0].node)->code.size();
        processWhile((yyval.node), n, (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8862 "y.tab.c"
    break;

  case 245: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf  */
#line 4868 "parser.yacc"
    {
         struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        /**********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *******************************************/
        vector<Node* > temp = {t1, (yyvsp[-4].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyvsp[0].node)->last - (yyvsp[0].node)->code.size();
        processWhile((yyval.node), n, (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8890 "y.tab.c"
    break;

  case 246: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf  */
#line 4892 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1, (yyvsp[-3].node),  (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        processWhile((yyval.node), (yyvsp[-3].node), (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[-3].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-3].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8921 "y.tab.c"
    break;

  case 247: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf  */
#line 4919 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1,  (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        //ircode.insert(ircode.end(), $7->code.begin(), $7->code.end());
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
        //$7->last = $5->last;
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyval.node)->last - (yyvsp[0].node)->code.size() - (yyvsp[-2].node)->code.size() ;
        processWhile((yyval.node), n, (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8958 "y.tab.c"
    break;

  case 248: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf  */
#line 4952 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-7].lexeme));
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        struct Node* t7 = new Node("Separator", $7);
        **************************************************/
        vector<Node* > temp = {t1, (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        processWhile((yyval.node), (yyvsp[-3].node), (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[-3].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-3].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 8989 "y.tab.c"
    break;

  case 249: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf  */
#line 4979 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-7].lexeme));
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        ***********************************************/
        vector<Node* > temp = {t1, (yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
        struct Node* n = new Node("Boolean", "true");
        n->last = (yyvsp[-5].node)->last;
        //$9->last = $7->last;
        processWhile((yyval.node), n , (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 9025 "y.tab.c"
    break;

  case 250: /* BasicForStatementNoShortIf: FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf  */
#line 5011 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-7].lexeme));
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        **********************************************/
        vector<Node* > temp = {t1, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), (yyvsp[-2].node)->code.begin(), (yyvsp[-2].node)->code.end());
        backpatch((yyvsp[0].node)->nextlist, m);
        (yyvsp[0].node)->nextlist = (yyvsp[-2].node)->nextlist;
        //$9->last = $7->last;
        processWhile((yyval.node), (yyvsp[-4].node), (yyvsp[0].node));
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if((yyvsp[-4].node)->type==BOOL_NUM && (yyvsp[0].node)->type==VOID_TYPE)
            (yyval.node)->type = VOID_TYPE;
    else if((yyvsp[-4].node)->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
#line 9064 "y.tab.c"
    break;

  case 251: /* ForInit: StatementExpressionList  */
#line 5048 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"StatementExpressionList->ForInit");
    }
#line 9073 "y.tab.c"
    break;

  case 252: /* ForInit: LocalVariableDeclaration  */
#line 5052 "parser.yacc"
                                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"LocalVariableDeclaration->ForInit");
    }
#line 9082 "y.tab.c"
    break;

  case 253: /* ForUpdate: StatementExpressionList  */
#line 5059 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        processPostIncre((yyvsp[0].node));
        for(int i = 0; i < (yyval.node)->code.size(); i++) {
            ircode.pop_back();

        }
        
        verbose(v,"StatementExpressionList->ForUpdate");
    }
#line 9097 "y.tab.c"
    break;

  case 254: /* StatementExpressionList: StatementExpression  */
#line 5072 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"StatementExpression->StatementExpressionList");
    }
#line 9106 "y.tab.c"
    break;

  case 255: /* StatementExpressionList: StatementExpression COMMA StatementExpression CommaStatementExpressions  */
#line 5077 "parser.yacc"
    {
       
        vector<Node*> temp = {(yyvsp[-3].node), (yyvsp[-1].node)};
        if((yyvsp[0].node)) temp.push_back((yyvsp[0].node));
        (yyval.node) = new Node("StatementExpressionList", temp);
        verbose(v,"StatementExpression COMMA StatementExpression CommaStatementExpressions->StatementExpressionList");
    }
#line 9118 "y.tab.c"
    break;

  case 256: /* EnhancedForStatement: FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS Statement  */
#line 5088 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        /*************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        **************************************************/
        
        vector<Node* > temp = {t1, (yyvsp[-4].node), (yyvsp[-2].node),(yyvsp[0].node)};
        (yyval.node) = new Node("EnhancedForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS Statement->EnhancedForStatement");
    }
#line 9135 "y.tab.c"
    break;

  case 257: /* EnhancedForStatementNoShortIf: FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS StatementNoShortIf  */
#line 5104 "parser.yacc"
    {
        struct Node* t1 = new Node("Keyword", (yyvsp[-6].lexeme));
        //struct Node* t2 = new Node("Separator", $2);
        //struct Node* t4 = new Node("Separator", $4);
        //struct Node* t6 = new Node("Separator", $6);
        
        vector<Node* > temp = {t1, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)};
        (yyval.node) = new Node("EnhancedForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS StatementNoShortIf->EnhancedForStatementNoShortIf");
    }
#line 9150 "y.tab.c"
    break;

  case 258: /* BreakStatement: BREAK SEMICOLON  */
#line 5117 "parser.yacc"
                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-1].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    struct Node* n = new struct Node("BreakStatement", temp);
    (yyval.node) = n;
    verbose(v,"BREAK SEMICOLON->BreakStatement");
    if(root->nlookup("while")==nullptr && root->nlookup("for")==nullptr && root->nlookup("do")==nullptr) 
    {
        cout<<"Error! Break statement outside loop on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = VOID_TYPE;
}
#line 9171 "y.tab.c"
    break;

  case 259: /* BreakStatement: BREAK IDENTIFIER SEMICOLON  */
#line 5133 "parser.yacc"
                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    struct Node* n2 = new struct Node("Identifier", (yyvsp[-1].lexeme));
    temp.push_back(n2);
    //struct Node* n3= new struct Node("Separator", $3);
    //temp.push_back(n3);
    struct Node* n = new struct Node("BreakStatement", temp);
    (yyval.node) = n;
    verbose(v,"BREAK IDENTIFIER SEMICOLON>BreakStatement");

    (yyval.node)->type = VOID_TYPE;
}
#line 9190 "y.tab.c"
    break;

  case 260: /* ContinueStatement: CONTINUE SEMICOLON  */
#line 5151 "parser.yacc"
                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-1].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    struct Node* n = new struct Node("ContinueStatement", temp);
    (yyval.node) = n;
    verbose(v,"CONTINUE SEMICOLON->ContinueStatement");
    if(root->nlookup("while")==nullptr && root->nlookup("for")==nullptr && root->nlookup("do")==nullptr) 
    {
        cout<<"Error! Break statement outside loop on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = VOID_TYPE;
}
#line 9211 "y.tab.c"
    break;

  case 261: /* ContinueStatement: CONTINUE IDENTIFIER SEMICOLON  */
#line 5167 "parser.yacc"
                                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    struct Node* n2 = new struct Node("Identifier", (yyvsp[-1].lexeme));
    temp.push_back(n2);
    //struct Node* n3= new struct Node("Separator", $3);
    //temp.push_back(n3);
    struct Node* n = new struct Node("ContinueStatement", temp);
    (yyval.node) = n;
    verbose(v,"CONTINUE IDENTIFIER SEMICOLON->ContinueStatement");

    (yyval.node)->type = VOID_TYPE;
}
#line 9230 "y.tab.c"
    break;

  case 262: /* ReturnStatement: RETURN SEMICOLON  */
#line 5184 "parser.yacc"
                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-1].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    struct Node* n = new struct Node("ReturnStatement", temp);
    (yyval.node) = n;
    
    Quadruple* q= new Quadruple(7, "return");
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    processPostIncre((yyval.node));
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"RETURN SEMICOLON->ReturnStatement");

    if(getmethodtype(root->currNode) != VOID_TYPE)
    {
        cout<<"Error on line number "<<yylineno<<". Return value should be of "<<typeroot->inv_types[root->currNode->returntype]<<" type."<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = VOID_TYPE;
}
#line 9258 "y.tab.c"
    break;

  case 263: /* ReturnStatement: RETURN Expression SEMICOLON  */
#line 5207 "parser.yacc"
                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    if((yyvsp[-1].node)->useful == false) {
                for(auto it: (yyvsp[-1].node)->children) {
                    temp.push_back(it);
                }
            }
    else temp.push_back((yyvsp[-1].node));
   
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    struct Node* n = new struct Node("ReturnStatement", temp);
    (yyval.node) = n;
   
    Quadruple* q= new Quadruple(7, "return", (yyvsp[-1].node)->varName );
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
     processPostIncre((yyval.node));
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"RETURN Expression SEMICOLON->ReturnStatement");

    if(getmethodtype(root->currNode) != (yyvsp[-1].node)->type)
    {
        cout<<"Error on line number "<<yylineno<<". Return value should be of "<<typeroot->inv_types[root->currNode->returntype]<<" type."<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = VOID_TYPE;
}
#line 9293 "y.tab.c"
    break;

  case 264: /* LeftRightSquareBrackets: %empty  */
#line 5240 "parser.yacc"
    {(yyval.node) = NULL;}
#line 9299 "y.tab.c"
    break;

  case 265: /* LeftRightSquareBrackets: LRSQUAREBRACKET LeftRightSquareBrackets  */
#line 5241 "parser.yacc"
                                            {
    vector<struct Node*> temp;
    
     if((yyvsp[0].node)) {
            if((yyvsp[0].node)->useful == false) {
                for(auto it: (yyvsp[0].node)->children) {
                    temp.push_back(it);
                }
            }
            else temp.push_back((yyvsp[0].node));
        }
    struct Node* n = new struct Node("LeftRightSquareBrackets", temp);
    (yyval.node) = n;
    verbose(v,"LRSQUAREBRACKET LeftRightSquareBrackets->LeftRightSquareBrackets");
}
#line 9319 "y.tab.c"
    break;

  case 266: /* Primary: PrimaryNoNewArray  */
#line 5264 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"PrimaryNoNewArray->Primary");
    }
#line 9328 "y.tab.c"
    break;

  case 267: /* Primary: ArrayCreationExpression  */
#line 5268 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ArrayCreationExpression->Primary");
    }
#line 9337 "y.tab.c"
    break;

  case 268: /* PrimaryNoNewArray: Literal  */
#line 5275 "parser.yacc"
            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Literal->PrimaryNoNewArray");
    }
#line 9346 "y.tab.c"
    break;

  case 269: /* PrimaryNoNewArray: ClassLiteral  */
#line 5279 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ClassLiteral->PrimaryNoNewArray");
    }
#line 9355 "y.tab.c"
    break;

  case 270: /* PrimaryNoNewArray: THIS  */
#line 5283 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"THIS->PrimaryNoNewArray");
}
#line 9365 "y.tab.c"
    break;

  case 271: /* PrimaryNoNewArray: Name DOT THIS  */
#line 5288 "parser.yacc"
                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("PrimaryNoNewArray", temp);
    (yyval.node) = n;
    verbose(v,"Name DOT THIS ->PrimaryNoNewArray");
}
#line 9381 "y.tab.c"
    break;

  case 272: /* PrimaryNoNewArray: LEFTPARENTHESIS Expression RIGHTPARENTHESIS  */
#line 5299 "parser.yacc"
                                                {
    vector<struct Node*> temp;
    //struct Node* n1 = new struct Node("Separator", $1);
    //temp.push_back(n1);
    if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-1].node));
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    //struct Node* n = new struct Node("PrimaryNoNewArray", temp);
    (yyval.node) = (yyvsp[-1].node);
    verbose(v,"LEFTPARENTHESIS Expression RIGHTPARENTHESIS->PrimaryNoNewArray");

    (yyval.node)->type = (yyvsp[-1].node)->type;
}
#line 9404 "y.tab.c"
    break;

  case 273: /* PrimaryNoNewArray: ClassInstanceCreationExpression  */
#line 5317 "parser.yacc"
                                    {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"ClassInstanceCreationExpression->PrimaryNoNewArray");
    }
#line 9413 "y.tab.c"
    break;

  case 274: /* PrimaryNoNewArray: FieldAccess  */
#line 5321 "parser.yacc"
                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"FieldAccess->PrimaryNoNewArray");
    }
#line 9422 "y.tab.c"
    break;

  case 275: /* PrimaryNoNewArray: ArrayAccess  */
#line 5325 "parser.yacc"
                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ArrayAccess->PrimaryNoNewArray");
    }
#line 9431 "y.tab.c"
    break;

  case 276: /* PrimaryNoNewArray: MethodInvocation  */
#line 5329 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"MethodInvocation->PrimaryNoNewArray");
    }
#line 9440 "y.tab.c"
    break;

  case 277: /* PrimaryNoNewArray: MethodReference  */
#line 5333 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"MethodReference->PrimaryNoNewArray");
    }
#line 9449 "y.tab.c"
    break;

  case 278: /* ClassLiteral: Name DOT CLASS  */
#line 5340 "parser.yacc"
                   {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("ClassLiteral", temp);
    (yyval.node) = n;
    verbose(v,"Name DOT CLASS->ClassLiteral");

    (yyval.node)->type = CLASS_LITERAL;
}
#line 9467 "y.tab.c"
    break;

  case 279: /* ClassLiteral: Name LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS  */
#line 5353 "parser.yacc"
                                                           {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-4].node));
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    
    //temp.push_back($3);
    struct Node* n3 = new struct Node("Operator", (yyvsp[-1].lexeme));
    temp.push_back(n3);
    struct Node* n4 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n4);
    struct Node* n = new struct Node("ClassLiteral", temp);
    (yyval.node) = n;
    verbose(v,"Name LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS");
    (yyval.node)->type = CLASS_LITERAL;
}
#line 9488 "y.tab.c"
    break;

  case 280: /* ClassLiteral: PrimitiveType DOT CLASS  */
#line 5369 "parser.yacc"
                            {
    vector<struct Node*> temp;
    if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-2].node));
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("ClassLiteral", temp);
    (yyval.node) = n;
    (yyval.node)->type = (yyvsp[-2].node)->type;
    verbose(v,"PrimitiveType DOT CLASS->ClassLiteral");

    (yyval.node)->type = CLASS_LITERAL;

}
#line 9513 "y.tab.c"
    break;

  case 281: /* ClassLiteral: PrimitiveType LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS  */
#line 5389 "parser.yacc"
                                                                       {
    vector<struct Node*> temp;
    if((yyvsp[-4].node)->useful == false) {
            for(auto it: (yyvsp[-4].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-4].node));
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    
    //temp.push_back($3);
    
    struct Node* n4 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n4);
    struct Node* n = new struct Node("ClassLiteral", temp);
    (yyval.node) = n;
    (yyval.node)->type = (yyvsp[-4].node)->type;
    verbose(v,"PrimitiveType LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS->ClassLiteral");

    (yyval.node)->type = CLASS_LITERAL;

}
#line 9541 "y.tab.c"
    break;

  case 282: /* ClassLiteral: VOID DOT CLASS  */
#line 5412 "parser.yacc"
                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Operator", $2);
    //temp.push_back(n2);
    struct Node* n3 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n3);
    struct Node* n = new struct Node("ClassLiteral", temp);
    (yyval.node) = n;
    verbose(v,"VOID DOT CLASS->ClassLiteral");

    (yyval.node)->type = CLASS_LITERAL;
}
#line 9560 "y.tab.c"
    break;

  case 283: /* NumericType: IntegralType  */
#line 5429 "parser.yacc"
                    {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"IntegralType->NumericType");

    }
#line 9570 "y.tab.c"
    break;

  case 284: /* NumericType: FloatingPointType  */
#line 5434 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"FloatingPointType->NumericType");
    }
#line 9579 "y.tab.c"
    break;

  case 285: /* IntegralType: BYTE  */
#line 5441 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    (yyval.node)->type = BYTE_NUM;
    verbose(v,"BYTE->IntegralType");
}
#line 9590 "y.tab.c"
    break;

  case 286: /* IntegralType: SHORT  */
#line 5447 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    (yyval.node)->type = SHORT_NUM;
    verbose(v,"SHORT->IntegralType");
}
#line 9601 "y.tab.c"
    break;

  case 287: /* IntegralType: INT  */
#line 5453 "parser.yacc"
        {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    (yyval.node)->type = INT_NUM;
    verbose(v,"INT->IntegralType");
}
#line 9612 "y.tab.c"
    break;

  case 288: /* IntegralType: LONG  */
#line 5459 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    (yyval.node)->type = LONG_NUM;
    verbose(v,"LONG->IntegralType");
}
#line 9623 "y.tab.c"
    break;

  case 289: /* IntegralType: CHAR  */
#line 5465 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    (yyval.node)->type = CHAR_NUM;
    verbose(v,"CHAR->IntegralType");
}
#line 9634 "y.tab.c"
    break;

  case 290: /* FloatingPointType: FLOAT  */
#line 5474 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    (yyval.node)->type = FLOAT_NUM;
    verbose(v,"FLOAT->FloatingPointType");
}
#line 9645 "y.tab.c"
    break;

  case 291: /* FloatingPointType: DOUBLE  */
#line 5480 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", (yyvsp[0].lexeme));
    (yyval.node) = n;
    (yyval.node)->type = DOUBLE_NUM;
    verbose(v,"DOUBLE->FloatingPointType");
}
#line 9656 "y.tab.c"
    break;

  case 292: /* ClassInstanceCreationExpression: UnqualifiedClassInstanceCreationExpression  */
#line 5489 "parser.yacc"
                                                 {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"UnqualifiedClassInstanceCreationExpression->ClassInstanceCreationExpression");
    }
#line 9665 "y.tab.c"
    break;

  case 293: /* ClassInstanceCreationExpression: Name DOT UnqualifiedClassInstanceCreationExpression  */
#line 5493 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[0].node));
    struct Node* n = new Node("ClassInstanceCreationExpression", temp);
    (yyval.node) = n;  
    verbose(v,"Name DOT UnqualifiedClassInstanceCreationExpression->ClassInstanceCreationExpression"); 

}
#line 9686 "y.tab.c"
    break;

  case 294: /* ClassInstanceCreationExpression: Primary DOT UnqualifiedClassInstanceCreationExpression  */
#line 5509 "parser.yacc"
                                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    //struct Node* n1 = new struct Node("Keyword", $2);
    //temp.push_back(n1);
    if((yyvsp[0].node)->useful == false) {
            for(auto it: (yyvsp[0].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[0].node));
    struct Node* n= new Node("ClassInstanceCreationExpression", temp);
    (yyval.node) = n;   
    verbose(v,"Primary DOT UnqualifiedClassInstanceCreationExpression->ClassInstanceCreationExpression");

}
#line 9707 "y.tab.c"
    break;

  case 295: /* UnqualifiedClassInstanceCreationExpression: NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS ClassBody  */
#line 5528 "parser.yacc"
                                                                      {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-5].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-4].node));
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-2].node));
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    (yyval.node) = n;
    (yyval.node)->varName = string("new ") + (yyvsp[-4].node)->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS ClassBody->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup((yyvsp[-4].node)->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<(yyvsp[-4].node)->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[(yyvsp[-4].node)->attr];
    for(auto it : (yyvsp[-2].node)->children)
    {
        args.push_back(it->type);
    }
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<(yyvsp[-4].node)->attr<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = res->type;
}
#line 9748 "y.tab.c"
    break;

  case 296: /* UnqualifiedClassInstanceCreationExpression: NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS ClassBody  */
#line 5564 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-3].node));
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    //struct Node* n3 = new struct Node("Operator", $4);
    //temp.push_back(n3);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    (yyval.node) = n;
    (yyval.node)->varName = string("new ") + (yyvsp[-3].node)->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS ClassBody->UnqualifiedClassInstanceCreationExpression");

    Symbol* sym = root->lookup((yyvsp[-3].node)->attr);
    if(!sym)
    {
        cout<<"Error on line number "<<yylineno<<". Class named "<<(yyvsp[-3].node)->attr<<" has not been declared";
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[(yyvsp[-3].node)->attr];
    args.push_back(-1);
    if((r->default_done || r->constr_args.size()>0) && !r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<(yyvsp[-3].node)->attr<<endl;
        yyerror("Error");
    }
    
    (yyval.node)->type = sym->type;
}
#line 9786 "y.tab.c"
    break;

  case 297: /* UnqualifiedClassInstanceCreationExpression: NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS  */
#line 5597 "parser.yacc"
                                                            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-3].node));
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-1].node));
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    (yyval.node) = n;
    (yyval.node)->varName = string("new ") + (yyvsp[-3].node)->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup((yyvsp[-3].node)->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<(yyvsp[-3].node)->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[(yyvsp[-3].node)->attr];
    for(auto it : (yyvsp[-1].node)->children)
    {
        args.push_back(it->type);
    }
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<(yyvsp[-3].node)->attr<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = res->type;
}
#line 9826 "y.tab.c"
    break;

  case 298: /* UnqualifiedClassInstanceCreationExpression: NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 5632 "parser.yacc"
                                                {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-3].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-2].node));
    /*****************************************
    struct Node* n2 = new struct Node("Operator", $3);
    temp.push_back(n2);
    struct Node* n3 = new struct Node("Operator", $4);
  
    temp.push_back(n3);
    *********************************************/
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    (yyval.node) = n;
    (yyval.node)->varName = string("new ") + (yyvsp[-2].node)->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup((yyvsp[-2].node)->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<(yyvsp[-2].node)->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[(yyvsp[-2].node)->attr];
    args.push_back(-1);
    if((r->default_done || r->constr_args.size()>0) && !r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<(yyvsp[-2].node)->attr<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = res->type;
}
#line 9865 "y.tab.c"
    break;

  case 299: /* UnqualifiedClassInstanceCreationExpression: NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS ClassBody  */
#line 5666 "parser.yacc"
                                                                    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-5].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-4].node));
   // struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-2].node));
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    (yyval.node) = n;
    (yyval.node)->varName = string("new ") + (yyvsp[-4].node)->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS ClassBody->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup((yyvsp[-4].node)->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<(yyvsp[-4].node)->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[(yyvsp[-4].node)->attr];
    args.push_back((yyvsp[-2].node)->type);
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<(yyvsp[-4].node)->attr<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = res->type;
}
#line 9903 "y.tab.c"
    break;

  case 300: /* UnqualifiedClassInstanceCreationExpression: NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS  */
#line 5699 "parser.yacc"
                                                            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-3].node));
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back((yyvsp[-1].node));
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    (yyval.node) = n;
    (yyval.node)->varName = string("new ") + (yyvsp[-3].node)->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup((yyvsp[-3].node)->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<(yyvsp[-3].node)->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[(yyvsp[-3].node)->attr];
    args.push_back((yyvsp[-1].node)->type);
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<(yyvsp[-3].node)->attr<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = res->type;
}
#line 9940 "y.tab.c"
    break;

  case 301: /* FieldAccess: Primary DOT IDENTIFIER  */
#line 5734 "parser.yacc"
                            {
    vector<struct Node*> temp;
    if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-2].node));
    
    struct Node* n3 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n3);
    struct Node* n = new struct Node("FieldAccess", temp);
    (yyval.node) = n;
    (yyval.node)->varName = (yyval.node)->attr = (yyvsp[-2].node)->varName + "." + (yyvsp[0].lexeme);
    verbose(v,"Primary DOT IDENTIFIER->FieldAccess");

    Symbol* res = root->lookup((yyvsp[0].lexeme));
    if(!res)
        yyerror("Variable not declared before");
    (yyval.node)->type = res->type;
}
#line 9966 "y.tab.c"
    break;

  case 302: /* FieldAccess: SUPER DOT IDENTIFIER  */
#line 5755 "parser.yacc"
                           {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
   
    struct Node* n3 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n3);
    struct Node* n = new struct Node("FieldAccess", temp);
    (yyval.node) = n;
    (yyval.node)->varName = (yyval.node)->attr = string((yyvsp[-2].lexeme)) + string(".") + string((yyvsp[0].lexeme));
    verbose(v,"SUPER DOT IDENTIFIER->FieldAccess");
    SymNode* r = root->currNode;
    while(r->parent&&r->name!="classextends")
        r = r->parent;
    if(!(r->parent))
        yyerror("No parent class exists for the specified class");
    Symbol* res = r->parent->scope_lookup((yyvsp[0].lexeme));
    if(!res)
        yyerror("Field with specified name not found");
    (yyval.node)->type = res->type;
}
#line 9992 "y.tab.c"
    break;

  case 303: /* FieldAccess: Name DOT SUPER DOT IDENTIFIER  */
#line 5776 "parser.yacc"
                                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-4].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n2);
    
    struct Node* n4 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n4);
    struct Node* n = new struct Node("FieldAccess", temp);
    (yyval.node) = n;
    (yyval.node)->varName = (yyval.node)->attr = (yyvsp[-4].node)->varName + "." + (yyvsp[-2].lexeme)+ "." + (yyvsp[0].lexeme);
    verbose(v,"Name DOT SUPER DOT IDENTIFIER->FieldAccess");

    SymNode* r = root->clookup((yyvsp[-4].node)->attr);
    if(!r)
        yyerror("No class with specified name found");
    if(!(r->parent))
        yyerror("No parent class exists for the given class");    
    Symbol* res = r->parent->scope_lookup((yyvsp[0].lexeme));
    if(!res)
        yyerror("Field with specified name not found");
    (yyval.node)->type = res->type;
}
#line 10021 "y.tab.c"
    break;

  case 304: /* ArrayAccess: Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  */
#line 5803 "parser.yacc"
                                                          {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-3].node));
    
    temp.push_back((yyvsp[-1].node));
   
    struct Node* n = new struct Node("ArrayAccess", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyvsp[-3].node)->attr;
    
    string resName = string("t") + to_string(varCnt);
    varCnt++;
    Symbol* ss = root->lookup((yyvsp[-3].node)->attr);
    if(!ss)
    {
        cout<<"Error on line number "<<yylineno<<". Name "<<ss->lexeme<<" has not been declared before."<<endl;
        yyerror("Error");
    }
    cout << ss->width1 << "947t9wefih\n";
    Quadruple* q= new Quadruple(string("*"), (yyvsp[-1].node)->varName, to_string(root->lookup((yyvsp[-3].node)->attr)->width),  resName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName2 = string("t") + to_string(varCnt);
    varCnt++;
    q= new Quadruple(string("="), string((yyvsp[-3].node)->varName) + string("[") + resName + string("]") ,  resName2);
    //cout << "hi\n";
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->varName = resName2;
    (yyval.node)->attr = (yyvsp[-3].node)->attr;
    (yyval.node)->type = root->lookup((yyvsp[-3].node)->varName)->type - 100;
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->ArrayAccess");
    (yyval.node)->cnt++;
    
}
#line 10062 "y.tab.c"
    break;

  case 305: /* ArrayAccess: Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  */
#line 5839 "parser.yacc"
                                                                                                          {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-6].node));
    
    temp.push_back((yyvsp[-4].node));
    temp.push_back((yyvsp[-1].node));
   
    struct Node* n = new struct Node("ArrayAccess", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyvsp[-6].node)->attr;
    string resName = string("t") + to_string(varCnt);
    varCnt++;
    Quadruple* q= new Quadruple(string("*"), root->lookup((yyvsp[-6].node)->varName)->width2, to_string(root->lookup((yyvsp[-6].node)->varName)->width),  resName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName1 = string("t") + to_string(varCnt);
    varCnt++;
    q= new Quadruple(string("*"), (yyvsp[-4].node)->varName, resName,  resName1);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName2 = string("t") + to_string(varCnt);
    varCnt++;
    q= new Quadruple(string("*"), (yyvsp[-1].node)->varName, to_string(root->lookup((yyvsp[-6].node)->varName)->width),  resName2);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName3 = string("t") + to_string(varCnt);
    varCnt++;
    q= new Quadruple(string("+"), resName1, resName2,  resName3);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName4 = string("t") + to_string(varCnt++);
    
    q= new Quadruple(string("="), string((yyvsp[-6].node)->varName) + string("[") + resName3 + string("]") ,  resName4);
   
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->varName = resName4;
    (yyval.node)->attr = (yyvsp[-6].node)->attr;
    (yyval.node)->last = ircode.size() - 1;
    (yyval.node)->type = root->lookup((yyvsp[-6].node)->varName)->type - 200;
    verbose(v,"Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->ArrayAccess");
    (yyval.node)->cnt++;
}
#line 10110 "y.tab.c"
    break;

  case 306: /* ArrayAccess: Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  */
#line 5882 "parser.yacc"
                                                                                                                                                          {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-9].node));
    
    temp.push_back((yyvsp[-7].node));
    temp.push_back((yyvsp[-4].node));
    temp.push_back((yyvsp[-1].node));
   
    struct Node* n = new struct Node("ArrayAccess", temp);
    (yyval.node) = n;
    (yyval.node)->attr = (yyvsp[-9].node)->attr;
    
    string resName = string("t") + to_string(varCnt);
    varCnt++;
    Quadruple* q= new Quadruple(string("*"), root->lookup((yyvsp[-9].node)->varName)->width3 , to_string(root->lookup((yyvsp[-9].node)->varName)->width),  resName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName1 = string("t") + to_string(varCnt);
    varCnt++;
    q= new Quadruple(string("*"), root->lookup((yyvsp[-9].node)->varName)->width2 , resName,  resName1);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName2 = string("t") + to_string(varCnt);
    varCnt++;
    q= new Quadruple(string("*"), (yyvsp[-4].node)->varName, resName,  resName2);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    resName = string("t") + to_string(varCnt);
    varCnt++;
    q= new Quadruple(string("*"), (yyvsp[-7].node)->varName, resName1,  resName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName3 = string("t") + to_string(varCnt++);
    q= new Quadruple(string("*"), (yyvsp[-1].node)->varName, to_string(root->lookup((yyvsp[-9].node)->varName)->width),  resName3);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName4 = string("t") + to_string(varCnt++);
    q= new Quadruple(string("+"), resName, resName2,  resName4);

    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName5 = string("t") + to_string(varCnt++);
    q= new Quadruple(string("+"), resName4, resName3,  resName5);

    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    string resName6 = string("t") + to_string(varCnt++);
    q= new Quadruple(string("="), string((yyvsp[-9].node)->varName) + string("[") + resName5 + string("]") ,  resName6);
   
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->varName = resName6;
    (yyval.node)->attr = (yyvsp[-9].node)->attr;
    (yyval.node)->type = root->lookup((yyvsp[-9].node)->varName)->type - 300;
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->ArrayAccess");
    (yyval.node)->cnt++;
}
#line 10173 "y.tab.c"
    break;

  case 307: /* MethodInvocation: Name LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 5942 "parser.yacc"
                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    verbose(v,"Name LEFTPARENTHESIS RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    string sp = spacestrip((yyvsp[-2].node)->attr);

    if(sp.length()<(yyvsp[-2].node)->attr.length() && !magic_ptr)
    {
        cout<<"Error on line number "<<yylineno<<". No such class!"<<endl;
        yyerror("Error");
    }

    SymNode* ex;
    
    if(magic_ptr->name=="Global")
        ex = root->flookup(sp, args);
    else
        ex = magic_ptr->scope_flookup(sp, args, false);

    if(!ex)
    {
        cout<<"Error on line number "<<yylineno<<". No matching function to call"<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = ex->returntype;
    Quadruple* q;
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-2].node)->attr, to_string(0), resName);
        (yyval.node)->varName = resName;
    }
    else 
    {
        q = q = new Quadruple(4, (yyvsp[-2].node)->attr, to_string(0));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
   
    
}
#line 10224 "y.tab.c"
    break;

  case 308: /* MethodInvocation: Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS  */
#line 5988 "parser.yacc"
                                                         {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-3].node));
  
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    verbose(v,"Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    for(auto ch : (yyvsp[-1].node)->children)
        args.push_back(ch->type);
    string sp = spacestrip((yyvsp[-3].node)->attr);
    if(sp.length()<(yyvsp[-3].node)->attr.length() && !magic_ptr)
    {
        cout<<"Error on line number "<<yylineno<<". No such class!"<<endl;
        yyerror("Error");
    }

    SymNode* ex;
    
    if(magic_ptr->name=="Global")
        ex = root->flookup(sp, args);
    else
    {    ex = magic_ptr->scope_flookup(sp, args, false);}
    if(!ex)
    {
        cout<<"Error on line number "<<yylineno<<". No matching function to call"<<endl;
        yyerror("Error");
    }
    (yyval.node)->type = ex->returntype;
    Quadruple* q;
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-3].node)->attr, to_string((yyvsp[-1].node)->code.size()), resName);
        (yyval.node)->varName = resName;
    }
    else 
    {
        q = q = new Quadruple(4, (yyvsp[-3].node)->attr, to_string((yyvsp[-1].node)->code.size()));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
}
#line 10275 "y.tab.c"
    break;

  case 309: /* MethodInvocation: Primary DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 6034 "parser.yacc"
                                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-4].node));
    
    struct Node* n2 = new struct Node("Identifier", (yyvsp[-2].lexeme));
    temp.push_back(n2);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q; 
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-4].node)->attr + string(".") + (yyvsp[-2].lexeme), to_string(0), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4, (yyvsp[-4].node)->attr + string(".") + (yyvsp[-2].lexeme), to_string(0));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Primary DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS->MethodInvocation");
}
#line 10303 "y.tab.c"
    break;

  case 310: /* MethodInvocation: Primary DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS  */
#line 6057 "parser.yacc"
                                                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-5].node));
    
    struct Node* n2 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n2);
   
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q;
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-5].node)->attr + string(".") + (yyvsp[-3].lexeme), to_string((yyvsp[-1].node)->code.size()), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4, (yyvsp[-5].node)->attr + string(".") + (yyvsp[-3].lexeme), to_string((yyvsp[-1].node)->code.size()));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Primary DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->MethodInvocation");
}
#line 10333 "y.tab.c"
    break;

  case 311: /* MethodInvocation: SUPER DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 6082 "parser.yacc"
                                                            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-4].lexeme));
    temp.push_back(n1);
    
    struct Node* n4 = new struct Node("Identifier", (yyvsp[-2].lexeme));
    temp.push_back(n4);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q;
    
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "",  (yyvsp[-4].lexeme) + string(".") + (yyvsp[-2].lexeme), to_string(0), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4,  (yyvsp[-4].lexeme) + string(".") + (yyvsp[-2].lexeme), to_string(0));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"SUPER DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    SymNode* chck = root->currNode;
    while(chck->parent&&chck->name!="classextends"){
        chck = chck->parent;
    }
    if(!(chck->parent))
    {
        cout<<"Error on line number "<<yylineno<<". No parent class exists for the given child class"<<endl;
        yyerror("Error");
    }
        // yyerror("No parent class exists for the given class");
    SymNode* res = chck->parent->scope_flookup((yyvsp[-2].lexeme), args);
    if(!res)
        yyerror("No such function declared before.");
    (yyval.node)->type = res->returntype;
}
#line 10379 "y.tab.c"
    break;

  case 312: /* MethodInvocation: SUPER DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS  */
#line 6123 "parser.yacc"
                                                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-5].lexeme));
    temp.push_back(n1);
    
    struct Node* n4 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n4);
    
    temp.push_back((yyvsp[-1].node));
   
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q;
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string((yyvsp[-1].node)->code.size()), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4, (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string((yyvsp[-1].node)->code.size()));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"SUPER DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    SymNode* chck = root->currNode;
    while(chck->parent&&chck->name!="classextends"){
        chck = chck->parent;
    }
    if(!(chck->parent))
    {
        cout<<"Error on line number "<<yylineno<<". No parent class exists for the given child class"<<endl;
        yyerror("Error");
    }
    for(auto ch : (yyvsp[-1].node)->children)
        args.push_back(ch->type);
    SymNode* res = chck->parent->scope_flookup((yyvsp[-3].lexeme), args);
    if(!res)
        yyerror("No such function declared before.");
    (yyval.node)->type = res->returntype;
}
#line 10427 "y.tab.c"
    break;

  case 313: /* MethodInvocation: Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 6166 "parser.yacc"
                                                                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-6].node));
    
   
    struct Node* n5 = new struct Node("Identifier", (yyvsp[-2].lexeme));
    temp.push_back(n5);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q ;
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-6].node)->attr + string(".") + (yyvsp[-4].lexeme) + string(".") + (yyvsp[-2].lexeme), to_string(0), resName);
        (yyval.node)->varName = resName;
    }
    else 
    {
        q = new Quadruple(4, (yyvsp[-6].node)->attr + string(".") + (yyvsp[-4].lexeme) + string(".") + (yyvsp[-2].lexeme), to_string(0));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    SymNode* par = root->clookup((yyvsp[-6].node)->attr);
    if(!par)
    {
        cout<<"Error on line number "<<yylineno<<"! Class with name "<<(yyvsp[-6].node)->attr<<" has not been declared before"<<endl;
        yyerror("Error");
    }
    //     yyerror("Not the name of a class");
    if(!(par->parent))
    {
        cout<<"Error on line number "<<yylineno<<". No parent class exists for the given child class"<<endl;
        yyerror("Error");
    }
    SymNode* res = par->parent->scope_flookup((yyvsp[-2].lexeme), args);
    if(!res)
        yyerror("No such function declared before.");
    (yyval.node)->type = res->returntype;
}
#line 10475 "y.tab.c"
    break;

  case 314: /* MethodInvocation: Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS  */
#line 6209 "parser.yacc"
                                                                                {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-7].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-5].lexeme));
    temp.push_back(n2);
    
    struct Node* n5 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n5);
    
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q;
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "",(yyvsp[-7].node)->attr + string(".") + (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string((yyvsp[-1].node)->code.size()), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4, (yyvsp[-7].node)->attr + string(".") + (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string((yyvsp[-1].node)->code.size()));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    for(auto ch : (yyvsp[-1].node)->children)
        args.push_back(ch->type);
    SymNode* par = root->clookup((yyvsp[-7].node)->attr);
    if(!par)
    {
        cout<<"Error on line number "<<yylineno<<"! Class with name "<<(yyvsp[-7].node)->attr<<" has not been declared before"<<endl;
        yyerror("Error");
    }
    if(!(par->parent))
    {
        cout<<"Error on line number "<<yylineno<<". No parent class exists for the given child class"<<endl;
        yyerror("Error");
    }  
    SymNode* res = par->parent->scope_flookup((yyvsp[-3].lexeme), args);
    if(!res)
        yyerror("No such function declared before.");
    (yyval.node)->type = res->returntype;
}
#line 10527 "y.tab.c"
    break;

  case 315: /* MethodInvocation: Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS  */
#line 6256 "parser.yacc"
                                                        {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-3].node));
    
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-1].node)->varName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-3].node)->attr, to_string(1), resName);
        (yyval.node)->varName = resName;
    }
    else 
    {
        q = q = new Quadruple(4, (yyvsp[-3].node)->attr, to_string(1));
    }

    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    args.push_back((yyvsp[-1].node)->type);
    string sp = spacestrip((yyvsp[-3].node)->attr);

    if(sp.length()<(yyvsp[-3].node)->attr.length() && !magic_ptr)
    {
        cout<<"Error on line number "<<yylineno<<". No such class!"<<endl;
        yyerror("Error");
    }

    SymNode* ex;
    
    if(magic_ptr->name=="Global")
        ex = root->flookup(sp, args);
    else
        ex = magic_ptr->scope_flookup(sp, args, false);

    if(!ex)
    {
        cout<<"Error on line number "<<yylineno<<". No matching function to call"<<endl;
        yyerror("Error");
    }
    // cout<<"sp is "<<sp<<endl;
    // SymNode* res = root->flookup(sp, args);
    // cout<<"HI HI HI H"<<endl;
    // if(!res)
    //     yyerror("No such function declared before.");
    // $$->type = res->returntype;
    (yyval.node)->type = ex->returntype;
}
#line 10588 "y.tab.c"
    break;

  case 316: /* MethodInvocation: Primary DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS  */
#line 6312 "parser.yacc"
                                                                        {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-5].node));
    
    struct Node* n2 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n2);
    
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-1].node)->varName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-5].node)->attr + string(".") + (yyvsp[-3].lexeme), to_string(1), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4, (yyvsp[-5].node)->attr + string(".") + (yyvsp[-3].lexeme), to_string(1));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Primary DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");
}
#line 10620 "y.tab.c"
    break;

  case 317: /* MethodInvocation: SUPER DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS  */
#line 6339 "parser.yacc"
                                                                        {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-5].lexeme));
    temp.push_back(n1);
    
    struct Node* n4 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n4);
    
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-1].node)->varName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "", (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string(1), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4, (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string(1));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"SUPER DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
     SymNode* chck = root->currNode;
    while(chck->parent&&chck->name!="classextends"){
        chck = chck->parent;
    }
    if(!(chck->parent))
    {
        cout<<"Error on line number "<<yylineno<<". No parent class exists for the given child class"<<endl;
        yyerror("Error");
    }
    for(auto ch : (yyvsp[-1].node)->children)
        args.push_back(ch->type);
    SymNode* res = chck->parent->scope_flookup((yyvsp[-3].lexeme), args);
    if(!res)
        yyerror("No such function declared before.");
    (yyval.node)->type = res->returntype;
}
#line 10670 "y.tab.c"
    break;

  case 318: /* MethodInvocation: Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS  */
#line 6384 "parser.yacc"
                                                                                {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-7].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-5].lexeme));
    temp.push_back(n2);
    
    struct Node* n5 = new struct Node("Identifier", (yyvsp[-3].lexeme));
    temp.push_back(n5);
    
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(5, (yyvsp[-1].node)->varName);
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    if((yyval.node)->type != VOID_TYPE) {
        string resName = string("t") + to_string(varCnt++);
        q = new Quadruple(4, "",(yyvsp[-7].node)->attr + string(".") + (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string(1), resName);
        (yyval.node)->varName = resName;
    }
    else {
        q = new Quadruple(4, (yyvsp[-7].node)->attr + string(".") + (yyvsp[-5].lexeme) + string(".") + (yyvsp[-3].lexeme), to_string(1));
    }
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    args.push_back((yyvsp[-1].node)->type);
    SymNode* r = root->clookup((yyvsp[-7].node)->attr);
    if(!r)
        yyerror("Class not declared");
    if(!(r->parent))
        yyerror("No parent class exists for the given class");
    SymNode* res = r->parent->scope_flookup((yyvsp[-3].lexeme), args);
    if(!res)
        yyerror("No such function declared before.");
    (yyval.node)->type = res->returntype;
}
#line 10717 "y.tab.c"
    break;

  case 319: /* MethodInvocation: SYSTEMOUTPRINTLN LEFTPARENTHESIS Expression RIGHTPARENTHESIS  */
#line 6426 "parser.yacc"
                                                                 {
    vector<struct Node*> temp;
    struct Node* n1 = new Node("Keyword", (yyvsp[-3].lexeme));
    temp.push_back(n1);
    
    temp.push_back((yyvsp[-1].node));
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    Quadruple* q = new Quadruple(7, "", "print", (yyvsp[-1].node)->varName, "");
   
    (yyval.node)->code.push_back(q);
    ircode.push_back(q);
    // if($$->type != VOID_TYPE) {
    //     string resName = string("t") + to_string(varCnt++);
    //     q = new Quadruple(4, "", $1->attr, to_string(1), resName);
    //     $$->varName = resName;
    // }
    // else 
    // {
    //     q = q = new Quadruple(4, $1->attr, to_string(1));
    // }

    // $$->code.push_back(q);
    // ircode.push_back(q);
    (yyval.node)->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    args.push_back((yyvsp[-1].node)->type);
    (yyval.node)->type = 8;
}
#line 10754 "y.tab.c"
    break;

  case 320: /* MethodInvocation: SYSTEMOUTPRINTLN LEFTPARENTHESIS RIGHTPARENTHESIS  */
#line 6458 "parser.yacc"
                                                       {
    vector<struct Node*> temp;
    struct Node* n1 = new Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    struct Node* n = new struct Node("MethodInvocation", temp);
    (yyval.node) = n;
    verbose(v,"Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");
    (yyval.node)->type = 8;
}
#line 10768 "y.tab.c"
    break;

  case 321: /* MethodReference: Name DOUBLECOLON IDENTIFIER  */
#line 6471 "parser.yacc"
                                  {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    
    struct Node* n2 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    (yyval.node) = n;
    (yyval.node)->varName = (yyvsp[-2].node)->varName + string("::") + string((yyvsp[0].lexeme)); 
    verbose(1,"Name DOUBLECOLON IDENTIFIER->MethodReference");
}
#line 10784 "y.tab.c"
    break;

  case 322: /* MethodReference: Primary DOUBLECOLON IDENTIFIER  */
#line 6482 "parser.yacc"
                                    {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    
    struct Node* n2 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    (yyval.node) = n;
    (yyval.node)->varName = (yyvsp[-2].node)->varName + string("::") + string((yyvsp[0].lexeme)); 
    verbose(1,"Primary DOUBLECOLON IDENTIFIER->MethodReference");
}
#line 10800 "y.tab.c"
    break;

  case 323: /* MethodReference: ArrayType DOUBLECOLON IDENTIFIER  */
#line 6493 "parser.yacc"
                                       {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    
    struct Node* n2 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    (yyval.node) = n;
    verbose(1,"ArrayType DOUBLECOLON IDENTIFIER->MethodReference");
}
#line 10815 "y.tab.c"
    break;

  case 324: /* MethodReference: SUPER DOUBLECOLON IDENTIFIER  */
#line 6503 "parser.yacc"
                                  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    
    struct Node* n3 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n3);
    struct Node* n = new struct Node("MethodReference", temp);
    (yyval.node) = n;
    //$$->varName = $1->varName + string("::") + string($3); 
    verbose(1,"SUPER DOUBLECOLON IDENTIFIER->MethodReference");
}
#line 10832 "y.tab.c"
    break;

  case 325: /* MethodReference: Name DOT SUPER DOUBLECOLON IDENTIFIER  */
#line 6515 "parser.yacc"
                                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-4].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n2);
    
    struct Node* n4 = new struct Node("Identifier", (yyvsp[0].lexeme));
    temp.push_back(n4);

    struct Node* n = new struct Node("MethodReference", temp);
    (yyval.node) = n;
    (yyval.node)->varName = (yyvsp[-4].node)->varName + string(".") + string("super") + string("::") + string((yyvsp[-2].lexeme)); 
    verbose(1,"Name DOT SUPER DOUBLECOLON IDENTIFIER->MethodReference");
}
#line 10852 "y.tab.c"
    break;

  case 326: /* MethodReference: Name DOUBLECOLON NEW  */
#line 6530 "parser.yacc"
                            {
    vector<struct Node*> temp;
    temp.push_back((yyvsp[-2].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    (yyval.node) = n;
    (yyval.node)->varName = (yyvsp[-2].node)->varName + string("::") + (yyvsp[0].lexeme); 
    verbose(1,"Name DOUBLECOLON NEW->MethodReference");
}
#line 10868 "y.tab.c"
    break;

  case 327: /* MethodReference: ArrayType DOUBLECOLON NEW  */
#line 6541 "parser.yacc"
                                {
    vector<struct Node*> temp;
    if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-2].node));
    
    struct Node* n2 = new struct Node("Keyword", (yyvsp[0].lexeme));
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    (yyval.node) = n;
    verbose(1,"ArrayType DOUBLECOLON NEW->MethodReference");
}
#line 10888 "y.tab.c"
    break;

  case 328: /* ArrayCreationExpression: NEW PrimitiveType DimExpr_  */
#line 6559 "parser.yacc"
                                 {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
    temp.push_back(n1);
    temp.push_back((yyvsp[-1].node));
    if((yyvsp[0].node)) temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("ArrayCreationExpression", temp);
    (yyval.node) = n;
    (yyval.node)->width1 = (yyvsp[0].node)->width1;
    (yyval.node)->width2 = (yyvsp[0].node)->width2;
    (yyval.node)->width3 = (yyvsp[0].node)->width3;
    (yyval.node)->arrayType = (yyvsp[0].node)->arrayType;
    (yyval.node)->type = (yyval.node)->arrayType*100 + (yyvsp[-1].node)->type;
    (yyval.node)->varName = string("new ") + (yyvsp[-1].node)->attr ;
    verbose(v,"NEW PrimitiveType DimExpr_ ->ArrayCreationExpression");
}
#line 10909 "y.tab.c"
    break;

  case 329: /* ArrayCreationExpression: NEW Name DimExpr_  */
#line 6575 "parser.yacc"
                        {
        vector<struct Node*> temp;
        struct Node* n1 = new struct Node("Keyword", (yyvsp[-2].lexeme));
        temp.push_back(n1);
        temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("ArrayCreationExpression", temp);
        (yyval.node) = n;
        (yyval.node)->width1 = (yyvsp[0].node)->width1;
        (yyval.node)->width2 = (yyvsp[0].node)->width2;
        (yyval.node)->width3 = (yyvsp[0].node)->width3;
        (yyval.node)->arrayType = (yyvsp[0].node)->arrayType;
        (yyval.node)->type = (yyval.node)->arrayType*100 + (yyvsp[-1].node)->type;
        (yyval.node)->varName = string("new ") + (yyvsp[-1].node)->attr ;
        verbose(v,"NEW Name DimExpr_->ArrayCreationExpression");
    }
#line 10930 "y.tab.c"
    break;

  case 330: /* ArrayCreationExpression: NEW PrimitiveType Dims ArrayInitializer  */
#line 6591 "parser.yacc"
                                            {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", (yyvsp[-3].lexeme));
    temp.push_back(n1);
    if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-2].node));
    if((yyvsp[-1].node)) temp.push_back((yyvsp[-1].node));
    temp.push_back((yyvsp[0].node));
    struct Node* n = new struct Node("ArrayCreationExpression", temp);
    (yyval.node) = (yyvsp[0].node);
    (yyval.node)->width1 = (yyvsp[-1].node)->width1;
    (yyval.node)->width2 = (yyvsp[-1].node)->width2;
    (yyval.node)->width3 = (yyvsp[-1].node)->width3;
    (yyval.node)->arrayType = (yyvsp[-1].node)->arrayType;
    (yyval.node)->type = (yyval.node)->arrayType*100 + (yyvsp[-2].node)->type;
    (yyval.node)->varName = string("new ") + (yyvsp[-2].node)->attr ;
    verbose(v,"NEW PrimitiveType Dims ArrayInitializer->ArrayCreationExpression");
}
#line 10957 "y.tab.c"
    break;

  case 331: /* DimExpr_: LRSQUAREBRACKET  */
#line 6616 "parser.yacc"
                   {
    (yyval.node) = NULL;
    verbose(v,"LRSQUAREBRACKET->DimExpr_");
}
#line 10966 "y.tab.c"
    break;

  case 332: /* DimExpr_: LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  */
#line 6620 "parser.yacc"
                                                    {
    vector<struct Node*> temp;

    if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-1].node));
   
    struct Node* n = new struct Node("DimExpr_", temp);
    (yyval.node) = n;
    (yyval.node)->width1 = (yyvsp[-1].node)->varName;
    (yyval.node)->arrayType = 1;
    //$$->width1 = stoi($2->attr);
    cout << "\n\nvarname ="<< (yyvsp[-1].node)->varName << "\n\n";
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->DimExpr_");
}
#line 10989 "y.tab.c"
    break;

  case 333: /* DimExpr_: LRSQUAREBRACKET LRSQUAREBRACKET  */
#line 6638 "parser.yacc"
                                      {
        (yyval.node) = NULL;
        verbose(v,"LRSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
    }
#line 10998 "y.tab.c"
    break;

  case 334: /* DimExpr_: LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET  */
#line 6642 "parser.yacc"
                                                                       {
    vector<struct Node*> temp;
    
    if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-2].node));
   
    struct Node* n = new struct Node("DimExpr_", temp);
    (yyval.node) = n;
    (yyval.node)->arrayType = 2;
    (yyval.node)->width1 = (yyvsp[-2].node)->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
#line 11019 "y.tab.c"
    break;

  case 335: /* DimExpr_: LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  */
#line 6658 "parser.yacc"
                                                                                                       {
    vector<struct Node*> temp;
    
    if((yyvsp[-4].node)->useful == false) {
            for(auto it: (yyvsp[-4].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-4].node));
    if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
      else temp.push_back((yyvsp[-1].node));  
    struct Node* n = new struct Node("DimExpr_", temp);
    (yyval.node) = n;
    (yyval.node)->arrayType = 2;
    (yyval.node)->width1 = (yyvsp[-4].node)->varName;
    (yyval.node)->width2 = (yyvsp[-1].node)->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression->DimExpr_");
}
#line 11046 "y.tab.c"
    break;

  case 336: /* DimExpr_: LRSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET  */
#line 6680 "parser.yacc"
                                                       {
    (yyval.node) = NULL;
    verbose(v,"LRSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
#line 11055 "y.tab.c"
    break;

  case 337: /* DimExpr_: LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET  */
#line 6684 "parser.yacc"
                                                                                    {
    vector<struct Node*> temp;
   
    if((yyvsp[-3].node)->useful == false) {
            for(auto it: (yyvsp[-3].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-3].node));
    
   
    struct Node* n = new struct Node("DimExpr_", temp);
    (yyval.node) = n;
   (yyval.node)->arrayType = 3;
   
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
#line 11077 "y.tab.c"
    break;

  case 338: /* DimExpr_: LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET  */
#line 6701 "parser.yacc"
                                                                                                                     {
    vector<struct Node*> temp;
    
    if((yyvsp[-5].node)->useful == false) {
            for(auto it: (yyvsp[-5].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-5].node));
    
    if((yyvsp[-2].node)->useful == false) {
            for(auto it: (yyvsp[-2].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-2].node));
    
    struct Node* n = new struct Node("DimExpr_", temp);
    (yyval.node) = n;
    (yyval.node)->arrayType = 3;
    (yyval.node)->width1 = (yyvsp[-5].node)->varName;
    (yyval.node)->width2 = (yyvsp[-2].node)->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
#line 11106 "y.tab.c"
    break;

  case 339: /* DimExpr_: LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  */
#line 6725 "parser.yacc"
                                                                                                                                                      {
    vector<struct Node*> temp;
   
    if((yyvsp[-7].node)->useful == false) {
            for(auto it: (yyvsp[-7].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-7].node));
   
   
    if((yyvsp[-4].node)->useful == false) {
            for(auto it: (yyvsp[-4].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-4].node));
   
    if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back((yyvsp[-1].node));
   
    struct Node* n = new struct Node("DimExpr_", temp);
    (yyval.node) = n;
    (yyval.node)->arrayType = 3;
    (yyval.node)->width1 = (yyvsp[-7].node)->varName;
    (yyval.node)->width2 = (yyvsp[-4].node)->varName;
    (yyval.node)->width3 = (yyvsp[-1].node)->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->DimExpr_");
}
#line 11144 "y.tab.c"
    break;

  case 340: /* Dims: LRSQUAREBRACKET  */
#line 6760 "parser.yacc"
                       {
       
        (yyval.node) = new Node("Dims");
        (yyval.node)->arrayType += 1;
    
        verbose(v,"LRSQUAREBRACKET->Dims");
    }
#line 11156 "y.tab.c"
    break;

  case 341: /* Dims: LRSQUAREBRACKET Dims  */
#line 6767 "parser.yacc"
                           {
        
        
        (yyval.node) = new Node("Dims");
        (yyval.node)->arrayType = (yyvsp[0].node)->arrayType+ 1;
        verbose(v,"LRSQUAREBRACKET Dims->Dims");
    }
#line 11168 "y.tab.c"
    break;

  case 342: /* Expression: AssignmentExpression  */
#line 6777 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"AssignmentExpression->Expression");
    }
#line 11177 "y.tab.c"
    break;

  case 343: /* CommaExpressions: %empty  */
#line 6784 "parser.yacc"
    {(yyval.node) = NULL;}
#line 11183 "y.tab.c"
    break;

  case 344: /* CommaExpressions: COMMA Expression CommaExpressions  */
#line 6785 "parser.yacc"
                                        {
        vector<struct Node*> temp;
            if((yyvsp[-1].node)->useful == false) {
                for(auto it: (yyvsp[-1].node)->children) {
                    temp.push_back(it);
                }
            }
            else temp.push_back((yyvsp[-1].node));
            if((yyvsp[0].node)) {
                for(auto it: temp) {
                    (yyvsp[0].node)->addChildToLeft(it);
                }
                (yyval.node) = (yyvsp[0].node);
            }
            else (yyval.node) = new Node("Expressions", temp);
            verbose(v,"COMMA Expression CommaExpressions->CommaExpressions");
}
#line 11205 "y.tab.c"
    break;

  case 345: /* AssignmentExpression: ConditionalExpression  */
#line 6806 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ConditionalExpression->AssignmentExpression");
    }
#line 11214 "y.tab.c"
    break;

  case 346: /* AssignmentExpression: Assignment  */
#line 6810 "parser.yacc"
                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"Assignment->AssignmentExpression");
    }
#line 11223 "y.tab.c"
    break;

  case 347: /* Assignment: LeftHandSide AssignmentOperatorEqual Expression  */
#line 6817 "parser.yacc"
                                                    {
        vector<Node*> temp;
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        //$2 = new Node("Operator", $2);
        for(auto it: temp) {
            (yyvsp[-1].node)->addChildToLeft(it);
        }
        
        temp.clear();
        //temp.push_back(t2);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            (yyvsp[-1].node)->addChild(it);
        }
        //struct Node* n = new struct Node("ExclusiveOrExpression", temp);
        (yyval.node) = (yyvsp[-1].node);
        Quadruple * q = new Quadruple(string("="), (yyvsp[0].node)->varName, (yyvsp[-2].node)->varName);
        if(isCond == 0) (yyval.node)->code.push_back(q);
        if(isCond == 0)ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
        verbose(v,"LeftHandSide AssignmentOperator Expression->Assignment");
        cout << (yyvsp[-2].node)->type << " " << (yyvsp[0].node)->type << endl;

        int lhstype=(yyvsp[-2].node)->type, rhstype=(yyvsp[0].node)->type;

        if((yyvsp[-2].node)->type==(yyvsp[0].node)->type || (typeroot->categorize(lhstype)==FLOATING_TYPE && typeroot->categorize(rhstype)==INTEGER_TYPE) || (lhstype==DOUBLE_NUM && rhstype==FLOAT_NUM))
        {
            (yyval.node)->type = (yyvsp[-2].node)->type;
            (yyval.node)->last = ircode.size() - 1;
            cout << (yyvsp[0].node)->label << "\n";
            if((yyvsp[0].node)->label == string("ArrayCreationExpression")) {
                cout << "\n\nhere\n\n";
                cout << (yyvsp[0].node)->width1 <<"\n";
                Symbol* sym = root->currNode->scope_lookup((yyvsp[-2].node)->varName);
                if(sym) {
                    sym->width1 = (yyvsp[0].node)->width1;
                    sym->width2 = (yyvsp[0].node)->width2;
                    sym->width3 = (yyvsp[0].node)->width3;
                }
            }
        }
        else
        {
            cout<<"Error! Type Mismatch : Cannot convert from "<<typeroot->inv_types[(yyvsp[0].node)->type]<<" to "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<endl;
            yyerror("Error");
        }
        isCond = 0;
        (yyval.node)->nextlist = (yyvsp[0].node)->nextlist;
    }
#line 11289 "y.tab.c"
    break;

  case 348: /* Assignment: LeftHandSide AssignmentOperator Expression  */
#line 6878 "parser.yacc"
                                                {
        vector<Node*> temp;
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            (yyvsp[-1].node)->addChildToLeft(it);
        }
        
        temp.clear();
        //temp.push_back(t2);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            (yyvsp[-1].node)->addChild(it);
        }
        //struct Node* n = new struct Node("ExclusiveOrExpression", temp);
        (yyval.node) = (yyvsp[-1].node);
        processAssignment((yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node), (yyval.node));
        (yyval.node)->last = ircode.size() - 1;
        verbose(v,"LeftHandSide AssignmentOperator Expression->Assignment");
        cout << (yyvsp[-2].node)->type << " " << (yyvsp[0].node)->type << endl;

        int lhstype=(yyvsp[-2].node)->type, rhstype=(yyvsp[0].node)->type;

        if((yyvsp[-2].node)->type==(yyvsp[0].node)->type || (typeroot->categorize(lhstype)==FLOATING_TYPE && typeroot->categorize(rhstype)==INTEGER_TYPE) || (lhstype==DOUBLE_NUM && rhstype==FLOAT_NUM))
        {
            (yyval.node)->type = (yyvsp[-2].node)->type;
            (yyval.node)->last = ircode.size() - 1;
            cout << (yyvsp[0].node)->label << "\n";
            if((yyvsp[0].node)->label == string("ArrayCreationExpression")) {
                cout << "\n\nhere\n\n";
                cout << (yyvsp[0].node)->width1 <<"\n";
                Symbol* sym = root->currNode->scope_lookup((yyvsp[-2].node)->varName);
                if(sym) {
                    sym->width1 = (yyvsp[0].node)->width1;
                    sym->width2 = (yyvsp[0].node)->width2;
                    sym->width3 = (yyvsp[0].node)->width3;
                }
            }
        }
        else
        {
            cout<<"Error! Type Mismatch : Cannot convert from "<<typeroot->inv_types[(yyvsp[0].node)->type]<<" to "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11350 "y.tab.c"
    break;

  case 349: /* LeftHandSide: Name  */
#line 6937 "parser.yacc"
            {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"Name->LeftHandSide");
    }
#line 11359 "y.tab.c"
    break;

  case 350: /* LeftHandSide: FieldAccess  */
#line 6941 "parser.yacc"
                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"FieldAccess->LeftHandSide");
    }
#line 11368 "y.tab.c"
    break;

  case 351: /* LeftHandSide: ArrayAccess  */
#line 6945 "parser.yacc"
                {
       
       (yyval.node) = (yyvsp[0].node);
       
       Quadruple* q = ircode.back();
       (yyval.node)->code.pop_back();
       //q = new Quadruple("", "*(" + $$->attr + "+" + $$->varName + ")", "", "")
       ircode.pop_back();
       varCnt-= 2;
       (yyval.node)->last -= 1;
       (yyval.node)->varName = string("t") + to_string(varCnt++);
       (yyval.node)->varName = "*(" + (yyval.node)->attr + "+" + (yyval.node)->varName + ")";
       verbose(v,"ArrayAccess->LeftHandSide");
    }
#line 11387 "y.tab.c"
    break;

  case 352: /* AssignmentOperatorEqual: ASSIGN  */
#line 6962 "parser.yacc"
           {
        struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
        (yyval.node) = n;
        verbose(v,"MULTEQUAL->AssignmentOperator");
    }
#line 11397 "y.tab.c"
    break;

  case 353: /* AssignmentOperator: MULTEQUAL  */
#line 6970 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"MULTEQUAL->AssignmentOperator");
}
#line 11407 "y.tab.c"
    break;

  case 354: /* AssignmentOperator: DIVEQUAL  */
#line 6975 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"DIVEQUAL->AssignmentOperator");
}
#line 11417 "y.tab.c"
    break;

  case 355: /* AssignmentOperator: MODEQUAL  */
#line 6980 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"MODEQUAL->AssignmentOperator");
}
#line 11427 "y.tab.c"
    break;

  case 356: /* AssignmentOperator: PLUSEQUAL  */
#line 6985 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"PLUSEQUAL->AssignmentOperator");
}
#line 11437 "y.tab.c"
    break;

  case 357: /* AssignmentOperator: MINUSEQUAL  */
#line 6990 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"MINUSEQUAL->AssignmentOperator");
}
#line 11447 "y.tab.c"
    break;

  case 358: /* AssignmentOperator: LSHIFTEQUAL  */
#line 6995 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"LSHIFTEQUAL->AssignmentOperator");
}
#line 11457 "y.tab.c"
    break;

  case 359: /* AssignmentOperator: RSHIFTEQUAL  */
#line 7000 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"RSHIFTEQUAL->AssignmentOperator");
}
#line 11467 "y.tab.c"
    break;

  case 360: /* AssignmentOperator: UNRSHIFTEQUAL  */
#line 7005 "parser.yacc"
                    {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"UNRSHIFTEQUAL->AssignmentOperator");
}
#line 11477 "y.tab.c"
    break;

  case 361: /* AssignmentOperator: BANDEQUAL  */
#line 7010 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"BANDEQUAL->AssignmentOperator");
}
#line 11487 "y.tab.c"
    break;

  case 362: /* AssignmentOperator: BOREQUAL  */
#line 7015 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"BOREQUAL->AssignmentOperator");
}
#line 11497 "y.tab.c"
    break;

  case 363: /* AssignmentOperator: BXOREQUAL  */
#line 7020 "parser.yacc"
                {
    struct Node* n = new struct Node("Operator", (yyvsp[0].lexeme));
    (yyval.node) = n;
    verbose(v,"BXOREQUAL->AssignmentOperator");
}
#line 11507 "y.tab.c"
    break;

  case 364: /* ColConditional: Expression COL  */
#line 7029 "parser.yacc"
    {
        //$$ = new Node("ColConditional", )
        for(int i = 0; i < (yyvsp[-1].node)->code.size(); i++) {
            ircode.pop_back();
        }
        Quadruple* q = new Quadruple("", (yyvsp[-1].node)->varName, "", condvar);
        (yyvsp[-1].node)->code.push_back(q);
        
        ircode.insert(ircode.end(), (yyvsp[-1].node)->code.begin(), (yyvsp[-1].node)->code.end());
        processPostIncre((yyvsp[-1].node));
        (yyvsp[-1].node)->last = ircode.size() - 1;
        (yyval.node) = (yyvsp[-1].node);
        (yyval.node)->nextlist = (yyvsp[-1].node)->nextlist;
        (yyval.node)->nextlist.push_back(ircode.size());
        //cout << ircode.size()<<"over here\n";
        q = new Quadruple(3, "", "","", "");
        ircode.push_back(q);
        (yyval.node)->code.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
        

    }
#line 11534 "y.tab.c"
    break;

  case 365: /* ConditionalExpression: ConditionalOrExpression  */
#line 7054 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ConditionalOrExpression->ConditionalExpression");
    }
#line 11543 "y.tab.c"
    break;

  case 366: /* ConditionalExpression: ConditionalOrExpression QUES ColConditional ConditionalExpression  */
#line 7058 "parser.yacc"
                                                                       {
        vector<struct Node*> temp;
        
        struct Node* t1 = new struct Node("Operator", (yyvsp[-2].lexeme));
        //struct Node* t2 = new struct Node("Operator", $4);
        
        if((yyvsp[-3].node)->useful == false) {
            for(auto it : (yyvsp[-3].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-3].node));
        temp.push_back(t1);
        if((yyvsp[-1].node)->useful == false) {
            for(auto it : (yyvsp[-1].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
       // temp.push_back(t2);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("ConditionalExpression", temp);
        (yyval.node) = n;
        
        for(int i = 0; i < (yyvsp[0].node)->code.size(); i++) {
            ircode.pop_back();
        }
        Quadruple* q = new Quadruple("", (yyvsp[0].node)->varName, "", condvar);
        q->print();
        (yyvsp[0].node)->code.push_back(q);
        ircode.insert(ircode.end(), (yyvsp[0].node)->code.begin(), (yyvsp[0].node)->code.end());
        int lastpos = (yyvsp[-3].node)->last + 1;
        backpatch((yyvsp[-3].node)->truelist, lastpos);
        backpatch((yyvsp[-3].node)->falselist, (yyvsp[-1].node)->last + 1);
        //cout << "Lastpos = " << max(lastpos, ($5->last) + 1)<< "\n";
        (yyval.node)->nextlist = (yyvsp[-1].node)->nextlist;
        (yyval.node)->nextlist.insert((yyval.node)->nextlist.end(), (yyvsp[0].node)->nextlist.begin(), (yyvsp[0].node)->nextlist.end());
        (yyval.node)->last= ircode.size() - 1;
        verbose(v,"ConditionalOrExpression QUES Expression COL ConditionalExpression->ConditionalExpression");
        (yyval.node)->type = (yyvsp[0].node)->type;
        isCond =1;
        /*if($1->type==BOOL_NUM)
        {
            $$->type = VOID_TYPE;
        }
        else
        {
            yyerror("Type Mismatch");
        }*/
    }
#line 11606 "y.tab.c"
    break;

  case 367: /* ConditionalOrExpression: ConditionalAndExpression  */
#line 7119 "parser.yacc"
                                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ConditionalAndExpression->ConditionalOrExpression");
    }
#line 11615 "y.tab.c"
    break;

  case 368: /* ConditionalOrExpression: ConditionalOrExpression OR ConditionalAndExpression  */
#line 7123 "parser.yacc"
                                                        {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        backpatch((yyvsp[-2].node)->falselist, (yyvsp[-2].node)->last + 1);
        (yyval.node)->truelist = (yyvsp[-2].node)->truelist;
        (yyval.node)->truelist.insert((yyval.node)->truelist.end(), (yyvsp[0].node)->truelist.begin(), (yyvsp[0].node)->truelist.end());
        (yyval.node)->falselist = (yyvsp[0].node)->falselist;
        cout << "Lastpos = " << (yyval.node)->last<< "\n";
        verbose(v,"ConditionalOrExpression OR ConditionalAndExpression->ConditionalOrExpression");
        //ircode.push_back($1->code);
        
        if((yyvsp[-2].node)->type==BOOL_NUM && (yyvsp[0].node)->type==BOOL_NUM)
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11663 "y.tab.c"
    break;

  case 369: /* ConditionalAndExpression: InclusiveOrExpression  */
#line 7169 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"InclusiveOrExpression->ConditionalAndExpression");
    }
#line 11672 "y.tab.c"
    break;

  case 370: /* ConditionalAndExpression: ConditionalAndExpression AND InclusiveOrExpression  */
#line 7173 "parser.yacc"
                                                        {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        backpatch((yyvsp[-2].node)->truelist, (yyvsp[-2].node)->last + 1);
        cout << (yyvsp[-2].node)->label << "\n";
        (yyval.node)->truelist = (yyvsp[0].node)->truelist;
        (yyval.node)->falselist = (yyvsp[-2].node)->falselist;
        (yyval.node)->falselist.insert((yyval.node)->falselist.end(), (yyvsp[0].node)->falselist.begin(), (yyvsp[0].node)->falselist.end());
        verbose(v,"ConditionalAndExpression AND InclusiveOrExpression->ConditionalAndExpression");
        
        if((yyvsp[-2].node)->type==BOOL_NUM && (yyvsp[0].node)->type==BOOL_NUM)
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11719 "y.tab.c"
    break;

  case 371: /* InclusiveOrExpression: ExclusiveOrExpression  */
#line 7218 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ExclusiveOrExpression->InclusiveOrExpression");
    }
#line 11728 "y.tab.c"
    break;

  case 372: /* InclusiveOrExpression: InclusiveOrExpression BOR ExclusiveOrExpression  */
#line 7222 "parser.yacc"
                                                    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes,(yyvsp[-1].lexeme));
        verbose(v,"InclusiveOrExpression BOR ExclusiveOrExpression->InclusiveOrExpression");

        if((yyvsp[-2].node)->type==INT_NUM && (yyvsp[0].node)->type==INT_NUM)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11772 "y.tab.c"
    break;

  case 373: /* ExclusiveOrExpression: AndExpression  */
#line 7264 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"AndExpression->ExclusiveOrExpression");
    }
#line 11781 "y.tab.c"
    break;

  case 374: /* ExclusiveOrExpression: ExclusiveOrExpression BXOR AndExpression  */
#line 7268 "parser.yacc"
                                                {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        verbose(v,"ExclusiveOrExpression BXOR AndExpression->ExclusiveOrExpression");

        if((yyvsp[-2].node)->type==INT_NUM && (yyvsp[0].node)->type==INT_NUM)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11825 "y.tab.c"
    break;

  case 375: /* AndExpression: EqualityExpression  */
#line 7310 "parser.yacc"
                        {
       (yyval.node) = (yyvsp[0].node);
       verbose(v,"EqualityExpression->AndExpression");
    }
#line 11834 "y.tab.c"
    break;

  case 376: /* AndExpression: AndExpression BAND EqualityExpression  */
#line 7314 "parser.yacc"
                                            {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        verbose(v,"AndExpression BAND EqualityExpression->AndExpression");

        if((yyvsp[-2].node)->type==INT_NUM && (yyvsp[0].node)->type==INT_NUM)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11878 "y.tab.c"
    break;

  case 377: /* EqualityExpression: RelationalExpression  */
#line 7356 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"RelationalExpression->EqualityExpression");
    }
#line 11887 "y.tab.c"
    break;

  case 378: /* EqualityExpression: EqualityExpression EQUAL RelationalExpression  */
#line 7360 "parser.yacc"
                                                    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*>nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processRelational(nodes, (yyvsp[-1].lexeme));
        verbose(v,"EqualityExpression EQUAL RelationalExpression->EqualityExpression");
        if((typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type)==FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type)==FLOATING_TYPE))
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11930 "y.tab.c"
    break;

  case 379: /* EqualityExpression: EqualityExpression NEQUAL RelationalExpression  */
#line 7398 "parser.yacc"
                                                     {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*>nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processRelational(nodes, (yyvsp[-1].lexeme));
        verbose(v,"EqualityExpression NEQUAL RelationalExpression->EqualityExpression");
        
        if((typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type)==FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type)==FLOATING_TYPE))
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 11974 "y.tab.c"
    break;

  case 380: /* RelationalExpression: ShiftExpression  */
#line 7440 "parser.yacc"
                    {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"ShiftExpression->RelationalExpression");
    }
#line 11983 "y.tab.c"
    break;

  case 381: /* RelationalExpression: RelationalExpression LSS ShiftExpression  */
#line 7444 "parser.yacc"
                                                 {
         vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*>nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processRelational(nodes, (yyvsp[-1].lexeme));
        verbose(v,"RelationalExpression LSS ShiftExpression->RelationalExpression");

        if((typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type)==FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type)==FLOATING_TYPE))
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12027 "y.tab.c"
    break;

  case 382: /* RelationalExpression: RelationalExpression GRT ShiftExpression  */
#line 7483 "parser.yacc"
                                                 {
         vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;

        vector<Node*>nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processRelational(nodes, (yyvsp[-1].lexeme));
        if((typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type)==FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type)==FLOATING_TYPE))
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12070 "y.tab.c"
    break;

  case 383: /* RelationalExpression: RelationalExpression LEQ ShiftExpression  */
#line 7521 "parser.yacc"
                                                {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*>nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processRelational(nodes, (yyvsp[-1].lexeme));
        verbose(v,"RelationalExpression LEQ ShiftExpression->RelationalExpression");

        if((typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type)==FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type)==FLOATING_TYPE))
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12114 "y.tab.c"
    break;

  case 384: /* RelationalExpression: RelationalExpression GEQ ShiftExpression  */
#line 7560 "parser.yacc"
                                                {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*>nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processRelational(nodes, (yyvsp[-1].lexeme));
        verbose(v,"RelationalExpression GEQ ShiftExpression->RelationalExpression");


        if((typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type)==FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type)==FLOATING_TYPE))
            (yyval.node)->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12159 "y.tab.c"
    break;

  case 385: /* RelationalExpression: InstanceofExpression  */
#line 7600 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"InstanceofExpression->RelationalExpression");
    }
#line 12168 "y.tab.c"
    break;

  case 386: /* InstanceofExpression: RelationalExpression INSTANCEOF ReferenceType  */
#line 7607 "parser.yacc"
                                                    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Keyword", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        temp.push_back(t);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("InstanceofExpression", temp);
        (yyval.node) = n;
        verbose(v,"RelationalExpression INSTANCEOF ReferenceType->InstanceofExpression");
    }
#line 12196 "y.tab.c"
    break;

  case 387: /* ShiftExpression: AdditiveExpression  */
#line 7633 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"AdditiveExpression->ShiftExpression");
    }
#line 12205 "y.tab.c"
    break;

  case 388: /* ShiftExpression: ShiftExpression LSHIFT AdditiveExpression  */
#line 7637 "parser.yacc"
                                                {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        verbose(v,"ShiftExpression LSHIFT AdditiveExpression->ShiftExpression");

        if(typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE && typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12249 "y.tab.c"
    break;

  case 389: /* ShiftExpression: ShiftExpression RSHIFT AdditiveExpression  */
#line 7676 "parser.yacc"
                                                {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        verbose(v,"ShiftExpression RSHIFT AdditiveExpression->ShiftExpression");

        if(typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE && typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12293 "y.tab.c"
    break;

  case 390: /* ShiftExpression: ShiftExpression UNRSHIFT AdditiveExpression  */
#line 7715 "parser.yacc"
                                                {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        verbose(v,"ShiftExpression UNRSHIFT AdditiveExpression->ShiftExpression");

        if(typeroot->categorize((yyvsp[-2].node)->type)==INTEGER_TYPE && typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12338 "y.tab.c"
    break;

  case 391: /* AdditiveExpression: MultiplicativeExpression  */
#line 7758 "parser.yacc"
                                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"MultiplicativeExpression->AdditiveExpression");
    }
#line 12347 "y.tab.c"
    break;

  case 392: /* AdditiveExpression: AdditiveExpression PLUS MultiplicativeExpression  */
#line 7762 "parser.yacc"
                                                        {
      vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        processPostIncre((yyval.node));
        verbose(v,"AdditiveExpression PLUS MultiplicativeExpression->AdditiveExpression");

        if((typeroot->categorize((yyvsp[-2].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type) == FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = typeroot->maxtype((yyvsp[0].node)->type, (yyvsp[0].node)->type);
        else if((yyvsp[-2].node)->type==STRING_NUM || (yyvsp[0].node)->type==STRING_NUM)
        {
            (yyval.node)->type = STRING_NUM;
        }
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12396 "y.tab.c"
    break;

  case 393: /* AdditiveExpression: AdditiveExpression MINUS MultiplicativeExpression  */
#line 7806 "parser.yacc"
                                                        {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        
        (yyval.node) = t;
        
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        processPostIncre((yyval.node));
        verbose(v,"AdditiveExpression MINUS MultiplicativeExpression->AdditiveExpression");

        if((typeroot->categorize((yyvsp[-2].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type) == FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = typeroot->maxtype((yyvsp[0].node)->type, (yyvsp[0].node)->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12442 "y.tab.c"
    break;

  case 394: /* MultiplicativeExpression: UnaryExpression  */
#line 7850 "parser.yacc"
                    {
        (yyval.node) =(yyvsp[0].node);
        verbose(v,"UnaryExpression->MultiplicativeExpression");
    }
#line 12451 "y.tab.c"
    break;

  case 395: /* MultiplicativeExpression: MultiplicativeExpression MULT UnaryExpression  */
#line 7854 "parser.yacc"
                                                    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes,(yyvsp[-1].lexeme));
        processPostIncre((yyval.node));
        verbose(v,"MultiplicativeExpression MULT UnaryExpression->MultiplicativeExpression");

        if((typeroot->categorize((yyvsp[-2].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type) == FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = typeroot->maxtype((yyvsp[0].node)->type, (yyvsp[0].node)->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12496 "y.tab.c"
    break;

  case 396: /* MultiplicativeExpression: MultiplicativeExpression DIV UnaryExpression  */
#line 7894 "parser.yacc"
                                                   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node),(yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        processPostIncre((yyval.node));
        verbose(v,"MultiplicativeExpression DIV UnaryExpression->MultiplicativeExpression");

        if((typeroot->categorize((yyvsp[-2].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type) == FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = typeroot->maxtype((yyvsp[0].node)->type, (yyvsp[0].node)->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12540 "y.tab.c"
    break;

  case 397: /* MultiplicativeExpression: MultiplicativeExpression MOD UnaryExpression  */
#line 7933 "parser.yacc"
                                                    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        
        if((yyvsp[-2].node)->useful == false) {
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        for(auto it: temp) {
            t->addChild(it);
        }
        (yyval.node) = t;
        vector<Node*> nodes = {(yyval.node), (yyvsp[-2].node), (yyvsp[0].node)};
        processArithmetic(nodes, (yyvsp[-1].lexeme));
        processPostIncre((yyval.node));
        verbose(v,"MultiplicativeExpression MOD UnaryExpression->MultiplicativeExpression");

        if((typeroot->categorize((yyvsp[-2].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->type) == FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = typeroot->maxtype((yyvsp[0].node)->type, (yyvsp[0].node)->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-2].node)->type]<<" and "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12584 "y.tab.c"
    break;

  case 398: /* UnaryExpression: PreIncrementExpression  */
#line 7975 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"PreIncrementExpression->UnaryExpression");
    }
#line 12593 "y.tab.c"
    break;

  case 399: /* UnaryExpression: PreDecrementExpression  */
#line 7979 "parser.yacc"
                            {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"PreDecrementExpression->UnaryExpression");
    }
#line 12602 "y.tab.c"
    break;

  case 400: /* UnaryExpression: PLUS UnaryExpression  */
#line 7983 "parser.yacc"
                            {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        temp.push_back(t);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("UnaryExpression", temp);
        (yyval.node) = n;
        (yyval.node)->varName= (yyval.node)->attr = string((yyvsp[-1].lexeme)) + (yyvsp[0].node)->varName;
        verbose(v,"PLUS UnaryExpression->UnaryExpression");

        if((typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = (yyvsp[0].node)->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12631 "y.tab.c"
    break;

  case 401: /* UnaryExpression: MINUS UnaryExpression  */
#line 8007 "parser.yacc"
                            {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        temp.push_back(t);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("UnaryExpression", temp);
        (yyval.node) = n;
        (yyval.node)->varName= (yyval.node)->attr = string((yyvsp[-1].lexeme)) + (yyvsp[0].node)->varName;
        verbose(v,"MINUS UnaryExpression->UnaryExpression");

        if((typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = (yyvsp[0].node)->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12660 "y.tab.c"
    break;

  case 402: /* UnaryExpression: UnaryExpressionNotPlusMinus  */
#line 8031 "parser.yacc"
                                {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"UnaryExpressionNotPlusMinus->UnaryExpression");
    }
#line 12669 "y.tab.c"
    break;

  case 403: /* PreIncrementExpression: INCRE UnaryExpression  */
#line 8037 "parser.yacc"
                            {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        temp.push_back(t);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("PreIncrementExpression", temp);
        (yyval.node) = n;
        Quadruple* q = new Quadruple(string("+int"), (yyvsp[0].node)->varName, string("1"), (yyvsp[0].node)->varName);
        (yyval.node)->code.push_back(q);
        
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
        (yyval.node)->varName= (yyvsp[0].node)->varName;
        //$$->type = $2->type;
        verbose(v,"INCRE UnaryExpression->PreIncrementExpression");

        if((typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = (yyvsp[0].node)->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12704 "y.tab.c"
    break;

  case 404: /* PreDecrementExpression: DECRE UnaryExpression  */
#line 8070 "parser.yacc"
                            {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        temp.push_back(t);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("PreDecrementExpression", temp);
        (yyval.node) = n;
        Quadruple* q = new Quadruple(string("-int"), (yyvsp[0].node)->varName, string("1"), (yyvsp[0].node)->varName);
        (yyval.node)->code.push_back(q);
        
        ircode.push_back(q);
        (yyval.node)->last = ircode.size() - 1;
        (yyval.node)->varName= (yyvsp[0].node)->varName;
        verbose(v,"DECRE UnaryExpression->PreDecrementExpression");

        if((typeroot->categorize((yyvsp[0].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = (yyvsp[0].node)->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12738 "y.tab.c"
    break;

  case 405: /* UnaryExpressionNotPlusMinus: PostfixExpression  */
#line 8102 "parser.yacc"
                        {
        (yyval.node) = (yyvsp[0].node);
        verbose(v,"PostfixExpression->UnaryExpressionNotPlusMinus");
    }
#line 12747 "y.tab.c"
    break;

  case 406: /* UnaryExpressionNotPlusMinus: TIL UnaryExpression  */
#line 8106 "parser.yacc"
                        {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        temp.push_back(t);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("UnaryExpressionNotPlusMinus", temp);
        (yyval.node) = n;
        (yyval.node)->varName= (yyval.node)->attr = string((yyvsp[-1].lexeme)) + (yyvsp[0].node)->varName;
        verbose(v,"TIL UnaryExpression->UnaryExpressionNotPlusMinus");

        if(typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12776 "y.tab.c"
    break;

  case 407: /* UnaryExpressionNotPlusMinus: NOT UnaryExpression  */
#line 8130 "parser.yacc"
                        {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", (yyvsp[-1].lexeme));
        temp.push_back(t);
        if((yyvsp[0].node)->useful == false) {
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));
        struct Node* n = new struct Node("UnaryExpressionNotPlusMinus", temp);
        (yyval.node) = n;
        (yyval.node)->varName= (yyval.node)->attr = string((yyvsp[-1].lexeme)) + (yyvsp[0].node)->varName;
        verbose(v,"NOT UnaryExpression->UnaryExpressionNotPlusMinus");
        (yyval.node)->truelist = (yyvsp[0].node)->falselist;
        (yyval.node)->falselist = (yyvsp[0].node)->truelist;
        if(typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE)
            (yyval.node)->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[-1].lexeme)<<": "<<typeroot->inv_types[(yyvsp[0].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12806 "y.tab.c"
    break;

  case 408: /* UnaryExpressionNotPlusMinus: CastExpression  */
#line 8155 "parser.yacc"
                    {
    (yyval.node) = (yyvsp[0].node);
    verbose(v, "CastExpression->UnaryExpressionNotPlusMinus");
}
#line 12815 "y.tab.c"
    break;

  case 409: /* CastExpression: LEFTPARENTHESIS PrimitiveType RIGHTPARENTHESIS UnaryExpression  */
#line 8162 "parser.yacc"
                                                                    {
        vector<struct Node*> temp;
      
        if((yyvsp[-2].node)->useful == false) {
            cout<<"not 2 useful"<<endl;
            for(auto it : (yyvsp[-2].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-2].node));

        if((yyvsp[0].node)->useful == false) {
            cout<<"not 4 useful"<<endl;
            for(auto it : (yyvsp[0].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[0].node));

        /*
        string resName = string("t") + to_string(varCnt);
        varCnt++;
        Quadruple q(string("cast_to_") + string($2), $4->varName, resName );
        ir.push_back(q);
        $$->varName = resName;
        $$->reduced = true;
        */

        struct Node* n = new struct Node("CastExpression", temp);
        (yyval.node) = n;

        verbose(v,"LEFTPARENTHESIS PrimitiveType RIGHTPARENTHESIS UnaryExpression->CastExpression");

        if((typeroot->categorize((yyvsp[-2].node)->attr)==INTEGER_TYPE || typeroot->categorize((yyvsp[-2].node)->attr)==FLOATING_TYPE) && (typeroot->categorize((yyvsp[0].node)->type)==INTEGER_TYPE || typeroot->categorize((yyvsp[0].node)->type)==FLOATING_TYPE))
        {
            (yyval.node)->type = typeroot->typewidth[(yyvsp[-2].node)->attr].first;
        }
        else
        {
            cout<<"Cannot type cast "<<typeroot->inv_types[(yyvsp[0].node)->type]<<" to type "<<(yyvsp[-2].node)->attr<<endl;
            yyerror("Type Conversion");
        }
    }
#line 12865 "y.tab.c"
    break;

  case 410: /* PostfixExpression: Primary  */
#line 8210 "parser.yacc"
            {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"Primary->PostfixExpression");
}
#line 12874 "y.tab.c"
    break;

  case 411: /* PostfixExpression: Name  */
#line 8214 "parser.yacc"
            {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"Name->PostfixExpression");
}
#line 12883 "y.tab.c"
    break;

  case 412: /* PostfixExpression: PostIncrementExpression  */
#line 8218 "parser.yacc"
                            {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"PostIncrementExpression->PostfixExpression");
}
#line 12892 "y.tab.c"
    break;

  case 413: /* PostfixExpression: PostDecrementExpression  */
#line 8222 "parser.yacc"
                            {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"PostDecrementExpression->PostfixExpression");
}
#line 12901 "y.tab.c"
    break;

  case 414: /* PostIncrementExpression: PostfixExpression INCRE  */
#line 8229 "parser.yacc"
                            {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it : (yyvsp[-1].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        struct Node* t = new Node("Operator", "++");
        temp.push_back(t);
        struct Node* n = new Node("PostIncrementExpression", temp);
        (yyval.node) = n;
        (yyval.node)->varName= (yyvsp[-1].node)->varName;
        Quadruple* q = new Quadruple(string("+int"), (yyvsp[-1].node)->varName, string("1"), (yyvsp[-1].node)->varName);
        //$$->code.push_back(q);
        residualCode.push_back(q);
        //ircode.push_back(q);
        
        verbose(v,"PostfixExpression INCRE->PostIncrementExpression");

        if((typeroot->categorize((yyvsp[-1].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-1].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = (yyvsp[-1].node)->type;
        else
            yyerror("Incompatible Types");

        if((typeroot->categorize((yyvsp[-1].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-1].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = (yyvsp[-1].node)->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[0].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-1].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12940 "y.tab.c"
    break;

  case 415: /* PostDecrementExpression: PostfixExpression DECRE  */
#line 8266 "parser.yacc"
                            {
        
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it : (yyvsp[-1].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        struct Node* t = new Node("Operator", "--");
        temp.push_back(t);
        struct Node* n = new Node("PostDecrementExpression", temp);
        (yyval.node) = n;
        (yyval.node)->varName=(yyvsp[-1].node)->varName;
        Quadruple* q = new Quadruple(string("-int"), (yyvsp[-1].node)->varName, string("1"), (yyvsp[-1].node)->varName);
        //$$->code.push_back(q);
        residualCode.push_back(q);
        verbose(v,"PostfixExpression DECRE->PostDecrementExpression");

        if((typeroot->categorize((yyvsp[-1].node)->type) == INTEGER_TYPE || typeroot->categorize((yyvsp[-1].node)->type) == FLOATING_TYPE))
            (yyval.node)->type = (yyvsp[-1].node)->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<(yyvsp[0].lexeme)<<": "<<typeroot->inv_types[(yyvsp[-1].node)->type]<<endl;
            yyerror("Error");
        }
    }
#line 12973 "y.tab.c"
    break;

  case 416: /* Type: PrimitiveType  */
#line 8300 "parser.yacc"
                    {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"PrimitiveType->Type");
}
#line 12982 "y.tab.c"
    break;

  case 417: /* Type: ReferenceType  */
#line 8304 "parser.yacc"
                    {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"ReferenceType->Type");
}
#line 12991 "y.tab.c"
    break;

  case 418: /* PrimitiveType: NumericType  */
#line 8311 "parser.yacc"
                {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"NumericType->PrimitiveType");
}
#line 13000 "y.tab.c"
    break;

  case 419: /* PrimitiveType: BOOLEAN  */
#line 8315 "parser.yacc"
            {
    struct Node* n = new struct Node("Keyword", "boolean");
    (yyval.node) = n;
    (yyval.node)->type = BOOL_NUM;
    verbose(v,"BOOLEAN->PrimitiveType");
}
#line 13011 "y.tab.c"
    break;

  case 420: /* PrimitiveType: STRING  */
#line 8321 "parser.yacc"
           {
    struct Node* n = new struct Node("Keyword", "String");
    (yyval.node) = n;
    (yyval.node)->type = STRING_NUM;
    verbose(v, "STRING->PrimitiveType");
}
#line 13022 "y.tab.c"
    break;

  case 421: /* ReferenceType: Name  */
#line 8330 "parser.yacc"
            {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"Name->ReferenceType");
}
#line 13031 "y.tab.c"
    break;

  case 422: /* ReferenceType: ArrayType  */
#line 8334 "parser.yacc"
                {
    (yyval.node) = (yyvsp[0].node);
    verbose(v,"ArrayType->ReferenceType");
}
#line 13040 "y.tab.c"
    break;

  case 423: /* ArrayType: PrimitiveType Dims  */
#line 8347 "parser.yacc"
                        {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it : (yyvsp[-1].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            if((yyvsp[0].node)->useful == false) {
                for(auto it : (yyvsp[0].node)->children)
                {
                    temp.push_back(it);
                }
            }
            else temp.push_back((yyvsp[0].node));
        }
        struct Node* n = new Node("ArrayType", temp);
        (yyval.node) = n;
        (yyval.node)->type = (yyvsp[0].node)->arrayType*100 + (yyvsp[-1].node)->type;
        (yyval.node)->arrayType = (yyvsp[0].node)->arrayType;
        verbose(v,"PrimitiveType Dims->ArrayType");
    }
#line 13069 "y.tab.c"
    break;

  case 424: /* ArrayType: Name Dims  */
#line 8371 "parser.yacc"
                {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it : (yyvsp[-1].node)->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            if((yyvsp[0].node)->useful == false) {
                for(auto it : (yyvsp[0].node)->children)
                {
                    temp.push_back(it);
                }
            }
            else temp.push_back((yyvsp[0].node));
        }
        struct Node* n = new Node("ArrayType", temp);
        (yyval.node) = n;
        (yyval.node)->type = (yyvsp[0].node)->arrayType*100 + (yyvsp[-1].node)->type;
        (yyval.node)->arrayType = (yyvsp[0].node)->arrayType;
        verbose(v,"Name Dims->ArrayType");
    }
#line 13098 "y.tab.c"
    break;

  case 425: /* TypeParameter: IDENTIFIER  */
#line 8398 "parser.yacc"
                {
        struct Node* n = new Node("Identifier", (yyvsp[0].lexeme));
        (yyval.node) = n;
        verbose(v,"IDENTIFIER->TypeParameter");
    }
#line 13108 "y.tab.c"
    break;

  case 426: /* TypeParameter: IDENTIFIER ClassExtends  */
#line 8403 "parser.yacc"
                            {
        struct Node* t1 = new Node ("Identifier", (yyvsp[-1].lexeme));
        vector<Node*> t = {t1, (yyvsp[0].node)};
        (yyval.node) = new Node("TypeParameter", t);
        verbose(v,"IDENTIFIER ClassExtends->TypeParameter");
    }
#line 13119 "y.tab.c"
    break;

  case 427: /* CommaTypeParameters: %empty  */
#line 8411 "parser.yacc"
    {(yyval.node) = NULL;}
#line 13125 "y.tab.c"
    break;

  case 428: /* CommaTypeParameters: COMMA TypeParameters CommaTypeParameters  */
#line 8412 "parser.yacc"
                                                {
    vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("TypeParameters", temp);
        verbose(v,"COMMA TypeParameters CommaTypeParameters->CommaTypeParameters");
}
#line 13147 "y.tab.c"
    break;

  case 429: /* Literal: INTEGERLITERAL  */
#line 8449 "parser.yacc"
                    {
        struct Node* n = new Node("Literal", (yyvsp[0].lexeme));
        n->attr = n->varName = (yyvsp[0].lexeme);
        cout << n->varName << "\n";
        (yyval.node) = n;
        verbose(v,"INTEGERLITERAL->Literal");

        (yyval.node)->type = INT_NUM;
    }
#line 13161 "y.tab.c"
    break;

  case 430: /* Literal: FLOATINGPOINTLITERAL  */
#line 8458 "parser.yacc"
                            {
        struct Node* n = new Node("Literal", (yyvsp[0].lexeme));
        n->attr = (yyvsp[0].lexeme);
        (yyval.node) = n;
        verbose(v,"FLOATINGPOINTLITERAL->Literal");

        (yyval.node)->type = FLOAT_NUM;
    }
#line 13174 "y.tab.c"
    break;

  case 431: /* Literal: BOOLEANLITERAL  */
#line 8466 "parser.yacc"
                    {
        struct Node* n = new Node("Literal", (yyvsp[0].lexeme));
        n->attr = (yyvsp[0].lexeme);
        (yyval.node) = n;
        verbose(v,"BOOLEANLITERAL->Literal");

        (yyval.node)->type = BOOL_NUM;
    }
#line 13187 "y.tab.c"
    break;

  case 432: /* Literal: STRINGLITERAL  */
#line 8474 "parser.yacc"
                    {
        struct Node* n = new Node("Literal", (yyvsp[0].lexeme));

        n->attr = (yyvsp[0].lexeme);
        (yyval.node) = n;
        verbose(v,"STRINGLITERAL->Literal");

        (yyval.node)->type = STRING_NUM;
    }
#line 13201 "y.tab.c"
    break;

  case 433: /* Literal: CHARACTERLITERAL  */
#line 8483 "parser.yacc"
                        {
        struct Node* n = new Node("Literal", (yyvsp[0].lexeme));
        n->attr = (yyvsp[0].lexeme);
        (yyval.node) = n;
        verbose(v,"CHARACTERLITERAL->Literal");

        (yyval.node)->type = CHAR_NUM;
    }
#line 13214 "y.tab.c"
    break;

  case 434: /* Literal: NULLLITERAL  */
#line 8491 "parser.yacc"
                {
        struct Node* n = new Node("Literal", (yyvsp[0].lexeme));
        n->attr = (yyvsp[0].lexeme);
        (yyval.node) = n;
        verbose(v,"NULLLITERAL->Literal");

        (yyval.node)->type = NULL_LIT;
    }
#line 13227 "y.tab.c"
    break;

  case 435: /* Literal: TEXTBLOCK  */
#line 8499 "parser.yacc"
                 {
        struct Node* n = new Node("Literal", (yyvsp[0].lexeme));
        n->attr = (yyvsp[0].lexeme);
        n->attr.pop_back();
        n->attr.pop_back();
        n->attr.pop_back();
        n->attr.erase(n->attr.begin());
        n->attr.erase(n->attr.begin());
        n->attr.erase(n->attr.begin());
        (yyval.node) = n;
        verbose(v,"TEXTBLOCKLITERAL->Literal");

        (yyval.node)->type = TEXT_BLOCK;
    }
#line 13246 "y.tab.c"
    break;

  case 436: /* ImportDeclarations: %empty  */
#line 8519 "parser.yacc"
    {(yyval.node) = NULL;}
#line 13252 "y.tab.c"
    break;

  case 437: /* ImportDeclarations: ImportDeclaration ImportDeclarations  */
#line 8520 "parser.yacc"
                                            {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("ImportDeclarations", temp);
    }
#line 13273 "y.tab.c"
    break;

  case 438: /* ModuleDirectives: %empty  */
#line 8539 "parser.yacc"
    {(yyval.node) = NULL;}
#line 13279 "y.tab.c"
    break;

  case 439: /* ModuleDirectives: ModuleDirective ModuleDirectives  */
#line 8540 "parser.yacc"
                                        {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("ModuleDirectives", temp);
    }
#line 13300 "y.tab.c"
    break;

  case 440: /* RequiresModifiers: %empty  */
#line 8559 "parser.yacc"
    {(yyval.node) = NULL;}
#line 13306 "y.tab.c"
    break;

  case 441: /* RequiresModifiers: RequiresModifier RequiresModifiers  */
#line 8560 "parser.yacc"
                                        {
        vector<struct Node*> temp;
        if((yyvsp[-1].node)->useful == false) {
            for(auto it: (yyvsp[-1].node)->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back((yyvsp[-1].node));
        if((yyvsp[0].node)) {
            for(auto it: temp) {
                (yyvsp[0].node)->addChildToLeft(it);
            }
            (yyval.node) = (yyvsp[0].node);
        }
        else (yyval.node) = new Node("RequiresModifiers", temp);
    }
#line 13327 "y.tab.c"
    break;


#line 13331 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 8579 "parser.yacc"



int main(int argc, char* argv[])
{   
    defineCastNames();
    if(argc==2){
        if(argv[1][0]=='-' && argv[1][1]=='-' && argv[1][2]=='h' && argv[1][3]=='e' && argv[1][4]=='l' && argv[1][5]=='p' && argv[1][6]=='\0')
        {
            cout<<"Usage: ./parser --input=<input_filename> --output=<output_filename>\noptions: --help,--verbose\n";
            return 0;
        }
        else{
            cout<<"Invalid argument. Use --help for usage.\n";
            return 0;
        }
    }
    else if(argc==1){
        cout<<"No file specified. Use --help for usage.\n";
        return 0;
    }
    else if(argc==3){
        vector<string>y;
        y.push_back(argv[1]);
        y.push_back(argv[2]);
        sort(y.begin(),y.end());
        if(y[0].rfind("--input",0)==0&&y[1].rfind("--output=",0)==0){
            string temp;
            for(int i=8;i<y[0].size();i++){
                temp=temp+y[0][i];
            }
            char a[temp.length()+1];
            strcpy(a,temp.c_str());
            yyin=fopen(a,"r");
            if(yyin==NULL){
                cout<<"File can't be open\n";
                return 0;
            }
            for(int i=9;i<y[1].size();i++){
                otpt=otpt+y[1][i];
            }
            char b[otpt.length()+1];
            strcpy(b,otpt.c_str());
        }
        else{
            cout<<"Invalid argument. Use --help for usage.\n";
            return 0;
        }
    }
    else if(argc==4){
        vector<string>y;
        y.push_back(argv[1]);
        y.push_back(argv[2]);
        y.push_back(argv[3]);
        sort(y.begin(),y.end());
        if(y[0].rfind("--input=",0)==0&&y[1].rfind("--output=",0)==0&&y[2].rfind("--verbose")==0){
            v=1;
            string temp;
            for(int i=8;i<y[0].size();i++){
                temp=temp+y[0][i];
            }
            char a[temp.length()+1];
            strcpy(a,temp.c_str());
            yyin=fopen(a,"r");
            if(yyin==NULL){
                cout<<"File can't be open\n";
                return 0;
            }
            for(int i=9;i<y[1].size();i++){
                otpt=otpt+y[1][i];
            }
            char b[otpt.length()+1];
            strcpy(b,otpt.c_str());
        }
        else{
            cout<<"Invalid argument. Use --help for usage.\n";
            return 0;
        }
    }
    else{

    }
    /* typeroot->typewidth["string"] = {18, } */

    yyparse();
    return 0;
}

void yyerror(const char* sp)
{
    char str[1000];
    strcpy(str, yytext);
    int strn = strlen(str); 
    if(strn>0)
    /* { */
        /* printf("Error : %s of length %d\n", yytext, strn); */
        // if(yytext[strn-1]=='\0')
        printf("Error due to %s at line number %d.  Aborting...\n", sp, yylineno);
        // printf("Error due to : %s ", sp);
        // errorlineno();
        // printf("Aborting....\n");
        // printf("Error at line number %d. Aborting...\n", yylineno);
    /* } */
    exit(0);
}
