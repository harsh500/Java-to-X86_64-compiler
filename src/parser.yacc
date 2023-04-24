%{
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
extern vector<string> field_vars;
extern vector<string> func_names;
// map<string, pair<int, int>> typeroot->typewidth;
extern map<string,SymNode*> list_class;
extern map<string, int> tempVars;
extern map<string, string> classfunc;
extern string otpt;

vector<string> static_funcs;
int scope_level = 0;

extern int startPos;

extern int temp ;

extern int varCnt;
extern int tempCnt;
extern int labelCnt ;
extern int importflag ;

string currFunc = "";
string currClass = "";

extern string condvar;
extern int isCond ;
extern TypeHandler* typeroot ;

extern vector<Quadruple*> residualCode;

extern vector<string> castName;


extern int whilepos ;
extern SymGlob* root ;
extern SymGlob* orig_root ;
extern SymNode* magic_ptr;
extern SymNode* origNode;
// magic_ptr = origNode;
int spacelast = 0;
%}

%locations 
%union{
    struct Node* node;
	char* lexeme;
}
%token<lexeme> BOOLEAN CHAR BYTE SHORT INT LONG FLOAT DOUBLE VOID EXTENDS SUPER STRING 
%token<lexeme> THIS VAR INSTANCEOF FINAL NEW CHARACTERLITERAL STRINGLITERAL
%token<lexeme> TEXTBLOCK NULLLITERAL CLASS PACKAGE IMPORT STATIC DO
%token<lexeme> INTEGERLITERAL FLOATINGPOINTLITERAL BOOLEANLITERAL
%token<lexeme> JAVALETTER JAVALETTERORDIGIT OPEN MODULE 
%token<lexeme> REQUIRES EXPORTS OPENS USES PROVIDES TO WITH TRANSITIVE
%token<lexeme> LEFTSQUAREBRACKET RIGHTSQUAREBRACKET LEFTCURLYBRACKET RIGHTCURLYBRACKET LEFTPARENTHESIS RIGHTPARENTHESIS
%token<lexeme> SEMICOLON COMMA DOT ELLIPSIS AT DOUBLECOLON
%token<lexeme> ASSIGN GRT LSS NOT TIL QUES COL ARW EQUAL GEQ IMPLEMENTS
%token<lexeme> LEQ NEQUAL AND OR INCRE DECRE PLUS MINUS MULT DIV BAND BOR BXOR 
%token<lexeme> MOD LSHIFT RSHIFT UNRSHIFT PLUSEQUAL MINUSEQUAL MULTEQUAL DIVEQUAL 
%token<lexeme> BANDEQUAL BOREQUAL BXOREQUAL MODEQUAL LSHIFTEQUAL RSHIFTEQUAL UNRSHIFTEQUAL
%token<lexeme> IF ELSE WHILE FOR RETURN CONTINUE BREAK YIELD SEALED PROTECTED
%token<lexeme> PUBLIC PRIVATE STRICTFP ABSTRACT DEFAULT INTERFACE PERMITS NONSEALED
%token<lexeme> TRANSIENT VOLATILE NATIVE SYNCHRONIZED THROWS
%token<lexeme> ASSERT IDENTIFIER RECORD
%token<lexeme> LRSQUAREBRACKET SYSTEMOUTPRINTLN

%type<node> CompilationUnit leftcurl OrdinaryCompilationUnit PackageDeclaration TopLevelClassOrInterfaceDeclaration Name ClassDeclaration NormalClassDeclaration Modifier TypeParameters TypeParameterList

%type <node> ClassExtends ClassPermits ClassBody ClassBodyDeclaration ClassMemberDeclaration FieldDeclaration VariableDeclaratorList VariableDeclarator VariableDeclaratorId VariableInitializer ArrayInitializer MethodDeclaration MethodHeader

%type<node> Result MethodDeclarator ReceiverParameter FormalParameterList FormalParameter VariableArityParameter VariableModifier MethodBody InstanceInitializer StaticInitializer ConstructorDeclaration ConstructorDeclarator ConstructorBody ExplicitConstructorInvocation ArgumentList Block BlockStatement LabeledStatementNoShortIf ForStatementNoShortIf

%type<node> LocalClassOrInterfaceDeclaration LocalVariableDeclarationStatement LocalVariableDeclaration LocalVariableType Statement StatementNoShortIf StatementWithoutTrailingSubstatement EmptyStatement LabeledStatement ExpressionStatement StatementExpression IfThenStatement IfThenElseStatement IfThenElseStatementNoShortIf

%type<node> AssertStatement WhileStatement WhileStatementNoShortIf ForStatement BasicForStatement BasicForStatementNoShortIf ForInit ForUpdate StatementExpressionList EnhancedForStatement EnhancedForStatementNoShortIf BreakStatement ContinueStatement ReturnStatement

%type<node> TopLevelClassOrInterfaceDeclarations CommaNames DotIdentifiers CommaFormalParameters CommaVariableDeclarators CommaTypeParameters LeftRightSquareBrackets VariableModifiers CommaExpressions Modifiers BlockStatements CommaStatementExpressions ClassBodyDeclarations CommaVariableInitializers

%type<node> Primary PrimaryNoNewArray ClassLiteral NumericType IntegralType FloatingPointType ClassInstanceCreationExpression UnqualifiedClassInstanceCreationExpression FieldAccess ArrayAccess MethodInvocation MethodReference ArrayCreationExpression DimExpr_ Dims Expression AssignmentExpression Assignment LeftHandSide AssignmentOperator ConditionalExpression ConditionalOrExpression ConditionalAndExpression InclusiveOrExpression ExclusiveOrExpression AndExpression EqualityExpression RelationalExpression InstanceofExpression ShiftExpression AdditiveExpression MultiplicativeExpression UnaryExpression PreIncrementExpression PreDecrementExpression UnaryExpressionNotPlusMinus

%type<node> PostfixExpression PostIncrementExpression PostDecrementExpression Type PrimitiveType ReferenceType ArrayType TypeParameter Literal

%type<node> ModularCompilationUnit ModuleDeclaration ImportDeclaration ImportDeclarations SingleTypeImportDeclaration SingleStaticImportDeclaration TypeImportOnDemandDeclaration StaticImportOnDemandDeclaration ModuleDirective ModuleDirectives CastExpression

%type<node> RequiresModifier RequiresModifiers DoStatement _StatementNoShortIf AssignmentOperatorEqual ColConditional
%type<lexeme> Class

%start CompilationUnit
%%

/****************************************************************************************************************
                                COMPILATION UNIT, IMPORT DECLARATIONS, MODULE DECLARATIONS
*******************************************************************************************************************/

CompilationUnit:
    OrdinaryCompilationUnit  {
        $1->changeLabel("CompilationUnit");
        $$ = $1;
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
            for(auto it: $$->code) {
                cout << "callong print\n";
                //it->print();
                cout << "print return";
            }
            cout << "Incremental IR \n\n";
            int cnt = 0;
            for(auto it: ircode) {
                cout << cnt << "\t:\t";
                cout << "callong print\n";
                //it->print();
                cnt++;
            }
        }
        root->dumpSymbolTable();
    }
 |   ModularCompilationUnit 
     {
        $1->changeLabel("CompilationUnit");
        $$ = $1;
        verbose(v,"ModularCompilationUnit->CompilationUnit");
        printTree($$);
       if(otpt.size()!=0){
            call_dotgen($$,otpt);
        }
        else{
            call_dotgen($$,"graph1.dot");
        }
        root->printTree();
        root->printFuncs();
     }
;

OrdinaryCompilationUnit:
    PackageDeclaration ImportDeclarations TopLevelClassOrInterfaceDeclarations{
        vector<Node*> temp;
        temp.push_back($1);
        if($2) {
            temp.push_back($2);
        }
        if($3) {
            temp.push_back($3);
        }
        struct Node* n = new Node("OrdinaryCompilationUnit", temp);
        n->useless();
        $$ = n;
        verbose(v,"PackageDeclaration ImportDeclarations TopLevelClassOrInterfaceDeclarations->OrdinaryCompilationUnit");
    }
|   ImportDeclarations TopLevelClassOrInterfaceDeclarations{
        vector<Node*> temp;
        //temp.push_back($1);
        if($1) {
            temp.push_back($1);
        }
        if($2) {
            temp.push_back($2);
        }
        struct Node* n = new Node("OrdinaryCompilationUnit", temp);
        n->useless();
        $$ = n;
        verbose(v,"ImportDeclarations TopLevelClassOrInterfaceDeclarations->OrdinaryCompilationUnit");
    }

;

PackageDeclaration:
    PACKAGE IDENTIFIER SEMICOLON    
    {
        $$ = new Node("PackageDeclaration", strcat($1, strcat(" ", $2)));
        verbose(v,"PACKAGE IDENTIFIER SEMICOLON->PackageDeclaration");

        Symbol* res = root->lookup($2);
        if(res)
        {
            yyerror("Package has already been declared");
        }
        res = new Symbol($2, PACKAGE_TYPE, yylineno);
        root->insert(res->lexeme, res);
    }
|   PACKAGE IDENTIFIER DOT IDENTIFIER DotIdentifiers SEMICOLON
    {
        
        string s = string($1) + string($2) + "." + string($4);
        
        if($5) 
            s += $5->attr;
        $$ = new Node("PackageDeclaration", s);
        verbose(v,"PACKAGE IDENTIFIER DOT IDENTIFIER DotIdentifiers SEMICOLON->PackageDeclaration");

        Symbol* res;
        if($5)
        {
            res = root->lookup($5->children.back()->attr);
            if(!res)
            {
                res = new Symbol($5->children.back()->attr, PACKAGE_TYPE, yylineno);
                root->insert(res->lexeme, res);

            }
        }
        else
        {
            res = root->lookup($4);
            if(!res)
            {
                res = new Symbol($5->children.back()->attr, PACKAGE_TYPE, yylineno);
                root->insert(res->lexeme, res);

            }
        }       
    }
;

ModularCompilationUnit:
    ModuleDeclaration   {
        $$ = $1;
        verbose(v,"ModuleDeclaration->ModularCompilationUnit");
    }
|   ImportDeclaration ImportDeclarations ModuleDeclaration    {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            temp.clear();
            temp.push_back($2);
        
        }
        
        
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        struct Node* n = new struct Node("ModularCompilationUnit", temp);
        n->useless();
        $$ = n;
      verbose(v,"ImportDeclaration ImportDeclarations ModuleDeclaration->ModularCompilationUnit");
    } 
;
ImportDeclaration:
    SingleTypeImportDeclaration {
        $$ = $1;
        verbose(v,"SingleTypeImportDeclaration->ImportDeclaration");
        importflag = 0;
    }
|   TypeImportOnDemandDeclaration   {
        $$ = $1;
        verbose(v,"TypeImportOnDemandDeclaration->ImportDeclaration");
        importflag = 0;
    }
|   SingleStaticImportDeclaration   {
        $$ = $1;
        verbose(v,"SingleStaticImportDeclaration->ImportDeclaration");
        importflag = 0;
    }
|   StaticImportOnDemandDeclaration {
        $$ = $1;
        verbose(v,"StaticImportOnDemandDeclaration->ImportDeclaration");
        importflag = 0;
    }
;

SingleTypeImportDeclaration:
    IMPORT Name SEMICOLON       {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        

        vector<Node*> t = {t1, $2};
        $$ = new struct Node("SingleTypeImportDeclaration", t);
        verbose(v,"IMPORT Name SEMICOLON->SingleTypeImportDeclaration");

        Symbol* res = root->lookup($2->attr);
        if(!res)
        {
            res = new Symbol($2->attr, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
;

TypeImportOnDemandDeclaration:
    IMPORT Name DOT MULT SEMICOLON  {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        
        struct Node* t3 = new struct Node ( "Operator" ,$4);
       

        vector<Node*> t = {t1, $2, t3};
        $$ = new struct Node("TypeImportOnDemandDeclaration", t);
        verbose(v,"IMPORT Name DOT MULT SEMICOLON->TypeImportOnDemandDeclaration");

        string name = $2->attr+".*";
        Symbol* res = root->lookup(name);
        if(!res)
        {
            res = new Symbol(name, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
;

SingleStaticImportDeclaration:
    IMPORT STATIC Name SEMICOLON {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$2);
       

        vector<Node*> t = {t1, t2, $3};
        $$ = new struct Node("SingleStaticImportDeclaration", t);
        verbose(v,"IMPORT STATIC Name SEMICOLON->SingleStaticImportDeclaration");

        Symbol* res = root->lookup($3->attr);
        if(!res)
        {
            res = new Symbol($3->attr, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
;

StaticImportOnDemandDeclaration:
    IMPORT STATIC Name DOT MULT SEMICOLON   {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$2);
        
        struct Node* t4 = new struct Node ( "Operator" ,$5);
      

        vector<Node*> t = {t1, t2, $3, t4};
        $$ = new struct Node("StaticImportOnDemandDeclaration", t);
        verbose(v,"IMPORT STATIC Name DOT MULT SEMICOLON->StaticImportOnDemandDeclaration");


        string name = $3->attr+".*";
        Symbol* res = root->lookup(name);
        if(!res)
        {
            res = new Symbol(name, IMPORT_TYPE, yylineno);
            root->insert(res->lexeme, res);
        }
    }
;

ModuleDeclaration:
    MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET    {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Identifier" ,$2);
       

        vector<Node*> t = {t1, t2};
        $$ = new struct Node("ModuleDeclaration", t);
        verbose(v,"MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    }
|   MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Identifier" ,$2);
       
        struct Node* t4 = new struct Node ( "Identifier" ,$4);
        

        vector<Node*> t = {t1, t2, t4};
        if($5) t.push_back($5);
        $$ = new struct Node("ModuleDeclaration", t);
        verbose(v,"MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    }
|   MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET   {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Identifier" ,$2);
        struct Node* t3 = new struct Node ( "Separator" ,$3);
      
        vector<Node*> t;
        if($5)  
            t = {t1, t2, $4, $5};
        else
            t = {t1, t2, $4};
        $$ = new struct Node("ModuleDeclaration", t);
        verbose(v,"MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET->ModuleDeclaration");
    }
|   MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET    {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Identifier" ,$2);
        
        struct Node* t4 = new struct Node ( "Identifier" ,$4);
        

        vector<Node*> temp; 
        
        if($5 && $8) {
            vector<Node*> t = {t1, t2, t4, $5, $7, $8};
            temp = t;
        }
        else if($5) {
            vector<Node*> t = {t1, t2, t4, $5, $7};
            temp = t;
        }
        else if($8) {
            vector<Node*> t = {t1, t2, t4, $7, $8};
            temp = t;
        }
        else {
            vector<Node*> t = {t1, t2,t4, $7};
            temp = t;
        }

        $$ = new struct Node("ModuleDeclaration", temp);
        verbose(v,"MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET->ModuleDeclaration");
    }
|   OPEN MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET   {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$2);
        struct Node* t3 = new struct Node ( "Identifier" ,$3);
        

        vector<Node*> t = {t1, t2, t3};
        $$ = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    }
|   OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET  {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ("Keyword",  $2);
        struct Node* t3 = new struct Node ( "Identifier" ,$3);
       
        struct Node* t5 = new struct Node ( "Identifier" ,$5);
       
        vector<Node*> t;
        
        if($6)
            t = {t1, t2, t3, t5, $6};
        else
            t = {t1, t2, t3, t5};
        $$ = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET RIGHTCURLYBRACKET->ModuleDeclaration");
    } 
|   OPEN MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET  {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ("Keyword",  $2);
        struct Node* t3 = new struct Node ( "Identifier" ,$3);
        
        vector<Node*> t;
        if($6)  
            t = {t1, t2, t3, $5, $6};
        else
            t = {t1, t2, t3,$5};
        $$ = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET->ModuleDeclaration");
    }
|   OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET   {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ("Keyword",  $2);
        struct Node* t3 = new struct Node ( "Identifier" ,$3);
        
        struct Node* t5 = new struct Node ( "Identifier" ,$5);
        

        vector<Node*> t; 
        
        if($6 && $9)
            t = {t1, t2, t3, t5, $6, $8, $9};
        else if($6)
            t = {t1, t2, t3, t5, $6, $8};
        else if($9)
            t = {t1, t2, t3, t5 , $8, $9};
        else
            t = {t1, t2, t3, t5, $8};
        $$ = new struct Node("ModuleDeclaration", t);
        verbose(v,"OPEN MODULE IDENTIFIER DOT IDENTIFIER DotIdentifiers LEFTCURLYBRACKET ModuleDirective ModuleDirectives RIGHTCURLYBRACKET>ModuleDeclaration");
    }
;
ModuleDirective:
    REQUIRES Name SEMICOLON {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        

        vector<Node*> t = {t1, $2};
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"REQUIRES Name SEMICOLON->ModuleDirective");
    }
|   REQUIRES RequiresModifier RequiresModifiers Name SEMICOLON   {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        

        vector<Node*> t = {t1};
        
        if($3) {
            $3->addChildToLeft($2);
            t.push_back($3);
        }
        else t.push_back($2);
        t.push_back($4);
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"REQUIRES RequiresModifier RequiresModifiers Name SEMICOLON->ModuleDirective");
    }
|   EXPORTS Name SEMICOLON  {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        

        vector<Node*> t = {t1, $2};
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"EXPORTS Name SEMICOLON->ModuleDirective");
    }
|   EXPORTS Name TO Name SEMICOLON  {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$3);
        

        vector<Node*> t = {t1, $2, t2, $4};
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"EXPORTS Name TO Name SEMICOLON->ModuleDirective");
    }
|   EXPORTS Name TO Name COMMA Name CommaNames SEMICOLON   {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$3);
        
        vector<Node*> t = {t1, $2, t2};
        
        if($7) {
            $7->addChildToLeft($6);
            $7->addChildToLeft($4);
            t.push_back($7);
        }
        else {t.push_back($4); t.push_back($6);}
        

        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"EXPORTS Name TO Name COMMA Name CommaNames SEMICOLON->ModuleDirective");
    }
|   OPENS Name SEMICOLON    {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        

        vector<Node*> t = {t1, $2};
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"OPENS Name SEMICOLON->ModuleDirective");
    }
|   OPENS Name TO Name COMMA Name CommaNames SEMICOLON {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$3);
     
        vector<Node*> t = {t1, $2};
        
        if($7) {
            $7->addChildToLeft($6);
            $7->addChildToLeft($4);
            t.push_back($7);
        }
        else { 
            t.push_back($4);
            t.push_back($6);
        }
        
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"OPENS Name TO Name COMMA Name CommaNames SEMICOLON->ModuleDirective");
    }
|   USES Name SEMICOLON {
        struct Node* t1 = new struct Node ("Keyword",  $1);
      

        vector<Node*> t = {t1, $2};
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"USES Name SEMICOLON->ModuleDirective");
    }
|   PROVIDES Name WITH Name SEMICOLON   {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$3);
       

        vector<Node*> t = {t1, $2, t2, $4};
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"PROVIDES Name WITH Name SEMICOLON->ModuleDirective");
    }
|   PROVIDES Name WITH Name COMMA Name CommaNames SEMICOLON    {
        struct Node* t1 = new struct Node ("Keyword",  $1);
        struct Node* t2 = new struct Node ( "Keyword" ,$3);
        

        vector<Node*> t = {t1, $2, t2};
        
        if($7) {
            $7->addChildToLeft($6);
            $7->addChildToLeft($4);
            t.push_back($7);
        }
        else t.push_back($6);
        
        $$ = new struct Node("ModuleDirective", t);
        verbose(v,"PROVIDES Name WITH Name COMMA Name CommaNames SEMICOLON->ModuleDirective");
    }
;

RequiresModifier:
    TRANSITIVE  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n = new struct Node("RequiresModifier", temp);
    $$ = n;
    verbose(v,"TRANSITIVE->RequiresModifier");
}
|   STATIC  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n = new struct Node("RequiresModifier", temp);
    $$ = n;
    verbose(v,"STATIC->RequiresModifier");
}
;


TopLevelClassOrInterfaceDeclaration:
    ClassDeclaration     {
        $$ = $1;
        verbose(v,"ClassDeclaration->TopLevelClassOrInterfaceDeclaration");
    }
;

TopLevelClassOrInterfaceDeclarations:
    {$$ = NULL;}
|   TopLevelClassOrInterfaceDeclaration TopLevelClassOrInterfaceDeclarations    {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            $$ = $2;
        }
        else $$ = new Node("TopLevelClassOrInterfaceDeclarations", temp);
        verbose(v,"TopLevelClassOrInterfaceDeclaration TopLevelClassOrInterfaceDeclarations->TopLevelClassOrInterfaceDeclarations");
    }
;

Name:
    IDENTIFIER 
    {
        $$ = new Node("Identifier", $1);
        verbose(v,"IDENTIFIER->Name");
        
        if(importflag==0)
        {
        Symbol* res = root->lookup($1);
        if(!res)
        {
            if(!root->flookup($1))
            {
                cout<<"hulhul Found "<<$1<<endl;
                cout<<"Error! Name "<<$1<<" has not been declared before"<<endl;
                yyerror("Error");
            }
        }
        else
        {
            
            if(res->isField == 1) {
                $$->varName = "this." + $$->varName;
                $$->attr = $$->varName;
            }
            $$->type = res->type;
        }
        }
     }
|   Name DOT IDENTIFIER 
    {
        string s = $1->attr + $2 + $3;
        $$ = new Node("Name", s);
        verbose(v,"Name DOT IDENTIFIER->Name");
        cout<<"Working here"<<endl;
        if(importflag==0)
        {
        int i = spacestripind($1->attr);
        string sp = ($1->attr).substr(i+1, $1->attr.length()-i-1);
        string spf;
        SymNode* tr = nullptr;
        if(i>0)
        {
            int j = i-1;
            while(j>=0 && $1->attr[j]!='.')
                j--;
            spf = ($1->attr).substr(j+1, i-j-1);
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
        SymNode* res = t->scope_flookup($3);
        Symbol* symb = t->scope_lookup($3);
        if(typ==r->lexeme && symb && symb->isStatic==false)
        {
            cout<<"Error on line number "<<yylineno<<". Non-static member cannot be accessed in a static way"<<endl;
            yyerror("Error");
        }
        if(!res && !symb)
        {
            cout<<"Error! Name "<<$3<<" has not been declared before"<<endl;
            yyerror("Error");
        }
        if(res)
            $$->type = res->returntype;
        else
            $$->type = symb->type;
        }
    }
;

DotIdentifiers:
    {$$ = NULL;}
|   DOT IDENTIFIER DotIdentifiers      {
    vector<struct Node*> temp;
    //struct Node* n1 = new struct Node("Separator", $1);
    //temp.push_back(n1);
    struct Node* t = new Node("Identifier", $2);
    temp.push_back(t);
    
    if($3) {
        for(auto it: temp) {
                $3->addChildToLeft(it);
            }
            $$ = $3;

    }
    else $$ = t;
    verbose(v,"DOT IDENTIFIER DotIdentifiers->DotIdentifiers");
}
;

CommaNames:
    {$$ = NULL;}
|   COMMA Name CommaNames   {
        vector<struct Node*> temp;
        if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        if($3) {
            for(auto it: temp) {
                $3->addChildToLeft(it);
            }
            $$ = $3;
        }
        else $$ = new Node("Names", temp);
        verbose(v,"COMMA Name CommaNames ->CommaNames");
}
;


/***************************************************************************************************************

                                    CLASS DECLARATIONS
****************************************************************************************************************/

Class:
    CLASS IDENTIFIER {
        $$ = $2;
        Quadruple* q= new Quadruple(7, "", string("beginclass ") + $2, "" );
        
        ircode.push_back(q);
       
    }
;

ClassDeclaration:
    NormalClassDeclaration  {
        $$= $1;
        verbose(v,"NormalClassDeclaration->ClassDeclaration");
    } 
    // EnumDeclaration
    // RecordDeclaration
;

NormalClassDeclaration:
    Class ClassBody
    {
        
        vector<Node*> temp ;
        if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        //vector<Node*> temp = {t1, t2, $3};
        $$ = new Node("NormalClassDeclaration", temp);
        //Quadruple* q = new Quadruple(7, "", "beginclass", $2);
        Quadruple* q1 = new Quadruple(7, "", "endclass", "" ); 
        $$->code.push_back(q1);
        ircode.push_back(q1);
        $$->last = ircode.size() - 1;
        
        verbose(v,"Class ClassBody->NormalClassDeclaration");
        
    }
|   Class TypeParameters ClassBody
    {
        
        vector<Node*> temp ;
        if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
       
        
        $$ = new Node("NormalClassDeclaration", temp);
        Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
        $$->code.push_back(q1);
        ircode.push_back(q1);
        $$->last = ircode.size() - 1;
        verbose(v,"Class TypeParameters ClassBody->NormalClassDeclaration");
    }
|   Class ClassExtends ClassBody  {
    vector<struct Node*> temp;
    
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
   Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
        $$->code.push_back(q1);
        ircode.push_back(q1);
    $$->last = ircode.size() - 1;
        
    verbose(v,"Class ClassExtends ClassBody->NormalClassDeclaration");
    SymNode* r = root->clookup($2->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$2->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   Class ClassPermits ClassBody  {
    vector<struct Node*> temp;
   
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Class ClassPermits ClassBody->NormalClassDeclaration");
}
|   Class TypeParameters ClassExtends ClassBody   {
    vector<struct Node*> temp;
    
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        if($4->useful == false) {
            for(auto it: $4->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($4);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Class TypeParameters ClassExtends ClassBody->NormalClassDeclaration");
    SymNode* r = root->clookup($3->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$3->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   Class TypeParameters ClassPermits ClassBody   {
    vector<struct Node*> temp;
    
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        if($4->useful == false) {
            for(auto it: $4->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($4);
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Class TypeParameters ClassPermits ClassBody->NormalClassDeclaration");
    SymNode* res = root->currNode->scope_clookup($1);
    if(res)
    {
        cout<<"Error! Class "<<$2<<" has already been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    res = new SymNode(root->currNode, "New class", CLASS_SYM);
    root->cinsert($1, res);
    Symbol* r = new Symbol($1, typeroot->addNewClassType(), yylineno);
    list_class[$1]=res;
    r->scope_level = scope_level;
    root->insert(r->lexeme, r);
}
|   Class ClassExtends ClassPermits ClassBody     {
    vector<struct Node*> temp;
    
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        if($4->useful == false) {
            for(auto it: $4->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($4);
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Class ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup($2->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$2->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    
    root->currNode->parent=r;
}
|   Class TypeParameters ClassExtends ClassPermits ClassBody {
    vector<struct Node*> temp;
    
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        if($4->useful == false) {
            for(auto it: $4->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($4);
    if($5->useful == false) {
            for(auto it: $5->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($5);
   
        
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Class TypeParameters ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup($3->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$3->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   Modifier Modifiers Class ClassBody   {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
    
    temp.push_back($4);
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassBody->NormalClassDeclaration");
}
|   Modifier Modifiers Class TypeParameters ClassBody    {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
    
    temp.push_back($4);
    temp.push_back($5);
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassBody->NormalClassDeclaration");
}
|   Modifier Modifiers Class ClassExtends ClassBody  {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
   
    temp.push_back($4);
    temp.push_back($5);
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup($4->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$4->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   Modifier Modifiers Class ClassPermits ClassBody  {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
    
    temp.push_back($4);
    temp.push_back($5);
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassPermits ClassBody->NormalClassDeclaration");
}
|   Modifier Modifiers Class TypeParameters ClassExtends ClassBody   {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
    
    temp.push_back($5);
    temp.push_back($6);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup($5->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$5->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   Modifier Modifiers Class TypeParameters ClassPermits ClassBody   {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
    
    temp.push_back($5);
    temp.push_back($6);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassPermits ClassBody->NormalClassDeclaration");
}
|   Modifier Modifiers Class ClassExtends ClassPermits ClassBody {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
    
    temp.push_back($5);
    temp.push_back($6);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->currNode->scope_clookup($4->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$4->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   Modifier Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody  {
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft($1);
        temp.push_back($2);
    }
    else temp.push_back($1);
    
    temp.push_back($5);
    temp.push_back($6);
    temp.push_back($7);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
   Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup($5->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$5->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   FINAL Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody  {
    struct Node * g= new Node("Keyword",$1);
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
    
    temp.push_back($5);
    temp.push_back($6);
    temp.push_back($7);
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"FINAL Modifiers Class TypeParameters ClassExtends ClassPermits ClassBody->NormalClassDeclaration");
    SymNode* r = root->clookup($5->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$5->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   FINAL Modifiers Class ClassBody   {
    struct Node * g= new Node("Keyword",$1);
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
    
  
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassBody->NormalClassDeclaration");
}
|   FINAL Modifiers Class TypeParameters ClassBody    {
    struct Node * g= new Node("Keyword",$1);
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
    
    temp.push_back($5);
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassBody->NormalClassDeclaration");
}
|   FINAL Modifiers Class ClassExtends ClassBody  {
    struct Node * g= new Node("Keyword",$1);
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
   
    temp.push_back($5);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"FINAL Modifiers Class ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup($4->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$4->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   FINAL Modifiers Class ClassPermits ClassBody  {
    struct Node * g= new Node("Keyword",$1);
    vector<struct Node*> temp;
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
   
    temp.push_back($5);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    verbose(v,"Modifier Modifiers Class ClassPermits ClassBody->NormalClassDeclaration");
}
|   FINAL Modifiers Class TypeParameters ClassExtends ClassBody   {
    vector<struct Node*> temp;
    struct Node * g= new Node("Keyword",$1);
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
    
    temp.push_back($5);
    temp.push_back($6);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassExtends ClassBody->NormalClassDeclaration");

    SymNode* r = root->clookup($5->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$5->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
}
|   FINAL Modifiers Class TypeParameters ClassPermits ClassBody   {
    vector<struct Node*> temp;
    struct Node * g= new Node("Keyword",$1);
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
    
    temp.push_back($5);
    temp.push_back($6);
    
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class TypeParameters ClassPermits ClassBody->NormalClassDeclaration");
}
|   FINAL Modifiers Class ClassExtends ClassPermits ClassBody {
    vector<struct Node*> temp;
    struct Node * g= new Node("Keyword",$1);
    if($2) {
        $2->addChildToLeft(g);
        temp.push_back($2);
    }
    else temp.push_back(g);
    
    temp.push_back($5);
    temp.push_back($6);
   
    struct Node* n = new struct Node("NormalClassDeclaration", temp);
    $$ = n;
    Quadruple* q1 = new Quadruple(7, "", "endclass", "" );
    $$->code.push_back(q1);
    ircode.push_back(q1);
    $$->last = ircode.size() - 1;
    verbose(v,"Modifier Modifiers Class ClassExtends ClassPermits ClassBody->NormalClassDeclaration");

    SymNode* r = root->currNode->scope_clookup($4->attr);
    if(!r)
    {
        cout<<"Error! Parent class "<<$4->attr<<" has not been declared"<<endl;
        yyerror("Error");
        // yyerror("Parent class not declared");
    }
    root->currNode->parent=r;
} 
;

Modifier:
    PUBLIC  {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    verbose(v,"PUBLIC ->Modifier");
    
}
|   PROTECTED   {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    verbose(v,"PROTECTED ->Modifier");
}
|   PRIVATE {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    verbose(v,"PRIVATE ->Modifier");
}

|   STATIC  {
    vector<struct Node*> temp;
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    verbose(v,"STATIC ->Modifier");
}
;

Modifiers:
    {$$ = NULL;}
|   Modifier Modifiers  {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            $$ = $2;
        }
        else $$ = new Node("Modifiers", temp);
        verbose(v,"Modifier Modifiers->Modifiers");
    }
;

TypeParameters:
    LSS TypeParameterList GRT   {
    vector<struct Node*> temp;
    
    temp.push_back($2);
    
    struct Node* n = new struct Node("TypeParameters", temp);
    $$ = n;
    verbose(v,"LSS TypeParameterList GRT ->TypeParameters");
}
;

TypeParameterList:
    TypeParameter    {
        $$ = $1;
        verbose(v,"TypeParameter->TypeParameterList");
    }
|   TypeParameter COMMA TypeParameter CommaTypeParameters {
    vector<struct Node*> temp;
    if($4) {
        $4->addChildToLeft($3);
        $4->addChildToLeft($1);
        temp.push_back($4);
    } 
    else {
        temp.push_back($1);
        temp.push_back($3);
    }
    struct Node* n = new struct Node("TypeParameterList", temp);
    $$ = n;
    verbose(v,"TypeParameter COMMA TypeParameter CommaTypeParameters->TypeParameterList");
}//here
;

ClassExtends:
    EXTENDS Name    {
    // vector<struct Node*> temp;
    // struct Node* n1 = new struct Node("Keyword", $1);
    // temp.push_back(n1);
    // temp.push_back($2);
    // struct Node* n = new struct Node("ClassExtends", temp);
    // $$ = n;
    root->currNode->name="classextends";
    root->currNode->ogparent=root->currNode->parent;
    root->currNode->parent->childscopes.pop_back();
    if(list_class.find($2->attr)==list_class.end()){
        cout<<"No such class exist error "<<$2->attr<<" on line number "<<yylineno<<endl;
        yyerror("error");
    }
    if(list_class[$2->attr]->isFinalClass==true){
        cout<<"Cannot be extended "<<$2->attr<<" on line number "<<yylineno<<endl;
        yyerror("error");
    }
    root->currNode->parent=list_class[$2->attr];
    root->currNode->parent->childscopes.push_back(root->currNode);
    $$ = $2;
    verbose(v,"EXTENDS Name->ClassExtends");
}
;


ClassPermits:
    PERMITS Name    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    struct Node* n = new struct Node("ClassPermits", temp);
    $$ = n;
    verbose(v,"PERMITS Name->ClassPermits");
}
|   PERMITS Name COMMA Name CommaNames {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    if($5) {
        $5->addChildToLeft($4);
        $5->addChildToLeft($2);
        temp.push_back($5);
    }
    else{
        temp.push_back($2);
        temp.push_back($4);
    } 
    
    struct Node* n = new struct Node("ClassPermits", temp);
    $$ = n;
    verbose(v,"PERMITS Name COMMA Name CommaNames->ClassPermits");
} //here
;

ClassBody:
    LEFTCURLYBRACKET RIGHTCURLYBRACKET     {
        vector<Node*>temp;
        $$ = new struct Node("ClassBody", temp);
        verbose(v,"LEFTCURLYBRACKET RIGHTCURLYBRACKET->ClassBody");
    }
|   LEFTCURLYBRACKET ClassBodyDeclaration ClassBodyDeclarations RIGHTCURLYBRACKET
    {
        //struct Node* t1 = new Node ("Separator",  "{");
        //struct Node* t2 = new Node ( "Separator", "}");
        
        vector<Node*> temp ;
        if($3) {
            $3->addChildToLeft($2);
            temp.push_back($3);
        }
        else temp.push_back($2);
        
       
        $$ = new Node("ClassBody", temp);
        verbose(v,"LEFTCURLYBRACKET ClassBodyDeclaration ClassBodyDeclarations RIGHTCURLYBRACKET->ClassBody");
    }
;

ClassBodyDeclaration:
    ClassMemberDeclaration{
        $$ = $1;
        verbose(v,"ClassMemberDeclaration->ClassBodyDeclaration");
    } 
|   InstanceInitializer {
        $$ = $1;
        verbose(v,"InstanceInitializer->ClassBodyDeclaration");
    } 
|   StaticInitializer   {
       $$ = $1;
       verbose(v,"StaticInitializer->ClassBodyDeclaration");
    } 
|   ConstructorDeclaration   {
       $$ = $1;
       verbose(v,"ConstructorDeclaration->ClassBodyDeclaration");
    } 
;

ClassBodyDeclarations:
    {$$ =NULL;}
|   ClassBodyDeclaration ClassBodyDeclarations  {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            $$ = $2;
        }
        else $$ = new Node("BlockStatements", temp);
        verbose(v,"ClassBodyDeclaration ClassBodyDeclarations->ClassBodyDeclarations");
    }
;

ClassMemberDeclaration:
    FieldDeclaration    {
       $$ = $1;
        verbose(v,"FieldDeclaration->ClassMemberDeclaration");
    }
|   MethodDeclaration   {
       $$ = $1;
       verbose(v,"MethodDeclaration->ClassMemberDeclaration");
    }
|   ClassDeclaration {
        $$=$1;
        verbose(v,"ClassDeclaration->ClassMemberDeclaration");
 }
  
;


FieldDeclaration:
    Type VariableDeclaratorList SEMICOLON
    {
        //struct Node* t = new Node("Separator", ";");
        cout<<"Here man man man man"<<endl;
        vector<Node*> temp;
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2->useful == false) {
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        //temp.push_back(t);
        $$ = new Node("FieldDeclaration", temp);
       
        verbose(v,"Type VariableDeclaratorList SEMICOLON->FieldDeclaration");

        for(auto ch : $2->children)
        {
            if(ch->attr=="=")
                field_vars.push_back(ch->children[0]->attr);
            else
                field_vars.push_back(ch->attr);
            int _type = $1->type;
            if(ch->arrayType && _type < 100) _type += ch->arrayType*100  ; 
            if(ch->children.size() > 1 && ch->children[1]->arrayType) {ch->children[1]->type = $1->type%100 + 100 * ch->children[1]->arrayType; 
                 if(ch->children[1]->label == "ArrayInitializer") {
                    if(ch->arrayType == 1 ) init1DArray(ch, $1->attr);
                    else if(ch->arrayType == 2 ) init2DArray(ch, $1->attr);
                    else if(ch->arrayType == 3 ) init3DArray(ch, $1->attr);
                }
                else if(ch->children[1]->label == "ArrayCreationExpression") {
                    cout << ch->children[0]->varName << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2\n";
                    Quadruple* q = new Quadruple("", ch->children[1]->varName, "", ch->children[0]->varName);
                    ircode.push_back(q);
                    $$->code.push_back(q);
                    //if(ch->arrayType == 1 ) init1DArray(ch, $1->attr);
                    //else if(ch->arrayType == 2 ) init2DArray(ch, $1->attr);
                    //else if(ch->arrayType == 3 ) init3DArray(ch, $1->attr);
                }
            }
            cout << ch->arrayType << "\n";
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[$1->attr].second);
            sym->isField = 1;
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                //ch->children[0]->attr += "`" + to_string(scope_level);  ch->children[0]->varName = ch->children[0]->attr;
                
                
                if(!ch->arrayType) processFieldDec($$, ch, _type);

            }
            else {
                    //ch->varName = ch->attr = ch->attr + "`" + to_string(scope_level);
                    //sym->lexeme = ch->attr;
                    
                    processUninitDec($$, ch, _type);
                
            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                //sym->type += 100*ch->arrayType;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           }
            sym->isField = 1;
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }
        processPostIncre($$);
    }
|   Modifier Modifiers Type VariableDeclaratorList SEMICOLON      {
       // struct Node* t = new struct Node("Separator", $5);
        vector<Node*> temp;
        if($2) {
            $2->addChildToLeft($1);
            temp.push_back($2);
        }
        else temp.push_back($1);
        
        temp.push_back($3);
        temp.push_back($4);
        $$ = new struct Node("FieldDeclaration", temp);
        verbose(v,"Modifier Modifiers Type VariableDeclaratorList SEMICOLON->FieldDeclaration");

        // cout<<"Here, field's accesss type is "<<$1->attr;
        int acc=PUBLIC_ACCESS, isStatic=0;
        if($1->attr=="private")
            acc = PRIVATE_ACCESS;
        else if($1->attr=="protected")
            acc = PROTECTED_ACCESS;

        if($1->attr=="static")
        {
            isStatic=1;
        }

        if($2)
        {
            for(auto ch : $2->children)
            {
                if(ch->attr=="private")
                    acc = PRIVATE_ACCESS;
                else if(ch->attr=="protected")
                    acc = PROTECTED_ACCESS;

                if(ch->attr=="static")
                    isStatic=1;
            }
        }
        for(auto ch : $4->children)
        {
            if(ch->attr=="=")
                field_vars.push_back(ch->children[0]->attr);
            else
                field_vars.push_back(ch->attr);
            int _type = $3->type;
            if(ch->arrayType) _type += ch->arrayType*100  ; 
            if(ch->children.size() > 1 && ch->children[1]->arrayType) 
            {
                ch->children[1]->type = $3->type + 100 * ch->children[1]->arrayType; 
                 if(ch->children[1]->label == "ArrayInitializer") {
                    if(ch->arrayType == 1 ) init1DArray(ch, $3->attr);
                    else if(ch->arrayType == 2 ) init2DArray(ch, $3->attr);
                    else if(ch->arrayType == 3 ) init3DArray(ch, $3->attr);
                }
                else if(ch->children[1]->label == "ArrayCreationExpression") {
                    cout << ch->children[0]->varName << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2\n";
                    Quadruple* q = new Quadruple("", ch->children[1]->varName, "", ch->children[0]->varName);
                    ircode.push_back(q);
                    $$->code.push_back(q);
                    //if(ch->arrayType == 1 ) init1DArray(ch, $1->attr);
                    //else if(ch->arrayType == 2 ) init2DArray(ch, $1->attr);
                    //else if(ch->arrayType == 3 ) init3DArray(ch, $1->attr);
                }
            }
            cout << ch->arrayType << "\n";
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[$3->attr].second, acc);
            
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                //ch->children[0]->attr += "`" + to_string(scope_level);  ch->children[0]->varName = ch->children[0]->attr;
                
               
                
                if(!ch->arrayType ) processFieldDec($$, ch, _type);
               
                

            }
             else {
                    //ch->varName = ch->attr = ch->attr + "`" + to_string(scope_level);
                    //sym->lexeme = ch->attr;
                    
                
                    processUninitDec($$, ch, _type);
                    
            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           } 
            sym->isField = 1;
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
            // cout<<"Inserted "<<sym->lexeme<<endl;
        }
        processPostIncre($$);
    }//here
|   FINAL Modifiers Type VariableDeclaratorList SEMICOLON      {
       // struct Node* t = new struct Node("Separator", $5);
        vector<Node*> temp;
        struct Node * g= new Node("Keyword",$1);
        if($2) {
            $2->addChildToLeft(g);
            temp.push_back($2);
        }
        else temp.push_back(g);
        
        temp.push_back($3);
        temp.push_back($4);
        $$ = new struct Node("FieldDeclaration", temp);
        verbose(v,"FINAL Modifiers Type VariableDeclaratorList SEMICOLON->FieldDeclaration");

        // cout<<"Here, field's accesss type is "<<$1->attr;
        int acc=PRIVATE_ACCESS;
        if($2&&$2->attr=="public")
            acc = PUBLIC_ACCESS;
        else if($2&&$2->attr=="protected")
            acc = PROTECTED_ACCESS;
        for(auto ch : $4->children)
        {
            if(ch->attr=="=")
                field_vars.push_back(ch->children[0]->attr);
            else
                field_vars.push_back(ch->attr);
            int _type = $3->type;
            if(ch->arrayType) _type += ch->arrayType*100  ; 
            if(ch->children.size() > 1 && ch->children[1]->arrayType) {ch->children[1]->type = $3->type + 100 * ch->children[1]->arrayType; 
                if(ch->arrayType == 1) init1DArray(ch, $3->attr);
                else if(ch->arrayType == 2) init2DArray(ch, $3->attr);
                else if(ch->arrayType == 3) init3DArray(ch, $3->attr);

            }
            else if(ch->children[1]->label == "ArrayCreationExpression") {
                    cout << ch->children[0]->varName << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2\n";
                    Quadruple* q = new Quadruple("", ch->children[1]->varName, "", ch->children[0]->varName);
                    ircode.push_back(q);
                    $$->code.push_back(q);
                    //if(ch->arrayType == 1 ) init1DArray(ch, $1->attr);
                    //else if(ch->arrayType == 2 ) init2DArray(ch, $1->attr);
                    //else if(ch->arrayType == 3 ) init3DArray(ch, $1->attr);
                }
            cout << ch->arrayType << "\n";
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[$3->attr].second, acc);
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
                if(!ch->arrayType) processFieldDec($$, ch, _type);
                //ch->children[0]->attr += "`" + to_string(scope_level);  ch->children[0]->varName = ch->children[0]->attr;
                

            }
             else {
                //ch->varName = ch->attr = ch->attr + "`" + to_string(scope_level); 
                //sym->lexeme = ch->attr;
                processUninitDec($$, ch, _type);
                
            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           }
            processPostIncre($$);
            sym->isField = 1;
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
            // cout<<"Inserted "<<sym->lexeme<<endl;
        }
    }//here
;

/***********************************************************************************************************
                                    VARIABLE DECLARATIONS
*************************************************************************************************************/

VariableDeclaratorList:
 
VariableDeclarator CommaVariableDeclarators //here 
    {
        //struct Node* t = new Node("Separator", ",");
        vector<struct Node*> temp;
        struct Node* n = new Node("VariableDeclaratorList", temp);
        n->addChild($1);
        Node* tt = $1;
        if($2) {
            for(auto it: $2->children) {
                //backpatch(tt->nextlist, tt->last + 1);
                tt = it;

                n->addChild(it);
            }
        }
        $$ = n;
        verbose(v,"VariableDeclarator CommaVariableDeclarators->VariableDeclaratorList");
    }

;

VariableDeclarator:
    VariableDeclaratorId
    {
        $$ = $1;
        verbose(v,"VariableDeclaratorId->VariableDeclarator");
    }
|   VariableDeclaratorId ASSIGN Expression
    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        // if($3)
        //     cout<<"exists exists "<<endl;
        if($3->useful == false) {
            cout<<"not 3 useful"<<endl;
            // if(!$3->children[0])
                // cout<<"Child laso exists"<<endl;
            // cout<<$3->children[0]->type<<endl;
            for(auto it : $3->children)
            {
                cout<<"pushed "<<it->label<<endl;
                temp.push_back(it);
            }
            cout<<"Came outside"<<endl;
        }
        else{
            if($3->children.size())
            cout<<"useful hai bhai "<<$3->label<<" with "<<$3->children[1]->attr<<endl;
            temp.push_back($3);}
        for(auto it: temp) {
            t->addChild(it);
        }
        
        $$ = t;
        
        $$->width1 = $3->width1;
        $$->width2 = $3->width2;
        $$->width3 = $3->width3;
        cout<<"The widths are : "<<$$->width1<<", "<<$$->width2<<", "<<$$->width3<<endl;
        $$->arrayType = $1->arrayType;
        $$->last = ircode.size() - 1;
        verbose(v,"VariableDeclaratorId ASSIGN Expression->VariableDeclarator");
        condvar = $1->attr;
        $$->isCond = isCond;
        isCond = 0;
       
    }
|   VariableDeclaratorId ASSIGN ArrayInitializer     {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        if($1->arrayType != $3->arrayType) {
            cout<<"Type Mismatch on line number "<<yylineno<<endl;
            yyerror("Error");
        }
        $$->arrayType = $3->arrayType;
        $$->width1 = $3->width1;
        $$->width2 = $3->width2;
        $$->width3 = $3->width3;
        verbose(v,"VariableDeclaratorId ASSIGN ArrayInitializer->VariableDeclaratorId");
} 
;

CommaVariableDeclarators:
    {$$ = NULL;}
|   COMMA VariableDeclarator CommaVariableDeclarators   {
        vector<struct Node*> temp;
        if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        if($3) {
            for(auto it: temp) {
                $3->addChildToLeft(it);
            }
            $$ = $3;
        }
        else $$ = new Node("VariableDeclarators", temp);
        verbose(v,"COMMA VariableDeclarator CommaVariableDeclarators->CommaVariableDeclarators");
}
;

VariableDeclaratorId:
    IDENTIFIER
    {
        $$ = new Node("Identifier", $1);
        verbose(v,"IDENTIFIER->VariableDeclaratorId");
        condvar = $1;
    }
|   IDENTIFIER Dims  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    //if($2)temp.push_back($2);
    struct Node* n = new struct Node("VariableDeclaratorId", $1, temp);
    $$ = n;
    $$->arrayType= $2->arrayType;
    cout<<"array with name : "<<$1<<" and type is "<<$$->arrayType<<" on line "<<yylineno<<endl;
    //$$->type = $1->type + 100*$2->arrayType;
    printf("\n\n%d\n\n", $2->arrayType);
    verbose(v,"IDENTIFIER Dims->VariableDeclaratorId");
} 
;


VariableInitializer:
    Expression  {
        $$ = $1;
        verbose(v,"Expression->VariableInitializer");
    }
|   ArrayInitializer    {
        $$ = $1;
        verbose(v,"ArrayInitializer->VariableInitializer");
    }
;

CommaVariableInitializers:
    {$$ = NULL;
        cout<< "epsilon -> CommaVariableInitializers\n";
    }
|  COMMA VariableInitializer CommaVariableInitializers  {
    vector<struct Node*> temp;
        if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        if($3) {
            for(auto it: temp) {
                $3->addChildToLeft(it);
            }
            $$ = $3;
        }
        else $$ = new Node("StatementExpressions", temp);
    verbose(v,"COMMA VariableInitializer CommaVariableInitializers->CommaVariableInitializers");
}
leftcurl: LEFTCURLYBRACKET {
    isarrayinit++;
    struct Node* n = new struct Node("LEFTCURLYBRACKET");
    $$ = n;
    verbose(v,"LEFTCURLYBRACKET->leftcurl");
}
ArrayInitializer:
    leftcurl RIGHTCURLYBRACKET   {
    
    struct Node* n = new struct Node("ArrayInitializer");
    $$ = n;
    $$->arrayType = 1;
    verbose(v,"LEFTCURLYBRACKET RIGHTCURLYBRACKET->ArrayInitializer");
}  
|   leftcurl VariableInitializer CommaVariableInitializers RIGHTCURLYBRACKET    {
    vector<struct Node*> temp;
    temp.push_back($2);
    if($3) {
        for(auto it: $3->children) {
            temp.push_back(it);
        }
    }
    struct Node* n = new struct Node("ArrayInitializer", temp);
    $$ = n;

    $$->arrayType = 1 + $2->arrayType;
    if($$->arrayType == 1) {
        $$->_width1 = 1 + (($3 != NULL) ? $3->children.size() : 0);
        $$->width1 = to_string($$->_width1);
    }
    else if($$->arrayType == 2) {
        $$->_width2 = $2->_width1;
        $$->_width1 = 1 + (($3 != NULL) ? $3->children.size() : 0);
        $$->width1 = to_string($$->_width1);
        $$->width2 = to_string($$->_width2);
        // cout<<"widths are "<<$$->width1<<", "<<$$->width2<<endl;
    }
    else{
        $$->_width3 = $2->_width2;
        $$->_width2 = $2->_width1;
        $$->_width1 = 1 + (($3 != NULL) ? $3->children.size() : 0);
        $$->width1 = to_string($$->_width1);
        $$->width2 = to_string($$->_width2);
        $$->width3 = to_string($$->_width3);
    }
    cout<<"The widths here3 are : "<<$$->width1<<", "<<$$->width2<<", "<<$$->width3<<endl;
    //if($3)
    verbose(v,"LEFTCURLYBRACKET VariableInitializer CommaVariableInitializers RIGHTCURLYBRACKET->ArrayInitializer");
} 
   
|   leftcurl COMMA RIGHTCURLYBRACKET   {
    struct Node* n = new struct Node("ArrayInitializer");
    $$ = n;
    $$->type = 202;
    verbose(v,"LEFTCURLYBRACKET COMMA RIGHTCURLYBRACKET->ArrayInitializer");
} 
;

/****************************************************************************************************************

                                    METHOD DECLARATIONS

****************************************************************************************************************/
MethodDeclaration:
    MethodHeader MethodBody  {
        vector<struct Node*> temp;
        temp = {$1, $2};
        struct Node* n = new struct Node("MethodDeclaration", temp);
        $$ = n;
        /*
        Quadruple* qa = new Quadruple(10, "rbp", "rsp");
        $$->code.push_back(qa);
        ircode.push_back(qa);

        Quadruple* qb = new Quadruple(11, "ebp");
        $$->code.push_back(qb);
        ircode.push_back(qb);
        */

        

        Quadruple* q = new Quadruple(7, string("endfunc") );
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;

        cout<<"currfunc is "<<currFunc<<" and tempcnt is "<<tempCnt<<"and varcnt is"<<varCnt<<endl;
        tempVars[currFunc] = varCnt;
        classfunc[currFunc] = currClass;
        varCnt = 0;
        tempCnt=0;
        //backpatch($2->nextlist, ircode.size() -1);
        
        //($1);
        //($2);
        verbose(v," MethodHeader MethodBody->MethodDeclaration");
        
    } 
|   Modifier Modifiers MethodHeader MethodBody   {
        vector<struct Node*> temp;
        if($2) {
            $2->addChildToLeft($1);
            temp.push_back($2);
        }
        else temp.push_back($1);
        temp.push_back($3);
        temp.push_back($4);
        struct Node* n = new struct Node("MethodDeclaration", temp);
        $$ = n;
        /*******************************************************
        Quadruple* qa = new Quadruple(10, "esp", "ebp");
        $$->code.push_back(qa);
        ircode.push_back(qa);

        Quadruple* qb = new Quadruple(11, "ebp");
        $$->code.push_back(qb);
        ircode.push_back(qb);
        *******************************************************/
    

        Quadruple* q = new Quadruple(7, string("endfunc") );
        $$->code.push_back(q);
        ircode.push_back(q);
        tempVars[currFunc] = varCnt;
        classfunc[currFunc] = currClass;
        varCnt = 0;
        tempCnt = 0;
        //backpatch($4->nextlist, ircode.size() -1);
        $$->last = ircode.size() - 1;
        //Quadruple* q = new Quadruple(7);
        //$$->code.push_back(q);
        verbose(v,"Modifier Modifiers MethodHeader MethodBody->MethodDeclaration");

        int acc=PUBLIC_ACCESS;
        int isStatic = 0;
        if($1->attr=="private")
            acc = PRIVATE_ACCESS;
        if($1->attr=="static")
            isStatic = 1;    
        
        if($2)
        {
            for(auto ch:$2->children)
            {
                if(ch->attr=="private")
                    acc = PRIVATE_ACCESS;
                if(ch->attr=="static")
                    isStatic=1;
            }
        }

        if(isStatic)
            static_funcs.push_back($3->children[1]->children[0]->attr);
        root->currNode->childscopes.back()->node_acc_type = acc;
    } //here
|   FINAL Modifiers MethodHeader MethodBody   {
        vector<struct Node*> temp;
        struct Node * g= new Node("Keyword",$1);
        if($2) {
            $2->addChildToLeft(g);
            temp.push_back($2);
        }
        else temp.push_back(g);
        temp.push_back($3);
        temp.push_back($4);
        struct Node* n = new struct Node("MethodDeclaration", temp);
        $$ = n;
        /*******************************************************
        Quadruple* qa = new Quadruple(10, "esp", "ebp");
        $$->code.push_back(qa);
        ircode.push_back(qa);

        Quadruple* qb = new Quadruple(11, "ebp");
        $$->code.push_back(qb);
        ircode.push_back(qb);
        *****************************************************/

        Quadruple* q = new Quadruple(7, string("endfunc") );
        $$->code.push_back(q);
        ircode.push_back(q);
        tempVars[currFunc] = varCnt;
        classfunc[currFunc] = currClass;
        varCnt = 0;
        tempCnt=0;
        //backpatch($4->nextlist, ircode.size() -1);
        $$->last = ircode.size() - 1;
        verbose(v,"Modifier Modifiers MethodHeader MethodBody->MethodDeclaration");

        int acc=PUBLIC_ACCESS;
        int isStatic = 0;   
        
        if($2)
        {
            for(auto ch:$2->children)
            {
                if(ch->attr=="private")
                    acc = PRIVATE_ACCESS;
                if(ch->attr=="static")
                    isStatic=1;
            }
        }

        if(isStatic)
            static_funcs.push_back($3->children[1]->children[0]->attr);
        root->currNode->childscopes.back()->node_acc_type = acc;
    } 
;


MethodHeader:
    Type MethodDeclarator    {
        vector<struct Node*> temp;
        temp = {$1, $2};
       
        struct Node* n = new struct Node("MethodHeader", temp);
        $$ = n;
        //($1);
        //($2);
        verbose(v,"Type MethodDeclarator->MethodHeader");
        currFunc = $2->children[0]->attr;

        vector<int> args;
        vector<string> params;
        for(int i=2; i<$2->children.size(); i+=2)
        {
            string s = $2->children[i]->attr;
            // if($2->children[i])
            if(s=="args")
                continue;
            params.push_back($2->children[i]->attr+"`"+to_string(scope_level+1));
        }

        for(int i=1; i<$2->children.size(); i+=2)
        {
            if($2->children[i+1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[$2->children[i]->attr].first+100*$2->children[i+1]->arrayType);
            }
            else
            {
                args.push_back(typeroot->typewidth[$2->children[i]->attr].first);
            }
        }

        SymNode* check = root->currNode->scope_flookup($2->children[0]->attr, args, typeroot->typewidth[$1->attr].first);
        SymNode* check2 = root->flookup($2->children[0]->attr, args, typeroot->typewidth[$1->attr].first);
        if(check2&&check2->isFinalId==true){
            cout<<"Error on line number "<<yylineno<<". Method with name "<<$2->children[0]->attr<<" has been declared before and set to final"<<endl;
            yyerror("Error");
        }
        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Method with name "<<$2->children[0]->attr<<" has been declared before"<<endl;
            yyerror("Error");
        }    
        SymNode* newf = new SymNode(root->currNode, "method", FUNC_SYM, args, typeroot->typewidth[$1->attr].first);
        scope_level++;
        if(fin==1){
            fin=0;
            newf->isFinalId=true;
        }
        root->currNode->childscopes.push_back(newf);
        root->currNode=newf;
        root->finsert($2->children[0]->attr, newf);
        root->currNode->name="method";
        for(int i=2; i<$2->children.size(); i+=2)
        {
            Symbol* sym = new Symbol($2->children[i]->attr,args[(i-1)/2], yylineno, typeroot->widths[args[(i-1)/2]%100]);
            // cout << typeroot->widths[args[(i-1)/2]] << "######################\n";
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }
        Quadruple* q = new Quadruple(6, $2->varName , params);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;

        //push ebp
        /**************************************************
        Quadruple* qa = new Quadruple(9, "ebp");
        $$->code.push_back(qa);
        ircode.push_back(qa);

        //move ebp, esp
        Quadruple* qb = new Quadruple(10, "ebp", "esp");
        $$->code.push_back(qb);
        ircode.push_back(qb);
        ***************************************************/


        // int last = $2->children.size()-1;
        // if(last%2)
        //     last--;
        // for(int i=2; i<=last; i+=2)
        // {
        //     Quadruple* q = new Quadruple(13, append_scope_level($2->children[i]->attr ));
        //     $$->code.push_back(q);
        //     ircode.push_back(q);       
        //     cout << "i = " << i << endl;     
        // }

        int space = 0;

        for(int i=0; i<args.size(); i++)
        {
            space += typeroot->widths[args[i]];
        }
        /***********************************************
        if(space>0)
        {
            Quadruple* qa = new Quadruple("+", );
            $$->code.push_back(qa);
            ircode.push_back(qa);
        }
        **********************************************/
    }   
|    VOID MethodDeclarator    {
        vector<struct Node*> temp;
        struct Node* t = new Node("Keyword", "void");
        temp = {t, $2};
        currFunc = $2->children[0]->attr;
        struct Node* n = new struct Node("MethodHeader", temp);
        $$ = n;
        //($1);
        //($2);
        verbose(v,"VOID MethodDeclarator->MethodHeader");


        // vector<int> args;
        // for(int i=1; i<$2->children.size(); i+=2)
        // {
        //     if($2->children[i+1]->arrayType > 0)
        //     {
        //         args.push_back(typeroot->typewidth[$2->children[i]->attr].first+100*$2->children[i+1]->arrayType);
        //     }
        //     else
        //     {
        //         args.push_back(typeroot->typewidth[$2->children[i]->attr].first);
        //     }
        // }

        vector<int> args;
        for(int i=1; i<$2->children.size(); i+=2)
        {
            if($2->children[i+1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[$2->children[i]->attr].first+100*$2->children[i+1]->arrayType);
            }
            else
                args.push_back(typeroot->typewidth[$2->children[i]->attr].first);
        }

        vector<string> params;
        for(int i=2; i<$2->children.size(); i+=2)
        {
            string s = $2->children[i]->attr;
            if(s=="args")
                continue;
            params.push_back($2->children[i]->attr+"`"+to_string(scope_level+1));
        }

        SymNode* check = root->currNode->scope_flookup($2->children[0]->attr, args, typeroot->typewidth[$1].first);
        SymNode* check2 = root->flookup($2->children[0]->attr, args, typeroot->typewidth[$1].first);
        if(check2&&check2->isFinalId==true){
            cout<<"Error on line number "<<yylineno<<". Method with name "<<$2->children[0]->attr<<" has been declared before and set to final"<<endl;
            yyerror("Error");
        }
        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Method with name "<<$2->children[0]->attr<<" has been declared before"<<endl;
            yyerror("Error");
        }         

        SymNode* newf = new SymNode(root->currNode, "New Function", FUNC_SYM, args, typeroot->typewidth["void"].first);
        if(fin==1){
            fin=0;
            newf->isFinalId=true;
        }
        scope_level++;
        root->currNode->childscopes.push_back(newf);
        root->currNode=newf;
        root->finsert($2->children[0]->attr, newf);
        root->currNode->name="method";
        for(int i=2; i<$2->children.size(); i+=2)
        {
            Symbol* sym = new Symbol($2->children[i]->attr,args[(i-1)/2], yylineno, typeroot->widths[args[(i-1)/2]]);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }

        Quadruple* q = new Quadruple(6, $2->varName , params);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;

        //push ebp
        /*******************************************
        Quadruple* qa = new Quadruple(9, "ebp");
        $$->code.push_back(qa);
        ircode.push_back(qa);

        //move ebp, esp
        Quadruple* qb = new Quadruple(10, "ebp", "esp");
        $$->code.push_back(qb);
        ircode.push_back(qb);
        **************************************************/

        // int last = $2->children.size()-1;
        // if(last%2)
        //     last--;
        // for(int i=2; i<=last; i+=2)
        // {
        //     Quadruple* q = new Quadruple(13, append_scope_level($2->children[i]->attr));
        //     $$->code.push_back(q);
        //     ircode.push_back(q);       
        //     cout << "i = " << i << endl;     
        // }

        int space = 0;

        for(int i=0; i<args.size(); i++)
        {
            space += typeroot->widths[args[i]];
        }
    }  
|   TypeParameters Result MethodDeclarator   {
        currFunc = $3->children[0]->attr;
        vector<struct Node*> temp;
        temp = {$1, $2, $3};
        struct Node* n = new struct Node("MethodHeader", temp);
        $$ = n;
        verbose(v,"TypeParameters Result MethodDeclarator->MethodHeader");
        vector<int> args;
        for(int i=1; i<$3->children.size(); i+=2)
        {
            if($3->children[i+1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[$3->children[i]->attr].first+100);
            }
            else
                args.push_back(typeroot->typewidth[$3->children[i]->attr].first);
        }

        SymNode* check = root->currNode->scope_flookup($3->children[0]->attr, args, typeroot->typewidth[$2->attr].first);

        if(check)
        {
            cout<<"Error on line number "<<yylineno<<". Method with name "<<$3->children[0]->attr<<" has been declared before"<<endl;
            yyerror("Error");
        }         

        SymNode* newf = new SymNode(root->currNode, "New Function", FUNC_SYM, args, typeroot->typewidth[$2->attr].first);
        root->currNode->childscopes.push_back(newf);
        root->currNode=newf;
        root->finsert($3->children[0]->attr, newf);
        scope_level++;
        root->currNode->name="method";
        for(int i=2; i<$3->children.size(); i+=2)
        {
            Symbol* sym = new Symbol($3->children[i]->attr,args[(i-1)/2], yylineno, typeroot->widths[args[(i-1)/2]]);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }
        vector<string> params;
        for(int i=2; i<$3->children.size(); i+=2)
        {
            string s = $3->children[i]->attr;
            if(s=="args")
                continue;
            params.push_back($3->children[i]->attr+"`"+to_string(scope_level+1));
        }
        Quadruple* q = new Quadruple(6, $3->varName , params);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;

        //push ebp
        /*****************************************************
        Quadruple* qa = new Quadruple(9, "ebp");
        $$->code.push_back(qa);
        ircode.push_back(qa);

        //move ebp, esp
        Quadruple* qb = new Quadruple(10, "ebp", "esp");
        $$->code.push_back(qb);
        ircode.push_back(qb);
        ******************************************************/
    }  
;

Result:
// UnannType
    Type    {
        $$ = $1;
        verbose(v,"Type->Result");
    }
|   VOID    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n = new struct Node("Result", temp);
    $$ = n;
    verbose(v,"VOID->Result");
} 
;

MethodDeclarator:
    IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    $$->attr = $$->varName = $1;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS->MethodDeclarator");
} 
|   IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
   
    // temp.push_back($3);
   
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    $$->attr = $$->varName = $1;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS->MethodDeclarator");

} 
|   IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);

    for(auto it:$3->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    $$->attr = $$->varName = $1;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS->MethodDeclarator");
} 
|   IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS         {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    
    // temp.push_back($3);
    for(auto it:$5->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    $$->attr = $$->varName = $1;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS->MethodDeclarator");
}  
|   IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS Dims    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
   
    // if($4) temp.push_back($4);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS Dims->MethodDeclarator");

    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
}    
|   IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS Dims    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    
    // temp.push_back($3);
    
    // if($6) temp.push_back($6);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    $$->attr = $$->varName = $1;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS Dims->MethodDeclarator");

    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
} 
|   IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS Dims    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    for(auto it:$3->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    // if($5)
        // temp.push_back($5);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    $$->attr = $$->varName = $1;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS Dims->MethodDeclarator");

    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
} 
|   IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS Dims    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    
    // temp.push_back($3);

    for(auto it:$5->children)
        for(auto ch : it->children)
            temp.push_back(ch); 
    // if($5)
    //     temp.push_back($5);
    struct Node* n = new struct Node("MethodDeclarator", temp);
    $$ = n;
    $$->attr = $$->varName = $1;
    verbose(v,"IDENTIFIER LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS Dims->MethodDeclarator");


    // SymNode* res = root->currNode->scope_flookup($1);
    // if(res)
    // {
    //     cout<<"Error on line number "<<yylineno<<"! Method "<<$1<<" has already been declared before"<<endl;
    //     yyerror("Error");
    // }
} 
;

ReceiverParameter:
    Type IDENTIFIER DOT THIS    {
    vector<struct Node*> temp;
    // temp.push_back($1);
    // struct Node* n1 = new struct Node("Identifier", $2);
    // temp.push_back(n1);
    
    // struct Node* n3 = new struct Node("Keyword", $4);
    // temp.push_back(n3);
    // struct Node* n = new struct Node("ReceiverParameter", temp);
    $$ = $1;
    verbose(v,"Type IDENTIFIER DOT THIS->ReceiverParameter");
} 
|   Type THIS   {
    // vector<struct Node*> temp;
    // temp.push_back($1);
    // struct Node* n1 = new struct Node("Keyword", $2);
    // temp.push_back(n1);
    // struct Node* n = new struct Node("VariableArityParameter", temp);
    $$ = $1;
    verbose(v,"Type THIS->ReceiverParameter");
} 
;

FormalParameterList:
    FormalParameter {
        vector<struct Node*> temp;
        temp.push_back($1);
        struct Node* n = new struct Node("FormalParameterList", temp);
        $$ = n;
        verbose(v,"FormalParameter->FormalParameterList");
    }
|   FormalParameter COMMA FormalParameter CommaFormalParameters {
    // vector<struct Node*> temp;
    // temp.push_back($1);
  
    // temp.push_back($3);
    // if($4)
    //     temp.push_back($4);
    // struct Node* n = new struct Node("FormalParameterList", temp);
    // $$ = n;

    vector<struct Node*> temp;
    struct Node* n = new Node("FormalParameterList", temp);
    n->addChild($1);
    n->addChild($3);
    if($4) {
        for(auto it: $4->children) {
            n->addChild(it);
        }
            //$2->addChildToLeft($1);
            //temp.push_back($2);
    }
    $$ = n;
    verbose(v,"FormalParameter COMMA FormalParameter CommaFormalParameter->FormalParameterList");
} //here
;

CommaFormalParameters:
{$$ = NULL;}
|   COMMA FormalParameter CommaFormalParameters {
    vector<struct Node*> temp;
    if($2->useful == false) {
        for(auto it: $2->children) {
            temp.push_back(it);
        }
    }
    else temp.push_back($2);
    if($3) {
        for(auto it: temp) {
            $3->addChildToLeft(it);
        }
        $$ = $3;
    }
    else $$ = new Node("FormalParameters", temp);
    verbose(v,"COMMA FormalParameter CommaFormalParameters->CommaFormalParameters");
}
;

FormalParameter:
    Type VariableDeclaratorId   {
        vector<struct Node*> temp;
        temp = {$1, $2};
        struct Node* n = new struct Node("FormalParameter", temp);
        $$ = n;
        verbose(v,"Type VariableDeclaratorId->FormalParameter");
    } 
|   VariableModifier VariableModifiers Type VariableDeclaratorId   {
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
        temp = {$3, $4};
        struct Node* n = new struct Node("FormalParameter", temp);
        $$ = n;        
        verbose(v,"VariableModifier VariableModifiers Type VariableDeclaratorId->FormalParameter");
    }   
|   VariableArityParameter  {
       $$ = $1;
       verbose(v,"VariableArityParameter->FormalParameter");
    }
;

VariableArityParameter:
    Type ELLIPSIS IDENTIFIER    {
    // vector<struct Node*> temp;
    // temp.push_back($1);
    
    // struct Node* n2 = new struct Node("Identifier", $3);
    // temp.push_back(n2);
    // struct Node* n = new struct Node("VariableArityParameter", temp);
    $$ = $1;
    verbose(v,"Type ELLIPSIS IDENTIFIER->VariableArityParameter");
} 
|   VariableModifier VariableModifiers Type ELLIPSIS IDENTIFIER {
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
    $$ = $3;
    verbose(v,"VariableModifier VariableModifiers Type ELLIPSIS IDENTIFIER->VariableArityParameter");
} 
;

VariableModifier:
    FINAL   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n = new struct Node("VariableModifier", temp);
    $$ = n;
    verbose(v,"FINAL->VariableModifier");
} 
;

VariableModifiers:
{$$ = NULL;}
|   VariableModifier VariableModifiers  {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            $$ = $2;
        }
        else $$ = new Node("VariableModifiers", temp);
        verbose(v,"VariableModifier VariableModifiers->VariableModifiers");
    }
;


MethodBody:
    Block   {

        if($1->children.size()==0 && root->currNode->childscopes.back()->returntype!=VOID_TYPE)
        {
            cout<<"Error on line number "<<yylineno<<". This method must return a result of type "<<typeroot->inv_types[root->currNode->childscopes.back()->returntype]<<endl;
            yyerror("Error");           
        }     

        if($1->children.size()>0 && $1->children[0]->label!="BlockStatements" && root->currNode->childscopes.back()->returntype!=VOID_TYPE && $1->children[$1->children.size()-1]->label!="ReturnStatement")
        {
            cout<<"Error on line number "<<yylineno<<". This method must return a result of type "<<typeroot->inv_types[root->currNode->childscopes.back()->returntype]<<endl;
            yyerror("Error");
        }

        else if($1->children.size()>0 && root->currNode->childscopes.back()->returntype!=VOID_TYPE && $1->children[0]->label=="BlockStatements" && $1->children[$1->children.size()-1]->children.back()->label!="ReturnStatement")
        {
            cout<<"Error on line number "<<yylineno<<". This method must return a result of type "<<typeroot->inv_types[root->currNode->childscopes.back()->returntype]<<endl;
            yyerror("Error");
        } 
        $$ = $1;
        verbose(v,"Block->MethodBody");
    }
|   SEMICOLON   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("","");
    temp.push_back(n1);
    struct Node* n = new struct Node("MethodBody", temp);
    $$ = n;
    processPostIncre($$);
    verbose(v,"SEMICOLON->MethodBody");
} 
;


InstanceInitializer:
     Block  {
       $$ = $1;
       verbose(v,"Block->InstanceInitializer");
    }
;

StaticInitializer:
    STATIC Block  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    struct Node* n = new struct Node("StaticInitializer", temp);
    $$ = n;
    verbose(v,"STATIC Block->StaticInitializer");
} 
;



ConstructorDeclaration:
    ConstructorDeclarator ConstructorBody   {
        vector<struct Node*> temp;
        temp = {$1, $2};
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
        $$ = n;
        Quadruple* q = new Quadruple(7, string("endfunc") );
        $$->code.push_back(q);
        ircode.push_back(q);
        tempVars[currFunc] = varCnt;
        cout<<"Adding "<<currClass<<" for "<<currFunc<<endl;
        classfunc[currFunc] = currClass;
        varCnt = 0;
        tempCnt = 0;
        $$->last = ircode.size() - 1;
        verbose(v,"ConstructorDeclarator ConstructorBody->ConstructorDeclaration");

        // cout<<"THISTISHTISHTSIOTHSOT"<<endl;
    }
|   Modifier Modifiers ConstructorDeclarator ConstructorBody     {
        vector<struct Node*> temp;
        if($2) {
            $2->addChildToLeft($1);
            temp.push_back($2);
        }
        else temp.push_back($1);
        temp.push_back($3);
        temp.push_back($4);
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
        $$ = n;
        Quadruple* q = new Quadruple(7, string("endfunc") );
        $$->code.push_back(q);
        ircode.push_back(q);
        tempVars[currFunc] = varCnt;
        cout<<"Adding "<<currClass<<" for "<<currFunc<<endl;
        classfunc[currFunc] = currClass;
        varCnt = 0;
        tempCnt = 0;
        $$->last = ircode.size() - 1;
        verbose(v,"Modifier Modifiers ConstructorDeclarator ConstructorBody ->ConstructorDeclaration");
    }
;

// Modifier:
//     PUBLIC
// |   PROTECTED 
// |   PRIVATE
// ;

ConstructorDeclarator:  
    Name LEFTPARENTHESIS RIGHTPARENTHESIS  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
    verbose(v,"Name LEFTPARENTHESIS RIGHTPARENTHESIS->ConstructorDeclarator");
    currFunc = $1->attr;

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

        SymNode* newf = new SymNode(root->currNode, "constructor", FUNC_SYM, args, typeroot->typewidth[$1->attr].first);
        root->currNode->childscopes.push_back(newf);
        root->currNode->constr_insert(args);
        root->currNode=newf;
        root->finsert($1->attr, newf);
        scope_level++;
        Quadruple* q = new Quadruple(6,  $1->varName);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;

} 
|   Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    // temp.push_back($3);
    currFunc = $1->attr;
   
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
    Quadruple* q = new Quadruple(6, $1->varName );
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS->ConstructorDeclarator");
}
|   Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    temp.push_back($1);
    temp.push_back($3);
    
    currFunc = $1->attr;
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
    scope_level++;
        vector<string> params;
        for(auto it : $3->children)
        {
            string s = it->children[1]->attr;
            if(s=="args")
                continue;
            params.push_back(it->children[1]->attr+"`"+to_string(scope_level));
        }
     Quadruple* q = new Quadruple(6, $1->varName , params);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");


        // vector<int> args;
        // for(auto it:$3->children)
        // {
        //     args.push_back(typeroot->typewidth[it->children[0]->attr].first);
        // }
        vector<int> args;
        for(auto it : $3->children)
        {
            if(it->children[1]->arrayType > 0)
            {
                args.push_back(typeroot->typewidth[it->children[0]->attr].first+100);
            }        
            else
                args.push_back(typeroot->typewidth[it->children[0]->attr].first);
                
                // cout<<"THISHTIS "<<it->children[1]->attr<<endl;
                // Quadruple* q = new Quadruple(13, append_scope_level(it->children[1]->attr ));
                // $$->code.push_back(q);
                // ircode.push_back(q); 
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
        root->finsert($1->attr, newf);
        // scope_level++;

        // for(aut$3->children)
        for(int i=0; i<$3->children.size(); i++)
        {
            Symbol* sym = new Symbol($3->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }

        // int last = $3->children.size()-1;
        // if(last%2)
        //     last--;
        // for(auto it : $3->children)
        // {
        //     Quadruple* q = new Quadruple(13, append_scope_level(it->children[1]->attr ));
        //     $$->code.push_back(q);
        //     ircode.push_back(q);       
        //     // cout << "i = " << i << endl;     
        // }
}
|   Name LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    temp.push_back($1);
    currFunc = $1->attr;
    temp.push_back($5);
   
   
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
        vector<string> params;
        for(auto it : $5->children)
        {
            string s = it->children[1]->attr;
            if(s=="args")
                continue;
            params.push_back(it->children[1]->attr+"`"+to_string(scope_level));
        }
     Quadruple* q = new Quadruple(6, $1->varName , params);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS ReceiverParameter COMMA FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");

        vector<int> args;
        for(auto it : $5->children)
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
        root->finsert($1->attr, newf);
        scope_level++;
        // for(aut$3->children)
        for(int i=0; i<$5->children.size(); i++)
        {
            Symbol* sym = new Symbol($5->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }
}
|   TypeParameters Name LEFTPARENTHESIS RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    temp.push_back($1);
    temp.push_back($2);
    currFunc = $2->attr;
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
     Quadruple* q = new Quadruple(6, $2->varName );
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS RIGHTPARENTHESIS->ConstructorDeclarator");
}
|   TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    // temp.push_back($1);
    temp.push_back($2);
    
    // temp.push_back($4);
    currFunc = $2->attr;
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
    Quadruple* q = new Quadruple(6, $2->varName );
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA RIGHTPARENTHESIS->ConstructorDeclarator");
}
|   TypeParameters Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    // temp.push_back($1);
    temp.push_back($2);
    temp.push_back($4);
   
    currFunc = $2->attr;
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
            vector<string> params;
        for(auto it : $4->children)
        {
            string s = it->children[1]->attr;
            if(s=="args")
                continue;
            params.push_back(it->children[1]->attr+"`"+to_string(scope_level));
        }
    Quadruple* q = new Quadruple(6, $2->varName , params);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");

        vector<int> args;
        for(auto it : $4->children)
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
        root->finsert($2->attr, newf);
        scope_level++;
        // for(aut$3->children)
        for(int i=0; i<$4->children.size(); i++)
        {
            Symbol* sym = new Symbol($4->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }
}  
|   TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA  FormalParameterList RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    // temp.push_back($1);
    temp.push_back($2);
    
    temp.push_back($6);
   
    for(auto it:$6->children)
        for(auto ch : it->children)
            temp.push_back(ch);
    currFunc = $2->attr;
    struct Node* n = new struct Node("ConstructorDeclarator", temp);
    $$ = n;
        vector<string> params;
        for(auto it : $6->children)
        {
            string s = it->children[1]->attr;
            if(s=="args")
                continue;
            params.push_back(it->children[1]->attr+"`"+to_string(scope_level));
        }
    Quadruple* q = new Quadruple(6, $2->varName , params);
        $$->code.push_back(q);
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
    verbose(v,"TypeParameters Name LEFTPARENTHESIS ReceiverParameter COMMA  FormalParameterList RIGHTPARENTHESIS->ConstructorDeclarator");

        vector<int> args;
        for(auto it : $6->children)
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
        root->finsert($2->attr, newf);
        scope_level++;
        // for(aut$3->children)
        for(int i=0; i<$6->children.size(); i++)
        {
            Symbol* sym = new Symbol($6->children[i]->children[1]->attr,args[i], yylineno);
            Symbol* res = root->currNode->scope_lookup(sym->lexeme);
            if(res)
            {
                cout<<"Error on line number "<<yylineno<<"! Variable "<<sym->lexeme<<" has already been declared in this scope on line number "<<sym->decl_line_no<<endl;
                yyerror("Error");
            }
                // yyerror("Variable already declared");
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }
}
;

ConstructorBody:
    LEFTCURLYBRACKET RIGHTCURLYBRACKET  {
    vector<struct Node*> temp;
    
    struct Node* n = new struct Node("ConstructorBody", temp);
    $$ = n;
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
|   LEFTCURLYBRACKET ExplicitConstructorInvocation RIGHTCURLYBRACKET    {
    vector<struct Node*> temp;
    
    temp.push_back($2);

    // if(root->currNode->name=="classextends" && root->currNode->parent->constr_args.size()==0)
    // {
    //     cout<<"Error on line number "<<yylineno<<". Explicit constructor invocation not required for this class"<<endl;
    //     yyerror("Error");
    // }
   
    struct Node* n = new struct Node("ConstructorBody", temp);
    $$ = n;
    verbose(v,"LEFTCURLYBRACKET ExplicitConstructorInvocation RIGHTCURLYBRACKET->ConstructorBody");
} 
|   LEFTCURLYBRACKET BlockStatement BlockStatements RIGHTCURLYBRACKET   {
    vector<struct Node*> temp;
   
    if($3) {
        $3->addChildToLeft($2);
        temp.push_back($3);
    }
    else temp.push_back($2);
    struct Node* n = new struct Node("ConstructorBody", temp);
    $$ = n;
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
|   LEFTCURLYBRACKET ExplicitConstructorInvocation BlockStatement BlockStatements RIGHTCURLYBRACKET {
    vector<struct Node*> temp;
    
    temp.push_back($2);
   if($4) {
        $4->addChildToLeft($3);
        temp.push_back($4);
    }
    else temp.push_back($3);

    struct Node* n = new struct Node("ConstructorBody", temp);
    $$ = n;
    verbose(v,"LEFTCURLYBRACKET ExplicitConstructorInvocation BlockStatement BlockStatements RIGHTCURLYBRACKET>ConstructorBody");
} 
;

ExplicitConstructorInvocation:
    THIS LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON     {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    
    //struct Node* n4 = new struct Node("Separator", $4);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    Quadruple* q = new Quadruple(4, "this", "0" );
    $$->code.push_back(q);

    ircode.push_back(q);
    $$->last = ircode.size() - 1;
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
|   THIS LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    
    temp.push_back($3);
   
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    
    int space = generateArgumentList($3->children, $3);
    
    Quadruple* q = new Quadruple(4, "this", to_string(space/8) );
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    verbose(v,"THIS LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON ->ExplicitConstructorInvocation");
    space = generateArgumentList($3->children, $3);
    SymNode* r = root->currNode->parent;
    vector<int> args;

    for(auto it : $3->children)
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

    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

} 
|   SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
   
    //struct Node* n4 = new struct Node("Separator", $4);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    Quadruple* q = new Quadruple(4, "super", "0" );
    $$->code.push_back(q);

    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
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
|   SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    
    temp.push_back($3);
   
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    int space = generateArgumentList($3->children, $3);
    Quadruple* q = new Quadruple(4, "super", to_string(space/8) );
    $$->code.push_back(q);

    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    verbose(v,"SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent->parent;
    if(root->currNode->parent->name != "classextends")
    {
        cout<<"Error on line number "<<yylineno<<". This class has no parent class"<<endl;
        yyerror("Error");
    }
    vector<int> args;
    for(auto ch : $3->children)
        args.push_back(ch->type);
    bool check = r->scope_constrlookup(args);
    if(!check)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }

    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
} 
|   Name DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON   {
    vector<struct Node*> temp;
    temp.push_back($1);
  
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    
   
    //struct Node* n5 = new struct Node("Separator", $6);
    //temp.push_back(n5); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    // Quadruple* q = new Quadruple(4, $1->varName + "." +"super", "0");
    // $$->code.push_back(q);

    // ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    verbose(v,"Name DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
} 
|   Name DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $5->children);
    temp.push_back(n2);
  
    temp.push_back($5);
   
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    int space = generateArgumentList( $5->children, $5);
    Quadruple* q = new Quadruple(4,  append_scope_level($1->varName)+ "." +"super", to_string(space/8) );
    $$->code.push_back(q);

    // ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Name DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
} 
|   Primary DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON    {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
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
    $$ = n;
    // Quadruple* q = new Quadruple(4, $1->varName + "." +"super", "0");
    // $$->code.push_back(q);

    // ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    verbose(v,"Primary DOT SUPER LEFTPARENTHESIS RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
} 
|   Primary DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON   {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    /*********************************************************
    struct Node* n3 = new struct Node("Separator", $4);
    temp.push_back(n3);
    
    struct Node* n4 = new struct Node("Separator", $6);
    temp.push_back(n4);
    struct Node* n5 = new struct Node("Separator", $7);
    temp.push_back(n5); 
    *********************************************************/
    temp.push_back($5);
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    int space = generateArgumentList( $5->children, $5);
    Quadruple* q = new Quadruple(4,  append_scope_level($1->varName)+ "." +"super", to_string(space/8) );
    $$->code.push_back(q);

    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
   
    $$->last = ircode.size() - 1;
    verbose(v,"Primary DOT SUPER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
} 
|   THIS LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back($3);
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;

    Quadruple* q = new Quadruple(5,  append_scope_level($3->varName));
    $$->code.push_back(q);

    q = new Quadruple("+", "stackpointer", "8", "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    ircode.push_back(q);
    q = new Quadruple(4, "this", "1" );
    $$->code.push_back(q);

    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    q = new Quadruple("-", "stackpointer", "8", "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    verbose(v,"THIS LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent;
    vector<int> args;

    args.push_back($3->type);

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

    int space = 8;
    if(space > 0) {
        Quadruple* q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer" );
        $$->code.push_back(q);
        ircode.push_back(q);
    }
}
|   SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back($3);
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    //struct Node* n4 = new struct Node("Separator", $5);
    //temp.push_back(n4);    
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    Quadruple* q = new Quadruple(5,  append_scope_level($3->varName));
    $$->code.push_back(q);
    ircode.push_back(q);

    int space = 8;
    if(space > 0) {
        Quadruple* q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
        $$->code.push_back(q);
        ircode.push_back(q);
    }

    q = new Quadruple(4, "super", "1" );
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    verbose(v,"SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");

    SymNode* r = root->currNode->parent->parent;
    if(root->currNode->parent->name != "classextends")
    {
        cout<<"Error on line number "<<yylineno<<". This class has no parent class"<<endl;
        yyerror("Error");
    }
    vector<int> args={$3->type};
    bool check = r->scope_constrlookup(args);

    if(!check)
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in this class"<<endl;
        yyerror("Error");
    }
}
|   Name DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON    {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    temp.push_back($5);
    //struct Node* n4 = new struct Node("Separator", $6);
    //temp.push_back(n4);
    //struct Node* n5 = new struct Node("Separator", $7);
    //temp.push_back(n5); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    


    Quadruple* q = new Quadruple(5,  append_scope_level($5->varName));
    $$->code.push_back(q);

     q = new Quadruple("+", "stackpointer", "8", "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    ircode.push_back(q);
    q = new Quadruple(4,  append_scope_level($1->varName)+ "." + "super", "1" );
    $$->code.push_back(q);

    ircode.push_back(q);
    $$->last = ircode.size() - 1;
     q = new Quadruple("-", "stackpointer", "8", "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    processPostIncre($$);
    verbose(v,"Name DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
} 
|   Primary DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    temp.push_back($5);
    //struct Node* n4 = new struct Node("Separator", $6);
    //temp.push_back(n4);
    //struct Node* n5 = new struct Node("Separator", $7);
    //temp.push_back(n5); 
    struct Node* n = new struct Node("ExplicitConstructorInvocation", temp);
    $$ = n;
    Quadruple* q = new Quadruple(5,  append_scope_level($5->varName));
    $$->code.push_back(q);
     q = new Quadruple("+", "stackpointer", "8", "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    ircode.push_back(q);
    q = new Quadruple(4,  append_scope_level($1->varName)+ "." + "super", "1" );
    $$->code.push_back(q);

    ircode.push_back(q);
    $$->last = ircode.size() - 1;
     q = new Quadruple("-", "stackpointer", "8", "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    processPostIncre($$);
    verbose(v,"Primary DOT SUPER LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON->ExplicitConstructorInvocation");
} 
;


ArgumentList:
    Expression COMMA Expression CommaExpressions    {
    vector<struct Node*> temp;

    if($4)
    {
        $4->addChildToLeft($3);
        $4->addChildToLeft($1);
        temp.push_back($4);
        $4->changeLabel("ArgumentList");
        $$ = $4;
    }
    else {
        temp.push_back($1);
        temp.push_back($3);
        $$ = new Node("ArgumentList", temp);
    }

    
    verbose(v,"Expression COMMA Expression CommaExpressions->ArgumentList");


} //here
;

/************************************************************************************************************

                                    BLOCKS, STATEMENTS AND PATTERNS

*************************************************************************************************************/

Block:
    LEFTCURLYBRACKET RIGHTCURLYBRACKET  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("<empty>", temp);
    $$ = n1;
    verbose(v,"LEFTCURLYBRACKET RIGHTCURLYBRACKET->Block");
}
|   LEFTCURLYBRACKET BlockStatement BlockStatements RIGHTCURLYBRACKET  
    {
         vector<struct Node*> temp;
        //struct Node* n1 = new struct Node("Identifier", $1);
        //temp.push_back(n1);
         
        if($3) {
            $3->addChildToLeft($2);
            temp.push_back($3);
        }
        else temp.push_back($2);
        //struct Node* n2 = new struct Node("Separator", $4);
        //temp.push_back(n2);
        struct Node* n = new struct Node("Block", temp);
        $$ = n;
        backpatch($2->nextlist, $2->last + 1);
        if($3) {$$->nextlist = $3->nextlist; }
        verbose(v,"LEFTCURLYBRACKET BlockStatement BlockStatements RIGHTCURLYBRACKET->Block");
    }
;

BlockStatement:
    LocalClassOrInterfaceDeclaration    {
       $$ = $1;
       verbose(v,"LocalClassOrInterfaceDeclaration->BlockStatement");
    }
|   LocalVariableDeclarationStatement   {
        $$ = $1;
        verbose(v,"LocalVariableDeclarationStatement->BlockStatement");
    }
|   Statement   {
        $$ = $1;
        verbose(v,"Statement->BlockStatement");
    }
;

BlockStatements:
    {$$ = NULL;}
|   BlockStatement BlockStatements  {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            
            $$ = $2;
        }
        else $$ = new Node("BlockStatements", temp);
        backpatch($1->nextlist, $1->last + 1);
        if($2) $$->nextlist = $2->nextlist;
        verbose(v,"BlockStatement BlockStatements->BlockStatements");
    }
;

LocalClassOrInterfaceDeclaration:
    ClassDeclaration    {
        $$ = $1;
        verbose(v,"ClassDeclaration->LocalClassOrInterfaceDeclaration");

        $$->type = VOID_TYPE;
    }
;// NormalInterfaceDeclaration

LocalVariableDeclarationStatement:
    LocalVariableDeclaration SEMICOLON  {
        vector<struct Node*> temp;
        temp.push_back($1);
        //struct Node* n1 = new struct Node("Separator", $2);
        //temp.push_back(n1);
        struct Node* n = new struct Node("LocalVariableDeclarationStatement", temp);
        $$ = n;
        verbose(v,"LocalVariableDeclaration SEMICOLON->LocalVariableDeclarationStatement");
        processPostIncre($$);
        $$->type = VOID_TYPE;
    } 
;

LocalVariableDeclaration:
    LocalVariableType VariableDeclaratorList     {
        vector<struct Node*> temp = {$1, $2};
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
        $$ = n;
        //($1);
        verbose(v,"LocalVariableType VariableDeclaratorList->LocalVariableDeclaration");
        int cc = 0;
        for(auto ch : $2->children)
        {
            int _type = $1->type;
            if(ch->arrayType && _type < 100) _type += ch->arrayType*100  ; 
            //cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << ch->children[1]->label << "\n";
            if(ch->children.size() > 1 && ch->children[1]->arrayType) {ch->children[1]->type = $1->type%100 + 100 * ch->children[1]->arrayType; 
                if(ch->children[1]->label == "ArrayInitializer") {
                    if(ch->arrayType == 1 ) init1DArray(ch, $1->attr);
                    else if(ch->arrayType == 2 ) init2DArray(ch, $1->attr);
                    else if(ch->arrayType == 3 ) init3DArray(ch, $1->attr);
                }
                
                else if(ch->children[1]->label == "ArrayCreationExpression") {
                    cout << ch->children[0]->varName << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2\n";
                    Quadruple* q = new Quadruple("", append_scope_level(ch->children[1]->varName), "", append_scope_level(ch->children[0]->varName));
                    ircode.push_back(q);
                    $$->code.push_back(q);
                    //if(ch->arrayType == 1 ) init1DArray(ch, $1->attr);
                    //else if(ch->arrayType == 2 ) init2DArray(ch, $1->attr);
                    //else if(ch->arrayType == 3 ) init3DArray(ch, $1->attr);
                }
                
            }

            // cout<<"For "<<ch->children[0]->attr<<", width is "<<typeroot->typewidth[$1->attr].second<<endl;
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[$1->attr].second);
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE) || (sym->type==DOUBLE_NUM && ch->children[1]->type==FLOAT_NUM)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                //ch->children[0]->attr += "`" + to_string(scope_level);  ch->children[0]->varName = ch->children[0]->attr;
                
                if(!ch->arrayType) processFieldDec($$, ch, _type);
                

            }
             else {
                    //ch->varName = ch->attr = ch->attr + "`" + to_string(scope_level); 
                    //sym->lexeme = ch->attr;
                    processUninitDec($$, ch, _type);
             
            }
            //sym->type += ch->arrayType * 100;
            sym->width1 = ch->width1;
            sym->width2 = ch->width2;
            sym->width3 = ch->width3;
            if(ch->arrayType > 0) {
                sym->isArray = true;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                // cout<<"Updated the widths hihi : "<<sym->width1<<sym->width2<<sym->width3<<endl;
                
                //sym->calcWidths();
           }
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
            // cout<<"For "<<sym->lexeme<<", width is "<<root->lookup(sym->lexeme)->width<<endl;
            //if(cc) backpatch((ch-1)->nextlist,(ch-1)->last + 1);
            cc++;
        }
        $$->last = ircode.size() - 1;

        int space = typeroot->widths[$1->type] * $2->children.size();

        for(auto ch : $2->children)
        {
            if(ch->attr=="=" && spacelast>0)
            {

                int space = spacelast;
                spacelast = 0;
                if(space>0)
                {
                Quadruple* q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
                $$->code.push_back(q);
                ircode.push_back(q);
                $$->last = ircode.size() - 1;
                }
            }
        }

        cout << "finished\n";
    }
|   VariableModifier VariableModifiers LocalVariableType VariableDeclaratorList {
        vector<struct Node*> temp;
         if($2) {
            $2->addChildToLeft($1);
            temp.push_back($2);
        }
        else temp.push_back($1);
        temp.push_back($3);
        temp.push_back($4);
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
        $$ = n;
        int isStatic = 0;
        verbose(v,"VariableModifier VariableModifiers LocalVariableType VariableDeclaratorList->LocalVariableDeclaration");
        for(auto ch : $4->children)
        {
            int _type = $3->type;
            if(ch->arrayType && _type < 100) _type += ch->arrayType*100  ; 
            if(ch->children.size() > 1 &&  ch->children[1]->arrayType) {ch->children[1]->type = $3->type%100 + 100 * ch->children[1]->arrayType;
                if(ch->children[1]->label == "ArrayInitializer"){    
                    if(ch->arrayType == 1) init1DArray(ch, $3->attr);
                    else if(ch->arrayType == 2) init2DArray(ch, $3->attr);
                    else if(ch->arrayType == 3) init3DArray(ch, $3->attr);
                }
                else if(ch->children[1]->label == "ArrayCreationExpression") {
                    cout << ch->children[0]->varName << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2\n";
                    Quadruple* q = new Quadruple("", ch->children[1]->varName, "", ch->children[0]->varName);
                    ircode.push_back(q);
                    $$->code.push_back(q);
                    //if(ch->arrayType == 1 ) init1DArray(ch, $1->attr);
                    //else if(ch->arrayType == 2 ) init2DArray(ch, $1->attr);
                    //else if(ch->arrayType == 3 ) init3DArray(ch, $1->attr);
                }
            
            }
            Symbol* sym = new Symbol(ch->attr, _type, yylineno, typeroot->typewidth[$3->attr].second);
            if($1->attr=="static")
            {
                isStatic=1;
            }
            if(sym->lexeme=="=")
            {
                if(!((sym->type == ch->children[1]->type) || (typeroot->categorize(sym->type)==FLOATING_TYPE && typeroot->categorize(ch->children[1]->type)==INTEGER_TYPE) || (sym->type==DOUBLE_NUM && ch->children[1]->type==FLOAT_NUM)))
                {
                    cout<<"Type Mismatch : cannot convert from "<<typeroot->inv_types[ch->children[1]->type]<<" to "<<typeroot->inv_types[sym->type]<<" on line number "<<yylineno<<endl;
                    yyerror("Error");
                    // yyerror("Type Mismatch Error! Incompatible types ");
                }
                sym->lexeme = ch->children[0]->attr;
                //ch->children[0]->attr += "`" + to_string(scope_level);  ch->children[0]->varName = ch->children[0]->attr;
                
                
                
                if(!ch->arrayType ) processFieldDec($$, ch, _type);
                

            }
             else {
                    //ch->varName = ch->attr = ch->attr + "`" + to_string(scope_level); 
                    //sym->lexeme = ch->attr;
                  
                    processUninitDec($$, ch, _type);
                   
                
            }
            if(ch->arrayType > 0) {
                sym->isArray = true;
                sym->width1 = ch->width1;
                sym->width2 = ch->width2;
                sym->width3 = ch->width3;
                //sym->calcWidths();
           }     
            sym->scope_level = scope_level;
            root->insert(sym->lexeme, sym);
        }
        $$->last = ircode.size() - 1;
        //($1);
        for(auto ch : $4->children)
        {
            if(ch->attr=="=" && spacelast>0)
            {

                int space = spacelast;
                spacelast = 0;
                if(space>0)
                {
                Quadruple* q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
                $$->code.push_back(q);
                ircode.push_back(q);
                $$->last = ircode.size() - 1;
                }
            }
        }
    }
;

LocalVariableType:
    PrimitiveType   {
       $$ = $1;
       verbose(v,"PrimitiveType->LocalVariableType");
    }
|   ReferenceType   {
      $$ = $1;
      verbose(v,"ReferenceType->LocalVariableType");
    }
|   VAR {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n = new struct Node("LocalVariableType", temp);
    $$ = n;
    verbose(v,"VAR->LocalVariableType");
}
;

Statement:
    StatementWithoutTrailingSubstatement    {
       $$ = $1;
       verbose(v,"StatementWithoutTrailingSubstatement->Statement");
    }
|   LabeledStatement    {
       $$ = $1;
       verbose(v,"LabeledStatement->Statement");
    }
|   IfThenStatement {
        $$ = $1;
        verbose(v,"IfThenStatement->Statement");
    }
|   IfThenElseStatement {
        $$ = $1;
        verbose(v,"IfThenElseStatement->Statement");
    }
|   WhileStatement  {
        $$ = $1;
        verbose(v,"WhileStatement->Statement");
    }
|   ForStatement    {
       $$ = $1;
       verbose(v,"ForStatement->Statement");
    }
;

StatementNoShortIf:
    StatementWithoutTrailingSubstatement    {
        $$ = $1;
        verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
|   LabeledStatementNoShortIf   {
       $$ = $1;
       verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
|   IfThenElseStatementNoShortIf    {
       $$ = $1;
       verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
|   WhileStatementNoShortIf {
       $$ = $1;
       verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
|   ForStatementNoShortIf   {
        $$ = $1;
        verbose(v,"StatementWithoutTrailingSubstatement->StatementNoShortIf");
    }
;

StatementWithoutTrailingSubstatement:
    Block   {
        $$ = $1;
        verbose(v,"Block->StatementWithoutTrailingSubstatement");
    }
|   EmptyStatement  {
        $$ = $1;
        verbose(v,"EmptyStatement->StatementWithoutTrailingSubstatement");
    }
|   ExpressionStatement {
       $$ = $1;
        verbose(v,"ExpressionStatement->StatementWithoutTrailingSubstatement");
    }
|   AssertStatement {
        $$ = $1;
        verbose(v,"AssertStatement->StatementWithoutTrailingSubstatement");
    }
// SwitchStatement
|   DoStatement    {
        $$ = $1;
        verbose(v,"DoStatement->StatementWithoutTrailingSubstatement");
    }
|   BreakStatement  {
        $$ = $1;
        verbose(v,"BreakStatement->StatementWithoutTrailingSubstatement");
    }
|   ContinueStatement   {
        $$ = $1;
        verbose(v,"ContinueStatement->StatementWithoutTrailingSubstatement");
    }
|   ReturnStatement {
        $$ = $1;
        verbose(v,"ReturnStatement->StatementWithoutTrailingSubstatement");
    }
// Syn/chronizedStatement
// ThrowStatement
// TryStatement
//|   YieldStatement
;


DoStatement:
    DO Statement WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS
     SEMICOLON  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n1);
    temp.push_back($2);
    temp.push_back(n2);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back($5);
    struct Node* n = new struct Node("LabeledStatement", temp);
    $$ = n;
    verbose(v,"DO Statement WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS SEMICOLON ->DoStatement");

    if($5->type==BOOL_NUM)
        $$->type = VOID_TYPE;
    else
        yyerror("Expression should be of boolean type");
    root->end_all_vulnerable();

   
    processDoWhile($$, $5, $2);
    processPostIncre($$);
}
;

EmptyStatement:
    SEMICOLON   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("<empty>");
    temp.push_back(n1);
    struct Node* n = new struct Node("EmptyStatement", temp);
    $$ = n;
    verbose(v,"SEMICOLON->EmptyStatement");

    $$->type = VOID_TYPE;
    processPostIncre($$);
}
;

LabeledStatement:
    IDENTIFIER COL Statement    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back($3);
    struct Node* n = new struct Node("LabeledStatement", temp);
    $$ = n;
    verbose(v,"IDENTIFIER COL Statement->LabeledStatement");

    $$->type = VOID_TYPE;
}
;

LabeledStatementNoShortIf:
    IDENTIFIER COL StatementNoShortIf   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Identifier", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back($3);
    struct Node* n = new struct Node("LabeledStatementNoShortIf", temp);
    $$ = n;
    verbose(v,"IDENTIFIER COL StatementNoShortIf->LabeledStatementNoShortIf");

    $$->type = VOID_TYPE;
}
;

ExpressionStatement:
    StatementExpression SEMICOLON   {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n = new struct Node("ExpressionStatement", temp);
    $$ = n;
    $$->last = $1->last;
    verbose(v,"StatementExpression SEMICOLON->ExpressionStatement");

    $$->type = VOID_TYPE;
    $$->nextlist = $1->nextlist;
    processPostIncre($$);
}
;

StatementExpression:
    Assignment  {
        $$ = $1;
        verbose(v,"Assignment->StatementExpression");
    }
|   PreIncrementExpression  {
       $$ = $1;
       verbose(v,"PreIncrementExpression->StatementExpression");
    }
|   PreDecrementExpression  {
        $$ = $1;
        verbose(v,"PreDecrementExpression->StatementExpression");
    }
|   PostIncrementExpression {
       $$ = $1;
       verbose(v,"PostIncrementExpression->StatementExpression");
    }
|   PostDecrementExpression {
        $$ = $1;
        verbose(v,"PostDecrementExpression->StatementExpression");
    }
|   MethodInvocation    {
        $$ = $1;
        verbose(v,"MethodInvocation->StatementExpression");
    }
|   ClassInstanceCreationExpression {
        $$ = $1;
        verbose(v,"ClassInstanceCreationExpression->StatementExpression");
    }
;

CommaStatementExpressions:
    {$$ = NULL;}
|   COMMA StatementExpression CommaStatementExpressions {
    vector<struct Node*> temp;
        if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        if($3) {
            for(auto it: temp) {
                $3->addChildToLeft(it);
            }
            $$ = $3;
        }
        else $$ = new Node("StatementExpressions", temp);
        verbose(v,"COMMA StatementExpression CommaStatementExpressions->CommaStatementExpressions");
}
;

IfThenStatement:
    IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        //struct Node* t2 = new Node("Separator", $2);
        //struct Node* t4 = new Node("Separator", $4);
        vector<Node* > temp = {t1, $3,$5};

        $$ = new Node("IfThenStatement", temp);
        backpatch($3->truelist, $3->last + 1);
        $$->nextlist = $3->falselist;
        $$->nextlist.insert($$->nextlist.end(), $5->nextlist.begin(), $5->nextlist.end());
        verbose(v,"IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement->IfThenStatement");

        if($3->type==BOOL_NUM && $5->type==VOID_TYPE)
            $$->type = VOID_TYPE;
        else if($3->type != BOOL_NUM)
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
;

_StatementNoShortIf:
StatementNoShortIf ELSE 
{
    struct Node* t = new Node("Keyword", $2);
    vector<Node*> temp = {$1, t};
    $$ = new Node("_StatementNoShortIf", temp);
    
    $$->nextlist = $1->nextlist;
    $$->nextlist.push_back(ircode.size());
    cout << ircode.size()<<"over here\n";
    Quadruple* q = new Quadruple(3, "", "","", "" );
    ircode.push_back(q);
    $$->code.push_back(q);
    $$->last = ircode.size() - 1;
};

IfThenElseStatement:
    IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS _StatementNoShortIf Statement
    {
        
        struct Node* t1 = new Node("Keyword", $1);
        vector<Node* > temp = {t1, $3, $5, $6};
        $$ = new Node("IfThenElseStatement", temp);
        int lastpos = $3->last + 1;
        backpatch($3->truelist, lastpos);
        backpatch($3->falselist, $5->last + 1);
        cout << "Lastpos = " << max(lastpos, ($5->last) + 1)<< "\n";
        $$->nextlist = $5->nextlist;
        $$->nextlist.insert($$->nextlist.end(), $6->nextlist.begin(), $6->nextlist.end());
        $$->last= ircode.size() - 1;
        verbose(v,"IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf ELSE Statement->IfThenElseStatement");

        if($3->type==BOOL_NUM && $5->type==VOID_TYPE && $6->type==VOID_TYPE)
            $$->type = VOID_TYPE;
        else if($3->type != BOOL_NUM)
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
;



IfThenElseStatementNoShortIf:
    IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS _StatementNoShortIf StatementNoShortIf 
    {struct Node* t1 = new Node("Keyword", $1);
        vector<Node* > temp = {t1, $3, $5, $6};
        $$ = new Node("IfThenElseStatement", temp);
        int lastpos = $3->last + 1;
        cout << "Lastpos = " << lastpos << "\n";
        backpatch($3->truelist, lastpos);
        backpatch($3->falselist, $5->last + 1);
        cout << "Lastpos = " << max(lastpos, ($5->last) + 1)<< "\n";
        $$->nextlist = $5->nextlist;
        $$->nextlist.insert($$->nextlist.end(), $6->nextlist.begin(), $6->nextlist.end());
        $$->last= ircode.size() - 1;
        verbose(v,"IF LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf ELSE StatementNoShortIf->IfThenElseStatementNoShortIf");

        if($3->type==BOOL_NUM && $5->type==VOID_TYPE && $6->type==VOID_TYPE)
            $$->type = VOID_TYPE;
        else if($3->type != BOOL_NUM)
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
;

AssertStatement:
    ASSERT Expression SEMICOLON {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    struct Node* n = new struct Node("AssertStatement", temp);
    $$ = n;
    verbose(v,"ASSERT Expression SEMICOLON ->AssertStatement");

    if($2->type==BOOL_NUM)
        $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Expression inside Assert Statement is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
}
|   ASSERT Expression COL Expression SEMICOLON  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    temp.push_back($4);
    //struct Node* n3 = new struct Node("Separator", $5);
    //temp.push_back(n3);
    struct Node* n = new struct Node("AssertStatement", temp);
    $$ = n;
    verbose(v,"ASSERT Expression COL Expression SEMICOLON->AssertStatement");

    if($2->type==BOOL_NUM && $4->type==BOOL_NUM)
        $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Expression inside Assert Statement is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
}
;

WhileStatement:
    WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($3);
    temp.push_back($5);
    struct Node* n = new struct Node("WhileStatement", temp);
    $$ = n;
    verbose(v,"WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS Statement->WhileStatement");

    if($3->type==BOOL_NUM && $5->type==VOID_TYPE)
        $$->type = VOID_TYPE;
    else if($3->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    processWhile($$, $3, $5);
}
;

WhileStatementNoShortIf:
    WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    temp.push_back($3);
    //struct Node* n3 = new struct Node("Separator", $4);
    //temp.push_back(n3);
    temp.push_back($5);
    struct Node* n = new struct Node("WhileStatementNoShortIf", temp);
    $$ = n;
    verbose(v,"WHILE LEFTPARENTHESIS Expression RIGHTPARENTHESIS StatementNoShortIf->WhileStatementNoShortIf");

    if($3->type==BOOL_NUM && $5->type==VOID_TYPE)
        $$->type = VOID_TYPE;
    else if($3->type!=BOOL_NUM)
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    
    processWhile($$, $3, $5);
}
;


ForStatement:
    BasicForStatement   {
        $$ = $1;
        verbose(v,"BasicForStatement->ForStatement");
    }
|   EnhancedForStatement    {
        $$ = $1;
        verbose(v,"EnhancedForStatement->ForStatement");
    }
;

ForStatementNoShortIf:
    BasicForStatementNoShortIf  {
       $$ = $1;
       verbose(v,"BasicForStatementNoShortIf->ForStatementNoShortIf");
    }
|   EnhancedForStatementNoShortIf   {
       $$ = $1;
       verbose(v,"EnhancedForStatementNoShortIf->BasicForStatement");
    }
;

BasicForStatement:
    FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /*************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        struct Node* t8 = new Node("Separator", $8);
        *************************************************/
        vector<Node* > temp = {t1, $3, $5, $7, $9};
        $$ = new Node("BasicForStatement", temp);

        int i;
        // pop the code for statement, forUpdate
        
        //ircode.insert(ircode.end(), $9->code.begin(), $9->code.end());
        int m = ircode.size();
        ircode.insert(ircode.end(), $7->code.begin(), $7->code.end());
        for(auto it: $9->nextlist) cout << it << " " ;
        cout << "\n";
        backpatch($9->nextlist, m);
        $9->nextlist = $7->nextlist;
        $9->last = ircode.size() - 1;
        //$9->last = $7->last;
        processWhile($$, $5, $9);
          verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if($5->type==BOOL_NUM && $9->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else if($5->type!=BOOL_NUM)
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
|   FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        *********************************************/
        vector<Node* > temp = {t1, $6};
        $$ = new Node("BasicForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");
        struct Node* n = new Node("Boolean", "true");
        n->last = $6->last - $6->code.size();
        processWhile($$, n, $6);
        if($6->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /**********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *******************************************/
        vector<Node* > temp = {t1, $3, $7};
        $$ = new Node("BasicForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");
        struct Node* n = new Node("Boolean", "true");
        n->last = $7->last - $7->code.size();
        processWhile($$, n, $7);
        if($7->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1, $4,  $7};
        $$ = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
       
        
        //$9->nextlist = $6->nextlist;
        //$8->last = $6->last;
        processWhile($$, $4, $7);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");

        if($4->type==BOOL_NUM && $7->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else if($4->type!=BOOL_NUM)
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
|   FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1,  $5, $7};
        $$ = new Node("BasicForStatement", temp);
         int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), $5->code.begin(), $5->code.end());
       
        backpatch($7->nextlist, m);
        $7->nextlist = $5->nextlist;
        $7->last = ircode.size() - 1;
        //$7->last = $5->last;
        struct Node* n = new Node("Boolean", "true");
        n->last = $$->last - $7->code.size() - $5->code.size() ;
        processWhile($$, n, $7);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if($7->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        struct Node* t7 = new Node("Separator", $7);
        **************************************************/
        vector<Node* > temp = {t1, $3, $5, $8};
        $$ = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
       
        
        //$9->nextlist = $7->nextlist;
        //$9->last = $7->last;
        processWhile($$, $5, $8);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS Statement->BasicForStatement");

        if($5->type==BOOL_NUM && $8->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else if($5->type!=BOOL_NUM)
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
|   FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        ***********************************************/
        vector<Node* > temp = {t1, $3, $6, $8};
        $$ = new Node("BasicForStatement", temp);
        int i;
        
        int m = ircode.size();
        ircode.insert(ircode.end(), $6->code.begin(), $6->code.end());
        backpatch($8->nextlist, m);
        $8->nextlist = $6->nextlist;
        struct Node* n = new Node("Boolean", "true");
        n->last = $3->last;
        $8->last = ircode.size()- 1;
        processWhile($$, n , $8);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if($8->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        **********************************************/
        vector<Node* > temp = {t1, $4, $6, $8};
        $$ = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), $6->code.begin(), $6->code.end());
        backpatch($8->nextlist, m);
        $8->nextlist = $6->nextlist;
        $8->last = ircode.size() - 1;
        //$9->last = $7->last;
        processWhile($$, $4, $8);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS Statement->BasicForStatement");

        if($4->type==BOOL_NUM && $8->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else if($4->type!=BOOL_NUM)
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
;

BasicForStatementNoShortIf:
    FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf
    {
       struct Node* t1 = new Node("Keyword", $1);
        vector<Node* > temp = {t1, $3, $5, $7, $9};
        $$ = new Node("BasicForStatement", temp);
        int i;
      
       
        int m = ircode.size();
        ircode.insert(ircode.end(), $7->code.begin(), $7->code.end());
        for(auto it: $9->nextlist) cout << it << " " ;
        cout << "\n";
        
        backpatch($9->nextlist, m);
        $9->nextlist = $7->nextlist;
       
        $9->last = ircode.size() - 1;
        processWhile($$, $5, $9);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($5->type==BOOL_NUM)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Expression inside While is not of boolean type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf
    {
       struct Node* t1 = new Node("Keyword", $1);
        /********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        *********************************************/
        vector<Node* > temp = {t1, $6};
        $$ = new Node("BasicForStatement", temp);
        struct Node* n = new Node("Boolean", "true");
        n->last = $6->last - $6->code.size();
        processWhile($$, n, $6);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($6->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf
    {
         struct Node* t1 = new Node("Keyword", $1);
        /**********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *******************************************/
        vector<Node* > temp = {t1, $3, $7};
        $$ = new Node("BasicForStatement", temp);
        struct Node* n = new Node("Boolean", "true");
        n->last = $7->last - $7->code.size();
        processWhile($$, n, $7);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($7->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf
    {
        struct Node* t1 = new Node("Keyword", $1);
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1, $4,  $7};
        $$ = new Node("BasicForStatement", temp);
        processWhile($$, $4, $7);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($4->type==BOOL_NUM && $7->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else if($4->type!=BOOL_NUM)
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
|   FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf
    {
        struct Node* t1 = new Node("Keyword", $1);
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        *************************************************/
        vector<Node* > temp = {t1,  $5, $7};
        $$ = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        //ircode.insert(ircode.end(), $7->code.begin(), $7->code.end());
        int m = ircode.size();
        ircode.insert(ircode.end(), $5->code.begin(), $5->code.end());
        backpatch($7->nextlist, m);
        $7->nextlist = $5->nextlist;
        //$7->last = $5->last;
        struct Node* n = new Node("Boolean", "true");
        n->last = $$->last - $7->code.size() - $5->code.size() ;
        processWhile($$, n, $7);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($7->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf
    {
        struct Node* t1 = new Node("Keyword", $1);
        /**************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        struct Node* t7 = new Node("Separator", $7);
        **************************************************/
        vector<Node* > temp = {t1, $3, $5, $8};
        $$ = new Node("BasicForStatement", temp);
        processWhile($$, $5, $8);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON Expression SEMICOLON RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($5->type==BOOL_NUM && $8->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else if($5->type!=BOOL_NUM)
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
|   FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf
    {
        struct Node* t1 = new Node("Keyword", $1);
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        ***********************************************/
        vector<Node* > temp = {t1, $3, $6, $8};
        $$ = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), $6->code.begin(), $6->code.end());
        backpatch($8->nextlist, m);
        $8->nextlist = $6->nextlist;
        struct Node* n = new Node("Boolean", "true");
        n->last = $3->last;
        //$9->last = $7->last;
        processWhile($$, n , $8);
        verbose(v,"FOR LEFTPARENTHESIS ForInit SEMICOLON SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($8->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else
    {
        cout<<"Error! Statement after while statement is not of void type on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    }
|   FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf
    {
        struct Node* t1 = new Node("Keyword", $1);
        /***********************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t3 = new Node("Separator", $3);
        struct Node* t5 = new Node("Separator", $5);
        struct Node* t7 = new Node("Separator", $7);
        **********************************************/
        vector<Node* > temp = {t1, $4, $6, $8};
        $$ = new Node("BasicForStatement", temp);
        int i;
        // pop the code for statement, forUpdate
        
        int m = ircode.size();
        ircode.insert(ircode.end(), $6->code.begin(), $6->code.end());
        backpatch($8->nextlist, m);
        $8->nextlist = $6->nextlist;
        //$9->last = $7->last;
        processWhile($$, $4, $8);
        verbose(v,"FOR LEFTPARENTHESIS SEMICOLON Expression SEMICOLON ForUpdate RIGHTPARENTHESIS StatementNoShortIf->BasicForStatementNoShortIf");

        if($4->type==BOOL_NUM && $8->type==VOID_TYPE)
            $$->type = VOID_TYPE;
    else if($4->type!=BOOL_NUM)
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
;

ForInit:
    StatementExpressionList {
        $$ = $1;
        verbose(v,"StatementExpressionList->ForInit");
    }
|   LocalVariableDeclaration    {
        $$ = $1;
        verbose(v,"LocalVariableDeclaration->ForInit");
    }
;

ForUpdate:
    StatementExpressionList {
        $$ = $1;
        processPostIncre($1);
        for(int i = 0; i < $$->code.size(); i++) {
            ircode.pop_back();

        }
        
        verbose(v,"StatementExpressionList->ForUpdate");
    }
;

StatementExpressionList:
    StatementExpression {
        $$ = $1;
        verbose(v,"StatementExpression->StatementExpressionList");
    }
|   StatementExpression COMMA StatementExpression CommaStatementExpressions //here
    {
       
        vector<Node*> temp = {$1, $3};
        if($4) temp.push_back($4);
        $$ = new Node("StatementExpressionList", temp);
        verbose(v,"StatementExpression COMMA StatementExpression CommaStatementExpressions->StatementExpressionList");
    }
;

EnhancedForStatement:
    FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS Statement
    {
        struct Node* t1 = new Node("Keyword", $1);
        /*************************************************
        struct Node* t2 = new Node("Separator", $2);
        struct Node* t4 = new Node("Separator", $4);
        struct Node* t6 = new Node("Separator", $6);
        **************************************************/
        
        vector<Node* > temp = {t1, $3, $5,$7};
        $$ = new Node("EnhancedForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS Statement->EnhancedForStatement");
    }
;

EnhancedForStatementNoShortIf:
    FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS StatementNoShortIf
    {
        struct Node* t1 = new Node("Keyword", $1);
        //struct Node* t2 = new Node("Separator", $2);
        //struct Node* t4 = new Node("Separator", $4);
        //struct Node* t6 = new Node("Separator", $6);
        
        vector<Node* > temp = {t1, $3, $5, $7};
        $$ = new Node("EnhancedForStatement", temp);
        verbose(v,"FOR LEFTPARENTHESIS LocalVariableDeclaration COL Expression RIGHTPARENTHESIS StatementNoShortIf->EnhancedForStatementNoShortIf");
    }
;

BreakStatement:
    BREAK SEMICOLON {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    struct Node* n = new struct Node("BreakStatement", temp);
    $$ = n;
    verbose(v,"BREAK SEMICOLON->BreakStatement");
    if(root->nlookup("while")==nullptr && root->nlookup("for")==nullptr && root->nlookup("do")==nullptr) 
    {
        cout<<"Error! Break statement outside loop on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    $$->type = VOID_TYPE;
}
|   BREAK IDENTIFIER SEMICOLON  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n2 = new struct Node("Identifier", $2);
    temp.push_back(n2);
    //struct Node* n3= new struct Node("Separator", $3);
    //temp.push_back(n3);
    struct Node* n = new struct Node("BreakStatement", temp);
    $$ = n;
    verbose(v,"BREAK IDENTIFIER SEMICOLON>BreakStatement");

    $$->type = VOID_TYPE;
}
;


ContinueStatement:
    CONTINUE SEMICOLON  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    struct Node* n = new struct Node("ContinueStatement", temp);
    $$ = n;
    verbose(v,"CONTINUE SEMICOLON->ContinueStatement");
    if(root->nlookup("while")==nullptr && root->nlookup("for")==nullptr && root->nlookup("do")==nullptr) 
    {
        cout<<"Error! Break statement outside loop on line number "<<yylineno<<endl;
        yyerror("Error");
    }
    $$->type = VOID_TYPE;
}
|   CONTINUE IDENTIFIER SEMICOLON   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n2 = new struct Node("Identifier", $2);
    temp.push_back(n2);
    //struct Node* n3= new struct Node("Separator", $3);
    //temp.push_back(n3);
    struct Node* n = new struct Node("ContinueStatement", temp);
    $$ = n;
    verbose(v,"CONTINUE IDENTIFIER SEMICOLON->ContinueStatement");

    $$->type = VOID_TYPE;
}
;

ReturnStatement:
    RETURN SEMICOLON    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Separator", $2);
    //temp.push_back(n2);
    struct Node* n = new struct Node("ReturnStatement", temp);
    $$ = n;
    
    Quadruple* q= new Quadruple(7, "return" );
    $$->code.push_back(q);
    ircode.push_back(q);
    processPostIncre($$);
    $$->last = ircode.size() - 1;
    verbose(v,"RETURN SEMICOLON->ReturnStatement");

    if(getmethodtype(root->currNode) != VOID_TYPE)
    {
        cout<<"Error on line number "<<yylineno<<". Return value should be of "<<typeroot->inv_types[root->currNode->returntype]<<" type."<<endl;
        yyerror("Error");
    }
    $$->type = VOID_TYPE;
}
|   RETURN Expression SEMICOLON {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    if($2->useful == false) {
                for(auto it: $2->children) {
                    temp.push_back(it);
                }
            }
    else temp.push_back($2);
   
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    struct Node* n = new struct Node("ReturnStatement", temp);
    $$ = n;
   
    Quadruple* q= new Quadruple(7, "return",  append_scope_level($2->varName) );
    $$->code.push_back(q);
    ircode.push_back(q);
     processPostIncre($$);
    $$->last = ircode.size() - 1;
    verbose(v,"RETURN Expression SEMICOLON->ReturnStatement");

    if(getmethodtype(root->currNode) != $2->type)
    {
        cout<<"Error on line number "<<yylineno<<". Return value should be of "<<typeroot->inv_types[root->currNode->returntype]<<" type."<<endl;
        yyerror("Error");
    }
    $$->type = VOID_TYPE;
}
;

LeftRightSquareBrackets:
    {$$ = NULL;}
|   LRSQUAREBRACKET LeftRightSquareBrackets {
    vector<struct Node*> temp;
    
     if($2) {
            if($2->useful == false) {
                for(auto it: $2->children) {
                    temp.push_back(it);
                }
            }
            else temp.push_back($2);
        }
    struct Node* n = new struct Node("LeftRightSquareBrackets", temp);
    $$ = n;
    verbose(v,"LRSQUAREBRACKET LeftRightSquareBrackets->LeftRightSquareBrackets");
}   
;


/************************************************************************************************************
                                    EXPRESSIONS
************************************************************************************************************/

Primary:
    PrimaryNoNewArray   {
        $$ = $1;
        verbose(v,"PrimaryNoNewArray->Primary");
    }
|   ArrayCreationExpression {
        $$ = $1;
        verbose(v,"ArrayCreationExpression->Primary");
    }
;

PrimaryNoNewArray:
    Literal {
        $$ = $1;
        verbose(v,"Literal->PrimaryNoNewArray");
    }
|   ClassLiteral    {
        $$ = $1;
        verbose(v,"ClassLiteral->PrimaryNoNewArray");
    }
|   THIS    {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    verbose(v,"THIS->PrimaryNoNewArray");
}
|   Name DOT THIS   {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Separator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("PrimaryNoNewArray", temp);
    $$ = n;
    verbose(v,"Name DOT THIS ->PrimaryNoNewArray");
}
|   LEFTPARENTHESIS Expression RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    //struct Node* n1 = new struct Node("Separator", $1);
    //temp.push_back(n1);
    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
    //struct Node* n2 = new struct Node("Separator", $3);
    //temp.push_back(n2);
    //struct Node* n = new struct Node("PrimaryNoNewArray", temp);
    $$ = $2;
    verbose(v,"LEFTPARENTHESIS Expression RIGHTPARENTHESIS->PrimaryNoNewArray");

    $$->type = $2->type;
}
|   ClassInstanceCreationExpression {
       $$ = $1;
       verbose(v,"ClassInstanceCreationExpression->PrimaryNoNewArray");
    }
|   FieldAccess {
        $$ = $1;
        verbose(v,"FieldAccess->PrimaryNoNewArray");
    }
|   ArrayAccess {
        $$ = $1;
        verbose(v,"ArrayAccess->PrimaryNoNewArray");
    }
|   MethodInvocation    {
        $$ = $1;
        verbose(v,"MethodInvocation->PrimaryNoNewArray");
    }
|   MethodReference {
        $$ = $1;
        verbose(v,"MethodReference->PrimaryNoNewArray");
    }
;

ClassLiteral:
    Name DOT CLASS {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("ClassLiteral", temp);
    $$ = n;
    verbose(v,"Name DOT CLASS->ClassLiteral");

    $$->type = CLASS_LITERAL;
}
|   Name LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    
    //temp.push_back($3);
    struct Node* n3 = new struct Node("Operator", $4);
    temp.push_back(n3);
    struct Node* n4 = new struct Node("Keyword", $5);
    temp.push_back(n4);
    struct Node* n = new struct Node("ClassLiteral", temp);
    $$ = n;
    verbose(v,"Name LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS");
    $$->type = CLASS_LITERAL;
}
|   PrimitiveType DOT CLASS {
    vector<struct Node*> temp;
    if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($1);
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("ClassLiteral", temp);
    $$ = n;
    $$->type = $1->type;
    verbose(v,"PrimitiveType DOT CLASS->ClassLiteral");

    $$->type = CLASS_LITERAL;

}
|   PrimitiveType LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS    {
    vector<struct Node*> temp;
    if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($1);
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    
    //temp.push_back($3);
    
    struct Node* n4 = new struct Node("Keyword", $5);
    temp.push_back(n4);
    struct Node* n = new struct Node("ClassLiteral", temp);
    $$ = n;
    $$->type = $1->type;
    verbose(v,"PrimitiveType LRSQUAREBRACKET LeftRightSquareBrackets DOT CLASS->ClassLiteral");

    $$->type = CLASS_LITERAL;

}
|   VOID DOT CLASS  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    //struct Node* n2 = new struct Node("Operator", $2);
    //temp.push_back(n2);
    struct Node* n3 = new struct Node("Keyword", $3);
    temp.push_back(n3);
    struct Node* n = new struct Node("ClassLiteral", temp);
    $$ = n;
    verbose(v,"VOID DOT CLASS->ClassLiteral");

    $$->type = CLASS_LITERAL;
}
;

NumericType:
    IntegralType    {
       $$ = $1;
       verbose(v,"IntegralType->NumericType");

    }
|   FloatingPointType   {
        $$ = $1;
        verbose(v,"FloatingPointType->NumericType");
    }
;

IntegralType:
    BYTE    {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    $$->type = BYTE_NUM;
    verbose(v,"BYTE->IntegralType");
}
|   SHORT   {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    $$->type = SHORT_NUM;
    verbose(v,"SHORT->IntegralType");
} 
|   INT {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    $$->type = INT_NUM;
    verbose(v,"INT->IntegralType");
} 
|   LONG    {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    $$->type = LONG_NUM;
    verbose(v,"LONG->IntegralType");
} 
|   CHAR    {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    $$->type = CHAR_NUM;
    verbose(v,"CHAR->IntegralType");
}
;

FloatingPointType:
    FLOAT   {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    $$->type = FLOAT_NUM;
    verbose(v,"FLOAT->FloatingPointType");
}
|   DOUBLE  {
    struct Node* n = new struct Node("Keyword", $1);
    $$ = n;
    $$->type = DOUBLE_NUM;
    verbose(v,"DOUBLE->FloatingPointType");
}
;

ClassInstanceCreationExpression:
    UnqualifiedClassInstanceCreationExpression   {
        $$ = $1;
        verbose(v,"UnqualifiedClassInstanceCreationExpression->ClassInstanceCreationExpression");
    }
|   Name DOT UnqualifiedClassInstanceCreationExpression {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Operator", $2);
    //temp.push_back(n1);
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($3);
    struct Node* n = new Node("ClassInstanceCreationExpression", temp);
    $$ = n;  
    verbose(v,"Name DOT UnqualifiedClassInstanceCreationExpression->ClassInstanceCreationExpression"); 

}
|   Primary DOT UnqualifiedClassInstanceCreationExpression  {
    vector<struct Node*> temp;
    temp.push_back($1);
    //struct Node* n1 = new struct Node("Keyword", $2);
    //temp.push_back(n1);
    if($3->useful == false) {
            for(auto it: $3->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($3);
    struct Node* n= new Node("ClassInstanceCreationExpression", temp);
    $$ = n;   
    verbose(v,"Primary DOT UnqualifiedClassInstanceCreationExpression->ClassInstanceCreationExpression");

}
;

UnqualifiedClassInstanceCreationExpression:
    NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS ClassBody  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back($4);
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    temp.push_back($6);
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    $$ = n;
    $$->varName = string("new ") + $2->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS ClassBody->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup($2->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<$2->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[$2->attr];
    for(auto it : $4->children)
    {
        args.push_back(it->type);
    }
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<$2->attr<<endl;
        yyerror("Error");
    }
    $$->type = res->type;

    int space = generateArgumentList($4->children, $4);
    // Quadruple* q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    // $$->code.push_back(q);
    // ircode.push_back(q);
    // $$->last = ircode.size() - 1;
    spacelast = space;

} 
|   NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS ClassBody {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    //struct Node* n3 = new struct Node("Operator", $4);
    //temp.push_back(n3);
    temp.push_back($5);
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    $$ = n;
    $$->varName = string("new ") + $2->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS ClassBody->UnqualifiedClassInstanceCreationExpression");

    Symbol* sym = root->lookup($2->attr);
    if(!sym)
    {
        cout<<"Error on line number "<<yylineno<<". Class named "<<$2->attr<<" has not been declared";
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[$2->attr];
    args.push_back(-1);
    if((r->default_done || r->constr_args.size()>0) && !r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<$2->attr<<endl;
        yyerror("Error");
    }
    
    $$->type = sym->type;
}
|   NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back($4);
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    $$ = n;
    $$->varName = string("new ") + $2->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup($2->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<$2->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[$2->attr];
    for(auto it : $4->children)
    {
        args.push_back(it->type);
    }
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<$2->attr<<endl;
        yyerror("Error");
    }
    $$->type = res->type;

    int space = generateArgumentList($4->children, $4);
    // Quadruple* q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    // $$->code.push_back(q);
    // ircode.push_back(q);
    // $$->last = ircode.size() - 1;
    spacelast = space;
} 
|   NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    /*****************************************
    struct Node* n2 = new struct Node("Operator", $3);
    temp.push_back(n2);
    struct Node* n3 = new struct Node("Operator", $4);
  
    temp.push_back(n3);
    *********************************************/
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    $$ = n;
    $$->varName = string("new ") + $2->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS RIGHTPARENTHESIS->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup($2->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<$2->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[$2->attr];
    args.push_back(-1);
    if((r->default_done || r->constr_args.size()>0) && !r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<$2->attr<<endl;
        yyerror("Error");
    }
    $$->type = res->type;
} 
|   NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS ClassBody  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
   // struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back($4);
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    temp.push_back($6);
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    $$ = n;
    $$->varName = string("new ") + $2->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS ClassBody->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup($2->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<$2->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[$2->attr];
    args.push_back($4->type);
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<$2->attr<<endl;
        yyerror("Error");
    }
    $$->type = res->type;


    int space = 8;
    Quadruple* q = new Quadruple(5,  append_scope_level($4->varName));

    $$->code.push_back(q);
    ircode.push_back(q);

    q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    // q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    // $$->code.push_back(q);
    // ircode.push_back(q);
    // $$->last = ircode.size() - 1;
    spacelast = space;
} 
|   NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    //struct Node* n2 = new struct Node("Operator", $3);
    //temp.push_back(n2);
    temp.push_back($4);
    //struct Node* n3 = new struct Node("Operator", $5);
    //temp.push_back(n3);
    struct Node* n = new struct Node("UnqualifiedClassInstanceCreationExpression", temp);
    $$ = n;
    $$->varName = string("new ") + $2->attr; 
    verbose(v,"NEW Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->UnqualifiedClassInstanceCreationExpression");

    Symbol* res = root->lookup($2->attr);
    if(!res)
    {
        cout<<"Error on line number "<<yylineno<<". Class with name "<<$2->attr<<" has not been declared."<<endl;
        yyerror("Error");
    }

    vector<int> args;
    SymNode* r = list_class[$2->attr];
    args.push_back($4->type);
    if(!r->scope_constrlookup(args))
    {
        cout<<"Error on line number "<<yylineno<<". Constructor with specified arguments has not been declared in class "<<$2->attr<<endl;
        yyerror("Error");
    }
    $$->type = res->type;

    int space = 8;
    Quadruple* q = new Quadruple(5,  append_scope_level($4->varName));

    $$->code.push_back(q);
    ircode.push_back(q);

    q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    // q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    // $$->code.push_back(q);
    // ircode.push_back(q);
    // $$->last = ircode.size() - 1;
    spacelast = space;
} 
;

FieldAccess:
    Primary DOT IDENTIFIER  {
    vector<struct Node*> temp;
    if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($1);
    
    struct Node* n3 = new struct Node("Identifier", $3);
    temp.push_back(n3);
    struct Node* n = new struct Node("FieldAccess", temp);
    $$ = n;
    $$->varName = $$->attr = $1->varName + "." + $3;
    verbose(v,"Primary DOT IDENTIFIER->FieldAccess");

    Symbol* res = root->lookup($3);
    if(!res)
        yyerror("Variable not declared before");
    $$->type = res->type;
} 
|   SUPER DOT IDENTIFIER   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
   
    struct Node* n3 = new struct Node("Identifier", $3);
    temp.push_back(n3);
    struct Node* n = new struct Node("FieldAccess", temp);
    $$ = n;
    $$->varName = $$->attr = string($1) + string(".") + string($3);
    verbose(v,"SUPER DOT IDENTIFIER->FieldAccess");
    SymNode* r = root->currNode;
    while(r->parent&&r->name!="classextends")
        r = r->parent;
    if(!(r->parent))
        yyerror("No parent class exists for the specified class");
    Symbol* res = r->parent->scope_lookup($3);
    if(!res)
        yyerror("Field with specified name not found");
    $$->type = res->type;
} 
|   Name DOT SUPER DOT IDENTIFIER   {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    
    struct Node* n4 = new struct Node("Identifier", $5);
    temp.push_back(n4);
    struct Node* n = new struct Node("FieldAccess", temp);
    $$ = n;
    $$->varName = $$->attr = $1->varName + "." + $3+ "." + $5;
    verbose(v,"Name DOT SUPER DOT IDENTIFIER->FieldAccess");

    SymNode* r = root->clookup($1->attr);
    if(!r)
        yyerror("No class with specified name found");
    if(!(r->parent))
        yyerror("No parent class exists for the given class");    
    Symbol* res = r->parent->scope_lookup($5);
    if(!res)
        yyerror("Field with specified name not found");
    $$->type = res->type;
}   
;

ArrayAccess:
    Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    temp.push_back($3);
   
    struct Node* n = new struct Node("ArrayAccess", temp);
    $$ = n;
    $$->attr = $1->attr;

    if(typeroot->categorize($3->type) != INTEGER_TYPE)
    {
        cout<<"Error on line number"<<yylineno<<"! Array index should be of type int"<<endl;
        yyerror("Error");
    }
    
    
    Symbol* ss = root->lookup($1->attr);
    if(!ss)
    {
        cout<<"Error on line number "<<yylineno<<". Name "<<ss->lexeme<<" has not been declared before."<<endl;
        yyerror("Error");
    }
    // cout << ss->width1 << "947t9wefih\n";
    /***************************************************************************
    Quadruple* q= new Quadruple(string("*"),  append_scope_level($3->varName), to_string(4),  resName );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName2 = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    q= new Quadruple(string("="), string( append_scope_level($1->varName)) + string("[") + resName + string("]") ,  resName2 );
    //cout << "hi\n";
    $$->code.push_back(q);
    ircode.push_back(q);
    ******************************************************/
    $$->varName = append_scope_level($1->varName) + "[" + append_scope_level($3->varName) + "]";
    $$->attr = $1->attr;
    cout << $$->code.size() << "\n";
    $$->type = root->lookup($1->varName)->type - 100;
   // $$->last = ircode.size() - 1;
    verbose(v,"Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->ArrayAccess");
    $$->cnt++;
    
}     
|   Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    temp.push_back($3);
    temp.push_back($6);

    if(typeroot->categorize($3->type) != INTEGER_TYPE || typeroot->categorize($6->type) != INTEGER_TYPE)
    {
        cout<<"Error on line number"<<yylineno<<"! Array index should be of type int"<<endl;
        yyerror("Error");
    }
   
    struct Node* n = new struct Node("ArrayAccess", temp);
    $$ = n;
    $$->attr = $1->attr;
    /**************************************************************
    string resName = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    Quadruple* q= new Quadruple(string("*"), root->lookup($1->varName)->width2, to_string(root->lookup($1->varName)->width),  resName );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName1 = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    q= new Quadruple(string("*"),  append_scope_level($3->varName), resName,  resName1 );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName2 = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    q= new Quadruple(string("*"),  append_scope_level($6->varName), to_string(root->lookup($1->varName)->width),  resName2 );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName3 = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    q= new Quadruple(string("+"), resName1, resName2,  resName3 );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName4 = string("_q") + to_string(varCnt++);
    tempCnt++;
    
    q= new Quadruple(string("="), string( append_scope_level($1->varName)) + string("[") + resName3 + string("]") ,  resName4 );
   
    $$->code.push_back(q);
    ircode.push_back(q);
    **********************************************************/
    $$->varName = append_scope_level($1->varName) + "[" + append_scope_level($3->varName)+ "][" + append_scope_level($6->varName)+ "]";
    $$->attr = $1->attr;
    //$$->last = ircode.size() - 1;
    $$->type = root->lookup($1->varName)->type - 200;
    verbose(v,"Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->ArrayAccess");
    $$->cnt++;
}
|   Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    temp.push_back($3);
    temp.push_back($6);
    temp.push_back($9);

    if(typeroot->categorize($3->type) != INTEGER_TYPE || typeroot->categorize($6->type) != INTEGER_TYPE || typeroot->categorize($9->type) != INTEGER_TYPE)
    {
        cout<<"Error on line number"<<yylineno<<"! Array index should be of type int"<<endl;
        yyerror("Error");
    }    
   
    struct Node* n = new struct Node("ArrayAccess", temp);
    $$ = n;
    $$->attr = $1->attr;
    /********************************************************
    string resName = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    Quadruple* q= new Quadruple(string("*"), root->lookup($1->varName)->width3 , to_string(root->lookup($1->varName)->width),  resName );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName1 = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    q= new Quadruple(string("*"), root->lookup($1->varName)->width2 , resName,  resName1 );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName2 = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    q= new Quadruple(string("*"),  append_scope_level($6->varName), resName,  resName2 );
    $$->code.push_back(q);
    ircode.push_back(q);
    resName = string("_q") + to_string(varCnt);
    varCnt++; tempCnt++;
    q= new Quadruple(string("*"),  append_scope_level($3->varName), resName1,  resName );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName3 = string("_q") + to_string(varCnt++);tempCnt++;
    q= new Quadruple(string("*"),  append_scope_level($9->varName), to_string(root->lookup($1->varName)->width),  resName3 );
    $$->code.push_back(q);
    ircode.push_back(q);
    string resName4 = string("_q") + to_string(varCnt++);tempCnt++;
    q= new Quadruple(string("+"), resName, resName2,  resName4 );

    $$->code.push_back(q);
    ircode.push_back(q);
    string resName5 = string("_q") + to_string(varCnt++); tempCnt++;
    q= new Quadruple(string("+"), resName4, resName3,  resName5 );

    $$->code.push_back(q);
    ircode.push_back(q);
    string resName6 = string("_q") + to_string(varCnt++); tempCnt++;
    q= new Quadruple(string("="), string( append_scope_level($1->varName)) + string("[") + resName5 + string("]") ,  resName6 );
   
    $$->code.push_back(q);
    ircode.push_back(q);
    ***********************************************/
    $$->varName =  append_scope_level($1->varName) + "[" + append_scope_level($3->varName) + "][" + append_scope_level($6->varName) + "][" + append_scope_level($9->varName);
    $$->attr = $1->attr;
    $$->type = root->lookup($1->varName)->type - 300;
    //$$->last = ircode.size() - 1;
    verbose(v,"Name LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->ArrayAccess");
    $$->cnt++;
};

MethodInvocation:
    Name LEFTPARENTHESIS RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    temp.push_back($1);

    func_names.push_back($1->attr);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    verbose(v,"Name LEFTPARENTHESIS RIGHTPARENTHESIS->MethodInvocation");

   
    string sp = spacestrip($1->attr);

    if(sp.length()<$1->attr.length() && !magic_ptr)
    {
        cout<<"Error on line number "<<yylineno<<". No such class!"<<endl;
        yyerror("Error");
    }

    SymNode* ex;
    vector<int> args;
    // if(magic_ptr->name=="Global")
    //     ex = root->flookup(sp, args);
    // else
    //     ex = magic_ptr->scope_flookup(sp, args, false);

    if(magic_ptr == origNode)
    {
        cout<<"Searching for root"<<endl;
            ex = root->flookup(sp, args);}
    else
    { 
        cout<<"Seaerching thru magic pointer and changing"<<endl;
           ex = magic_ptr->scope_flookup(sp, args, false);
        magic_ptr = origNode;
    }

    if(!ex)
    {
        cout<<"Error on line number "<<yylineno<<". No matching function to call"<<endl;
        yyerror("Error");
    }
    $$->type = ex->returntype;
    Quadruple* q;
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", append_scope_level($1->attr), to_string(0), resName );
        $$->varName = resName;
    }
    else 
    {
        q = q = new Quadruple(4, append_scope_level($1->attr), to_string(0) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);

    $$->last = ircode.size() - 1;
   
    
} 
|   Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    temp.push_back($1);
        func_names.push_back($1->attr);
  
    temp.push_back($3);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    verbose(v,"Name LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->MethodInvocation");
    int space = generateArgumentList($3->children, $3);
    vector<int> args;
    for(auto it: $3->children) {args.push_back(it->type);}
    string sp = spacestrip($1->attr);
    if(sp.length()<$1->attr.length() && !magic_ptr)
    {
        cout<<"Error on line number "<<yylineno<<". No such class!"<<endl;
        yyerror("Error");
    }

    cout<<"To search for : "<<sp<<endl;
    SymNode* ex;
    
    if(magic_ptr == origNode)
    {
        cout<<"Searching for root"<<endl;
            ex = root->flookup(sp, args);}
    else
    { 
        cout<<"Seaerching thru magic pointer and changing"<<endl;
           ex = magic_ptr->scope_flookup(sp, args, false);
        magic_ptr = origNode;
    }
    if(!ex)
    {
        cout<<"Error on line number "<<yylineno<<". No matching function to call"<<endl;
        yyerror("Error");
    }
    $$->type = ex->returntype;

    Quadruple* q;
   cout<<"Called thist thisi sish t"<<$1->attr<<endl;
    
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", append_scope_level($1->attr), to_string($3->children.size()), resName );
        $$->varName = resName;
    }
    else 
    {
        q = new Quadruple(4, append_scope_level($1->attr), to_string($3->children.size()) );
    }

    $$->code.push_back(q);
    ircode.push_back(q);

  
    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
} 
|   Primary DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Identifier", $3);
    temp.push_back(n2);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    Quadruple* q; 
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", $1->attr + string(".") + $3, to_string(0), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4, $1->attr + string(".") + $3, to_string(0) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Primary DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS->MethodInvocation");
} 
|   Primary DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Identifier", $3);
    temp.push_back(n2);
   
    temp.push_back($5);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    int space = generateArgumentList($5->children, $5);
    vector<int> args;
    for(auto it: $5->children) {
        args.push_back(it->type);
    }
    Quadruple* q;
    
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", $1->attr + string(".") + $3, to_string($5->children.size()), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4, $1->attr + string(".") + $3, to_string($5->children.size()) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Primary DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->MethodInvocation");
    
} 
|   SUPER DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    
    struct Node* n4 = new struct Node("Identifier", $3);
    temp.push_back(n4);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    Quadruple* q;
    
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "",  $1 + string(".") + $3, to_string(0), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4,  $1 + string(".") + $3, to_string(0) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
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
    SymNode* res = chck->parent->scope_flookup($3, args);
    if(!res)
        yyerror("No such function declared before.");
    $$->type = res->returntype;
} 
|   SUPER DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    
    struct Node* n4 = new struct Node("Identifier", $3);
    temp.push_back(n4);
    
    temp.push_back($5);
   
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    int space = generateArgumentList($5->children, $5);
    Quadruple* q;
    
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", $1 + string(".") + $3, to_string($5->children.size()), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4, $1 + string(".") + $3, to_string($5->children.size()) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
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
    for(auto ch : $5->children)
        args.push_back(ch->type);
    SymNode* res = chck->parent->scope_flookup($3, args);
    if(!res)
        yyerror("No such function declared before.");
    $$->type = res->returntype;
} 
|   Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
   
    struct Node* n5 = new struct Node("Identifier", $5);
    temp.push_back(n5);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    Quadruple* q ;
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", $1->attr + string(".") + $3 + string(".") + $5, to_string(0), resName );
        $$->varName = resName;
    }
    else 
    {
        q = new Quadruple(4, $1->attr + string(".") + $3 + string(".") + $5, to_string(0) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    SymNode* par = root->clookup($1->attr);
    if(!par)
    {
        cout<<"Error on line number "<<yylineno<<"! Class with name "<<$1->attr<<" has not been declared before"<<endl;
        yyerror("Error");
    }
    //     yyerror("Not the name of a class");
    if(!(par->parent))
    {
        cout<<"Error on line number "<<yylineno<<". No parent class exists for the given child class"<<endl;
        yyerror("Error");
    }
    SymNode* res = par->parent->scope_flookup($5, args);
    if(!res)
        yyerror("No such function declared before.");
    $$->type = res->returntype;
} 
|   Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    
    struct Node* n5 = new struct Node("Identifier", $5);
    temp.push_back(n5);
    
    temp.push_back($7);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    int space = generateArgumentList($7->children, $7);
    Quadruple* q;
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "",$1->attr + string(".") + $3 + string(".") + $5, to_string($7->children.size()), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4, $1->attr + string(".") + $3 + string(".") + $5, to_string($7->children.size()) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS ArgumentList RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    for(auto ch : $7->children)
        args.push_back(ch->type);
    SymNode* par = root->clookup($1->attr);
    if(!par)
    {
        cout<<"Error on line number "<<yylineno<<"! Class with name "<<$1->attr<<" has not been declared before"<<endl;
        yyerror("Error");
    }
    if(!(par->parent))
    {
        cout<<"Error on line number "<<yylineno<<". No parent class exists for the given child class"<<endl;
        yyerror("Error");
    }  
    SymNode* res = par->parent->scope_flookup($5, args);
    if(!res)
        yyerror("No such function declared before.");
    $$->type = res->returntype;
}  
|   Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    temp.push_back($1);
        func_names.push_back($1->attr);
    temp.push_back($3);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
     Quadruple* q = new Quadruple(5,  append_scope_level($3->varName));
    $$->code.push_back(q);
    ircode.push_back(q);
    int space = 8;
    if(space > 0) {
        Quadruple* q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer" );
        $$->code.push_back(q);
        ircode.push_back(q);
    }
    // $$->type = ex->returntype;

        //     string resName = string("_t") + to_string(varCnt++); tempCnt++
        // q = new Quadruple(4, "", $1->attr, to_string($3->children.size()), resName);
        // $$->varName = resName;
   
    // if($$->type != VOID_TYPE) {
    //     string resName = string("_t") + to_string(varCnt++); tempCnt++
    //     q = new Quadruple(4, "", $1->attr, to_string(1), resName);
    //     $$->varName = resName;
    // }
    // else 
    // {
    //     q = q = new Quadruple(4, $1->attr, to_string(1));
    // }

    // $$->code.push_back(q);
    // ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    
    string sp = spacestrip($1->attr);

    cout<<"This method invocation"<<endl;
    vector<int> args = {$3->type};

   
    if(sp.length()<$1->attr.length() && !magic_ptr)
    {
        cout<<"Error on line number "<<yylineno<<". No such class!"<<endl;
        yyerror("Error");
    }

    SymNode* ex;
    
    if(magic_ptr==origNode)
        ex = root->flookup(sp, args);
    else
    {
        ex = magic_ptr->scope_flookup(sp, args, false);
        magic_ptr = origNode;
}
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
    $$->type = ex->returntype;

    space = 8;
    // q = new Quadruple(5, $3->varName);

    // $$->code.push_back(q);
    // ircode.push_back(q);

    // q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
    // $$->code.push_back(q);
    // ircode.push_back(q);
    // $$->last = ircode.size() - 1;

    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", append_scope_level($1->attr), to_string(1), resName );
        $$->varName = resName;
    }
    else 
    {
        q = q = new Quadruple(4, append_scope_level($1->attr), to_string(1) );
    }

    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
} 
|   Primary DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Identifier", $3);
    temp.push_back(n2);
    
    temp.push_back($5);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    Quadruple* q = new Quadruple(5,  append_scope_level($5->varName));
    $$->code.push_back(q);
    ircode.push_back(q);
    int space = 8;
    if(space > 0) {
        Quadruple* q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
        $$->code.push_back(q);
        ircode.push_back(q);
    }
    
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", $1->attr + string(".") + $3, to_string(1), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4, $1->attr + string(".") + $3, to_string(1) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Primary DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    space = 8;
    q = new Quadruple(5,  append_scope_level($5->varName));

    $$->code.push_back(q);
    ircode.push_back(q);

    q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
} 
|   SUPER DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS    {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    
    struct Node* n4 = new struct Node("Identifier", $3);
    temp.push_back(n4);
    
    temp.push_back($5);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
     Quadruple* q = new Quadruple(5,  append_scope_level($5->varName));
    $$->code.push_back(q);
    ircode.push_back(q);
    int space = 8;

    if(space > 0) {
        Quadruple* q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
        $$->code.push_back(q);
        ircode.push_back(q);
    }
    
   
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "", $1 + string(".") + $3, to_string(1), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4, $1 + string(".") + $3, to_string(1) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
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
    for(auto ch : $5->children)
        args.push_back(ch->type);
    SymNode* res = chck->parent->scope_flookup($3, args);
    if(!res)
        yyerror("No such function declared before.");
    $$->type = res->returntype;

    space = 8;
    q = new Quadruple(5,  append_scope_level($5->varName));

    $$->code.push_back(q);
    ircode.push_back(q);

    q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;

    q = new Quadruple("-", "stackpointer", to_string(space), "stackpointer");
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
}
|   Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS   {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    
    struct Node* n5 = new struct Node("Identifier", $5);
    temp.push_back(n5);
    
    temp.push_back($7);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    Quadruple* q = new Quadruple(5,  append_scope_level($7->varName));
    $$->code.push_back(q);
    ircode.push_back(q);
    int space = 8;
    if(space > 0) {
        Quadruple* q = new Quadruple("+", "stackpointer", to_string(space), "stackpointer" );
        $$->code.push_back(q);
        ircode.push_back(q);
    }
    
    if($$->type != VOID_TYPE) {
        string resName = string("_t") + to_string(varCnt++); tempCnt++;
        q = new Quadruple(4, "",$1->attr + string(".") + $3 + string(".") + $5, to_string(1), resName );
        $$->varName = resName;
    }
    else {
        q = new Quadruple(4, $1->attr + string(".") + $3 + string(".") + $5, to_string(1) );
    }
    $$->code.push_back(q);
    ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Name DOT SUPER DOT IDENTIFIER LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    args.push_back($7->type);
    SymNode* r = root->clookup($1->attr);
    if(!r)
        yyerror("Class not declared");
    if(!(r->parent))
        yyerror("No parent class exists for the given class");
    SymNode* res = r->parent->scope_flookup($5, args);
    if(!res)
        yyerror("No such function declared before.");
    $$->type = res->returntype;
}
|   SYSTEMOUTPRINTLN LEFTPARENTHESIS Expression RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    struct Node* n1 = new Node("Keyword", $1);
    temp.push_back(n1);
    
    temp.push_back($3);
    
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    //Quadruple* q = new Quadruple(5,  append_scope_level($3->varName));
    //$$->code.push_back(q);
    //ircode.push_back(q);
    //q = new Quadruple("+", "stackpointer", "8", "stackpointer" );
    //$$->code.push_back(q);
    //ircode.push_back(q);
    Quadruple* q = new Quadruple(7, "", "print",  append_scope_level($3->varName), "" );
   
    $$->code.push_back(q);
    ircode.push_back(q);
    // if($$->type != VOID_TYPE) {
    //     string resName = string("_t") + to_string(varCnt++); tempCnt++
    //     q = new Quadruple(4, "", $1->attr, to_string(1), resName);
    //     $$->varName = resName;
    // }
    // else 
    // {
    //     q = q = new Quadruple(4, $1->attr, to_string(1));
    // }

    // $$->code.push_back(q);
    // ircode.push_back(q);
    //q = new Quadruple("-", "stackpointer", "8", "stackpointer");
    //$$->code.push_back(q);
    //ircode.push_back(q);
    $$->last = ircode.size() - 1;
    verbose(v,"Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");

    vector<int> args;
    args.push_back($3->type);
    $$->type = 8;
}
|   SYSTEMOUTPRINTLN LEFTPARENTHESIS  RIGHTPARENTHESIS {
    vector<struct Node*> temp;
    struct Node* n1 = new Node("Keyword", $1);
    temp.push_back(n1);
    struct Node* n = new struct Node("MethodInvocation", temp);
    $$ = n;
    verbose(v,"Name LEFTPARENTHESIS Expression RIGHTPARENTHESIS->MethodInvocation");
    $$->type = 8;
}
;


MethodReference:
    Name DOUBLECOLON IDENTIFIER   {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Identifier", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    $$ = n;
    $$->varName = $1->varName + string("::") + string($3); 
    verbose(1,"Name DOUBLECOLON IDENTIFIER->MethodReference");
}    
|   Primary DOUBLECOLON IDENTIFIER  {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Identifier", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    $$ = n;
    $$->varName = $1->varName + string("::") + string($3); 
    verbose(1,"Primary DOUBLECOLON IDENTIFIER->MethodReference");
}      
|   ArrayType DOUBLECOLON IDENTIFIER   {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Identifier", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    $$ = n;
    verbose(1,"ArrayType DOUBLECOLON IDENTIFIER->MethodReference");
}       
|   SUPER DOUBLECOLON IDENTIFIER  {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    
    struct Node* n3 = new struct Node("Identifier", $3);
    temp.push_back(n3);
    struct Node* n = new struct Node("MethodReference", temp);
    $$ = n;
    //$$->varName = $1->varName + string("::") + string($3); 
    verbose(1,"SUPER DOUBLECOLON IDENTIFIER->MethodReference");
}      
|   Name DOT SUPER DOUBLECOLON IDENTIFIER   {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    
    struct Node* n4 = new struct Node("Identifier", $5);
    temp.push_back(n4);

    struct Node* n = new struct Node("MethodReference", temp);
    $$ = n;
    $$->varName = $1->varName + string(".") + string("super") + string("::") + string($3); 
    verbose(1,"Name DOT SUPER DOUBLECOLON IDENTIFIER->MethodReference");
}       
|   Name DOUBLECOLON NEW    {
    vector<struct Node*> temp;
    temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    $$ = n;
    $$->varName = $1->varName + string("::") + $3; 
    verbose(1,"Name DOUBLECOLON NEW->MethodReference");
}
|   ArrayType DOUBLECOLON NEW   {
    vector<struct Node*> temp;
    if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($1);
    
    struct Node* n2 = new struct Node("Keyword", $3);
    temp.push_back(n2);
    struct Node* n = new struct Node("MethodReference", temp);
    $$ = n;
    verbose(1,"ArrayType DOUBLECOLON NEW->MethodReference");
}
;

ArrayCreationExpression:
    NEW PrimitiveType DimExpr_   {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    temp.push_back($2);
    if($3) temp.push_back($3);
    struct Node* n = new struct Node("ArrayCreationExpression", temp);
    $$ = n;
    $$->width1 = $3->width1;
    $$->width2 = $3->width2;
    $$->width3 = $3->width3;
    cout<<"The widths here1 are : "<<$$->width1<<", "<<$$->width2<<", "<<$$->width3<<endl;
    $$->arrayType = $3->arrayType;
    $$->type = $$->arrayType*100 + $2->type;
    $$->varName = string("new ") + $2->attr + $3->attr ;
    verbose(v,"NEW PrimitiveType DimExpr_ ->ArrayCreationExpression");
}
|   NEW Name DimExpr_   {
        vector<struct Node*> temp;
        struct Node* n1 = new struct Node("Keyword", $1);
        temp.push_back(n1);
        temp.push_back($2);
        if($3) temp.push_back($3);
        struct Node* n = new struct Node("ArrayCreationExpression", temp);
        $$ = n;
        $$->width1 = $3->width1;
        $$->width2 = $3->width2;
        $$->width3 = $3->width3;
        $$->arrayType = $3->arrayType;
        $$->type = $$->arrayType*100 + $2->type;
        $$->varName = string("new ") + $2->attr +$3->attr;
        verbose(v,"NEW Name DimExpr_->ArrayCreationExpression");
    }
|   NEW PrimitiveType Dims ArrayInitializer {
    vector<struct Node*> temp;
    struct Node* n1 = new struct Node("Keyword", $1);
    temp.push_back(n1);
    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
    if($3) temp.push_back($3);
    temp.push_back($4);
    struct Node* n = new struct Node("ArrayCreationExpression", temp);
    $$ = $4;
    $$->width1 = $4->width1;
    $$->width2 = $4->width2;
    $$->width3 = $4->width3;
    cout<<"The widths here2 are : "<<$$->width1<<", "<<$$->width2<<", "<<$$->width3<<endl;
    $$->arrayType = $3->arrayType;
    $$->type = $$->arrayType*100 + $2->type;
    $$->varName = string("new ") + $2->attr  + $3->attr;
    verbose(v,"NEW PrimitiveType Dims ArrayInitializer->ArrayCreationExpression");
}

;
DimExpr_:
    LRSQUAREBRACKET{
    $$ = NULL;
    verbose(v,"LRSQUAREBRACKET->DimExpr_");
}
|   LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET {
    vector<struct Node*> temp;

    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
   
    struct Node* n = new struct Node("DimExpr_", temp);
    $$ = n;
    $$->width1 = $2->varName;
    $$->arrayType = 1;
    $$->attr = string($1) + $2->varName + string($3);
    //$$->width1 = stoi($2->attr);
    cout << "\n\nvarname ="<< $2->varName << "\n\n";
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->DimExpr_");
}
|   LRSQUAREBRACKET LRSQUAREBRACKET   {
        $$ = NULL;
        verbose(v,"LRSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
    }
|   LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET    {
    vector<struct Node*> temp;
    
    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
   
    struct Node* n = new struct Node("DimExpr_", temp);
    $$ = n;
    $$->arrayType = 2;
    $$->width1 = $2->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
|   LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET    {
    vector<struct Node*> temp;
    
    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
    if($5->useful == false) {
            for(auto it: $5->children) {
                temp.push_back(it);
            }
        }
      else temp.push_back($5);  
    struct Node* n = new struct Node("DimExpr_", temp);
    $$ = n;
    $$->arrayType = 2;
    $$->attr = string($1) + $2->varName + string($3) + string($4) + $5->varName + string($6);
    $$->width1 = $2->varName;
    $$->width2 = $5->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression->DimExpr_");
}
|   LRSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET    {
    $$ = NULL;
    verbose(v,"LRSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
|   LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET {
    vector<struct Node*> temp;
   
    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
    
   
    struct Node* n = new struct Node("DimExpr_", temp);
    $$ = n;
   $$->arrayType = 3;
   
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
|   LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET  {
    vector<struct Node*> temp;
    
    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
    
    if($5->useful == false) {
            for(auto it: $5->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($5);
    
    struct Node* n = new struct Node("DimExpr_", temp);
    $$ = n;
    $$->arrayType = 3;
    $$->width1 = $2->varName;
    $$->width2 = $5->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LRSQUAREBRACKET->DimExpr_");
}
|   LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET   {
    vector<struct Node*> temp;
   
    if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($2);
   
   
    if($5->useful == false) {
            for(auto it: $5->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($5);
   
    if($8->useful == false) {
            for(auto it: $8->children) {
                temp.push_back(it);
            }
        }
    else temp.push_back($8);
   
    struct Node* n = new struct Node("DimExpr_", temp);
    $$ = n;
    $$->arrayType = 3;
    $$->attr = string($1) + $2->varName + string($3) + string($4) + $5->varName + string($6) + string($7) + $8->varName + string($9);
    $$->width1 = $2->varName;
    $$->width2 = $5->varName;
    $$->width3 = $8->varName;
    verbose(v,"LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET LEFTSQUAREBRACKET Expression RIGHTSQUAREBRACKET->DimExpr_");
}
;
Dims:
    LRSQUAREBRACKET    {
       
        $$ = new Node("Dims");
        $$->arrayType += 1;
    
        verbose(v,"LRSQUAREBRACKET->Dims");
    }
|   LRSQUAREBRACKET Dims   {
        
        
        $$ = new Node("Dims");
        $$->arrayType = $2->arrayType+ 1;
        verbose(v,"LRSQUAREBRACKET Dims->Dims");
    }
;
Expression:

    AssignmentExpression    {
        $$ = $1;
        verbose(v,"AssignmentExpression->Expression");
    }
;

CommaExpressions:
    {$$ = NULL;}
|   COMMA Expression CommaExpressions   {
        vector<struct Node*> temp;
            if($2->useful == false) {
                for(auto it: $2->children) {
                    temp.push_back(it);
                }
            }
            else temp.push_back($2);
            if($3) {
                for(auto it: temp) {
                    $3->addChildToLeft(it);
                }
                $$ = $3;
            }
            else $$ = new Node("Expressions", temp);
            verbose(v,"COMMA Expression CommaExpressions->CommaExpressions");
}
;


AssignmentExpression:
    ConditionalExpression   {
        $$ = $1;
        verbose(v,"ConditionalExpression->AssignmentExpression");
    }
|   Assignment  {
        $$ = $1;
        verbose(v,"Assignment->AssignmentExpression");
    }
;

Assignment:
    LeftHandSide AssignmentOperatorEqual Expression {
        cout << "here\n";
        vector<Node*> temp;
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        cout << "Reached here\n";
        //$2 = new Node("Operator", $2);
        for(auto it: temp) {
            $2->addChildToLeft(it);
        }
        cout << "Reached here2\n";
        temp.clear();
        //temp.push_back(t2);
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        cout << "Reached here3\n";
        for(auto it: temp) {
            $2->addChild(it);
        }

        cout<<"HERE MF"<<endl;
        //struct Node* n = new struct Node("ExclusiveOrExpression", temp);
        $$ = $2;
        int lhstype=$1->type, rhstype=$3->type;

        cout<<"Types are "<<lhstype<<", "<<rhstype<<endl;
        if($1->type==$3->type || (typeroot->categorize(lhstype)==FLOATING_TYPE && typeroot->categorize(rhstype)==INTEGER_TYPE) || (lhstype==DOUBLE_NUM && rhstype==FLOAT_NUM))
        {
            $$->type = $1->type;
            $$->last = ircode.size() - 1;
            cout << $3->label << "\n";
            if($3->label == string("ArrayCreationExpression")) {
                cout << "\n\nhere\n\n";
                cout << $3->width1 <<"\n";
                Symbol* sym = root->currNode->scope_lookup($1->varName);
                
                if(sym) {
                    sym->width1 = $3->width1;
                    sym->width2 = $3->width2;
                    sym->width3 = $3->width3;
                    if(sym->isField == 1) {
                        $1->varName = "this."+ $1->varName;
                        $1->attr = $1->varName;
                    }
                }
            }
        }
        else
        {
            cout<<"Error on line number "<<yylineno<<"! Type Mismatch : Cannot convert from "<<typeroot->inv_types[$3->type]<<" to "<<typeroot->inv_types[$1->type]<<endl;
            yyerror("Error");
        }
        Quadruple * q = new Quadruple(string("="),  append_scope_level($3->varName),  append_scope_level($1->varName));
        if(isCond == 0) $$->code.push_back(q);
        if(isCond == 0)ircode.push_back(q);
        $$->last = ircode.size() - 1;
        verbose(v,"LeftHandSide AssignmentOperator Expression->Assignment");
        cout << $1->type << " " << $3->type << endl;

        

        
        isCond = 0;
        $$->nextlist = $3->nextlist;
    }  
|   LeftHandSide AssignmentOperator Expression  {
        vector<Node*> temp;
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            $2->addChildToLeft(it);
        }
        
        temp.clear();
        //temp.push_back(t2);
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            $2->addChild(it);
        }
        //struct Node* n = new struct Node("ExclusiveOrExpression", temp);
        $$ = $2;
        processAssignment($1, $2, $3, $$);
        cout<< "handled\n";
        $$->last = ircode.size() - 1;
        verbose(v,"LeftHandSide AssignmentOperator Expression->Assignment");
        cout << $1->type << " " << $3->type << endl;

       

        int lhstype=$1->type, rhstype=$3->type;

        cout<<"Types are "<<lhstype<<", "<<rhstype<<endl;
        if($1->type==$3->type || (typeroot->categorize(lhstype)==FLOATING_TYPE && typeroot->categorize(rhstype)==INTEGER_TYPE) || (lhstype==DOUBLE_NUM && rhstype==FLOAT_NUM))
        {
            $$->type = $1->type;
            $$->last = ircode.size() - 1;
            cout << $3->label << "\n";
            if($3->label == string("ArrayCreationExpression")) {
                cout << "\n\nhere\n\n";
                cout << $3->width1 <<"\n";
                Symbol* sym = root->currNode->scope_lookup($1->varName);
                
                if(sym) {
                    sym->width1 = $3->width1;
                    sym->width2 = $3->width2;
                    sym->width3 = $3->width3;
                    if(sym->isField == 1) {
                        $1->varName = "this."+ $1->varName;
                        $1->attr = $1->varName;
                    }
                }
            }
        }
        else
        {
            cout<<"Error! Type Mismatch : Cannot convert from "<<typeroot->inv_types[$3->type]<<" to "<<typeroot->inv_types[$1->type]<<endl;
            yyerror("Error");
        }
    }
;

LeftHandSide:
    Name    {
    $$ = $1;
    verbose(v,"Name->LeftHandSide");
    }
|   FieldAccess {
        $$ = $1;
        verbose(v,"FieldAccess->LeftHandSide");
    }
|   ArrayAccess {
       
       $$ = $1;
       
       
       verbose(v,"ArrayAccess->LeftHandSide");
    }
;

AssignmentOperatorEqual:
    ASSIGN {
        struct Node* n = new struct Node("Operator", $1);
        $$ = n;
        verbose(v,"->AssignmentOperator");
    }
;

AssignmentOperator:
    MULTEQUAL   {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"MULTEQUAL->AssignmentOperator");
}
|   DIVEQUAL    {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"DIVEQUAL->AssignmentOperator");
}
|   MODEQUAL    {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"MODEQUAL->AssignmentOperator");
}
|   PLUSEQUAL   {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"PLUSEQUAL->AssignmentOperator");
}
|   MINUSEQUAL  {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"MINUSEQUAL->AssignmentOperator");
}
|   LSHIFTEQUAL {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"LSHIFTEQUAL->AssignmentOperator");
}
|   RSHIFTEQUAL {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"RSHIFTEQUAL->AssignmentOperator");
}
|   UNRSHIFTEQUAL   {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"UNRSHIFTEQUAL->AssignmentOperator");
}
|   BANDEQUAL   {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"BANDEQUAL->AssignmentOperator");
}
|   BOREQUAL    {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"BOREQUAL->AssignmentOperator");
}
|   BXOREQUAL   {
    struct Node* n = new struct Node("Operator", $1);
    $$ = n;
    verbose(v,"BXOREQUAL->AssignmentOperator");
}
;

ColConditional:
    Expression COL 
    {
        //$$ = new Node("ColConditional", )
        for(int i = 0; i < $1->code.size(); i++) {
            ircode.pop_back();
        }
        Quadruple* q = new Quadruple("",  append_scope_level($1->varName), "", condvar );
        $1->code.push_back(q);
        
        ircode.insert(ircode.end(), $1->code.begin(), $1->code.end());
        processPostIncre($1);
        $1->last = ircode.size() - 1;
        $$ = $1;
        $$->nextlist = $1->nextlist;
        $$->nextlist.push_back(ircode.size());
        //cout << ircode.size()<<"over here\n";
        q = new Quadruple(3, "", "","", "" );
        ircode.push_back(q);
        $$->code.push_back(q);
        $$->last = ircode.size() - 1;
        

    }
;

ConditionalExpression:
    ConditionalOrExpression {
        $$ = $1;
        verbose(v,"ConditionalOrExpression->ConditionalExpression");
    }
|   ConditionalOrExpression QUES ColConditional ConditionalExpression  {
        vector<struct Node*> temp;
        
        struct Node* t1 = new struct Node("Operator", $2);
        //struct Node* t2 = new struct Node("Operator", $4);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        temp.push_back(t1);
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
       // temp.push_back(t2);
        if($4->useful == false) {
            for(auto it : $4->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($4);
        struct Node* n = new struct Node("ConditionalExpression", temp);
        $$ = n;
        
        for(int i = 0; i < $4->code.size(); i++) {
            ircode.pop_back();
        }
        Quadruple* q = new Quadruple("",  append_scope_level($4->varName), "", condvar );
        cout << "called print\n";
        //q->print();
        cout << "hey me\n";
        $4->code.push_back(q);
        ircode.insert(ircode.end(), $4->code.begin(), $4->code.end());
        int lastpos = $1->last + 1;
        backpatch($1->truelist, lastpos);
        backpatch($1->falselist, $3->last + 1);
        //cout << "Lastpos = " << max(lastpos, ($5->last) + 1)<< "\n";
        $$->nextlist = $3->nextlist;
        $$->nextlist.insert($$->nextlist.end(), $4->nextlist.begin(), $4->nextlist.end());
        $$->last= ircode.size() - 1;
        verbose(v,"ConditionalOrExpression QUES Expression COL ConditionalExpression->ConditionalExpression");
        $$->type = $4->type;
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
;

ConditionalOrExpression:
    ConditionalAndExpression    {
        $$ = $1;
        verbose(v,"ConditionalAndExpression->ConditionalOrExpression");
    }
|   ConditionalOrExpression OR ConditionalAndExpression {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        backpatch($1->falselist, $1->last + 1);
        $$->truelist = $1->truelist;
        $$->truelist.insert($$->truelist.end(), $3->truelist.begin(), $3->truelist.end());
        $$->falselist = $3->falselist;
        cout << "Lastpos = " << $$->last<< "\n";
        verbose(v,"ConditionalOrExpression OR ConditionalAndExpression->ConditionalOrExpression");
        //ircode.push_back($1->code);
        
        if($1->type==BOOL_NUM && $3->type==BOOL_NUM)
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

ConditionalAndExpression:
    InclusiveOrExpression   {
        $$ = $1;
        verbose(v,"InclusiveOrExpression->ConditionalAndExpression");
    }
|   ConditionalAndExpression AND InclusiveOrExpression  {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        backpatch($1->truelist, $1->last + 1);
        cout << $1->label << "\n";
        $$->truelist = $3->truelist;
        $$->falselist = $1->falselist;
        $$->falselist.insert($$->falselist.end(), $3->falselist.begin(), $3->falselist.end());
        verbose(v,"ConditionalAndExpression AND InclusiveOrExpression->ConditionalAndExpression");
        
        if($1->type==BOOL_NUM && $3->type==BOOL_NUM)
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

InclusiveOrExpression:
    ExclusiveOrExpression   {
        $$ = $1;
        verbose(v,"ExclusiveOrExpression->InclusiveOrExpression");
    }
|   InclusiveOrExpression BOR ExclusiveOrExpression {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes,$2);
        verbose(v,"InclusiveOrExpression BOR ExclusiveOrExpression->InclusiveOrExpression");

        if($1->type==INT_NUM && $3->type==INT_NUM)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

ExclusiveOrExpression:
    AndExpression   {
        $$ = $1;
        verbose(v,"AndExpression->ExclusiveOrExpression");
    }
|   ExclusiveOrExpression BXOR AndExpression    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        verbose(v,"ExclusiveOrExpression BXOR AndExpression->ExclusiveOrExpression");

        if($1->type==INT_NUM && $3->type==INT_NUM)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

AndExpression:
    EqualityExpression  {
       $$ = $1;
       verbose(v,"EqualityExpression->AndExpression");
    }
|   AndExpression BAND EqualityExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        verbose(v,"AndExpression BAND EqualityExpression->AndExpression");

        if($1->type==INT_NUM && $3->type==INT_NUM)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

EqualityExpression:
    RelationalExpression    {
        $$ = $1;
        verbose(v,"RelationalExpression->EqualityExpression");
    }
|   EqualityExpression EQUAL RelationalExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*>nodes = {$$, $1, $3};
        processRelational(nodes, $2);
        verbose(v,"EqualityExpression EQUAL RelationalExpression->EqualityExpression");
        if((typeroot->categorize($1->type)==INTEGER_TYPE || typeroot->categorize($1->type)==FLOATING_TYPE) && (typeroot->categorize($3->type)==INTEGER_TYPE || typeroot->categorize($3->type)==FLOATING_TYPE))
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   EqualityExpression NEQUAL RelationalExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*>nodes = {$$, $1, $3};
        processRelational(nodes, $2);
        verbose(v,"EqualityExpression NEQUAL RelationalExpression->EqualityExpression");
        
        if((typeroot->categorize($1->type)==INTEGER_TYPE || typeroot->categorize($1->type)==FLOATING_TYPE) && (typeroot->categorize($3->type)==INTEGER_TYPE || typeroot->categorize($3->type)==FLOATING_TYPE))
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

RelationalExpression:
    ShiftExpression {
        $$ = $1;
        verbose(v,"ShiftExpression->RelationalExpression");
    }
|   RelationalExpression LSS ShiftExpression     {
         vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*>nodes = {$$, $1, $3};
        processRelational(nodes, $2);
        verbose(v,"RelationalExpression LSS ShiftExpression->RelationalExpression");

        if((typeroot->categorize($1->type)==INTEGER_TYPE || typeroot->categorize($1->type)==FLOATING_TYPE) && (typeroot->categorize($3->type)==INTEGER_TYPE || typeroot->categorize($3->type)==FLOATING_TYPE))
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   RelationalExpression GRT ShiftExpression     {
         vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;

        vector<Node*>nodes = {$$, $1, $3};
        processRelational(nodes, $2);
        if((typeroot->categorize($1->type)==INTEGER_TYPE || typeroot->categorize($1->type)==FLOATING_TYPE) && (typeroot->categorize($3->type)==INTEGER_TYPE || typeroot->categorize($3->type)==FLOATING_TYPE))
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   RelationalExpression LEQ ShiftExpression    {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*>nodes = {$$, $1, $3};
        processRelational(nodes, $2);
        verbose(v,"RelationalExpression LEQ ShiftExpression->RelationalExpression");

        if((typeroot->categorize($1->type)==INTEGER_TYPE || typeroot->categorize($1->type)==FLOATING_TYPE) && (typeroot->categorize($3->type)==INTEGER_TYPE || typeroot->categorize($3->type)==FLOATING_TYPE))
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   RelationalExpression GEQ ShiftExpression    {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*>nodes = {$$, $1, $3};
        processRelational(nodes, $2);
        verbose(v,"RelationalExpression GEQ ShiftExpression->RelationalExpression");


        if((typeroot->categorize($1->type)==INTEGER_TYPE || typeroot->categorize($1->type)==FLOATING_TYPE) && (typeroot->categorize($3->type)==INTEGER_TYPE || typeroot->categorize($3->type)==FLOATING_TYPE))
            $$->type = BOOL_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   InstanceofExpression    {
        $$ = $1;
        verbose(v,"InstanceofExpression->RelationalExpression");
    }
;

InstanceofExpression:
    RelationalExpression INSTANCEOF ReferenceType   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Keyword", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        temp.push_back(t);
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        struct Node* n = new struct Node("InstanceofExpression", temp);
        $$ = n;
        verbose(v,"RelationalExpression INSTANCEOF ReferenceType->InstanceofExpression");
    }
;

ShiftExpression:
    AdditiveExpression  {
        $$ = $1;
        verbose(v,"AdditiveExpression->ShiftExpression");
    }
|   ShiftExpression LSHIFT AdditiveExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        verbose(v,"ShiftExpression LSHIFT AdditiveExpression->ShiftExpression");

        if(typeroot->categorize($1->type)==INTEGER_TYPE && typeroot->categorize($3->type)==INTEGER_TYPE)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   ShiftExpression RSHIFT AdditiveExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        verbose(v,"ShiftExpression RSHIFT AdditiveExpression->ShiftExpression");

        if(typeroot->categorize($1->type)==INTEGER_TYPE && typeroot->categorize($3->type)==INTEGER_TYPE)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   ShiftExpression UNRSHIFT AdditiveExpression {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        //struct Node* n = new struct Node("ConditionalOrExpression", temp);
        
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        verbose(v,"ShiftExpression UNRSHIFT AdditiveExpression->ShiftExpression");

        if(typeroot->categorize($1->type)==INTEGER_TYPE && typeroot->categorize($3->type)==INTEGER_TYPE)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

AdditiveExpression:
    MultiplicativeExpression    {
        $$ = $1;
        verbose(v,"MultiplicativeExpression->AdditiveExpression");
    }
|   AdditiveExpression PLUS MultiplicativeExpression    {
      vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        processPostIncre($$);
        verbose(v,"AdditiveExpression PLUS MultiplicativeExpression->AdditiveExpression");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE) && (typeroot->categorize($3->type) == INTEGER_TYPE || typeroot->categorize($3->type) == FLOATING_TYPE))
            $$->type = typeroot->maxtype($3->type, $3->type);
        else if($1->type==STRING_NUM || $3->type==STRING_NUM)
        {
            $$->type = STRING_NUM;
        }
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   AdditiveExpression MINUS MultiplicativeExpression   {
       vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        
        $$ = t;
        
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        processPostIncre($$);
        verbose(v,"AdditiveExpression MINUS MultiplicativeExpression->AdditiveExpression");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE) && (typeroot->categorize($3->type) == INTEGER_TYPE || typeroot->categorize($3->type) == FLOATING_TYPE))
            $$->type = typeroot->maxtype($3->type, $3->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

MultiplicativeExpression:
    UnaryExpression {
        $$ =$1;
        verbose(v,"UnaryExpression->MultiplicativeExpression");
    }
|   MultiplicativeExpression MULT UnaryExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes,$2);
        processPostIncre($$);
        verbose(v,"MultiplicativeExpression MULT UnaryExpression->MultiplicativeExpression");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE) && (typeroot->categorize($3->type) == INTEGER_TYPE || typeroot->categorize($3->type) == FLOATING_TYPE))
            $$->type = typeroot->maxtype($3->type, $3->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    } 
|   MultiplicativeExpression DIV UnaryExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        $$ = t;
        vector<Node*> nodes = {$$, $1,$3};
        processArithmetic(nodes, $2);
        processPostIncre($$);
        verbose(v,"MultiplicativeExpression DIV UnaryExpression->MultiplicativeExpression");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE) && (typeroot->categorize($3->type) == INTEGER_TYPE || typeroot->categorize($3->type) == FLOATING_TYPE))
            $$->type = typeroot->maxtype($3->type, $3->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
|   MultiplicativeExpression MOD UnaryExpression    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $2);
        
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        for(auto it: temp) {
            t->addChild(it);
        }
        temp.clear();
        if($3->useful == false) {
            for(auto it : $3->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($3);
        for(auto it: temp) {
            t->addChild(it);
        }
        $$ = t;
        vector<Node*> nodes = {$$, $1, $3};
        processArithmetic(nodes, $2);
        processPostIncre($$);
        verbose(v,"MultiplicativeExpression MOD UnaryExpression->MultiplicativeExpression");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE) && (typeroot->categorize($3->type) == INTEGER_TYPE || typeroot->categorize($3->type) == FLOATING_TYPE))
            $$->type = typeroot->maxtype($3->type, $3->type);
        else
        {            
            cout<<"Error! Incompatible types for "<<$2<<": "<<typeroot->inv_types[$1->type]<<" and "<<typeroot->inv_types[$3->type]<<endl;
            yyerror("Error");
        }
    }
;

UnaryExpression:
    PreIncrementExpression  {
        $$ = $1;
        verbose(v,"PreIncrementExpression->UnaryExpression");
    }
|   PreDecrementExpression  {
        $$ = $1;
        verbose(v,"PreDecrementExpression->UnaryExpression");
    }
|   PLUS UnaryExpression    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $1);
        temp.push_back(t);
        if($2->useful == false) {
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        struct Node* n = new struct Node("UnaryExpression", temp);
        $$ = n;
        $$->varName= $$->attr = string($1) + $2->varName;
        verbose(v,"PLUS UnaryExpression->UnaryExpression");

        if((typeroot->categorize($2->type) == INTEGER_TYPE || typeroot->categorize($2->type) == FLOATING_TYPE))
            $$->type = $2->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<$1<<": "<<typeroot->inv_types[$2->type]<<endl;
            yyerror("Error");
        }
    }
|   MINUS UnaryExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $1);
        temp.push_back(t);
        if($2->useful == false) {
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        struct Node* n = new struct Node("UnaryExpression", temp);
        $$ = n;
        $$->varName= $$->attr = string($1) + $2->varName;
        verbose(v,"MINUS UnaryExpression->UnaryExpression");

        if((typeroot->categorize($2->type) == INTEGER_TYPE || typeroot->categorize($2->type) == FLOATING_TYPE))
            $$->type = $2->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<$1<<": "<<typeroot->inv_types[$2->type]<<endl;
            yyerror("Error");
        }
    }
|   UnaryExpressionNotPlusMinus {
        $$ = $1;
        verbose(v,"UnaryExpressionNotPlusMinus->UnaryExpression");
    }
;
PreIncrementExpression:
   INCRE UnaryExpression    {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $1);
        temp.push_back(t);
        if($2->useful == false) {
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        struct Node* n = new struct Node("PreIncrementExpression", temp);
        $$ = n;
        Quadruple* q = new Quadruple(string("+"),  append_scope_level($2->varName), string("1"),  append_scope_level($2->varName));
        $$->code.push_back(q);
        
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
        $$->varName= $2->varName;
        //$$->type = $2->type;
        verbose(v,"INCRE UnaryExpression->PreIncrementExpression");

        if((typeroot->categorize($2->type) == INTEGER_TYPE || typeroot->categorize($2->type) == FLOATING_TYPE))
            $$->type = $2->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<$1<<": "<<typeroot->inv_types[$2->type]<<endl;
            yyerror("Error");
        }
    }
;

PreDecrementExpression:
    DECRE UnaryExpression   {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $1);
        temp.push_back(t);
        if($2->useful == false) {
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        struct Node* n = new struct Node("PreDecrementExpression", temp);
        $$ = n;
        Quadruple* q = new Quadruple(string("-"),  append_scope_level($2->varName), string("1"),  append_scope_level($2->varName));
        $$->code.push_back(q);
        
        ircode.push_back(q);
        $$->last = ircode.size() - 1;
        $$->varName= $2->varName;
        verbose(v,"DECRE UnaryExpression->PreDecrementExpression");

        if((typeroot->categorize($2->type) == INTEGER_TYPE || typeroot->categorize($2->type) == FLOATING_TYPE))
            $$->type = $2->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<$1<<": "<<typeroot->inv_types[$2->type]<<endl;
            yyerror("Error");
        }
    }
;

UnaryExpressionNotPlusMinus:
    PostfixExpression   {
        $$ = $1;
        verbose(v,"PostfixExpression->UnaryExpressionNotPlusMinus");
    }
|   TIL UnaryExpression {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $1);
        temp.push_back(t);
        if($2->useful == false) {
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        struct Node* n = new struct Node("UnaryExpressionNotPlusMinus", temp);
        $$ = n;
        $$->varName= $$->attr = string($1) + $2->varName;
        verbose(v,"TIL UnaryExpression->UnaryExpressionNotPlusMinus");

        if(typeroot->categorize($2->type)==INTEGER_TYPE)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible type for "<<$1<<": "<<typeroot->inv_types[$2->type]<<endl;
            yyerror("Error");
        }
    }
|   NOT UnaryExpression {
        vector<struct Node*> temp;
        struct Node* t = new struct Node("Operator", $1);
        temp.push_back(t);
        if($2->useful == false) {
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        struct Node* n = new struct Node("UnaryExpressionNotPlusMinus", temp);
        $$ = n;
        $$->varName= $$->attr = string($1) + $2->varName;
        verbose(v,"NOT UnaryExpression->UnaryExpressionNotPlusMinus");
        $$->truelist = $2->falselist;
        $$->falselist = $2->truelist;
        if(typeroot->categorize($2->type)==INTEGER_TYPE)
            $$->type = INT_NUM;
        else
        {            
            cout<<"Error! Incompatible type for "<<$1<<": "<<typeroot->inv_types[$2->type]<<endl;
            yyerror("Error");
        }
    }
|   CastExpression  {
    $$ = $1;
    verbose(v, "CastExpression->UnaryExpressionNotPlusMinus");
}
;

CastExpression:
    LEFTPARENTHESIS PrimitiveType RIGHTPARENTHESIS UnaryExpression  {
        vector<struct Node*> temp;
      
        if($2->useful == false) {
            cout<<"not 2 useful"<<endl;
            for(auto it : $2->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);

        if($4->useful == false) {
            cout<<"not 4 useful"<<endl;
            for(auto it : $4->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($4);

        /*
        string resName = string("_t") + to_string(varCnt);
        varCnt++;
        Quadruple q(string("cast_to_") + string($2),  append_scope_level($4->varName), resName );
        ir.push_back(q);
        $$->varName = resName;
        $$->reduced = true;
        */

        struct Node* n = new struct Node("CastExpression", temp);
        $$ = n;

        verbose(v,"LEFTPARENTHESIS PrimitiveType RIGHTPARENTHESIS UnaryExpression->CastExpression");

        if((typeroot->categorize($2->attr)==INTEGER_TYPE || typeroot->categorize($2->attr)==FLOATING_TYPE) && (typeroot->categorize($4->type)==INTEGER_TYPE || typeroot->categorize($4->type)==FLOATING_TYPE))
        {
            $$->type = typeroot->typewidth[$2->attr].first;
        }
        else
        {
            cout<<"Cannot type cast "<<typeroot->inv_types[$4->type]<<" to type "<<$2->attr<<endl;
            yyerror("Type Conversion");
        }
    }
;

PostfixExpression:
    Primary {
    $$ = $1;
    verbose(v,"Primary->PostfixExpression");
}
|   Name    {
    $$ = $1;
    verbose(v,"Name->PostfixExpression");
}
|   PostIncrementExpression {
    $$ = $1;
    verbose(v,"PostIncrementExpression->PostfixExpression");
}
|   PostDecrementExpression {
    $$ = $1;
    verbose(v,"PostDecrementExpression->PostfixExpression");
}
;

PostIncrementExpression:
   PostfixExpression INCRE  {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        struct Node* t = new Node("Operator", "++");
        temp.push_back(t);
        struct Node* n = new Node("PostIncrementExpression", temp);
        $$ = n;
        $$->varName= $1->varName;
        Quadruple* q = new Quadruple(string("+"),  append_scope_level($1->varName), string("1"),  append_scope_level($1->varName));
        //$$->code.push_back(q);
        residualCode.push_back(q);
        //ircode.push_back(q);
        
        verbose(v,"PostfixExpression INCRE->PostIncrementExpression");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE))
            $$->type = $1->type;
        else
            yyerror("Incompatible Types");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE))
            $$->type = $1->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<$2<<": "<<typeroot->inv_types[$1->type]<<endl;
            yyerror("Error");
        }
    }
;

PostDecrementExpression:
    PostfixExpression DECRE {
        
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        struct Node* t = new Node("Operator", "--");
        temp.push_back(t);
        struct Node* n = new Node("PostDecrementExpression", temp);
        $$ = n;
        $$->varName=$1->varName;
        Quadruple* q = new Quadruple(string("-"),  append_scope_level($1->varName), string("1"),  append_scope_level($1->varName));
        //$$->code.push_back(q);
        residualCode.push_back(q);
        verbose(v,"PostfixExpression DECRE->PostDecrementExpression");

        if((typeroot->categorize($1->type) == INTEGER_TYPE || typeroot->categorize($1->type) == FLOATING_TYPE))
            $$->type = $1->type;
        else
        {            
            cout<<"Error! Incompatible type for "<<$2<<": "<<typeroot->inv_types[$1->type]<<endl;
            yyerror("Error");
        }
    }
;


//-----------------Types, values and variables-----//

Type:
    PrimitiveType   {
    $$ = $1;
    verbose(v,"PrimitiveType->Type");
}
|   ReferenceType   {
    $$ = $1;
    verbose(v,"ReferenceType->Type");
}
;

PrimitiveType:
    NumericType {
    $$ = $1;
    verbose(v,"NumericType->PrimitiveType");
}
|   BOOLEAN {
    struct Node* n = new struct Node("Keyword", "boolean");
    $$ = n;
    $$->type = BOOL_NUM;
    verbose(v,"BOOLEAN->PrimitiveType");
}
|   STRING {
    struct Node* n = new struct Node("Keyword", "String");
    $$ = n;
    $$->type = STRING_NUM;
    verbose(v, "STRING->PrimitiveType");
}
;

ReferenceType:
    Name    {
    $$ = $1;
    verbose(v,"Name->ReferenceType");
}
|   ArrayType   {
    $$ = $1;
    verbose(v,"ArrayType->ReferenceType");
}
;

//ClassType:
//    IDENTIFIER 
//|   Name DOT IDENTIFIER
//|   ClassType DOT IDENTIFIER 
//;

ArrayType:
    PrimitiveType Dims  {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            if($2->useful == false) {
                for(auto it : $2->children)
                {
                    temp.push_back(it);
                }
            }
            else temp.push_back($2);
        }
        struct Node* n = new Node("ArrayType", temp);
        $$ = n;
        $$->type = $2->arrayType*100 + $1->type;
        $$->arrayType = $2->arrayType;
        verbose(v,"PrimitiveType Dims->ArrayType");
    }
|   Name Dims   {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it : $1->children)
            {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            if($2->useful == false) {
                for(auto it : $2->children)
                {
                    temp.push_back(it);
                }
            }
            else temp.push_back($2);
        }
        struct Node* n = new Node("ArrayType", temp);
        $$ = n;
        $$->type = $2->arrayType*100 + $1->type;
        $$->arrayType = $2->arrayType;
        verbose(v,"Name Dims->ArrayType");
    }  
;

TypeParameter:
    IDENTIFIER  {
        struct Node* n = new Node("Identifier", $1);
        $$ = n;
        verbose(v,"IDENTIFIER->TypeParameter");
    }
|   IDENTIFIER ClassExtends {
        struct Node* t1 = new Node ("Identifier", $1);
        vector<Node*> t = {t1, $2};
        $$ = new Node("TypeParameter", t);
        verbose(v,"IDENTIFIER ClassExtends->TypeParameter");
    }
;
CommaTypeParameters:
    {$$ = NULL;}
|   COMMA TypeParameters CommaTypeParameters    {
    vector<struct Node*> temp;
        if($2->useful == false) {
            for(auto it: $2->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($2);
        if($3) {
            for(auto it: temp) {
                $3->addChildToLeft(it);
            }
            $$ = $3;
        }
        else $$ = new Node("TypeParameters", temp);
        verbose(v,"COMMA TypeParameters CommaTypeParameters->CommaTypeParameters");
}
;


//TypeBound:
//    EXTENDS Name
// |   EXTENDS ClassType AdditionalBounds
;

/* 
Wildcard:
    QUES
|   QUES WildcardBounds
;

WildcardBounds:
    EXTENDS ReferenceType
|   SUPER ReferenceType 
; */

Literal:
    INTEGERLITERAL  {
        struct Node* n = new Node("Literal", $1);
        n->attr = n->varName = $1;
        cout << n->varName << "\n";
        $$ = n;
        verbose(v,"INTEGERLITERAL->Literal");

        $$->type = INT_NUM;
    }
|   FLOATINGPOINTLITERAL    {
        struct Node* n = new Node("Literal", $1);
        n->attr = $1;
        $$ = n;
        verbose(v,"FLOATINGPOINTLITERAL->Literal");

        $$->type = FLOAT_NUM;
    }
|   BOOLEANLITERAL  {
        struct Node* n = new Node("Literal", $1);
        n->attr = $1;
        $$ = n;
        verbose(v,"BOOLEANLITERAL->Literal");

        $$->type = BOOL_NUM;
    }
|   STRINGLITERAL   {
        struct Node* n = new Node("Literal", $1);

        n->attr = $1;
        $$ = n;
        verbose(v,"STRINGLITERAL->Literal");

        $$->type = STRING_NUM;
    }
|   CHARACTERLITERAL    {
        struct Node* n = new Node("Literal", $1);
        n->attr = $1;
        $$ = n;
        verbose(v,"CHARACTERLITERAL->Literal");

        $$->type = CHAR_NUM;
    }
|   NULLLITERAL {
        struct Node* n = new Node("Literal", $1);
        n->attr = $1;
        $$ = n;
        verbose(v,"NULLLITERAL->Literal");

        $$->type = NULL_LIT;
    }
|   TEXTBLOCK    {
        struct Node* n = new Node("Literal", $1);
        n->attr = $1;
        n->attr.pop_back();
        n->attr.pop_back();
        n->attr.pop_back();
        n->attr.erase(n->attr.begin());
        n->attr.erase(n->attr.begin());
        n->attr.erase(n->attr.begin());
        $$ = n;
        verbose(v,"TEXTBLOCKLITERAL->Literal");

        $$->type = TEXT_BLOCK;
    }
;

/********************************************************************
            IMPORT DECLARATIONS
*******************************************************************/
ImportDeclarations:
    {$$ = NULL;}
|   ImportDeclaration ImportDeclarations    {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            $$ = $2;
        }
        else $$ = new Node("ImportDeclarations", temp);
    }
;

ModuleDirectives:
    {$$ = NULL;}
|   ModuleDirective ModuleDirectives    {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            $$ = $2;
        }
        else $$ = new Node("ModuleDirectives", temp);
    }
;

RequiresModifiers:
    {$$ = NULL;}
|   RequiresModifier RequiresModifiers  {
        vector<struct Node*> temp;
        if($1->useful == false) {
            for(auto it: $1->children) {
                temp.push_back(it);
            }
        }
        else temp.push_back($1);
        if($2) {
            for(auto it: temp) {
                $2->addChildToLeft(it);
            }
            $$ = $2;
        }
        else $$ = new Node("RequiresModifiers", temp);
    }
;


%%


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
    /* if(strn>0) */
    /* { */
        /* printf("Error : %s of length %d\n", yytext, strn); */
        // if(yytext[strn-1]=='\0')
        /* printf("Error due to %s at line number %d.  Aborting...\n", sp, yylineno); */
        // printf("Error due to : %s ", sp);
        // errorlineno();
        // printf("Aborting....\n");
        // printf("Error at line number %d. Aborting...\n", yylineno);
    /* } */
    exit(0);
}
