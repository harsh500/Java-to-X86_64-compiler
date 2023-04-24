/*
Function naming convention: <className>.<functionName> as is in 3ac

*/

#include <bits/stdc++.h>
#include <unistd.h>
#define PUSHQ "pushq\t"
#define MOVL "movl\t"
#define MOVQ "movq\t"
#define SUBQ "subq\t"
#define RBP "%rbp"
#define RSP "%rsp"
#define ADDQ "addq\t"
#define SUBQ "subq\t"
#define MULQ "imulq\t"
#define DIVQ "divq\t"
#define XORQ "xorq\t"
#define DEBUG 1
#define DBG if (DEBUG)
using namespace std;

string consOrVar(string);
int temporary_size; // hold the size of temporary variable used in a given fucntion
vector<char> ops = {'+', '-', '*', '/', '^', '&', '%'};
vector<string> relOps = {"<=", ">=", "==", "!=", ">", "<"};

map<string, int> sizes;

map<char, string> opConv;
map<string, vector<int>> var; // it keeps the data
                              //  its struct is map<var_name,{type,isarg,w1,w2,w3}>
map<string, string> relConv;

map<string, int> addressRegDes; // address descriptor
map<string, int> addressDes;    // describes the address relative to rbp for a given variable
map<string, bool> mem;          // whether value in memory is correct value of the variable
map<string, int> classSize;     // size of a class object assuming size of int as 8 bytes
map<string, string> objClass;   // keeps track of classes of object

string currClassName = ""; // current class being handled
string currFuncName = "";  // current function being dealt with in the code
string mainClassName = ""; // class having the main function

vector<int> genregs = {0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15};

vector<bool> islabel(2e5 + 1, false);

void beg_func(string x, vector<string> &funCode);
void func_call(vector<string> a, vector<string> &funcCode);
void ary_acc(string lex, int tp, string v1, string v2, string v3, string r, vector<string> &funCode);
void ary_ass(string lex, int tp, string v1, string v2, string v3, string val, vector<string> &funCode);
void call_malloc(string name, int tp, string s1, string s2, string s3, vector<string> &funCode);

void call_malloc(string name, int tp, string s1, string s2, string s3, vector<string> &funCode);
/************REG CLASS***********/
class reg
{
    int id;
    string regName;
    map<string, int> regDes; // register descriptor, as in slides, assuming a reg
public:
    reg(){};
    void init(int id, string regName);
    reg(int id, string regName);
    string getname();
    void addRegDes(string s);
    void rmRegDes(string s);
    bool getRegDes(string s);
    int getRegDesSize();
    bool isEmpty();
    vector<string> storeall();

} r[16];
string GetCurrentWorkingDir(void)
{
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}
void reg::init(int id, string regName)
{
    this->id = id;
    this->regName = regName;
}

std::string reg::getname()
{
    return regName;
}

int reg::getRegDesSize()
{
    int cnt = 0;
    for (auto it : regDes)
    {
        if (it.second == true)
            cnt++;
    }
    return cnt;
}

reg::reg(int id, string regName)
{
    this->id = id;
    this->regName = regName;
}

void declareRegs()
{
    // %rax, %rbx, %rcx, %rdx: 64 bit regs any purpose
    // %rax is used for 1st argument of syscall
    r[0].init(0, "%rax");
    r[1].init(1, "%rbx");
    r[2].init(2, "%rcx");
    r[3].init(3, "%rdx");
    r[4].init(4, "%rsi");
    r[5].init(5, "%rdi");
    r[6].init(6, "%rbp");
    r[7].init(7, "%rsp");
    r[8].init(8, "%r8");
    r[9].init(9, "%r9");
    r[10].init(10, "%r10");
    r[11].init(11, "%r11");
    r[12].init(12, "%r12");
    r[13].init(13, "%r13");
    r[14].init(14, "%r14");
    r[15].init(15, "%r15");
}

/************UTILITY FUNCTIONS***************/
bool is_number(const std::string &s)
{
    static const std::regex re("-?[0-9]+(\\.[0-9]*)?");
    return std::regex_match(s, re);
}

int countOccurrences(char c, string str)
{
    int count = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == c)
        {
            count++;
        }
    }
    return count;
}

int getAddressDes(string varname)
{
    // if(varname.find('`') != string::npos) {
    //     varname = varname.substr(0, varname.find('`'));
    // }
    // // if(varname.find("this") != string::npos) {
    //     return addressDes[currClassName + "::::" + varname];
    // }
    cout << "looking for " << currClassName + "::" + currFuncName + "::" + varname << "\n";
    // for (auto it : addressDes)
    // {
    //     cout << it.first << " , ";
    // }

    if (addressDes.find(currClassName + "::" + currFuncName + "::" + varname) != addressDes.end())
        return addressDes[currClassName + "::" + currFuncName + "::" + varname];
    else if(addressDes.find(currClassName + "::::" + varname) != addressDes.end())
        return addressDes[currClassName + "::::" + varname];
    else {
        if(varname.find('`') != string::npos) {
            varname = varname.substr(0, varname.find('`'));
            return addressDes[currClassName + "::" + currFuncName + "::" + varname];
        }
        return addressDes[currClassName + "::::" + varname];
    }
}

vector<string> getArrayIndices(string s)
{
    vector<string> ans;
    string t1 = "", t2 = "", t3 = "";
    int numBrackets = countOccurrences('[', s);
    string t = s;
    if (numBrackets >= 1)
    {
        int startpos = t.find('[');
        int endpos = t.find(']');
        t1 = t.substr(startpos + 1, endpos - startpos - 1);
        cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$dim1 = " << t1 << "\n";
        t = t.substr(endpos+1);
    }
    if (numBrackets >= 2)
    {
        int startpos = t.find('[');
        int endpos = t.find(']');
        cout << "T = " << t << endl;
        cout << "issue2\n";
        cout << t.substr(startpos + 1, endpos - startpos - 1) << "\n";
        t2 = t.substr(startpos + 1, endpos - startpos - 1);
        t = t.substr(endpos+1);
    }
    if (numBrackets >= 3)
    {
        int startpos = t.find('[');
        int endpos = t.find(']');
        cout << "iss\n";
        t3 = t.substr(startpos + 1, endpos - startpos - 1);
        // t = t.substr();
    }
    if (is_number(t1))
        t1 = string("$") + t1;
    else
        t1 = to_string(getAddressDes(t1)) + "(%rbp)";
    if (is_number(t2))
        t2 = string("$") + t2;
    else
        t2 = to_string(getAddressDes(t2)) + "(%rbp)";
    if (is_number(t3))
        t3 = string("$") + t3;
    else
        t3 = to_string(getAddressDes(t3)) + "(%rbp)";

    ans.push_back(t1);
    ans.push_back(t2);
    ans.push_back(t3);
    return ans;
}

string trimInstr(string instr)
{
    if (instr.find(":") != string::npos)
    {
        return instr.substr(instr.find(":") + 1);
    }
    return instr;
}

string strip(string s)
{
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

// get line no of 3ac instruction
int getLineNo(string instr)
{
    int i = 0;
    int lineNo = 0;
    while (i < instr.size() && isdigit(instr[i]))
    {
        lineNo = lineNo * 10 + (instr[i] - '0');
        i++;
    }
    return lineNo;
}

// check if the passed string is a constant or a variable
string consOrVar(string x)
{
    if (x.rfind("pushparam", 0) != 0)
    {
        cout << "Error the instruction not start with pushparam\n";
    }
    string temp;
    int f = 0;
    for (int j = 0; j < x.size(); j++)
    {
        if (x[j] == ' ')
        {
            f = 1;
        }
        else
        {
            if (f == 1)
            {
                temp.push_back(x[j]);
            }
        }
    }
    f = 0;
    for (int j = 0; j < temp.size(); j++)
    {

        if (!is_number(strip(temp)))
        {
            f = 1;
            break;
        }
    }
    string ans;
    if (f == 1)
    {
        cout << "******************x = " << temp << "\n";
        ans = to_string(getAddressDes(strip(temp))) + "(%rbp)";
    }
    else
    {
        ans = string("$") + strip(temp);
    }
    return ans;
}

vector<vector<string>> read_csv(string filename)
{
    vector<vector<string>> data;
    ifstream file;
    file.open(filename);
    if (!file)
    {
        cout << "RRRR-" << filename << "can'tbe opened\n";
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        vector<string> row;

        string cell;
        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        data.push_back(row);
    }
    file.close();
    return data;
}

/*
General tips:
- %rsi and %rdi are used for passing function arguments



*/

// class for a register

// return move instruction
string genMove(string src, string dest)
{
    // check if src is a number
    cout << src << " in genMove\n";
    if (is_number(src))
    {
        // int val = stoi(src);
        src = string("$") + src;
    }
    else if (src[0] != '%')
    {
        src = to_string(getAddressDes(src)) + string("(%rbp)");
    }
    if (dest[0] != '%')
    {
        dest = to_string(getAddressDes(dest)) + string("(%rbp)");
    }

    // else {
    //     src = to_string(addressDes[src]) + string("(%rbp)");
    // }
    return MOVQ + src + ", " + dest;
}

string genArithmetic(string op, string src, string dest)
{
    if (op.size() == 0)
        return string("error");
    return opConv[op[0]] + src + ", " + dest;
}

vector<string> cmpOps = {"==", "<=", ">=", ">", "<", "!="};

vector<string> identifyInstr(string instr)
{
    /*


    assume nstruction starts immediately after colon
    */
    // string instr = lines[0];
    vector<string> ans;
    if (DEBUG)
        cout << "hey there\n";
    size_t colpos = instr.find(":");

    int line_num = getLineNo(instr);

    // put label if required
    if (islabel[line_num])
    {
        string ins = ".L" + to_string(line_num) + ":";
        ans.push_back(ins);
    }

    // consider instr only after colon, line no already dealt with
    instr = instr.substr(colpos + 1);
    DBG cout << "hey\n";
    size_t eqpos = instr.find('=');
    if (eqpos != string::npos && !(instr.find("!=") != string::npos) && !(instr.find("==") != string::npos) && !(instr.find(">=") != string::npos) && !(instr.find("<=") != string::npos))
    {
        //
        DBG cout << "hey here\n";
        string s = instr.substr(eqpos + 1);
        int flag = 0; // to check for arithmetic ops
        string x = instr.substr(0, eqpos);
        int dimx, dimy, dimz;
        dimx = countOccurrences('[', x);
        if (DEBUG)
            cout << "x != " << x << " " << instr << "\n";
        for (auto op : ops)
        {
            if (s.find(op) != string::npos)
            {
                string y = s.substr(0, s.find(op));
                string z = s.substr(s.find(op) + 1);
                int dimlhs = 0, dimrhs = 0;

                dimy = countOccurrences('[', y);
                dimz = countOccurrences('[', z);

                if (DEBUG)
                    cout << "y, z = " << y << ", " << z << "\n";
                // move to rbx and rcx
                // string ins1 = genMove(y, "%rbx");
                // string ins2 = genMove(z, "%rcx");
                if (dimy > 0)
                {
                    vector<string> ind = getArrayIndices(y);

                    ary_acc(y.substr(0, y.find('[')), dimy * 100, ind[0], ind[1], ind[2], "%rbx", ans);
                }
                else if (y.find("this") != string::npos)
                {
                    // assumong to be of type this.simething
                    int relpos = getAddressDes(y);
                    cout << "yy = " << y << "\n";
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rax";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rax";
                    ans.push_back(instr);
                    instr = string(MOVQ) + "(%rax), %rbx";
                    ans.push_back(instr);
                }
                else
                {
                    ans.push_back(genMove(y, "%rbx"));
                    cout << "y = " << y << "\n";
                }
                if (dimz > 0)
                {
                    vector<string> ind = getArrayIndices(z);

                    ary_acc(z.substr(0, z.find('[')), dimz * 100, ind[0], ind[1], ind[2], "%rcx", ans);
                }
                else if (z.find("this") != string::npos)
                {
                    // assumong to be of type this.simething
                    int relpos = getAddressDes(z);
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rax";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rax";
                    ans.push_back(instr);
                    instr = string(MOVQ) + "(%rax), %rbx";
                    ans.push_back(instr);
                }
                else
                {
                    ans.push_back(genMove(z, "%rcx"));
                }

                string ins3 = genArithmetic(s.substr(s.find(op), 1), "%rcx", "%rbx");
                string ins4 = genMove("%rbx", x);
                // ans.push_back(ins1);
                ans.push_back(ins3);
                if (x.find("this") == string::npos)
                {
                    if (dimx == 0)
                        ans.push_back(ins4);
                    else
                    {
                        vector<string> ind = getArrayIndices(x);
                        ary_ass(x.substr(0, x.find('[')), dimx * 100, ind[0], ind[1], ind[2], "%rcx", ans);
                    }
                }

                else
                {
                    int relpos = getAddressDes(x);
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rax";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rax";
                    ans.push_back(instr);
                    instr = MOVQ + string("%rcx, (%rax)");
                    ans.push_back(instr);
                }

                // add code
                // let x, y, x be st x = y + z
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            // copy instruction

            string s = instr.substr(eqpos + 1);   // rhs
            string var1 = instr.substr(0, eqpos); // lhs
            int dimlhs = countOccurrences('[', var1);
            int dimrhs = countOccurrences('[', s);
            if (s.substr(0, 7) == "new int")
            {
                vector<string> temp = getArrayIndices(s.substr(4));
                cout << temp[0] << " = temp"
                     << "\n";
                // for(int i = 0; i < dimrhs; i++) {
                //      if(is_number(temp[i])) {
                //         temp[i] = string("$") + temp[i];
                //      }
                //      else if(temp[i]!= "") {
                //         temp[i] = getAddressDes(temp[i]) + "(%rbp)";
                //      }
                // }
                cout << temp[0] << " = temp"
                     << "\n";
                call_malloc(var1, dimrhs * 100, temp[0], temp[1], temp[2], ans);
                return ans;
            }
            if (s.substr(0, 3) == "new")
            {
                // class constructor is being called

                string classname= s.substr(s.find(' ') + 1);

                call_malloc(var1,4*100,var1, to_string(classSize[classname]), "" , ans);
                string ins = string(MOVQ) + to_string(getAddressDes(var1)) + string("(%rbp), %rdi");

                ans.push_back(ins);
                // ins = "call\t" + objClass[var1] + "-" + objClass[var1];
                // ans.push_back(ins);
                return ans;
            }
            if (s.size() && is_number(s))
            {
                // rhs is a number
                // string var2 = instr.substr(0, eqpos);
                DBG cout << "getting address for " << var1 << " as " << getAddressDes(var1) << "\n";
                string ins = MOVQ + string("$") + s + string(", ") + to_string(getAddressDes(var1)) + string("(%rbp)");
                if (dimlhs == 0)
                    ans.push_back(ins);
                else if (var1.find("this") != string::npos)
                {
                    int relpos = getAddressDes(var1);
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rax";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rax";
                    ans.push_back(instr);
                    instr = MOVQ + string("$") + s + string(", (%rax)");
                    ans.push_back(instr);
                }
                else
                {
                    vector<string> ind = getArrayIndices(var1);
                    ary_ass(var1.substr(0, var1.find('[')), dimlhs * 100, ind[0], ind[1], ind[2], string("$") + s, ans);
                }
            }
            else
            {
                // rhs is a variable
                // move rhs to %rbx
                if (dimrhs > 0)
                {
                    vector<string> ind = getArrayIndices(s);
                    ary_acc(s.substr(0, s.find('[')), dimrhs * 100, ind[0], ind[1], ind[2], "%rbx", ans);
                }
                else if (s.find("this") != string::npos)
                {
                    // assumong to be of type this.simething
                    int relpos = getAddressDes(s);
                    cout << "s = " << s <<'\n';
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rax";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rax";
                    ans.push_back(instr);
                    instr = string(MOVQ) + "(%rax), %rbx";
                    ans.push_back(instr);
                }

                else
                {
                    string ins = MOVQ + to_string(getAddressDes(s)) + string("(%rbp), %rbx");
                    ans.push_back(ins);
                }
                string ins = MOVQ + string("%rbx, ") + to_string(getAddressDes(var1)) + string("(%rbp)");
                if (dimlhs == 0)
                {
                    if (var1.find("this") != string::npos)
                    {
                        int relpos = getAddressDes(var1);
                        cout << "var1 = " << var1 << "\n";
                        cout << "relpos  = "<< relpos << "\n";
                        string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rax";
                        ans.push_back(instr);
                        instr = ADDQ + string("$") + to_string(relpos) + ", %rax";
                        ans.push_back(instr);
                        instr = MOVQ + string("%rbx, (%rax)");
                        ans.push_back(instr);
                    }
                    else
                    {
                        ans.push_back(ins);
                    }
                }
                else
                {
                    vector<string> ind = getArrayIndices(var1);
                    ary_ass(var1.substr(0, var1.find('[')), dimlhs * 100, ind[0], ind[1], ind[2], "%rbx", ans);
                }
            }
        }
    }
    else
    {
        // if then else, goto, call, pushparam, popparam

        if (instr.find("if") != string::npos)
        {
            cout << "if found\n";
            string t = instr.substr(instr.find("if"));
            string var1, var2, gotoloc;
            int gotopos = t.find("goto");
            ans.push_back(".L" + to_string(line_num) + ":");
            int relpos = 0;
            for (auto ch : relOps)
            {
                if (t.find(ch) != string::npos)
                {
                    relpos = t.find(ch);
                    break;
                }
            }
            var1 = t.substr(3, relpos - 3);
            int relEnd = relpos;
            if (!isalnum(t[relpos + 1]))
                relEnd++;
            var2 = t.substr(relEnd + 1, gotopos - relEnd - 2);

            DBG cout << var1 << " var 1 " << var2 << "\n";
            int dimlhs = 0, dimrhs = 0;

                dimlhs = countOccurrences('[', var1);
                dimrhs = countOccurrences('[', var2);
                string y = var1, z = var2;
                int dimy = dimlhs, dimz = dimrhs;
                // if (DEBUG)
                //     cout << "y, z = " << y << ", " << z << "\n";
                // move to rbx and rcx
                // string ins1 = genMove(y, "%rbx");
                // string ins2 = genMove(z, "%rcx");
                if (dimlhs > 0)
                {
                    vector<string> ind = getArrayIndices(var1);

                    ary_acc(y.substr(0, y.find('[')), dimlhs * 100, ind[0], ind[1], ind[2], "%rax", ans);
                }
                else if (y.find("this") != string::npos)
                {
                    // assumong to be of type this.simething
                    int relpos = getAddressDes(y);
                    cout << "yy = " << y << "\n";
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rbx";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rbx";
                    ans.push_back(instr);
                    instr = string(MOVQ) + "(%rbx), %rax";
                    ans.push_back(instr);
                }
                else
                {
                    ans.push_back(genMove(y, "%rax"));
                    cout << "y = " << y << "\n";
                }
                if (dimz > 0)
                {
                    vector<string> ind = getArrayIndices(z);

                    ary_acc(z.substr(0, z.find('[')), dimz * 100, ind[0], ind[1], ind[2], "%rcx", ans);
                }
                else if (z.find("this") != string::npos)
                {
                    // assumong to be of type this.simething
                    int relpos = getAddressDes(z);
                    cout << "yy = " << z << "\n";
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rbx";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rbx";
                    ans.push_back(instr);
                    instr = string(MOVQ) + "(%rbx), %rcx";
                    ans.push_back(instr);
                }
                else
                {
                    ans.push_back(genMove(z, "%rcx"));
                }

            string ins = "cmpq\t%rcx, %rax";
            string ins_ = relConv[t.substr(relpos, relEnd - relpos + 1)] + " .L" + t.substr(gotopos + 5);
            DBG cout << ins_ << "\n";
            

            // ans.push_back(ins1);
            // ans.push_back(ins2);
            ans.push_back(ins);
            ans.push_back(ins_);
            gotoloc = t.substr(gotopos + 5);
            cout << "use\n";
            int gotoval = stoi(gotoloc);
            islabel[gotoval] = true;
            return ans;
        }
        else if (instr.find("goto") != string::npos)
        {
            // pure goto instruction with no if
            cout << "use\n";
            int gotoval = stoi(instr.substr(5));
            islabel[gotoval] = true;
             
            string ins = string("jmp .L") + to_string(gotoval);
            ans.push_back((ins));
        }
        else if (instr.substr(0, 5) == "print")
        {
            string ins1 = MOVQ + string("$0, %rax");
            string varName = instr.substr(6);
            // cout << line << "\n";
            if (DEBUG)
                cout << varName << " heree var name\n";
            string t;
            t = to_string(getAddressDes(varName)) + string("(%rbp)");
            string ins2 = MOVQ + string("$printfmt, %rdi");
            string ins3 = MOVQ + t + string(", %rsi");
            
            // ans.push_back(ins3);
            int dimz = countOccurrences('[', varName);
            string z = varName;
            if (dimz > 0)
                {
                    vector<string> ind = getArrayIndices(varName);

                    ary_acc(z.substr(0, z.find('[')), dimz * 100, ind[0], ind[1], ind[2], "%r10", ans);
                }
                else if (z.find("this") != string::npos)
                {
                    // assumong to be of type this.simething
                    int relpos = getAddressDes(z);
                    cout << "yy = " << z << "\n";
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rbx";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rbx";
                    ans.push_back(instr);
                    instr = string(MOVQ) + "(%rbx), %r10";
                    ans.push_back(instr);
                }
                else
                {
                    ans.push_back(genMove(z, "%r10"));
                }
                ans.push_back(ins1);
                ans.push_back(ins2);
                ans.push_back(MOVQ  + string("%r10, %rsi"));
            ans.push_back("call printf");
            return ans;
        }
        else if (instr.substr(0, 6) == "return" && instr.length() > string("return").length())
        {
            // put return value in %rax since a value is being returned here

            string retvar = instr.substr(instr.find(' ') + 1);
            int dimz = countOccurrences('[', retvar);
            string z = retvar;
            if (dimz > 0)
                {
                    vector<string> ind = getArrayIndices(retvar);

                    ary_acc(z.substr(0, z.find('[')), dimz * 100, ind[0], ind[1], ind[2], "%rax", ans);
                }
                else if (z.find("this") != string::npos)
                {
                    // assumong to be of type this.simething
                    int relpos = getAddressDes(z);
                    cout << "yy = " << z << "\n";
                    string instr = MOVQ + to_string(getAddressDes("this")) + "(%rbp), %rbx";
                    ans.push_back(instr);
                    instr = ADDQ + string("$") + to_string(relpos) + ", %rbx";
                    ans.push_back(instr);
                    instr = string(MOVQ) + "(%rbx), %rax";
                    ans.push_back(instr);
                }
                else
                {
                    ans.push_back(genMove(z, "%rax"));
                }
                return ans;
                // ans.push_back(ins1);
                // ans.push_back(ins2);
            // ans.push_back("call printf");
            // string ins = genMove(retvar, "%rax");
            // ans.push_back(ins);
        }
    }
    return ans;
}

// generate the assembly code for a function
vector<string> genfunc(string funcName)
{
    // open the csv corresponding to the function name

    ifstream file2(GetCurrentWorkingDir() + "/temporary/" + currClassName + "_" + currFuncName + ".3ac");
    // vector<vector<string>> data;
    cout << currClassName + "_" + currFuncName + ".3ac"
         << "uuuuuuu\n";
    vector<string> funcCode;
    string y;
    if (funcName == "main")
    {
        y = "\n\n.globl main";
    }
    else
    {
        y = "\n\n.globl " + currClassName + "_" + funcName;
    }
    funcCode.push_back(y);
    string line;
    getline(file2, line);
    getline(file2, line);
    cout << "line: " << trimInstr(line) << "\n";
    beg_func(trimInstr(line), funcCode);

    if (!file2.is_open())
    {
        cout << "Error opening file" << endl;
        return funcCode;
    }

    // string line;
    cout << "file opened!\n";
    int linecnt = 0;
    int cnt = 0;
    while (getline(file2, line))
    {
        cnt++;
        cout << "cnt = " << cnt << "\n";
        if (DEBUG)
            cout << "line: " << line << "\n";
        vector<string> lines;
        int isfunc = 0;
        while (line.find("pushparam") != string::npos)
        {
            lines.push_back(trimInstr(line));
            if (islabel[getLineNo(line)])
            {
                string ins = ".L" + to_string(getLineNo(line)) + ":";
                funcCode.push_back(ins);
            }
            isfunc = 1;
            getline(file2, line);
            cout << "##################Line is : " << line << "\n";
            // lines.push_back(line);
        }

        if (isfunc)
        {
            getline(file2, line);
            cout << "##################Line is : " << line << "\n";
            // getline(file2, line);
            int isret = 0;
            if (trimInstr(line).find('=') != string::npos)
            {
                // function is returning
                string t = line.substr(line.find('=') + 1);
                lines.push_back(t);
                isret = 1;
            }
            else
                lines.push_back(trimInstr(line));
            // lines.push_back(trimInstr(line));
            cout << "##################Line is : " << line << "\n";
            if (line.find("call") != string::npos)
            {
                func_call(lines, funcCode);
            }
            if (line.find("new") != string::npos)
            {
                // possibly a constructor for a class
                lines.pop_back();
                string lhs = trimInstr(line).substr(0, trimInstr(line).find('='));
                string rhs = trimInstr(line).substr(trimInstr(line).find('=') + 1);
                string classname = rhs.substr(rhs.find(' ') + 1);
                // class constructor is being called
                call_malloc(lhs, 4 * 100, lhs, to_string(classSize[classname]), "", funcCode);
                string ins = string(MOVQ) + to_string(getAddressDes(lhs)) + string("(%rbp), %rdi");
                funcCode.push_back(ins);
                ins = "call " + classname;
                cout << "ins = " << ins << "\n";
                lines.push_back(ins);
                ins = string(MOVQ) + to_string(getAddressDes(lhs)) + "(%rbp), %rdi";
                funcCode.push_back(ins);
                func_call(lines, funcCode);
                isret = 0;
                // ans.push_back(ins);
                // return ans;
            }
            if (isret)
            {
                funcCode.push_back(genMove("%rax", trimInstr(line).substr(0, trimInstr(line).find('='))));
            }
            getline(file2, line);
            cout << "##################Line is : " << line << "\n";
            continue;
        }
        if (line.find("call") != string::npos)
        {
            if (islabel[getLineNo(line)])
            {
                string ins = ".L" + to_string(getLineNo(line)) + ":";
                funcCode.push_back(ins);
            }
            lines.push_back(line);
            func_call(lines, funcCode);
            continue;
        }

        // int numBrackets = 0;
        // if((numBrackets = countOccurrences('[', line)) > 0){
        //     if(numBrackets == 1) {

        //     }
        // }
        cout << trimInstr(line) << "\n";
        cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
        if (trimInstr(line).substr(0, 7) == "endfunc")
        {
            if (islabel[getLineNo(line)])
            {
                funcCode.push_back(".L" + to_string(getLineNo(line)) + ":");
            }
            funcCode.push_back("leave");
            funcCode.push_back("ret");
            currFuncName = "";
            file2.close();
            return funcCode;
        }

        vector<string> t = identifyInstr(line);
        cout << "identify called\n";
        for (auto it : t)
        {
            DBG cout << it << "\n";
        }
        if (t.size())
        {
            DBG cout << "ret\n";
            cout << "size is " << funcCode.size() << "\n";
            for (auto it : t)
            {
                funcCode.push_back(it);
            }
            DBG cout << "done\n";
            cout << "printing funccode\n";
            for (auto it : funcCode)
            {
                cout << it << "\n";
            }
        }
        cout << "funccode size: " << funcCode.size() << "\n";
    }

    // closing the function code

    file2.close();
    return funcCode;
    // Access the data by index
    // cout << data[0][0] << endl; // Print the first cell of the first row
}

void handleClassDec(string filename)
{
    string line;
    temporary_size = 0;
    ifstream fp(GetCurrentWorkingDir() + "/temporary/" + filename);
    getline(fp, line);
    line = trimInstr(line);
    if (strip(line.substr(0, 10)) == "beginclass")
    {
        currClassName = strip(line.substr(11));
        cout << "Curr class declared as: " << currClassName << "!\n";
        vector<vector<string>> data = read_csv(GetCurrentWorkingDir() + "/temporary/" + currClassName + ".csv");
        getline(fp, line);
        cout << "line = " << line << " " << (line.find("endclass") == string::npos) << "\n";

        while (line.find("endclass") == string::npos)
        {
            // getline(file2, line);
            // two things, integer variable or array declaration, or even class declaration
            // addressDes[currClassName + "::" + line.substr()]
            // assuming there is a csv file having variable info, type of variable and name of variable
            cout << "the hash is here\n";
            int pos = -8;
            classSize[currClassName] = 0;
            for (int i = 1; i < data.size(); i++)
            {
                string var = data[i][2];

                string typeName = data[i][3];
                objClass[var] = typeName;
                cout << "type = " << typeName << " " << var << "\n";
                if (sizes.find(typeName) != sizes.end())
                {
                    classSize[currClassName] += sizes[typeName];
                    addressDes[currClassName + "::" + currFuncName + "::" + var] = pos;
                    pos -= 8;
                    cout << "dec here :" << currClassName + "::" + currFuncName + "::" + var << classSize[currClassName] << "\n";
                }
                else if (classSize.find(data[i][2]) != classSize.end())
                {
                    classSize[currClassName] += classSize[data[i][3]];
                    addressDes[currClassName + "::" + var] = pos;
                    pos = pos - classSize[data[i][3]];
                }
                else
                {
                    // array type
                    int dim = 0;
                    int w1 = 0, w2 = 0, w3 = 0;
                    if (data[i][3].length() < 5)
                    {
                        cout << "symbol table type undefined\n";
                    }
                    else
                        dim++;
                    w1 = data[i][3][4] - '0';
                    if (data[i][3].length() > 7)
                    {
                        w2 = data[i][3][7] - '0';
                        dim++;
                    }
                    else {
                        //array type
                        // int dim = 0;
                        // int w1=0, w2=0, w3=0;
                        // if(data[i][3].length() < 5) {cout<< "symbol table type undefined\n";}
                        // else dim++;
                        // w1 = data[i][3][4] - '0';
                        // if(data[i][3].length() > 7) {w2 = data[i][3][7]- '0'; dim++;}
                        // if(data[i][3].length() > 10) {w3 = data[i][3][10] - '0'; dim++;}
                        // // only int arrays assumed of size 8 bytes
                        // addressDes[currClassName + "::" + data[i][2]] = pos;
                        // // if(dim == 1) {
                        // //     pos = pos - 8 * w1;
                        // // }
                        // // else if(dim == 2) {
                        // //     pos = pos - 8 * w1 * w2;
                        // // }
                        // // else {
                        // //     pos = pos - 8 * w1 * w2 * w3;
                        // // }
                    }
                }
            }
            getline(fp, line);
        }
    }
    // temporary_size += (max(1, classSize[currClassName]/8));
    fp.close();
}

void finalCodeGen(vector<string> &funcCode, string otpt)
{
    ofstream fout;
    fout.open(otpt);
    if (!fout)
    {
        cout << "RRRR-"
             << "can't be opened\n";
    }
    fout << ".text\n";
    // fout << ".globl " + mainClassName + "-" + "main" + "\n";
    for (auto s : funcCode)
    {
        fout << s << "\n";
    }
    fout << "movq $60, %rax\n";
    fout << "xorq %rbx, %rbx\n";
    fout << "syscall\n";

    fout << "printfmt: \n";
    fout << ".string \"%d\\n\"";
    fout << "\n";
    fout.close();
    return;
}

string getfuncName(string x)
{
    if (x.rfind("call", 0) != 0 && x.find("new") == string::npos)
    {
        cout << "Error the instruction not start with call\n";
    }
    x = x.substr(x.find(' ') + 1);
    string temp = currClassName + "_" + x;
    if (x.find(".") != string::npos)
    {
        string var = x.substr(0, x.find('.'));
        string func = x.substr(x.find('.') + 1);
        cout << "var = " << var <<"\n";
        cout << "func = " << func << "\n";
        // a class object's function is being called
        // need to consult the symbol table for this current function, or class
        string filename = currClassName + "_" + currFuncName + ".csv";
        vector<vector<string>> data = read_csv(GetCurrentWorkingDir() + "/temporary/" + filename);
        for (int i = 1; i < data.size(); i++)
        {
            cout << "j=" << i << "\n";
            if (data[i][2] == var)
            {
                temp = data[i][3] + "_" + func;
                cout << "temp = " << temp;
            }
        }
        //std::cout << "temp =" << temp << "\n";
        std::cout << "var = " << var << '\n';
    }
    else {
        string filename = currClassName + "_" + currFuncName + ".csv";
        vector<vector<string>> data = read_csv(GetCurrentWorkingDir() + "/temporary/" + filename);
        for (int i = 1; i < data.size(); i++)
        {
            cout << "j=" << i << "\n";
            if (data[i][3] == x)
            {
                temp = data[i][3] + "_" + x;
                cout << "temp = " << temp;
            }
        }
    }
    return temp;
}
string smplPush(string x)
{
    if (x.rfind("pushparam", 0) != 0)
    {
        cout << "Not a push instruction";
    }
    string ans;
    int f = 0;
    for (int j = 0; j < x.size(); j++)
    {
        if (x[j] == ' ')
        {
            f = 1;
        }
        else if (f == 1)
        {
            ans.push_back(x[j]);
        }
    }
    return ans;
}

void modify_var_arg(string s)
{
    cout << "called\n";
    cout << "s=" << s << "\n";
    s = currClassName + "::" + currFuncName + "::" + s;
    if (var.find(s) != var.end())
    {
        var[s][1] = 1;
        cout << "done\n";
    }
    else
    {
        cout << s << " not found\n";
    }
    cout << "returning\n";
    return;
}
vector<int> var_info(string x)
{
    // //string x;
    cout << "ar info\n";
    vector<int> ans;
    for (auto it : var)
    {
        cout << it.first << " ";
        if (it.first == x)
        {
            cout << "found\n";
        }
    }
    cout << "\n";
    x = currClassName + "::" + currFuncName + "::" + x;
    if (var.find(x) == var.end())
    {
        cout << "not f\n";
        return ans;
    }
    cout << "come\n";
    return var[x];
}
int sz_func()
{
    int ans = 0;
    vector<vector<string>> data = read_csv(GetCurrentWorkingDir() + "/temporary/" + currClassName + "_" + currFuncName + ".csv");
    for (int i = 1; i < data.size(); i++)
    {
    }
    ans += temporary_size * 8 + 8;

    return ans;
}
int string_to_int(string x)
{ // replacement for stoi
cout << "string to int\n";
    return stoi(x);
}
void fill_var_temp_sz(string x)
{
    ifstream file;
    cout << "x = " << x << "\n";
    file.open(GetCurrentWorkingDir() + "/temporary/" + currClassName + "_" + x + ".3ac");
    if (!file)
    {
        cout << "RRRR-file not opening\n";
    }
    cout << currClassName + "_" + x << "\n";
    string line;
    getline(file, line);
    file.close();
    cout << "the line is " << line << "\n";
    line = line.substr(line.find(',') + 1);

    temporary_size += string_to_int(line);
    vector<string> v;
    file.open(GetCurrentWorkingDir() + "/temporary/" + currClassName + "_" + x + ".csv");
    if (!file)
    {
        cout << "RRRR- file not openong\n";
    }
    getline(file, line);
    while (getline(file, line))
    {
        v.push_back(line);
    }
    file.close();
    var.clear();
    for (int j = 0; j < v.size(); j++)
    {
        int cnt = 0;
        int k = 0;
        for (k = 0; k < v[j].size(); k++)
        {
            if (v[j][k] == ',')
            {
                cnt++;
            }
            if (cnt == 2)
            {
                break;
            }
        }
        k++;
        string nm;
        for (; k < v[j].size(); k++)
        {
            if (v[j][k] == ',')
                break;
            nm.push_back(v[j][k]);
        }
        nm = currClassName + "::" + currFuncName + "::" + nm;
        vector<int> temp;
        k++;
        string tp;
        for (; k < v[j].size(); k++)
        {
            if (v[j][k] == ',')
                break;
            tp.push_back(v[j][k]);
        }
        if (tp == "int")
        {
            temp = {1, 0};
        }
        else
        {
            int w = 0;
            int cnt = 0;
            for (int k = 0; k < tp.size(); k++)
            {
                if (tp[k] == 'w')
                {
                    w = 1;
                }
                if (tp[k] == ']')
                {
                    cnt++;
                }
            }
            if (w == 1)
            {
                if (cnt == 1)
                {
                    temp = {100, 1};
                }
                if (cnt == 2)
                {
                    temp = {200, 1};
                }
                if (cnt == 3)
                {
                    temp = {300, 1};
                }
            }
            else
            {
                vector<int> u = {cnt * 100, 0};
                w = 0;
                string p;
                for (int k = 0; k < tp.size(); k++)
                {
                    if (tp[k] == '[')
                    {
                        w = 1;
                    }
                    else if (w == 1)
                    {
                        if (tp[k] == ']')
                        {
                            u.push_back(string_to_int(p));
                            w = 0;
                            p = "";
                        }
                        else
                        {
                            p.push_back(tp[k]);
                        }
                    }
                }
                temp = u;
            }
        }
        var[nm] = temp;
        cout << "Name = " << nm << "as\n";
    }
    cout << "fill var returned\n";
}

void ary_ass(string lex, int tp, string v1, string v2, string v3, string val, vector<string> &funCode)
{
    // here lex is the array name , tp is type: 100 for 1d,200 for 2d..., v1,v2,v3 is the index you want to acess
    //  val is the valyue to be assigned send it as $const if it is const else send it the memory loc i.e. -8(%rbp)
    if (tp == 100)
    {
        // put val in rdx
        string instr = "movq " + val + ", %rdx";
        funCode.push_back(instr);
        // put index in rax
        instr = "movq " + v1 + ", %rax";
        funCode.push_back(instr);
        instr = "salq $3, %rax";
        funCode.push_back(instr);
        instr = "movq " + to_string(getAddressDes(lex)) + "(%rbp), %rdi";
        funCode.push_back(instr);
        instr = "addq %rax, %rdi";
        funCode.push_back(instr);
        // finally assignment
        instr = "movq %rdx, (%rdi)";
        funCode.push_back(instr);
    }
    if (tp == 200)
    {
        // put val in rdx
        string instr = "movq " + val + ", %rdx";
        funCode.push_back(instr);
        // put v1 in rax
        instr = "movq " + v1 + ", %rax";
        funCode.push_back(instr);
        // put v2 in rax
        instr = "movq " + v2 + ", %rdi";
        funCode.push_back(instr);
        // v1*w2+v2
        instr = "movq " + to_string(getAddressDes(lex + "_w2")) + "(%rbp), %rsi";
        funCode.push_back(instr);
        instr = "imulq %rsi, %rax";
        funCode.push_back(instr);
        instr = "addq %rsi, %rax";
        funCode.push_back(instr);
        // finally assignment
        instr = "salq $3, %rax";
        funCode.push_back(instr);
        instr = "movq " + to_string(getAddressDes(lex)) + "(%rbp), %rdi";
        funCode.push_back(instr);
        instr = "addq %rax, %rdi";
        funCode.push_back(instr);
        // finally assignment
        instr = "movq %rdx, (%rdi)";
        funCode.push_back(instr);
    }
    if (tp == 300)
    {
        // put val in rdx
        string instr = "movq " + val + ", %rdx";
        funCode.push_back(instr);
        // put v1 in rax
        instr = "movq " + v1 + ", %rax";
        funCode.push_back(instr);
        // put v2 in rdi
        instr = "movq " + v2 + ", %rdi";
        funCode.push_back(instr);
        // put v3 in r8
        instr = "movq " + v2 + ", %r8";
        funCode.push_back(instr);
        // put w2 in rsi
        instr = "movq " + to_string(getAddressDes(lex + "_w2")) + "(%rbp), %rsi";
        funCode.push_back(instr);

        // put w3 in r9
        instr = "movq " + to_string(getAddressDes(lex + "_w3")) + "(%rbp), %r9";
        funCode.push_back(instr);
        // v3+v1*w2*w3+v2*w3
        instr = "imulq %rsi, %rax";
        funCode.push_back(instr);
        instr = "imulq %r9, %rax";
        funCode.push_back(instr);
        instr = "imulq %r9, %rdi";
        funCode.push_back(instr);
        instr = "addq %rdi, %rax";
        funCode.push_back(instr);
        instr = "addq %r8, %rax";
        funCode.push_back(instr);
        // finally assignment
        instr = "salq $3, %rax";
        funCode.push_back(instr);
        instr = "movq " + to_string(getAddressDes(lex)) + "(%rbp), %rdi";
        funCode.push_back(instr);
        instr = "addq %rax, %rdi";
        funCode.push_back(instr);
        // finally assignment
        instr = "movq %rdx, (%rdi)";
        funCode.push_back(instr);
    }
}

void ary_acc(string lex, int tp, string v1, string v2, string v3, string r, vector<string> &funCode)
{
    // here lex is the array name , tp is type: 100 for 1d,200 for 2d..., v1,v2,v3 is the index you want to acess
    // here you will get your value in register r, send r as %rdx
    string instr;
    if (tp == 100)
    {
        // put index in rax
        instr = "movq " + v1 + ", %rax";
        funCode.push_back(instr);
        // finally acces
        instr = "salq $3, %rax";
        funCode.push_back(instr);
        instr = "movq " + to_string(getAddressDes(lex)) + "(%rbp), %rdi";
        funCode.push_back(instr);
        instr = "addq %rax, %rdi";
        funCode.push_back(instr);
        // finally assignment
        instr = "movq (%rdi), " + r;
        funCode.push_back(instr);
    }
    if (tp == 200)
    {
        // put v1 in rax
        instr = "movq " + v1 + ", %rax";
        funCode.push_back(instr);
        // put v2 in rax
        instr = "movq " + v2 + ", %rdi";
        funCode.push_back(instr);
        // v1*w2+v2
        instr = "movq " + to_string(getAddressDes(lex + "_w2")) + "(%rbp), %rsi";
        funCode.push_back(instr);
        instr = "imulq %rsi, %rax";
        funCode.push_back(instr);
        instr = "addq %rsi, %rax";
        funCode.push_back(instr);
        // finally assignment
        instr = "salq $3, %rax";
        funCode.push_back(instr);
        instr = "movq " + to_string(getAddressDes(lex)) + "(%rbp), %rdi";
        funCode.push_back(instr);
        instr = "addq %rax, %rdi";
        funCode.push_back(instr);
        // finally assignment
        instr = "movq (%rdi), " + r;
        funCode.push_back(instr);
    }
    if (tp == 300)
    {
        // put v1 in rax
        instr = "movq " + v1 + ", %rax";
        funCode.push_back(instr);
        // put v2 in rdi
        instr = "movq " + v2 + ", %rdi";
        funCode.push_back(instr);
        // put v3 in r8
        instr = "movq " + v2 + ", %r8";
        funCode.push_back(instr);
        // put w2 in rsi
        instr = "movq " + to_string(getAddressDes(lex + "_w2")) + "(%rbp), %rsi";
        funCode.push_back(instr);
        // put w3 in r9
        instr = "movq " + to_string(getAddressDes(lex + "_w3")) + "(%rbp), %r9";
        funCode.push_back(instr);
        // v3+v1*w2*w3+v2*w3
        instr = "imulq %rsi, %rax";
        funCode.push_back(instr);
        instr = "imulq %r9, %rax";
        funCode.push_back(instr);
        instr = "imulq %r9, %rdi";
        funCode.push_back(instr);
        instr = "addq %rdi, %rax";
        funCode.push_back(instr);
        instr = "addq %r8, %rax";
        funCode.push_back(instr);
        // finally assignment
        instr = "salq $3, %rax";
        funCode.push_back(instr);
        instr = "movq " + to_string(getAddressDes(lex)) + "(%rbp), %rdi";
        funCode.push_back(instr);
        instr = "addq %rax, %rdi";
        funCode.push_back(instr);
        // finally assignment
        instr = "movq (%rdi), " + r;
        funCode.push_back(instr);
    }
}
void insert_arg(vector<string> arg, vector<string> &funCode)
{
    int cnt = 0;
    DBG cout << "insert arg calleds\n";
    vector<string> arg_name;
    for (int j = 0; j < arg.size(); j++)
    {
        cout << "j=" << j << "\n";
        arg_name.push_back(arg[j]);
        cout << "hey]h\n";
        vector<int> info = var_info(arg[j]);

        if (info.size() == 0)
        {
            cout << "No such variable exist\n";
        }
        if (info[0] == 100)
        {
            arg_name.push_back(arg[j] + "_w1");
        }
        if (info[0] == 200)
        {
            arg_name.push_back(arg[j] + "_w1");
            arg_name.push_back(arg[j] + "_w2");
        }
        if (info[0] == 300)
        {
            arg_name.push_back(arg[j] + "_w1");
            arg_name.push_back(arg[j] + "_w2");
            arg_name.push_back(arg[j] + "_w3");
        }
    }
    cout << "reavched th\n";
    string instr = MOVQ + string("%rdi, ") + to_string(getAddressDes("this")) + "(%rbp)";
    funCode.push_back(instr);
    vector<string> rg = {"%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    for (int j = 0; j < arg_name.size(); j++)
    {
        cnt++;

        if (cnt <= 5)
        {
            funCode.push_back("movq " + rg[cnt - 1] + ", " + to_string(getAddressDes(arg_name[j])) + "(%rbp)");
        }
        else
        {
            funCode.push_back("movq " + to_string(16 + ((arg_name.size() - 6) - (cnt - 6)) * 8) + "(%rbp)" + ", %rdx");
            funCode.push_back("movq %rdx, " + to_string(getAddressDes(arg_name[j])) + "(%rbp)");
        }
    }
    cout << "insert ags ret\n";
}
pair<int, int> declareLocalVars()
{
    // ifstream fp(currClassName + "-" + currFuncName + ".csv");
    cout << "Declaring local vars****************\n";
    vector<vector<string>> data = read_csv(GetCurrentWorkingDir() + "/temporary/" + currClassName + "_" + currFuncName + ".csv");
    cout << "read csv " << data.size() << "\n";
    int pos = -8;
    int sz = -8;

    for (int i = 1; i < data.size(); i++)
    {
        int dim1 = 1, dim2 = 1, dim3 = 1;
        cout << "i = " << i << "\n";
        int numBrackets = countOccurrences('[', data[i][3]);
        cout << "i = " << i << "\n";
        string t = data[i][3];
        cout << " t = " << data[i][3] << "!\n";
        if (t.substr(0, 3) != "int")
        {
            // not an int variable, look if its a class object
            // for(auto it: classSize) {
            //     cout << it.first << " " ;
            // }
            cout << "\n";
            if (classSize.find(t) != classSize.end())
            {
                string tt = currClassName + "::" + currFuncName + "::" + data[i][2];
                addressDes[tt] = pos;
                cout << "~~~~~~~~~~~~~~tt = " << tt << "\n";
                pos = pos - (classSize[t] != 0 ? classSize[t] : 8);
                sz = pos;
            }
            else
            {
                cout << "ERROR IN PROGRAM, CLASS NOT DECLARED\n";
            }
        }
        else
        {
            sz = sz - 8 * (numBrackets + 1);
        }
        string tt = currClassName + "::" + currFuncName + "::" + data[i][2];

        cout << "sz = " << sz << "\n";

        cout << "t=" << tt << endl;
        addressDes[tt] = pos;
        pos = pos - 8;
        // sz = pos;
        cout << "dec\n";

        if (numBrackets >= 1)
        {
            addressDes[tt + "_w1"] = pos;
            pos -= 8;
            sz -= 8;
        }
        if (numBrackets >= 2)
        {
            addressDes[tt + "_w2"] = pos;
            pos -= 8;
            sz -= 8;
        }
        if (numBrackets >= 3)
        {
            addressDes[tt + "_w3"] = pos;
            pos -= 8;
            sz -= 8;
        }
    }
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1tmp size = " << temporary_size << "\n";
    // sz -= 8 * temporary_size;
    for (int i = 0; i < temporary_size; i++)
    {
        string tempName = "_t" + to_string(i);
        string tt = currClassName + "::" + currFuncName + "::" + tempName;
        addressDes[tt] = pos;
        pos -= 8;
        // sz = pos;
    }
    addressDes[currClassName + "::" + currFuncName + "::this"] = pos;
    data = read_csv(GetCurrentWorkingDir() + "/temporary/" + currClassName + ".csv");
    int pp = 0;
    for (int i = 1; i < data.size(); i++)
    {
        addressDes[currClassName + "::" + currFuncName + "::this." + data[i][2]] = pp;
        pp += 8;
        cout << currClassName + "::" + currFuncName + "::this." + data[i][2] << "\n";
        

    }
    pos -= 8;
    sz -= 8;
    return {abs(pos), abs(pos)};
}

// handles starting code of function, initialize stack space etc
void beg_func(string x, vector<string> &funCode)
{
    if (x.rfind("beginfunc", 0) != 0)
    {
        cout << "Not a start of the function\n";
    }
    int f = 0;
    string xx = x.substr(x.find(' ') + 1);
    string func_nm = xx.substr(0, xx.find(' '));

    currFuncName = func_nm;
    fill_var_temp_sz(func_nm);
    vector<string> arg_name;
    string temp;
    int pos = string("beginfunc").length() + 1 + func_nm.length() + 1;
    cout << "pos = " << pos << "\n";
    for (int j = pos; j < x.size(); j++)
    {
        if (x[j] == ',')
        {
            arg_name.push_back(temp);
            temp = "";
        }
        else
        {
            temp.push_back(x[j]);
        }
    }
    if (temp.length() > 0)
        arg_name.push_back(temp);

    string instr = currClassName + "_" + func_nm + ":";
    if (func_nm == "main")
    {
        instr = "main:";
    }
    funCode.push_back(instr);
    pair<int, int> p = declareLocalVars();
    instr = "pushq %rbp";
    funCode.push_back(instr);
    instr = "movq %rsp, %rbp";
    funCode.push_back(instr);
    instr = "subq $" + to_string(p.first) + ", %rsp";
    funCode.push_back(instr);
    insert_arg(arg_name, funCode);
}
void func_call(vector<string> a, vector<string> &funcCode)
{
    vector<string> ans_reg; // keep all the register instruction
    vector<string> ans_st;  // keep all the pushq instruction
    vector<pair<string, int>> things;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2num of arguments = " << a.size() - 1 << "\n";
    for (int j = a.size() - 2; j >= 0; j--)
    {
        string y = consOrVar(a[j]);
        if (y[0] == '$')
        {
            things.push_back({y, 0});
        }
        else
        {
            // string x=currClassName + "::" + currFuncName + "::" + smplPush(a[j]);
            string x = smplPush(a[j]);
            cout << "x = " << x << "\n";
            if (var_info(x).size() == 0)
            {
                cout << "No such variable exist\n";
                if (x[0] == '_')
                {
                    things.push_back({y, 0});
                }
            }
            else
            {
                vector<int> info = var_info(x);
                if (info[0] == 1)
                { // int
                    things.push_back({y, 0});
                }
                if (info[0] >= 100)
                {                                                                  // array
                    things.push_back({to_string(getAddressDes(x)) + "(%rbp)", 1}); // for this array we have width in stack
                    if (info[0] == 100)
                    { // 1darray
                        things.push_back({to_string(getAddressDes(x + "_w1")) + "(%rbp)", 0});
                    }
                    if (info[0] == 200)
                    { // 2darray
                        things.push_back({to_string(getAddressDes(x + "_w1")) + "(%rbp)", 0});
                        things.push_back({to_string(getAddressDes(x + "_w2")) + "(%rbp)", 0});
                    }
                    if (info[0] == 300)
                    { // 3darray
                        things.push_back({to_string(getAddressDes(x + "_w1")) + "(%rbp)", 0});
                        things.push_back({to_string(getAddressDes(x + "_w2")) + "(%rbp)", 0});
                        things.push_back({to_string(getAddressDes(x + "_w3")) + "(%rbp)", 0});
                    }
                }
            }
        }
    }
    vector<string> rg = {"%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    for (int j = 0; j < things.size(); j++)
    {
        if (j < 5)
        {
            if (things[j].second == 0)
                ans_reg.push_back("movq  " + things[j].first + ", " + rg[j]);
            else
                ans_reg.push_back("movq  " + things[j].first + ", " + rg[j]);
        }
        else
        {
            if (things[j].second == 0)
                ans_st.push_back("movq  " + things[j].first + ", %rdx");
            else
                ans_st.push_back("movq  " + things[j].first + ", " + rg[j]);
            ans_st.push_back("pushq  %rdx");
        }
    }
    string instr;
    instr = "call " + getfuncName(a[a.size() - 1]);
    ans_reg.push_back(instr);
    int lineNum = getLineNo(a[0]);
    if (islabel[lineNum])
    {
        string ins = ".L" + to_string(lineNum) + ":";
        funcCode.push_back(ins);
    }
    for (auto y : ans_st)
    {
        funcCode.push_back(y);
    }
    for (auto y : ans_reg)
    {
        funcCode.push_back(y);
    }
}
void call_malloc(string name, int tp, string s1, string s2, string s3, vector<string> &funCode)
{
    // s is the registeer or the memory where sie is present send it as (%rbp) or $5
    // except rdi
    string instr;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!tp = " << tp << "\n";
    if (tp == 100)
    {
        instr = "movq " + s1 + ",%rdi";
        funCode.push_back(instr);
        instr = "salq $3,%rdi";
        funCode.push_back(instr);
        instr = "movq " + s1 + ",%r8";
        funCode.push_back(instr);
        instr = "movq %r8," + to_string(getAddressDes(name + "_w1")) + "(%rbp)";
        cout << "the name is " << name << "\n";
        funCode.push_back(instr);
    }
    if (tp == 200)
    {
        instr = "movq " + s1 + ",%rdi";
        funCode.push_back(instr);
        instr = "imulq " + s2 + ",%rdi";
        funCode.push_back(instr);
        instr = "salq $3,%rdi";
        funCode.push_back(instr);
        instr = "movq " + s1 + ",%r8";
        funCode.push_back(instr);
        instr = "movq %r8," + to_string(getAddressDes(name + "_w1")) + "(%rbp)";
        funCode.push_back(instr);
        instr = "movq " + s2 + ",%r8";
        funCode.push_back(instr);
        instr = "movq %r8," + to_string(getAddressDes(name + "_w2")) + "(%rbp)";
        funCode.push_back(instr);
    }
    if (tp == 300)
    {
        instr = "movq " + s1 + ",%rdi";
        funCode.push_back(instr);
        instr = "imulq " + s2 + ",%rdi";
        funCode.push_back(instr);
        instr = "imulq " + s3 + ",%rdi";
        funCode.push_back(instr);
        instr = "salq $3,%rdi";
        funCode.push_back(instr);
        instr = "movq " + s1 + ",%r8";
        funCode.push_back(instr);
        instr = "movq %r8," + to_string(getAddressDes(name + "_w1")) + "(%rbp)";

        funCode.push_back(instr);
        instr = "movq " + s2 + ",%r8";
        funCode.push_back(instr);
        instr = "movq %r8," + to_string(getAddressDes(name + "_w2")) + "(%rbp)";
        funCode.push_back(instr);
        instr = "movq " + s3 + ",%r8";
        funCode.push_back(instr);
        instr = "movq %r8," + to_string(getAddressDes(name + "_w3")) + "(%rbp)";
        funCode.push_back(instr);
    }
    if (tp == 400)
    {
        instr = string(MOVQ) + string("$") + s2 + string(", %rdi");
        funCode.push_back(instr);
    }
    instr = "call\tmalloc";
    funCode.push_back(instr);

    cout << "S1 = " << s1 << " " << objClass[s1] << " \n";
    instr = string(MOVQ) + string("%rax, ") + to_string(getAddressDes(name)) + string("(%rbp)");
    funCode.push_back(instr);
}

int main(int argc, char *argv[])
{

    // if(argc < 2) {
    //     cout << "Wrong input format\n";
    //     return 0;
    // }

    declareRegs();
    opConv['+'] = ADDQ;
    opConv['-'] = SUBQ;
    opConv['*'] = MULQ;
    opConv['/'] = DIVQ;
    opConv['^'] = XORQ;
    relConv["<"] = "jl";
    relConv[">"] = "jg";
    relConv[">="] = "jge";
    relConv["<="] = "jle";
    relConv["!="] = "jne";
    relConv["=="] = "je";
    sizes["int"] = sizes["byte"] = sizes["short"] = sizes["long"] = 8;
    string otpt;
    vector<string> inp;
    if (argc == 2)
    {
        if (argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] == 'h' && argv[1][3] == 'e' && argv[1][4] == 'l' && argv[1][5] == 'p' && argv[1][6] == '\0')
        {
            cout << "Usage: ./asmgen --input=<input_filename> --output=<output_filename>\noptions: --help\n";
            return 0;
        }
        else
        {
            cout << "Invalid argument. Use --help for usage.\n";
            return 0;
        }
    }
    else if (argc == 1)
    {
        cout << "No file specified. Use --help for usage.\n";
        return 0;
    }
    else if (argc == 3)
    {
        vector<string> y;
        y.push_back(argv[1]);
        y.push_back(argv[2]);
        sort(y.begin(), y.end());
        if (y[0].rfind("--input", 0) == 0 && y[1].rfind("--output=", 0) == 0)
        {
            string temp;
            for (int i = 8; i < y[0].size(); i++)
            {
                temp = temp + y[0][i];
            }
            ifstream file;
            file.open(GetCurrentWorkingDir() + "/temporary/" + temp);
            if (!file)
            {
                cout << "hello\n";
                cout << "RRRR- file not openong\n";
            }
            string line;
            while (getline(file, line))
            {
                inp.push_back(line);
            }
            file.close();
            for (int i = 9; i < y[1].size(); i++)
            {
                otpt = otpt + y[1][i];
            }
        }
        else
        {
            cout << "Invalid argument. Use --help for usage.\n";
            return 0;
        }
    }
    vector<string> classes;
    map<string, vector<string>> funcs;
    if (inp.size() == 0)
    {
        cout << "File can't be open\n";
    }
    else
    {
        string currclass = "";
        for (auto it : inp)
        {
            string ins = trimInstr(it);
            if (ins.rfind("beginclass", 0) == 0)
            {
                string temp;
                int f = 0;
                for (int j = 0; j < ins.size(); j++)
                {
                    if (ins[j] == ' ')
                    {
                        f = 1;
                    }
                    else if (f == 1)
                    {
                        temp.push_back(ins[j]);
                    }
                }
                classes.push_back(temp);
                currclass = temp;
            }
            if (ins.rfind("beginfunc", 0) == 0)
            {
                string temp;
                int f = 0;
                for (int j = 0; j < ins.size(); j++)
                {
                    if (f == 1 && ins[j] == ' ')
                        break;
                    if (ins[j] == ' ')
                    {
                        f = 1;
                    }
                    else if (f == 1)
                    {
                        temp.push_back(ins[j]);
                    }
                }
                funcs[currclass].push_back(temp);
            }
        }
    }
    vector<string> code;
    for (auto it : classes)
    {
        cout << "---" << it << "\n";
        handleClassDec(it + ".3ac");
        for (auto it : funcs[it])
        {
            currFuncName = it;
            cout << "it = " << it << "\n";
            vector<string> t = genfunc(it);
            for (auto it : t)
            {
                code.push_back(it);
            }
            // cout << "printing code till fnow\n";
            // for(auto it: code) {
            //     cout  << it << "\n";
            // }
        }
    }
    if (otpt.size() == 0)
    {
        otpt = "asm.s";
    }
    finalCodeGen(code, otpt);
}