#include "utils.h"

using namespace std;
#include <unistd.h>
int v = 0;
int cnt = 1;
int isarrayinit = 0;
// map<string, pair<int, int>> typeroot->typewidth;
map<string, SymNode *> list_class;
string otpt;
vector<string> func_names;
vector<string> field_vars;
int startPos = 0;
extern int yylineno;
int temp = 0;
vector<tuple<string, int, int>> funcs;
int varCnt = 0;
int tempCnt = 0;
int labelCnt = 0;
int importflag = 0;
extern int scope_level;
map<string, int> ir_gen_dup;

map<string, string> classfunc;
map<string, int> tempVars;
extern vector<string> static_funcs;
string condvar;
int isCond = 0;
TypeHandler *typeroot = new TypeHandler();

vector<Quadruple *> residualCode;

vector<string> castName(20);
void defineCastNames()
{
    castName[BYTE_NUM] = "byte";
    castName[SHORT_NUM] = "short";
    castName[INT_NUM] = "int";
    castName[LONG_NUM] = "long";
    castName[FLOAT_NUM] = "float";
    castName[DOUBLE_NUM] = "double";
    castName[BOOL_NUM] = "boolean";
    castName[CHAR_NUM] = "char";
    castName[VOID_TYPE] = "void";
}
int whilepos = 0;
SymGlob *root = new SymGlob();
SymGlob *orig_root = root;
SymNode* origNode = new SymNode(nullptr, "global");
SymNode *magic_ptr = origNode;

string spacerem(string s)
{
    string ans = "";
    for(int i=0; i<s.length(); i++)
    {
        if(s[i]==' ')
            continue;
        ans.push_back(s[i]);
    }
    return ans;
}

string append_scope_level(string s)
{
    // if((s[0]>='0' && s[0]<='9') || (s.length()>2 && s[0]=='_' && s[1]=='t' && s[2]>='0' && s[2]<='9') || s[s.length()-1]==')')
    // 

    // cout<<"field vars are"<<endl;
    // for(auto it : field_vars)
    // {
    //     if(it==s)
    //         return s;
    // }
    // cout<<endl;

    cout<<"String is sdfsd "<<s<<" on line "<<yylineno<<endl;

    if(s.find('`') != string::npos || s[s.length()-1]==']')
        return s;
    int isthis = 0;
    int isdot = 0;
    int ind=-1;
    for(int i=0; i<s.length(); i++)
    {
        if(s[i]=='.')
        {
            ind = i;
            break;
        }
    }

    for(auto it : func_names)
    {
        if(it==s && ind==-1)
            return s;
    }

    if(ind>-1 && s.substr(0,4)!="this")
    {
        cout<<"Dot for "<<s<<endl;
        string suf = s.substr(ind+1, s.length()-ind-1);
        s = s.substr(0, ind);
        if(s.length()>4 && s.substr(0,4)=="this")
        {
            s = s.substr(5, s.length()-5);
            isthis = 1;
        }
    // if(scope_level==-1)
    //     cout<<-1<<"for "<<s<<endl;
    string st = spacerem(s);
    if(st.substr(0,3)=="new" && list_class.find(st.substr(3, st.length()-3))!=list_class.end())
        return s;
    cout<<"Couldn't findn "<<s<<" in classes"<<endl;
    if(((s[0]>='a' && s[0]<='z') || (s[0]>='A' && s[0]<='Z')) && s!="true" && s!="false")  
    {
        SymNode* temp = root->currNode;
        Symbol* res;
        while(temp && temp->name!="class")
        {
            res = temp->scope_lookup(s);
            if(res)
                break;
            temp = temp->parent;
        }
        if(temp)
            res=temp->scope_lookup(s);
        cout<<"string is "<<s<<" and global scope level is "<<scope_level<<endl;
        if(res)
            cout<<"Symbol exists and its scope level is "<<res->scope_level<<endl;
        int scope_attach = (res)?max(res->scope_level, 0):max(scope_level, 0);
        if(scope_attach > 10000)
            scope_attach = 0;
        if(isthis)
            s = "this."+s;
        if(!res)
            return s+"`"+to_string(scope_attach)+"."+suf;
        if(res && res->isField)
            return s;
        return s+"`"+to_string(scope_attach)+"."+suf;
    }
    return s;       
    }


    if(s.substr(0,4)=="this")
    {
        s = s.substr(5, s.length()-5);
        isthis = 1;
    }
    // if(scope_level==-1)
    //     cout<<-1<<"for "<<s<<endl;
    string st = spacerem(s);
    if(st.substr(0,3)=="new" && list_class.find(st.substr(3, st.length()-3))!=list_class.end())
        return s;
    cout<<"Couldn't findn "<<s<<" in classes"<<endl;
    if(((s[0]>='a' && s[0]<='z') || (s[0]>='A' && s[0]<='Z')) && s!="true" && s!="false")  
    {
        SymNode* temp = root->currNode;
        Symbol* res;
        while(temp && temp->name!="class")
        {
            res = temp->scope_lookup(s);
            if(res)
                break;
            temp = temp->parent;
        }
        if(temp)
            res=temp->scope_lookup(s);
        cout<<"string is "<<s<<" and global scope level is "<<scope_level<<endl;
        if(res)
            cout<<"Symbol exists and its scope level is "<<res->scope_level<<endl;
        int scope_attach = (res)?max(res->scope_level, 0):max(scope_level, 0);
        if(scope_attach > 10000)
            scope_attach = 0;
        if(isthis)
            s = "this."+s;
        if(!res)
            return s+"`"+to_string(scope_attach);
        // if(res && res->isField)
        //     return s;
        return s+"`"+to_string(scope_attach);
    }
    return s;
}

void ir_class_gen(int index, vector<Quadruple*> ircode, string fln)
{
    ofstream otherFile;
    otherFile.open(GetCurrentWorkingDir()+"/temporary/"+fln+".3ac");
    // cout<<"opened"<<endl;
    // otherFile<<tempVars[fln]<<endl;
    int cnt=0;
    for(int i=index; i<ircode.size(); i++) {
        auto it = ircode[i];
        otherFile << cnt++ << "\t:";
        // cout<<"Started for i : "<<i<<endl;
        if (it->type == 1)
        {
            otherFile << "if" << it->arg1 << "then ";
            continue;
        }
        if (it->type == 2)
        {
            otherFile << "if " << it->arg1 << it->op << it->arg2 << " goto " << it->result << "\n";
            continue;
        }
        if (it->type == 3)
        {
            otherFile << "goto " << it->result << "\n";
            continue;
        }
        else if (it->type == 4)
        {
            if (it->result == "")
                otherFile << "call " << it->arg1  << "\n";
            else
                otherFile << it->result << "=call " << it->arg1  << "\n";
            continue;
        }
        else if (it->type == 5)
        {
            otherFile << "pushparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 6)
        {
            otherFile<<"endclass\n";
            otherFile.close();
            return;
        }
        else if (it->type == 7)
        {
            otherFile << it->arg1 << " " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 8)
        {
            otherFile << "sub  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 9)
        {
            otherFile << "push  " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 10)
        {
            otherFile << "mov  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 11)
        {
            // otherFile << "popparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 12)
        {
            ;
        }
        else if (it->type == 13)
        {
            // otherFile << it->arg1 << "=popparam"
                //    << "\n";
            continue;
        }

        if (it->label != "")
        {
            otherFile << it->label << ": ";
        }
        if (it->arg2 != "")
            otherFile << it->result << "=" << it->arg1 << it->op << it->arg2 << "\n";
        else
            otherFile << it->result << "=" << it->arg1 << "\n";
    }
    otherFile.close();
}

void func_gen_wrapper()
{
    for(auto it : funcs)
    {
        ofstream otherFile;
        string fln = get<0>(it);
        int ind = get<2>(it);
        int lineno = get<1>(it);
        int cnt=0;
        cout<<"Arguments are "<<fln<<", "<<lineno<<", "<<ind<<endl;
        otherFile.open(GetCurrentWorkingDir()+"/temporary/"+classfunc[fln]+"_"+fln+".3ac");
        otherFile<<classfunc[fln]<<","<<tempVars[fln]<<endl;
        while(cnt<lineno)
        {
            // otherFile<<cnt++<<":\n";
            cnt++;
        }

        for(int i=ind; i<ircode.size(); i++) {
        auto it = ircode[i];
        otherFile << cnt++ << "\t:";
        // cout<<"Started for i : "<<i<<endl;
        if (it->type == 1)
        {
            otherFile << "if" << it->arg1 << "then ";
            continue;
        }
        if (it->type == 2)
        {
            otherFile << "if " << it->arg1 << it->op << it->arg2 << " goto " << it->result << "\n";
            continue;
        }
        if (it->type == 3)
        {
            otherFile << "goto " << it->result << "\n";
            continue;
        }
        else if (it->type == 4)
        {
            if (it->result == "")
                otherFile << "call " << it->arg1 << "\n";
            else
                otherFile << it->result << "=call " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 5)
        {
            otherFile << "pushparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 6)
        {
            // cout<<"Entered this"<<endl;
            // ir_func_gen(i, ircode, it->arg1+".3ac");
            otherFile << "beginfunc " << it->arg1 << " ";
            // cout << "beginfunc " << it->arg1 << endl;
            if(it->params.size()==0)
            {
                otherFile<<"\n";
            }
            else
            {
                for(int i=0; i<it->params.size()-1; i++)
                {
                    otherFile<<it->params[i]<<",";
                }
                otherFile<<it->params.back()<<"\n";
            }
            continue;
        }
        else if (it->type == 7)
        {
            
            otherFile << it->arg1 << " " << it->arg2 << "\n";
            cout << "closing the file***************************\n";
            if(it->arg1 == "endfunc" ) { 
                otherFile.close();
                break;
            }
            continue;
        }
        else if (it->type == 8)
        {
            otherFile << "sub  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 9)
        {
            otherFile << "push  " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 10)
        {
            otherFile << "mov  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 11)
        {
            // otherFile << "popparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 12)
        {
            ;
        }
        else if (it->type == 13)
        {
            // otherFile << it->arg1 << " = popparam"
                //    << "\n";
            continue;
        }

        if (it->label != "")
        {
            otherFile << it->label << ": ";
        }
        if (it->arg2 != "")
            otherFile << it->result << "=" << it->arg1 << it->op << it->arg2 << "\n";
        else
            otherFile << it->result << "=" << it->arg1 << "\n";
    }

    }
}

void ir_func_gen(int index, vector<Quadruple*> ircode, string fln)
{
    ofstream otherFile;
    otherFile.open(GetCurrentWorkingDir()+"/temporary/"+classfunc[fln]+"_"+fln+".3ac");
    otherFile<<classfunc[fln]<<","<<tempVars[fln]<<endl;
    int cnt=0;

    for(int i=index; i<ircode.size(); i++) {
        auto it = ircode[i];
        otherFile << cnt++ << "\t:";
        cout << "cnt = " << cnt << "\n";
        // cout<<"Started for i : "<<i<<endl;
        if (it->type == 1)
        {
            otherFile << "if" << it->arg1 << "then ";
            continue;
        }
        if (it->type == 2)
        {
            otherFile << "if " << it->arg1 << it->op << it->arg2 << " goto " << it->result << "\n";
            continue;
        }
        if (it->type == 3)
        {
            otherFile << "goto " << it->result << "\n";
            continue;
        }
        else if (it->type == 4)
        {
            if (it->result == "")
                otherFile << "call " << it->arg1 << "\n";
            else
                otherFile << it->result << "=call " << it->arg1  << "\n";
            continue;
        }
        else if (it->type == 5)
        {
            otherFile << "pushparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 6)
        {
            cout<<"Entered this for "<<it->arg1<<endl;
            // ir_func_gen(i, ircode, it->arg1+".3ac");
            otherFile << "beginfunc " << it->arg1 << " ";
            // cout << "beginfunc " << it->arg1 << endl;
            if(it->params.size()==0)
            {
                otherFile<<"\n";
            }
            else
            {
                for(int i=0; i<it->params.size()-1; i++)
                {
                    otherFile<<it->params[i]<<",";
                }
                otherFile<<it->params.back()<<"\n";
            }
            continue;
        }
        else if (it->type == 7)
        {
            otherFile << it->arg1 << " " << it->arg2 << "\n";
            otherFile.close();
            cout << "*******************closed\n";
            return;
        }
        else if (it->type == 8)
        {
            otherFile << "sub  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 9)
        {
            otherFile << "push  " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 10)
        {
            otherFile << "mov  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 11)
        {
            // otherFile << "popparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 12)
        {
            ;
        }
        else if (it->type == 13)
        {
            // otherFile << it->arg1 << " = popparam"
                //    << "\n";
            continue;
        }

        if (it->label != "")
        {
            otherFile << it->label << ": ";
        }
        if (it->arg2 != "")
            otherFile << it->result << "=" << it->arg1 << it->op << it->arg2 << "\n";
        else
            otherFile << it->result << "=" << it->arg1 << "\n";
    }
    otherFile.close();
}

void ir_gen(vector<Quadruple *> ircode, string fln)
{
    ofstream myFile;
    myFile.open(GetCurrentWorkingDir()+"/temporary/"+fln);
    int cnt = 0;
    for(int i=0; i<ircode.size(); i++) {
        auto it = ircode[i];
        myFile << cnt++ << "\t:";
        if (it->type == 1)
        {
            myFile << "if" << it->arg1 << "then ";
            continue;
        }
        if (it->type == 2)
        {
            myFile << "if " << it->arg1 << it->op << it->arg2 << " goto " << it->result << "\n";
            continue;
        }
        if (it->type == 3)
        {
            myFile << "goto " << it->result << "\n";
            continue;
        }
        else if (it->type == 4)
        {
            if (it->result == "")
                myFile << "call " << it->arg1 << "\n";
            else
                myFile << it->result << "=call " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 5)
        {
            myFile << "pushparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 6)
        {
            // cout<<"Entered this"<<endl;
            cout<<"Arguments are "<<it->arg1<<", "<<cnt-1<<", "<<i<<endl;
            funcs.push_back({it->arg1, cnt-1, i});
            // ir_func_gen(i, ircode, it->arg1);
            myFile << "beginfunc " << it->arg1 << " ";
            // cout << "beginfunc " << it->arg1 << endl;
            if(it->params.size()==0)
            {
                myFile<<"\n";
            }
            else
            {
                for(int i=0; i<it->params.size()-1; i++)
                {
                    myFile<<it->params[i]<<",";
                }
                myFile<<it->params.back()<<"\n";
            }
            continue;
        }
        else if (it->type == 7)
        {
            string s = it->arg1;
            if(s.substr(0, 10)=="beginclass")
                ir_class_gen(i, ircode, s.substr(11, s.length()-11));
            myFile << it->arg1 << " " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 8)
        {
            myFile << "sub  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 9)
        {
            myFile << "push  " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 10)
        {
            myFile << "mov  " << it->arg1 << ", " << it->arg2 << "\n";
            continue;
        }
        else if (it->type == 11)
        {
            // myFile << "popparam " << it->arg1 << "\n";
            continue;
        }
        else if (it->type == 12)
        {
            ;
        }
        else if (it->type == 13)
        {
            // myFile << it->arg1 << " = popparam"
                //    << "\n";
            continue;
        }

        if (it->label != "")
        {
            myFile << it->label << ": ";
        }
        if (it->arg2 != "")
            myFile << it->result << "=" << it->arg1 << it->op << it->arg2 << "\n";
        else
            myFile << it->result << "=" << it->arg1 << "\n";
    }
    myFile.close();
    func_gen_wrapper();
}

void backpatch(vector<int> &lst, int n)
{
    for (auto it : lst)
    {
        std::cout << "calling print\n";
        //ircode[it]->print();
        ircode[it]->set_result(to_string(n));
    }
}

void processFieldDec(Node *n, Node *n1, int type)
{

    if (n1->isCond == 1)
        return;
    cout << "hey\n";
    string resName = (n1->children.size() > 1) ? append_scope_level(n1->children[1]->varName) : "0";
    cout << "hey\n";
    if (n1->children.size() > 1 && type != n1->children[1]->type)
    {
        resName = string("_t") + to_string(varCnt++);
        Quadruple *q = new Quadruple("", "cast_to_" + castName[type] + " ", append_scope_level(n1->children[1]->varName), resName);
        n->code.push_back(q);
        ircode.push_back(q);
    }

    // n1->children[0]->varName = append_scope_level(n1->children[0]->varName);
    Quadruple *q = new Quadruple("=", resName, append_scope_level(n1->children[0]->varName));
    n->code.push_back(q);
    ircode.push_back(q);
    n->last = ircode.size() - 1;
}

void processUninitDec(Node *n, Node *n1, int _type)
{
    if (!n || !n1)
        return;
    string var = n1->attr;
    // var = append_scope_level(var);
    Quadruple* q;
    if(_type >= 1000) {
        q = new Quadruple("=", "0", "static " + append_scope_level(var));
    }
    else {
        q = new Quadruple("=", "0", append_scope_level(var));
    }
    n->code.push_back(q);
    ircode.push_back(q);        
    n->last = ircode.size() - 1;
}
    


int getmethodtype(SymNode *n)
{
    SymNode *temp = n;
    while (temp->name != "method")
    {
        temp = temp->parent;
    }
    return temp->returntype;
}

// n is the VariableDeclarator node,
void init1DArray(Node *n, string type)
{
    Node *arrayName = n->children[0];
    Node *arrayInit = n->children[1]; // ArrayInitializer node
    int sizeArray = arrayInit->children.size();
    for (int i = 0; i < sizeArray; i++)
    {
        Quadruple *q = new Quadruple("", append_scope_level(arrayInit->children[i]->varName), "",append_scope_level(arrayName->varName) + "[" + to_string((i * (typeroot->typewidth[type].second))) + "]");
        ircode.push_back(q);
        n->code.push_back(q);
    }

    n->last = ircode.size() - 1;
}

void init2DArray(Node *n, string type)
{
    Node *arrayName = n->children[0];
    Node *arrayInit = n->children[1]; // ArrayInitializer node
    int sizeArray = arrayInit->children.size();
    int n2 = arrayInit->children[0]->children.size();
    for (int i = 0; i < sizeArray; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            Quadruple *q = new Quadruple("", append_scope_level(arrayInit->children[i]->children[j]->varName), "",append_scope_level(arrayName->varName) + "[" + to_string(((n2 * i + j) * typeroot->typewidth[type].second)) + "]");
            ircode.push_back(q);
            n->code.push_back(q);
        }
    }

    n->last = ircode.size() - 1;
}

void init3DArray(Node *n, string type)
{
    Node *arrayName = n->children[0];
    Node *arrayInit = n->children[1]; // ArrayInitializer node
    int sizeArray = arrayInit->children.size();
    int n2 = arrayInit->children[0]->children.size();
    int n3 = arrayInit->children[0]->children[0]->children.size();
    for (int i = 0; i < sizeArray; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            for (int k = 0; k < n3; k++)
            {
                Quadruple *q = new Quadruple("", append_scope_level(arrayInit->children[i]->children[j]->children[k]->varName), "", append_scope_level(arrayName->varName) + "[" + to_string(((i * n2 * n3 + j * n3 + k) * typeroot->typewidth[type].second)) + "]");
                ircode.push_back(q);
                n->code.push_back(q);
            }
        }
    }

    n->last = ircode.size() - 1;
}

void processPostIncre(Node *n)
{
    if (residualCode.size() == 0)
        return;
    if (n == NULL)
        return;
    ircode.insert(ircode.end(), residualCode.begin(), residualCode.end());
    n->code.insert(n->code.end(), residualCode.begin(), residualCode.end());
    residualCode.clear();
    n->last = ircode.size() - 1;
}

void processAssignment(Node *n1, Node *n2, Node *n3, Node *n)
{
    char oper = n2->attr[0];
    vector<Node *> nodes = {n, n1, n3};

    processArithmetic(nodes, string(1, oper));
    // cout << oper<< " dsad\n";
    // if(oper != "=")
    Quadruple *q = new Quadruple("=", append_scope_level(n->varName),append_scope_level(n1->varName));
    n->varName = n1->varName;
    n->code.push_back(q);
    ircode.push_back(q);
    n->last = ircode.size() - 1;

    return;
}

int spacestripind(string s)
{
    int n = s.length();
    int i = n - 1;
    while (i >= 0 && s[i] != '.')
        i--;
    return i;
}

string spacestrip(string s)
{
    int n = s.length();
    int i = n - 1;
    while (i >= 0 && s[i] != '.')
        i--;
    return s.substr(i + 1, n - i - 1);
}

void processRelational(vector<Node *> nodes, string op)
{
    nodes[0]->truelist.push_back(ircode.size());
    nodes[0]->falselist.push_back(ircode.size() + 1);

    Quadruple *q = new Quadruple(2, op,append_scope_level(nodes[1]->varName),append_scope_level(nodes[2]->varName), "");
    nodes[0]->code.push_back(q);
    ircode.push_back(q);
    q = new Quadruple(3, "", "", "", "");
    nodes[0]->code.push_back(q);
    ircode.push_back(q);

    nodes[0]->last = ircode.size() - 1;
    nodes[0]->last = ircode.size() - 1;
}

void processArithmetic(vector<Node *> nodes, string op)
{
    string resName = string("_t") + to_string(varCnt);
    varCnt++;
    int type1 = typeroot->categorize(nodes[1]->type);
    int type2 = typeroot->categorize(nodes[2]->type);
    int flag = 0;
    if (type1 == FLOATING_TYPE && type2 == INTEGER_TYPE)
    {
        string resName = string("_t") + to_string(varCnt++);
        Quadruple *q = new Quadruple("", "cast_to_float ",append_scope_level(nodes[2]->varName), resName);
        nodes[0]->code.push_back(q);
        ircode.push_back(q);
        flag = 2;
    }
    else if (type1 == INTEGER_TYPE && type2 == FLOATING_TYPE)
    {
        string resName = string("_t") + to_string(varCnt++);
        Quadruple *q = new Quadruple("", "cast_to_float ",append_scope_level(nodes[1]->varName), resName);
        nodes[0]->code.push_back(q);
        ircode.push_back(q);
        flag = 2;
    }
    if (type1 == FLOATING_TYPE || type2 == FLOATING_TYPE)
        flag = 2;
    if (type1 == INTEGER_TYPE && type2 == INTEGER_TYPE)
        flag = 1;
    Quadruple *q = new Quadruple(op, append_scope_level(nodes[1]->varName),append_scope_level(nodes[2]->varName), resName);
    nodes[0]->varName = resName;
    nodes[0]->code.push_back(q);
    ircode.push_back(q);
    nodes[0]->last = ircode.size() - 1;
}

void processDoWhile(Node *n, Node *n1, Node *n2)
{
    backpatch(n2->nextlist, n2->last + 1);
    backpatch(n1->truelist, n1->last + 1);
    n->nextlist = n1->falselist;
    Quadruple *q = new Quadruple(3, "", "", "", to_string(n2->last + 1 - n2->code.size()));
    ircode.push_back(q);
    n->code.push_back(q);
    n->last = ircode.size() - 1;
}

void processWhile(Node *n, Node *n1, Node *n2)
{
    backpatch(n2->nextlist, n1->last + 1 - n1->code.size());
    backpatch(n1->truelist, n1->last + 1);
    n->nextlist = n1->falselist;
    Quadruple *q = new Quadruple(3, "", "", "", to_string(n1->last + 1 - n1->code.size()));
    ircode.push_back(q);
    n->code.push_back(q);
    n->last = ircode.size() - 1;
}

int generateArgumentList(vector<Node *> nodes, Node *n)
{
    int space = 0;
    for (int i = nodes.size() - 1; i >= 0; i--)
    {
        Node *node = nodes[i];
        Quadruple *q = new Quadruple(5, append_scope_level(node->varName));
        if (typeroot->widths[node->type] == 0)
            space += 8;
        else
            space += 8;

        n->code.push_back(q);
        ircode.push_back(q);
    }
    Quadruple *q = new Quadruple("+ ", "stackpointer", to_string(space), "stackpointer");
    n->code.push_back(q);
    ircode.push_back(q);
    n->last = ircode.size() - 1;
    return space;
}

void verbose(int v, string h)
{
    if (v == 1)
    {
        cout << cnt << ") " << h << "\n";
        cnt++;
    }
}
