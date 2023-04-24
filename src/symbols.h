#include <bits/stdc++.h>
using namespace std;

#define VAR_SYM 0
#define FUNC_SYM 1
#define CLASS_SYM 2

#define PUBLIC_ACCESS 0
#define PRIVATE_ACCESS 1
#define PROTECTED_ACCESS 2

class Symbol{ //represents individual record of a given symbol table
    public:

    string lexeme;
    int type;
    bool is_decl=false;
    bool isArray = false;
    bool isFinal=false;
    int decl_line_no;
    int width ;
    string width1 , width2, width3; // width 1 for general use, rest for arrays
    int _width1 = 0, _width2 = 0, _width3 = 0;
    int num_elems1=0, num_elems2=0, num_elems3=0;      // only useful in case of arrays
    int access_type=PUBLIC_ACCESS;
    int isStatic = 0;
    int scope_level=0;
    int isField = 0;

    Symbol(string lexeme, int type, int lineno, int width);
    Symbol(string lexeme, int type, int lineno);
    Symbol(string lexeme, int type, int lineno, int width, int access_type);
    //void calcWidths();

    void printSym();
};

class SymNode{ //represents individual scope in case of nested scopes/ individual function in case of multiple functions
    public:
    bool vulnerable=false;   // to check if we have to delete this scope on ending the inner scope
    bool isIfScope=false;
    bool isForScope=false;
    bool isRightPar=false; //check if we have scanned right paranthesis in case of for
    bool isThisDead=false;
    bool isClassName=true;
    bool isFinalClass=false;
    bool isFinalId=false;
    bool isDoRun=false;//used for do while
    int cnt_paran=0;//used for do while
    string name; //this symbol table is created for this name entity
    int symtype; //0=variable, 1=function, 2=class
    map<string, Symbol*> mp; //maintains the symbol table as a hash table
    map<string, SymNode*> fmp; //maintains the symbol table as a hash table
    map<string, SymNode*> cmp; 

    SymNode* parent=nullptr; //parent scope of the scope represented by this SymNode
    SymNode* ogparent=nullptr; //this is used in case of class extends
    vector<SymNode*> childscopes; //subscopes of the current scope
    //used in case of a function node
    vector<int> args;
    vector<vector<int>> constr_args;
    int returntype;
    bool strict = false;
    bool default_done = false;
    // string par_class;
    //used in case of a class node
    int node_acc_type = PUBLIC_ACCESS;

    SymNode(SymNode* _parent, string _name);
    SymNode(SymNode* _parent, string _name, int _symtype);
    SymNode(SymNode* _parent, string _name, int _symtype, vector<int> _args);
    SymNode(SymNode* _parent, string _name, int _symtype, vector<int> _args, int _returntype);


//At the time of declarations, use scope lookups for vars, funcs, and classes. Use the global lookup functions while invoking/creating instances
    Symbol* scope_lookup(string lex);
    SymNode* scope_flookup(string lex, vector<int> args, int returntype);
    SymNode* scope_flookup(string lex, vector<int> args);
    SymNode* scope_flookup(string lex, vector<int> args, int returntype, bool strict);
    SymNode* scope_flookup(string lex, vector<int> args, bool strict);
    SymNode* scope_flookup(string lex);
    SymNode* scope_clookup(string lex);
    bool scope_constrlookup(vector<int> args);
    void constr_insert(vector<int> args);
};

class SymGlob{ //global scope
    public:

    SymNode* currNode=nullptr;
    map<string, SymNode*> func_map; //maps function name to its corresponding symbol table node
    map<string, SymNode*> class_map; //maps class name to its corresponding symbol table node

    SymGlob();
    
    Symbol* lookup(string lex); //look for declaration of a variable
    SymNode* flookup(string lex, vector<int> args, int returntype); //look for declaration of a function with given input args and return type (used when a method is invoked)
    SymNode* flookup(string lex, vector<int> args);
    SymNode* flookup(string lex);
    SymNode* clookup(string lex);
    SymNode* nlookup(string name);  // search for a scope with given name
    void update(string lex, Symbol* sym); //when updating some information during use of some variable
    void insert(string lex, Symbol* sym); //when a variable is declared
    void par_insert(string lex, Symbol* sym);//add to parent of currnode
    void finsert(string lex, SymNode* symfunc); //when a method is declared
    void cinsert(string lex, SymNode* symclass);
    void addNewScope(); //when a new subscope starts
    void endcurrScope(); //when the current scope ends
    void end_all_vulnerable(); //end all the vulnerable scopes
    void printTree();   //print the level wise scopes
    void printFuncs();  //print the declared functions
    void dumpClassSymbols();
    void dumpSymbolTable();
};

bool argsMatch(vector<int> def, vector<int> given); //def->as defined, given->passed as arguments
