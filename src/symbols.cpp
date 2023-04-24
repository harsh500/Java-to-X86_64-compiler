#include "symbols.h"
#include "types.h"
#include <unistd.h>
using namespace std;
extern int yylineno;
extern void yyerror(const char *sp);
extern SymGlob *orig_root;
extern int scope_level;
map<string, int> csv_gen;
extern map<string, SymNode *> list_class;
extern map<string, string> classfunc;

extern TypeHandler *typeroot;
string GetCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  getcwd( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}
int conv_int(string a)
{
    int ans = 0;
    int x = 1;
    for (int j = a.size() - 1; j >= 0; j--)
    {
        ans += (a[j] - '0') * x;
        x = x * 10;
    }
    return ans;
}
//----Constructors----//
Symbol::Symbol(string lexeme, int type, int lineno, int width)
{
    this->lexeme = lexeme;
    this->type = type;
    this->decl_line_no = lineno;
    this->width = width;
    this->is_decl = true;
    this->access_type = PUBLIC_ACCESS;
    width1 = "w";
    width2 = "w";
    width3 = "w";
}

bool argsMatch(vector<int> def, vector<int> given) // def->as defined, given->passed as arguments
{
    if (def.size() != given.size())
        return false;
    for (int i = 0; i < def.size(); i++)
    {
        if (def[i] != given[i])
        {
            if (!(typeroot->categorize(def[i]) == FLOATING_TYPE && typeroot->categorize(given[i]) == INTEGER_TYPE))
                return false;
        }
    }
    return true;
}

Symbol::Symbol(string lexeme, int type, int lineno)
{
    this->lexeme = lexeme;
    this->type = type;
    this->decl_line_no = lineno;
    this->is_decl = true;
    this->access_type = PUBLIC_ACCESS;
    width1 = "w";
    width2 = "w";
    width3 = "w";
}

Symbol::Symbol(string lexeme, int type, int lineno, int width, int access_type)
{
    this->lexeme = lexeme;
    this->type = type;
    this->decl_line_no = lineno;
    this->is_decl = true;
    this->width = width;
    this->access_type = access_type;
    width1 = "w";
    width2 = "w";
    width3 = "w";
}

/****************************************
void Symbol::calcWidths()
{
    _width3 = width3;
    _width2 = width2;
    _width1 = width1;
    if(width3) {
        _width2 = width * _width3;
        _width1 = _width2* width1;
        _width3 = width;
    }
    else if(width2) {
        _width1 = width* width2;
        _width2 = width;
    }
    else {
        _width1 = width * width1;
    }


}
***********************************************/

SymNode::SymNode(SymNode *_parent, string _name)
{
    this->parent = _parent;
    this->name = _name;
}

SymNode::SymNode(SymNode *_parent, string _name, int _symtype)
{
    this->parent = _parent;
    this->name = _name;
    this->symtype = _symtype;
}

SymNode::SymNode(SymNode *_parent, string _name, int _symtype, vector<int> _args)
{
    this->parent = _parent;
    this->name = _name;
    this->symtype = _symtype;
    this->args = _args;
}

SymNode::SymNode(SymNode *_parent, string _name, int _symtype, vector<int> _args, int _returntype)
{
    this->parent = _parent;
    this->name = _name;
    this->symtype = _symtype;
    this->args = _args;
    this->returntype = _returntype;
}

SymGlob::SymGlob()
{
    this->currNode = new SymNode(nullptr, "Global");
}

//---Methods-------//
void Symbol::printSym()
{
    cout << this->lexeme << " " << this->type << " " << this->decl_line_no << endl;
    if (this->isArray)
    {
        cout << this->width1 << " " << this->width2 << " " << this->width3 << "\n";
    }
}

Symbol *SymGlob::lookup(string lex)
{
    SymNode *temp = currNode;
    while (temp)
    {
        auto mp = temp->mp;
        if (mp.find(lex) != mp.end())
        {
            Symbol *ret = mp[lex];
            if (ret->access_type == PUBLIC_ACCESS)
                return ret;
            if ((ret->access_type == PRIVATE_ACCESS && (temp == currNode || (find(temp->childscopes.begin(), temp->childscopes.end(), currNode) != temp->childscopes.end()))))
                return ret;
            else if (ret->access_type == PRIVATE_ACCESS)
            {
                // cout<<"Identifier "<<lex<<" has access type "<<ret->access_type<<endl;
                cout << "Access Permission Denied for the name : " << lex << " !" << endl;
                yyerror("Error");
                return nullptr;
            }
        }
        // cout<<"Didn't find identifier "<<lex<<endl;
        temp = temp->parent;
    }
    return nullptr;
}

SymNode *SymGlob::nlookup(string name) // look for the scope with given name
{
    SymNode *temp = currNode;
    while (temp)
    {
        if (temp->name == name)
        {
            return temp;
        }
        temp = temp->parent;
    }
    return nullptr;
}

SymNode *SymGlob::flookup(string lex, vector<int> args, int returntype)
{
    SymNode *temp = currNode;
    SymNode *res;
    while (temp)
    {
        res = temp->scope_flookup(lex);
        if (res && res->returntype == returntype && argsMatch(res->args, args))
        {
            if (res->node_acc_type == PUBLIC_ACCESS)
                return res;
            if ((res->node_acc_type == PRIVATE_ACCESS && (temp == currNode || (find(temp->childscopes.begin(), temp->childscopes.end(), currNode) != temp->childscopes.end()))))
                return res;
            else if (res->node_acc_type == PRIVATE_ACCESS)
            {
                // cout<<"Identifier "<<lex<<" has access type "<<ret->access_type<<endl;
                cout << "Access Permission Denied for the name : " << lex << " !" << endl;
                yyerror("Error");
                return nullptr;
            }
            break;
        }
        // cout<<"Searching serching searchinng"<<endl;
        if (temp->name == "Global")
            break;
        temp = temp->parent;
    }

    if (res && temp && argsMatch(res->args, args) && res->returntype == returntype)
    {
        return res;
    }
    return nullptr;
    // if(func_map.find(lex)==func_map.end())
    // {
    //     // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
    //     return nullptr;
    // }
    // SymNode* temp = func_map[lex];
    // if(temp->args == args && temp->returntype==returntype)
    //     return temp;
    // // cout<<"Error! No matching function to call. "<<endl;
    // return nullptr;
}

SymNode *SymGlob::flookup(string lex, vector<int> args)
{

    SymNode *temp = currNode;
    SymNode *res;
    while (temp)
    {
        res = temp->scope_flookup(lex);
        if (res && argsMatch(res->args, args))
        {
            if (res->node_acc_type == PUBLIC_ACCESS)
                return res;
            if ((res->node_acc_type == PRIVATE_ACCESS && (temp == currNode || (find(temp->childscopes.begin(), temp->childscopes.end(), currNode) != temp->childscopes.end()))))
                return res;
            else if (res->node_acc_type == PRIVATE_ACCESS)
            {
                // cout<<"Identifier "<<lex<<" has access type "<<ret->access_type<<endl;
                cout << "Access Permission Denied for the name : " << lex << " !" << endl;
                yyerror("Error");
                return nullptr;
            }
            break;
        }
        temp = temp->parent;
    }

    if (res && temp && argsMatch(res->args, args))
    {
        return res;
    }
    return nullptr;

    // if(func_map.find(lex)==func_map.end())
    // {
    //     // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
    //     return nullptr;
    // }
    // SymNode* temp = func_map[lex];
    // if(temp->args == args)
    //     return temp;
    // // cout<<"Error! No matching function to call. "<<endl;
    // return nullptr;
}

SymNode *SymGlob::flookup(string lex)
{

    SymNode *temp = currNode;
    SymNode *res;
    while (temp)
    {
        res = temp->scope_flookup(lex);
        if (res)
            break;
        temp = temp->parent;
    }

    if (res && temp)
    {
        return res;
    }
    return nullptr;

    // if(func_map.find(lex)==func_map.end())
    // {
    //     // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
    //     return nullptr;
    // }
    // SymNode* temp = func_map[lex];
    // return temp;
}

SymNode *SymGlob::clookup(string lex)
{
    SymNode *temp = currNode;
    while (temp)
    {
        if (temp->cmp.find(lex) != temp->cmp.end())
        {
            // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
            return temp->cmp[lex];
        }
        temp = temp->parent;
    }
    return nullptr;
}

void SymGlob::update(string lex, Symbol *sym)
{
    Symbol *res = lookup(lex);
    if (res == nullptr)
    {
        cout << "Error! The name " << lex << " hasn't been declared before!!" << endl;
        return;
    }
    currNode->mp[lex] = sym;
}

void SymGlob::insert(string lex, Symbol *sym)
{
    Symbol *res = nullptr;
    SymNode *temp = currNode;
    while (temp && temp->name != "class" && temp->name != "classextends")
    {
        res = temp->scope_lookup(lex);
        if (res)
            break;
        temp = temp->parent;
    }
    if (res)
    {
        cout << "Error! The name " << lex << " has been declared already on line number : " << res->decl_line_no << endl;
        yyerror("Error");
        return;
    }
    if (currNode->isThisDead == true && currNode->vulnerable == true)
    {
        while (currNode->vulnerable == true)
        {
            currNode = currNode->parent;
            if (!currNode)
            {
                cout << "Error!! No global scope defined." << endl;
                break;
            }
        }
    }
    (currNode->mp).insert({lex, sym});
}

void SymGlob::par_insert(string lex, Symbol *sym)
{
    Symbol *res = currNode->parent->scope_lookup(lex);
    if (res)
    {
        cout << "Error! The name " << lex << " has been declared already on line number : " << res->decl_line_no << endl;
        yyerror("Error");
        return;
    }
    if (currNode->isThisDead == true && currNode->vulnerable == true)
    {
        while (currNode->vulnerable == true)
        {
            currNode = currNode->parent;
            if (!currNode)
            {
                cout << "Error!! No global scope defined." << endl;
                break;
            }
        }
    }
    (currNode->parent->mp).insert({lex, sym});
}

void SymGlob::finsert(string lex, SymNode *symfunc)
{
    SymNode *res = currNode->parent->scope_flookup(lex, symfunc->args, symfunc->returntype);
    if (res)
    {
        cout << "Error!! Method with name " << lex << " and specified parameters has already been declared." << endl;
        yyerror("Error");
        return;
    }
    func_map.insert({lex, symfunc});
    currNode->parent->fmp.insert({lex, symfunc});
}

void SymGlob::cinsert(string lex, SymNode *symclass)
{
    SymNode *res = currNode->scope_clookup(lex);
    if (res)
    {
        cout << "Error!! class with name " << lex << " and specified parameters has already been declared." << endl;
        yyerror("Error");
        return;
    }
    class_map.insert({lex, symclass});
    currNode->parent->cmp.insert({lex, symclass});
}

void SymGlob::addNewScope()
{
    SymNode *temp = new SymNode(currNode, "New Scope");
    currNode->childscopes.push_back(temp);
    currNode = temp;
}

void SymGlob::endcurrScope()
{
    scope_level++;
    if (currNode->name == "classextends")
    {
        if (currNode && currNode->ogparent)
            currNode = currNode->ogparent;
        else
            cout << "Error!! No global scope defined." << endl;
    }
    else
    {
        if (currNode && currNode->parent)
            currNode = currNode->parent;
        else
            cout << "Error!! No global scope defined." << endl;
    }
}
void SymGlob::end_all_vulnerable()
{
    // cout<<">>>>"<<currNode->name<<"\n";
    if (currNode->name == "classextends")
    {
        if (currNode && currNode->ogparent)
            currNode = currNode->ogparent;
        else
            cout << "Error!! No global scope defined." << endl;
        scope_level++;
    }
    else
    {
        if (currNode && currNode->parent)
            currNode = currNode->parent;
        else
            cout << "Error!! No global scope defined." << endl;
        scope_level++;
    }

    while (currNode != NULL && currNode->vulnerable == true)
    {
        scope_level++;
        if (currNode->name == "classextends")
        {
            if (currNode && currNode->ogparent)
                currNode = currNode->ogparent;
            else
                cout << "Error!! No global scope defined." << endl;
        }
        else
        {
            if (currNode && currNode->parent)
                currNode = currNode->parent;
            else
                cout << "Error!! No global scope defined." << endl;
        }
        if (!currNode)
        {
            cout << "Error!! No global scope defined." << endl;
            break;
        }
    }
}
void SymGlob::printTree()
{
    queue<pair<SymNode *, int>> q;
    q.push({currNode, 0});
    int c = 0;
    while (!q.empty())
    {
        // cout<<"Entering "<<endl;
        pair<SymNode *, int> p = q.front();

        if (p.second > c)
        {
            c++;
            cout << '\n';
        }

        q.pop();
        cout << "Scope level : " << p.second << " bdbjb " << endl;
        cout << "The variables are : " << endl;
        for (auto it : p.first->mp)
        {
            cout << "lexeme : " << it.first << ", record : ";
            it.second->printSym();
        }
        cout << "-------------------------------------" << endl;

        cout << "The functions are : " << endl;
        for (auto it : p.first->fmp)
        {
            cout << "Function name is : " << it.first << endl;
            cout << "Return type is : " << it.second->returntype << endl;
            cout << "The argument types are : ";
            for (auto arg : it.second->args)
            {
                cout << arg << " ";
            }
            cout << endl;
        }
        cout << "-------------------------------------" << endl;

        cout << "The classes are : " << endl;
        for (auto it : p.first->cmp)
        {
            cout << "Class name is : " << it.first << endl;
            cout << endl;
        }

        cout << "The constructors are : " << endl;
        for (auto it : p.first->constr_args)
        {
            for (auto typ : it)
            {
                cout << typ << ", ";
            }
            cout << endl;
        }
        cout << "-------------------------------------" << endl;
        for (auto it : p.first->childscopes)
        {
            q.push({it, p.second + 1});
        }
        cout << endl;
        cout << "-------------------------------------" << endl;
        cout << "-------------------------------------" << endl;
    }
    cout << "-------------------------------------" << endl;
}

void SymGlob::printFuncs()
{
    for (auto it : func_map)
    {
        cout << "Function name is : " << it.first << endl;
        cout << "Return type is : " << it.second->returntype << endl;
        cout << "The argument types are : ";
        for (auto arg : it.second->args)
        {
            cout << arg << " ";
        }
        cout << endl;
    }
}

Symbol *SymNode::scope_lookup(string lex)
{
    if (mp.find(lex) == mp.end())
    {
        // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
        return nullptr;
    }
    Symbol *temp = mp[lex];
    return temp;
}

SymNode *SymNode::scope_flookup(string lex, vector<int> args, int returntype)
{
    if (fmp.find(lex) == fmp.end())
    {
        // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
        return nullptr;
    }
    SymNode *temp = fmp[lex];

    if (temp->args == args && temp->returntype == returntype)
    {
        if (temp->node_acc_type == PRIVATE_ACCESS)
        {
            cout << "Error on line number " << yylineno << ". Access Permission denied." << endl;
            yyerror("Error");
        }
        return temp;
    }
    // cout<<"Error! No matching function to call. "<<endl;
    return nullptr;
}

SymNode *SymNode::scope_flookup(string lex, vector<int> args)
{
    if (fmp.find(lex) == fmp.end())
    {
        // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
        return nullptr;
    }
    SymNode *temp = fmp[lex];
    if (temp && temp->node_acc_type == PRIVATE_ACCESS)
    {
        cout << "Error on line number " << yylineno << ". Access Permission denied." << endl;
        yyerror("Error");
    }
    if (temp->args == args)
    {
        if (temp->node_acc_type == PRIVATE_ACCESS)
        {
            cout << "Error on line number " << yylineno << ". Access Permission denied." << endl;
            yyerror("Error");
        }
        return temp;
    }
    // cout<<"Error! No matching function to call. "<<endl;
    return nullptr;
}

SymNode *SymNode::scope_flookup(string lex)
{
    if (fmp.find(lex) == fmp.end())
    {
        // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
        return nullptr;
    }
    SymNode *temp = fmp[lex];
    if (temp && temp->node_acc_type == PRIVATE_ACCESS)
    {
        cout << "Error on line number " << yylineno << ". Access Permission denied." << endl;
        yyerror("Error");
    }
    return temp;
}

SymNode *SymNode::scope_flookup(string lex, vector<int> args, int returntype, bool strict)
{
    if (fmp.find(lex) == fmp.end())
    {
        // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
        return nullptr;
    }
    SymNode *temp = fmp[lex];
    if (!strict)
    {
        if (argsMatch(temp->args, args) && temp->returntype == returntype)
            return temp;
        return nullptr;
    }
    if (temp->args == args && temp->returntype == returntype)
        return temp;
    // cout<<"Error! No matching function to call. "<<endl;
    return nullptr;
}

SymNode *SymNode::scope_flookup(string lex, vector<int> args, bool strict)
{
    if (fmp.find(lex) == fmp.end())
    {
        // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
        return nullptr;
    }
    SymNode *temp = fmp[lex];
    if (!strict)
    {
        if (argsMatch(temp->args, args))
            return temp;
        return nullptr;
    }
    if (temp->args == args)
        return temp;
    // cout<<"Error! No matching function to call. "<<endl;
    return nullptr;
}

SymNode *SymNode::scope_clookup(string lex)
{
    if (cmp.find(lex) == cmp.end())
    {
        // cout<<"Error! the function with name "<<lex<<" has not been declared!"<<endl;
        return nullptr;
    }
    SymNode *temp = cmp[lex];
    return temp;
}

bool SymNode::scope_constrlookup(vector<int> args)
{
    for (auto it : constr_args)
    {
        if (it == args)
            return true;
    }
    return false;
}
void SymNode::constr_insert(vector<int> args)
{
    constr_args.push_back(args);
}

void SymGlob::dumpClassSymbols()
{
    for (auto it : list_class)
    {
        int scope_num = 0;
        ofstream fout;
        string nm = GetCurrentWorkingDir()+"/temporary/"+(it.first) + ".csv";
        fout.open(nm);
        SymNode *res = it.second;
        int c = 0;
        fout << "Num,Syntactic Category,Lexeme,Type,Line of Declaration" << endl;
        for (auto ch : res->mp)
        {
            fout << scope_num++ << ",";
            fout << "Identifier,";
            Symbol *temp = ch.second;
            fout << temp->lexeme << ",";
            if (temp->type < 100)
            {
                fout << typeroot->inv_types[temp->type] << ",";
            }
            else
            {
                fout << typeroot->inv_types[temp->type % 100];

                if (temp->type > 100)
                {
                    fout << "[";
                    temp->num_elems1 = conv_int(temp->width1);
                    if (temp->num_elems1 > 0 && temp->width1 != "w")
                    {
                        fout << temp->num_elems1;
                    }
                    else
                    {
                        fout << temp->width1;
                    }
                    fout << "]";
                }
                if (temp->type > 200)
                {
                    fout << "[";
                    temp->num_elems2 = conv_int(temp->width2);
                    if (temp->num_elems2 > 0 && temp->width2 != "w")
                    {
                        fout << temp->num_elems2;
                    }
                    else
                    {
                        fout << temp->width2;
                    }
                    fout << "]";
                }
                if (temp->type > 300)
                {
                    fout << "[";
                    temp->num_elems3 = conv_int(temp->width3);
                    if (temp->num_elems3 > 0 && temp->width3 != "w")
                    {
                        fout << temp->num_elems3;
                    }
                    else
                    {
                        fout << temp->width3;
                    }
                    fout << "]";
                }
                fout << ",";
            }
            fout << temp->decl_line_no << endl;
        }
    }
}

void SymGlob::dumpSymbolTable()
{
    this->dumpClassSymbols();
    queue<SymNode *> qg;
    qg.push(orig_root->currNode);
    while (!qg.empty())
    {

        SymNode *cu = qg.front();
        qg.pop();
        if (cu->name == "class" || cu->name == "classextends")
        {

            auto cfmp = cu->fmp;
            for (auto it : cfmp)
            {
                int scope_num = 0;
                ofstream fout;
                string nm = GetCurrentWorkingDir()+"/temporary/"+classfunc[it.first] + "_" + (it.first) + ".csv";
                fout.open(nm);
                SymNode *res = it.second;
                queue<pair<SymNode *, int>> q;
                q.push({res, 0});
                int c = 0;
                fout << "Num,Syntactic Category,Lexeme,Type,Line of Declaration" << endl;
                while (!q.empty())
                {
                    pair<SymNode *, int> p = q.front();

                    if (p.second > c)
                    {
                        c++;
                        cout << '\n';
                    }

                    q.pop();
                    for (auto ch : p.first->mp)
                    {
                        Symbol *temp = ch.second;
                        if(temp->lexeme=="args" && temp->type>100)
                            continue;
                        fout << scope_num++ << ",";
                        fout << "Identifier,";
                        fout << temp->lexeme << "`" << temp->scope_level << ",";
                        if (temp->type < 100)
                        {
                            fout << typeroot->inv_types[temp->type] << ",";
                        }
                        else
                        {
                            fout << typeroot->inv_types[temp->type % 100];

                            if (temp->type > 100)
                            {
                                fout << "[";
                                temp->num_elems1 = conv_int(temp->width1);
                                if (temp->num_elems1 > 0 && temp->width1 != "w")
                                {
                                    fout << temp->num_elems1;
                                }
                                else
                                {
                                    fout << temp->width1;
                                }
                                fout << "]";
                            }
                            if (temp->type > 200)
                            {
                                fout << "[";
                                temp->num_elems2 = conv_int(temp->width2);
                                if (temp->num_elems2 > 0 && temp->width2 != "w")
                                {
                                    fout << temp->num_elems2;
                                }
                                else
                                {
                                    fout << temp->width2;
                                }
                                fout << "]";
                            }
                            if (temp->type > 300)
                            {
                                fout << "[";
                                temp->num_elems3 = conv_int(temp->width3);
                                if (temp->num_elems3 > 0 && temp->width3 != "w")
                                {
                                    fout << temp->num_elems3;
                                }
                                else
                                {
                                    fout << temp->width3;
                                }
                                fout << "]";
                            }
                            fout << ",";
                        }
                        fout << temp->decl_line_no << endl;
                    }

                    for (auto it : p.first->childscopes)
                    {
                        q.push({it, p.second + 1});
                    }
                }
                // fout.close();
            }
        }

        for (auto ch : cu->childscopes)
        {
            if (ch->name != "method")
                qg.push(ch);
        }
    }
}