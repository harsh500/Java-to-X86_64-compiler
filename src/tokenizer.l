%{
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"
    #include "symbols.h"
    #include "types.h"
    extern SymGlob* root;
    extern TypeHandler* typeroot;
    extern map<string,SymNode*> list_class;
    extern void yyerror(const char*sp);
    extern int importflag;
    extern int isarrayinit;
    extern string currClass;
    int fin=0;
    extern int scope_level;
%}

%x COMMENT1 COMMENT2

%option noyywrap
%option yylineno


JavaDigit       [0-9]

JavaLetter  [a-zA-Z_]
ID          ({JavaLetter}({JavaLetter}|{JavaDigit})*)

LineTerminator  (\r\n)|(\n)|(\r)
WhiteSpace      [ \t\f]

SingleCharacter     [^'\\\n\r]
StringCharacter     [^"\\\n\r]

OctalDigit          [0-7]
ZeroToThree         [0-3]
OctalEscape         \\(({OctalDigit}{OctalDigit}?)|({ZeroToThree}{OctalDigit}{OctalDigit}))
EscapeSequenceCharString      ("\\b")|("\\s")|("\\t")|("\\n")|("\\f")|("\\r")|("\\\"")|("\\\'")|("\\\\")|{OctalEscape}
EscapeSequence              {EscapeSequenceCharString}|"\\"{LineTerminator}



IntegerTypeSuffix   [1L]
BinaryDigit         [0-1]
BinaryDigitOrUnderscore     {BinaryDigit}|"_"
BinaryDigitsAndUnderscores  {BinaryDigitOrUnderscore}{BinaryDigitOrUnderscore}*
BinaryDigits        {BinaryDigit}|({BinaryDigit}{BinaryDigitsAndUnderscores}?{BinaryDigit})
BinaryNumeral       (0b{BinaryDigits})|(0B{BinaryDigits})

Underscore  "_"
Underscores         {Underscore}+
NonZeroDigit        [1-9]
Digit               [0-9]
DigitOrUnderscore   {Digit}|"_"
DigitAndUnderscores {DigitOrUnderscore}+
Digits              {Digit}|({Digit}{DigitAndUnderscores}?{Digit})
DecimalNumeral      0|({NonZeroDigit}{Digits}?)|({NonZeroDigit}{Underscores}{Digits})

HexDigit            [0-9a-fA-F]
HexDigitOrUnderscore    {HexDigit}|{Underscore}
HexDigitAndUnderscores  {HexDigitOrUnderscore}+
HexDigits                {HexDigit}|{HexDigit}{HexDigitAndUnderscores}?{HexDigit}
HexNumeral              0[xX]{HexDigits}


OctalDigitOrUnderscore  {OctalDigit}|{Underscore}
OctalDigitAndUnderscores    {OctalDigitOrUnderscore}+
OctalDigits             {OctalDigit}|({OctalDigit}{OctalDigitAndUnderscores}?{OctalDigit})
OctalNumeral        (0{OctalDigits})|(0{Underscores}{OctalDigits})


DecimalIntegerLiteral   {DecimalNumeral}{IntegerTypeSuffix}?
HexIntegerLiteral       {HexNumeral}{IntegerTypeSuffix}?
OctalIntegerLiteral     {OctalNumeral}{IntegerTypeSuffix}?
BinaryIntegerLiteral    {BinaryNumeral}{IntegerTypeSuffix}?

IntegerLiteral      {DecimalIntegerLiteral}|{HexIntegerLiteral}|{OctalIntegerLiteral}|{BinaryIntegerLiteral}  

Dot       "."
Sign        [+-]
FloatTypeSuffix  [fFdD]
ExponentIndicator   [eE]
SignedInteger   {Sign}?{Digits}
ExponentPart    {ExponentIndicator}{SignedInteger}
DecimalFloatingPointLiteral     ({Digits}{Dot}{Digits}?{ExponentPart}?{FloatTypeSuffix}?)|({Dot}{Digits}{ExponentPart}?{FloatTypeSuffix}?)|({Digits}{ExponentPart}{FloatTypeSuffix}?)|({Digits}{ExponentPart}?{FloatTypeSuffix})


BinaryExponentIndicator     [pP]
BinaryExponent      {BinaryExponentIndicator}{SignedInteger}
HexSignificand      ({HexNumeral}{Dot}?)|(0[xX]{HexDigits}?{Dot}{HexDigits})
HexadecimalFloatingPointLiteral     {HexSignificand}{BinaryExponent}{FloatTypeSuffix}?

FloatingPointLiteral    {DecimalFloatingPointLiteral}|{HexadecimalFloatingPointLiteral}

CharacterLiteral    \'({SingleCharacter}|{EscapeSequenceCharString})\'
StringLiteral       \"({StringCharacter}|{EscapeSequenceCharString})*\"

TextBlockWhiteSpace     {WhiteSpace}
TextBlockCharacter      ([^"\\]|\"([^"\\]|\"[^"\\]))|({LineTerminator})|({EscapeSequence})
TextBlock               \"\"\"({TextBlockWhiteSpace}{LineTerminator}{TextBlockCharacter})\"\"\"

ESCS [\\][b]|[\\][s]|[\\][t]|[\\][n]|[\\]\[f]|[\\][r]|[\\][\\]|[\\][\"]|[\\][\']|[\\][0-7]|[\\][0-7][0-7]|[\\][0-3][0-7][0-7]|[\\][u][0-9a-fA-F]{4}

Separator           "("|")"|"{"|"}"|"["|"]"|";"|","|"..."|"."|"@"|"::"
%%

{LineTerminator}+  {}
{WhiteSpace}+     {/* ignore whitespace  */ }

"//"        BEGIN(COMMENT1);
<COMMENT1>[^\n\r] /* ignore anything except a  line */
<COMMENT1>{LineTerminator}    {BEGIN(INITIAL);}

"/*"        BEGIN(COMMENT2);
<COMMENT2>[^\n]    {}
<COMMENT2>"*"+[^/\n]  
<COMMENT2>\n {}
<COMMENT2>"*"+"/"     BEGIN(INITIAL);

"boolean"       {yylval.lexeme = strdup(yytext); return BOOLEAN;}
"char"          {yylval.lexeme = strdup(yytext); return CHAR;}
"byte"          {yylval.lexeme = strdup(yytext); return BYTE;}
"short"         {yylval.lexeme = strdup(yytext); return SHORT;}
"int"           {yylval.lexeme = strdup(yytext); return INT;}
"long"          {yylval.lexeme = strdup(yytext); return LONG;}
"float"         {yylval.lexeme = strdup(yytext); return FLOAT;}
"double"        {yylval.lexeme = strdup(yytext); return DOUBLE;}
"void"          {yylval.lexeme = strdup(yytext); return VOID;}
"extends"       {yylval.lexeme = strdup(yytext); return EXTENDS;}
"super"         {yylval.lexeme = strdup(yytext); return SUPER;}
"this"          {yylval.lexeme = strdup(yytext); return THIS;}
"instanceof"    {yylval.lexeme = strdup(yytext); return INSTANCEOF;}
"final"         {fin=1;yylval.lexeme = strdup(yytext); return FINAL;}
"new"           {yylval.lexeme = strdup(yytext); return NEW;}
"String"           {yylval.lexeme = strdup(yytext); return STRING;}
"class"         {   
                    root->addNewScope();
                    scope_level++;
                    if(fin==1){
                        fin=0;
                        root->currNode->isFinalClass=true;
                    }
                    root->currNode->name="class";
                    root->currNode->isClassName=false;
                    yylval.lexeme = strdup(yytext);
                    return CLASS;
                }
"package"       {yylval.lexeme = strdup(yytext); return PACKAGE;}
"import"        {yylval.lexeme = strdup(yytext); importflag=1; return IMPORT;}
"implements"    {yylval.lexeme = strdup(yytext); return IMPLEMENTS;}
"static"        {yylval.lexeme = strdup(yytext); return STATIC;}
"transient"     {yylval.lexeme = strdup(yytext); return TRANSIENT;}
"public"        {yylval.lexeme = strdup(yytext); return PUBLIC;}
"protected"     {yylval.lexeme = strdup(yytext); return PROTECTED;}
"private"       {yylval.lexeme = strdup(yytext); return PRIVATE;}
"abstract"      {yylval.lexeme = strdup(yytext); return ABSTRACT;}
"default"       {yylval.lexeme = strdup(yytext); return DEFAULT;}
"volatile"      {yylval.lexeme = strdup(yytext); return VOLATILE;}
"synchronized"  {yylval.lexeme = strdup(yytext); return SYNCHRONIZED;}
"native"        {yylval.lexeme = strdup(yytext); return NATIVE;}
"strictfp"      {yylval.lexeme = strdup(yytext); return STRICTFP;}
"throws"        {yylval.lexeme = strdup(yytext); return THROWS;}
"assert"        {yylval.lexeme = strdup(yytext); return ASSERT;}
"if"            {root->addNewScope();root->currNode->vulnerable=true;scope_level++;root->currNode->isIfScope=true;root->currNode->name="if";yylval.lexeme = strdup(yytext); return IF;}
"else"          {root->addNewScope();root->currNode->vulnerable=true;scope_level++;root->currNode->name="else";yylval.lexeme = strdup(yytext); return ELSE;}
"while"         {
                    if(!(root->currNode->isDoRun==true&&root->currNode->cnt_paran==0)){
                        root->addNewScope();
                        scope_level++;
                        root->currNode->vulnerable=true;
                        root->currNode->name="while";
                    }
                    yylval.lexeme = strdup(yytext); 
                    return WHILE;
                }
"for"           {root->addNewScope();scope_level++;root->currNode->vulnerable=true;root->currNode->isForScope=true;root->currNode->name="for";yylval.lexeme = strdup(yytext); return FOR;}
"return"        {yylval.lexeme = strdup(yytext); return RETURN;}
"continue"      {yylval.lexeme = strdup(yytext); return CONTINUE;}
"break"         {yylval.lexeme = strdup(yytext); return BREAK;}
"do"            {root->addNewScope();scope_level++;root->currNode->isDoRun=true;root->currNode->name="do";yylval.lexeme = strdup(yytext); return DO;}
"true"          {yylval.lexeme = strdup(yytext);return BOOLEANLITERAL;}
"false"         {yylval.lexeme = strdup(yytext); return BOOLEANLITERAL;}



"exports"       {yylval.lexeme = strdup(yytext); return EXPORTS;}
"opens"         {yylval.lexeme = strdup(yytext); return OPENS;}
"uses"          {yylval.lexeme = strdup(yytext); return USES;}
"requires"      {yylval.lexeme = strdup(yytext); return REQUIRES;}
"yield"         {yylval.lexeme = strdup(yytext); return YIELD;}
"module"        {root->addNewScope();scope_level++;yylval.lexeme = strdup(yytext); return MODULE;}
"sealed"        {yylval.lexeme = strdup(yytext); return SEALED;}
"non-sealed"    {yylval.lexeme = strdup(yytext); return NONSEALED;}
"permits"       {yylval.lexeme = strdup(yytext); return PERMITS;}
"var"           {yylval.lexeme = strdup(yytext); return VAR;}
"to"            {yylval.lexeme = strdup(yytext); return TO;}
"provides"      {yylval.lexeme = strdup(yytext); return PROVIDES;}
"with"          {yylval.lexeme = strdup(yytext); return WITH;}
"transitive"    {yylval.lexeme = strdup(yytext); return TRANSITIVE;}
"open"          {yylval.lexeme = strdup(yytext); return OPEN;}
"record"        {yylval.lexeme = strdup(yytext); return RECORD;}
"null"          {yylval.lexeme = strdup(yytext); return NULLLITERAL;}
"System.out.println" {yylval.lexeme = strdup(yytext); return SYSTEMOUTPRINTLN;}
{ID}            {   
                    if((root->currNode->name=="class"||root->currNode->name=="classextends")&&
                    root->currNode->isClassName==false){
                        currClass = yytext;
                        SymNode* res = root->currNode->scope_clookup(yytext);
                        if(res)
                        {
                            yyerror("Class already declared.");
                        }
                        res = new SymNode(root->currNode, "class", CLASS_SYM);
                        root->cinsert(yytext, res);
                        Symbol* r = new Symbol(yytext, typeroot->addNewClassType(yytext), yylineno);
                        root->par_insert(r->lexeme, r);
                        list_class[yytext]=root->currNode;
                        root->currNode->isClassName=true;
                    }
                    yylval.lexeme = strdup(yytext);
                    return IDENTIFIER;
                }
"[]"            {yylval.lexeme = strdup(yytext); return LRSQUAREBRACKET;}
"["             {yylval.lexeme = strdup(yytext); return LEFTSQUAREBRACKET;}
"]"             {yylval.lexeme = strdup(yytext); return RIGHTSQUAREBRACKET;}
"("             {
                    yylval.lexeme = strdup(yytext);
                    return LEFTPARENTHESIS;
                }
")"             {
                    if(root->currNode->isForScope){
                        root->currNode->isRightPar=true;
                    }
                    yylval.lexeme = strdup(yytext);
                    return RIGHTPARENTHESIS;
                }
"}"             {       
                    if(root->currNode->isDoRun==true){
                        root->currNode->cnt_paran--;
                    }
                    if(root->currNode->name=="classextends" && root->currNode->parent->default_done==0 && root->currNode->parent->constr_args.size()>0 && root->currNode->constr_args.size()==0)
                    {
                        cout<<"Error on line number "<<yylineno<<". This class must include an explicit constructor declaration"<<endl;
                        yyerror("Error");
                    }
                    if(isarrayinit==0){
                        if(root->currNode->vulnerable==false&&
                        root->currNode->isIfScope==true){
                            root->endcurrScope();
                            scope_level++;
                            root->currNode->isThisDead=true;
                        }
                        else
                        if(root->currNode->vulnerable==false&&
                        root->currNode->isIfScope==false&&root->currNode->name!="do"){
                            root->end_all_vulnerable();
                        }
                    }
                    else{
                        isarrayinit--;
                    }
                    // scope_level--;
                    yylval.lexeme = strdup(yytext); 
                    return RIGHTCURLYBRACKET;
                }
"{"             {   
                    if(root->currNode->isDoRun==true){
                        root->currNode->cnt_paran++;
                    }
                    root->currNode->vulnerable=false;
                    yylval.lexeme = strdup(yytext);
                    return LEFTCURLYBRACKET;
                }
";"             {
                    if(root->currNode->vulnerable==true&&
                    root->currNode->isIfScope==true){
                        root->endcurrScope();
                        root->currNode->isThisDead=true;
                    }
                    else
                    if(root->currNode->vulnerable==true&&
                    root->currNode->isIfScope==false&&
                    root->currNode->isForScope==true&&
                    root->currNode->isRightPar==true){
                        root->end_all_vulnerable();
                    }
                    else
                    if(root->currNode->vulnerable==true&&
                    root->currNode->isIfScope==false&&
                    root->currNode->isForScope==false){
                        root->end_all_vulnerable();
                    }
                    yylval.lexeme = strdup(yytext);
                    return SEMICOLON;
                }
","             {yylval.lexeme = strdup(yytext); return COMMA;}
"..."           {yylval.lexeme = strdup(yytext); return ELLIPSIS;}
"."             {yylval.lexeme = strdup(yytext); return DOT;}
"@"             {yylval.lexeme = strdup(yytext); return AT;}
"::"            {yylval.lexeme = strdup(yytext); return DOUBLECOLON;}
"="             {yylval.lexeme = strdup(yytext); return ASSIGN;}  
">"             {yylval.lexeme = strdup(yytext); return GRT;}
"<"             {yylval.lexeme = strdup(yytext); return LSS;}
"!"             {yylval.lexeme = strdup(yytext); return NOT;}
"~"             {yylval.lexeme = strdup(yytext); return TIL;}
"?"             {yylval.lexeme = strdup(yytext); return QUES;}
":"             {yylval.lexeme = strdup(yytext); return COL;}
"->"            {yylval.lexeme = strdup(yytext); return ARW;}
"=="            {yylval.lexeme = strdup(yytext); return EQUAL;}
">="            {yylval.lexeme = strdup(yytext); return GEQ;}
"<="            {yylval.lexeme = strdup(yytext); return LEQ;}
"!="            {yylval.lexeme = strdup(yytext); return NEQUAL;}
"&&"            {yylval.lexeme = strdup(yytext); return AND;}
"||"            {yylval.lexeme = strdup(yytext); return OR;}
"++"            {yylval.lexeme = strdup(yytext); return INCRE;}
"--"            {yylval.lexeme = strdup(yytext); return DECRE;}
"+"             {yylval.lexeme = strdup(yytext); return PLUS;}
"-"             {yylval.lexeme = strdup(yytext); return MINUS;}
"*"             {yylval.lexeme = strdup(yytext); return MULT;}
"/"             {yylval.lexeme = strdup(yytext); return DIV;}
"&"             {yylval.lexeme = strdup(yytext); return BAND;}
"|"             {yylval.lexeme = strdup(yytext); return BOR;}
"^"             {yylval.lexeme = strdup(yytext); return BXOR;}
"%"             {yylval.lexeme = strdup(yytext); return MOD;}
"<<"            {yylval.lexeme = strdup(yytext); return LSHIFT;}
">>"            {yylval.lexeme = strdup(yytext); return RSHIFT;}
">>>"           {yylval.lexeme = strdup(yytext); return UNRSHIFT;}
"+="            {yylval.lexeme = strdup(yytext); return PLUSEQUAL;}
"-="            {yylval.lexeme = strdup(yytext); return MINUSEQUAL;}
"*="            {yylval.lexeme = strdup(yytext); return MULTEQUAL;}
"/="            {yylval.lexeme = strdup(yytext); return DIVEQUAL;}
"&="            {yylval.lexeme = strdup(yytext); return BANDEQUAL;}
"|="            {yylval.lexeme = strdup(yytext); return BOREQUAL;}
"^="            {yylval.lexeme = strdup(yytext); return BXOREQUAL;}
"%="            {yylval.lexeme = strdup(yytext); return MODEQUAL;}
"<<="           {yylval.lexeme = strdup(yytext); return LSHIFTEQUAL;}
">>="           {yylval.lexeme = strdup(yytext); return RSHIFTEQUAL;}
">>>="          {yylval.lexeme = strdup(yytext); return UNRSHIFTEQUAL;}
{IntegerLiteral}            {yylval.lexeme = strdup(yytext);return INTEGERLITERAL;}
{FloatingPointLiteral}        {yylval.lexeme = strdup(yytext);return FLOATINGPOINTLITERAL;}
{StringLiteral} {yylval.lexeme = strdup(yytext);return STRINGLITERAL;}           
{CharacterLiteral}      {yylval.lexeme = strdup(yytext);return CHARACTERLITERAL;}
{TextBlock}     {yylval.lexeme = strdup(yytext);return TEXTBLOCK;}
.			    {printf("Unrecognized character: %s at line no %d\n", yytext, yylineno); return 0;}
%%
