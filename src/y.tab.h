/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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
#line 60 "parser.yacc"

    struct Node* node;
	char* lexeme;

#line 316 "y.tab.h"

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
