/*
Function naming convention: <className>.<functionName> as is in 3ac




*/


#include <bits/stdc++.h>
#define PUSHQ "pushq\t"
#define MOVL "movl\t"
#define MOVQ "movq\t"
#define SUBQ "subq\t"
#define RBP "%rbp"
#define RSP "%rsp"
#define ADDQ "addq\t"
#define SUBQ "subq\t"
#define MULQ "mulq\t"
#define DIVQ "divq\t"
#define XORQ "xorq\t"
#define DEBUG 1
#define DBG if(DEBUG)
using namespace std;

string consOrVar(string);
int temporary_size;            //hold the size of temporary variable used in a given fucntion
vector<char> ops = {'+', '-', '*', '/', '^', '&', '%'};
vector<string> relOps = {"<=", ">=", "==" ,"!=", ">", "<"};

map<string,int> sizes;

map<char, string> opConv;
map<string,vector<int>> var;    //it keeps the data 
                                // its struct is map<var_name,{type,isarg,w1,w2,w3}>
map<string, string> relConv;

map<string, int> addressRegDes; // address descriptor
map<string, int> addressDes;    // describes the address relative to rbp for a given variable
map<string, bool> mem;          // whether value in memory is correct value of the variable
map<string, int> classSize;     // size of a class object assuming size of int as 8 bytes

string currClassName;   // current class being handled

void func_call(vector<string>a,vector<string>&funcCode);

int getAddressDes(string varname)
{
    if(addressDes.find(varname) != addressDes.end()) return addressDes[varname];
    else return addressDes[currClassName + "::" + varname];
}


vector<vector<string> > read_csv(string filename)
{
    vector<vector<string>> data;
    ifstream file(filename);
    //vector<string> funcCode;

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

vector<int> genregs = {0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15};

string reg::getname()
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
// store all the variables pointing to this register in memory
vector<string> reg::storeall()
{
    vector<string> ans;
    for (auto it : regDes)
    {
        if (mem[it.first] == true)
            continue;
        else
        {
            ans.push_back(string(MOVQ) + regName + string(",") + to_string(addressDes[it.first]) + string("(%rbp)"));
            mem[it.first] = true;
        }
    }
    regDes.clear();
    return ans;
}

bool reg::isEmpty()
{
    if (regDes.empty())
        return true;
    return false;
}

void reg::init(int id, string regName)
{
    this->id = id;
    this->regName = regName;
}

void reg::addRegDes(string s)
{
    regDes[s] = 1;
    return;
}

void reg::rmRegDes(string s)
{
    regDes[s] = 0;
    return;
}

bool reg::getRegDes(string s)
{
    if (regDes[s] == 1)
        return true;
    return false;
}

int checkRegsForVar(string varName)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        if (r[i].getRegDes(varName))
            return i;
    }
    return -1;
}

int checkRegsForOnlyVar(string varName)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        if (r[i].getRegDes(varName) && r[i].getRegDesSize() == 1)
            return i;
    }
    return -1;
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



// get a free register for calculations, need only two regs as result is stored in the second register
vector<int> getreg(string res, string a, string b, vector<string> &funcCode)
{
    // check in address descriptor
    vector<int> ans;
    // ans.push_back(0);
    int t = checkRegsForVar(a);
    if (t >= 0)
    {
        if (DEBUG)
            cout << "1here\n";
        ans.push_back(t);
    }
    else
    {
        if (DEBUG)
            cout << "1here\n";
        int flag = 0;
        for (auto it : genregs)
        {
            if (DEBUG)
                cout << "regn " << it << "\n";
            if (r[it].getRegDes(a) == 1)
            {
                if (DEBUG)
                    cout << "1here\n";
                ans.push_back(it);
                flag = 1;
                funcCode.push_back(string(MOVQ) + to_string(addressDes[a]) + "(%rbp)" + r[it].getname());
                r[it].addRegDes(a);
                break;
            }
            else if (r[it].getRegDesSize() == 0)
            {
                ans.push_back(it);
                if (DEBUG)
                    cout << "2here\n";
                flag = 1;
                funcCode.push_back(string(MOVQ) + to_string(addressDes[a]) + "(%rbp), " + r[it].getname());
                r[it].addRegDes(a);
                break;
            }
            else
            {
                cout << r[it].getRegDesSize() << "\n";
            }
        }
        if (flag == 0)
        {
            // allot %rbx to the s
            vector<string> t = r[1].storeall();
            
            funcCode.insert(funcCode.end(), t.begin(), t.end());
            funcCode.push_back(string(MOVQ) + to_string(addressDes[a]) + "(%rbp), " + ", %rbx");
            r[1].addRegDes(a);
            // addressRegDes[a] = 1;
            if (DEBUG)
                cout << "1here\n";
            ans.push_back(1);
            // r[1]
        }
    }
    if (DEBUG)
        cout << ":o\n";
    t = checkRegsForVar(b);
    if (t >= 0)
    {
        ans.push_back(t);
    }
    else
    {
        int flag = 0;
        for (auto it : genregs)
        {
            if (r[it].getRegDes(b) == 1)
            {
                ans.push_back(it);
                flag = 1;
                funcCode.push_back(string(MOVQ) + to_string(addressDes[b]) + "(%rbp), " + r[it].getname());
                r[it].addRegDes(b);
                break;
            }
            else if (r[it].getRegDesSize() == 0)
            {
                ans.push_back(it);
                flag = 1;
                funcCode.push_back(string(MOVQ) + to_string(addressDes[b]) + "(%rbp), " + r[it].getname());
                r[it].addRegDes(b);
                break;
            }
        }
        if (flag == 0)
        {
            // allot %rbx to the s
            vector<string> t = r[2].storeall();
            if (DEBUG)
                cout << t.size() << "aa\n";
            if (t.size() > 0)
                funcCode.insert(funcCode.end(), t.begin(), t.end());
            funcCode.push_back(string(MOVQ) + to_string(addressDes[b]) + "(%rbp)" + ", %rcx");
            r[2].addRegDes(res);
            // addressRegDes[b] = 2;
            ans.push_back(2);
            // r[1]
        }
    }
    mem[res] = false;
    r[ans.back()].addRegDes(res);
    return ans;
}

vector<bool> islabel(2e5+1, false);



bool is_number(const std::string& s)
{
    static const std::regex re("-?[0-9]+(\\.[0-9]*)?");
    return std::regex_match(s, re);
}

// return move instruction 
string genMove(string src, string dest) 
{
    // check if src is a number
    cout << src << " in genMove\n";
    if(is_number(src)) {
        // int val = stoi(src);
        src = string("$") + src;
    }
    else if(src[0] !='%') {
        src = to_string(addressDes[src]) + string("(%rbp)");
    }
    if(dest[0] != '%') {
        dest = to_string(addressDes[dest]) + string("(%rbp)");
    } 
    
    // else {
    //     src = to_string(addressDes[src]) + string("(%rbp)");
    // }
    return MOVQ + src + ", " + dest;
}

string genArithmetic(string op, string src, string dest)
{
    if(op.size() == 0) return string("error");
    return opConv[op[0]] + src + ", "+ dest;
}


int getOneReg(string var)
{
    int i;

    for(auto it: genregs) {
        if(r[it].getRegDes(var)) {
            return it;
        }
    }
    for(auto it: genregs) {
        if(r[it].getRegDesSize() == 0) {

            return it;
        }
    }
    // free %rbx 
    r[1].storeall();
    return 1;
}



// get line no of 3ac instruction
int getLineNo(string instr)
{
    int i=0;
    int lineNo = 0;
    while(i < instr.size() && isdigit(instr[i])) {
        lineNo = lineNo * 10 + (instr[i] - '0');
        i++;
    }
    return lineNo;
}   

vector<string> cmpOps = {"==", "<=", ">=", ">", "<", "!="};


vector<string> identifyInstr(string instr)
{
    /*
    case 1: arithmetic instruction of type var = var + var
    case 2: arithmetic instruction of type var = var + int
    case 3: arithmetic instruction of type var = int + var
    case 4: arithmetic instruction of type var = int + int
    case 5: copy instr var = var
    case 6: var = int
    case 7: array element init a[int] = int
    case 8: a[var] = int
    case 9: a[var] = var
    case 10: 

    
    assume instruction starts immediately after colon
    */
    // string instr = lines[0];
    vector<string> ans;
    if(DEBUG) cout << "hey there\n";
    size_t colpos= instr.find(":");
    int line_num = getLineNo(instr);
    if(islabel[line_num]) {
        string ins = ".L" + to_string(line_num) + ":";
        ans.push_back(ins);
    }
    instr = instr.substr(colpos+1);
    DBG cout << "hey\n";
    size_t eqpos = instr.find('=');
    if(eqpos != string::npos) {
        //     
        DBG cout << "hey here\n";
        string s = instr.substr(eqpos+1);
        int flag = 0;
        string x = instr.substr(0, eqpos);
        if(DEBUG) cout << "x = " << x << "\n";
        for(auto op: ops) {
            if(s.find(op)  != string::npos) {
                string y = s.substr(0, s.find(op));
                string z = s.substr(s.find(op) + 1);
                if(DEBUG) cout << "y, z = " << y << ", " << z << "\n";
                // move to rbx and rcx
                string ins1 = genMove(y, "%rbx");
                string ins2 = genMove(z, "%rcx");
                string ins3 = genArithmetic(s.substr(s.find(op), 1), "%rbx", "%rcx");
                string ins4 = genMove("%rcx", x);
                ans.push_back(ins1);
                ans.push_back(ins2);
                ans.push_back(ins3);
                ans.push_back(ins4);
                // add code 
                // let x, y, x be st x = y + z
                flag = 1;
                break;

            }
        }
        if(flag == 0) {
            // copy instruction
            
            string s = instr.substr(eqpos+1);
            string var1 = instr.substr(0, eqpos);
            if(s.size() && isdigit(s[0])) {
                // rhs is a number
                // string var2 = instr.substr(0, eqpos);
                
                string ins = MOVQ + string("$") + s +string(", ")+ to_string(addressDes[var1]) + string("(%rbp)");
                ans.push_back(ins);
            }
            else {
                // rhs is a variable
                // move rhs to %rbx
                
                string ins = MOVQ + to_string(addressDes[s]) + string("(%rbp), %rbx");
                ans.push_back(ins);
                ins = MOVQ + string("%rbx, ") + to_string(addressDes[var1]) + string("(%rbp");
                ans.push_back(ins);

            }
        }
    }
    else {
        // if then else, goto, call, pushparam, popparam
        // case 1: if then else 
        // if(instr.substr(0, 2) == "if") {
        //     size_t pos = instr.find("then");
        //     string compInstr = instr.substr(3, pos-2-3+1);

        // }
        // if(instr.fins("pushparam") != string::npos) {

        // }
        if(instr.find("if") != string::npos) {
            string t = instr.substr(instr.find("if"));
            string var1, var2, gotoloc;
            int gotopos = t.find("goto");

            int relpos = 0;
            for(auto ch: relOps) {
                if(t.find(ch ) != string::npos) {
                    relpos = t.find(ch);
                    break;
                }

            }
            var1 = t.substr(3, relpos-3);
            int relEnd = relpos;
            if(!isalnum(t[relpos + 1]) ) relEnd++;
            var2 = t.substr(relEnd+1, gotopos-relEnd-2 );

            DBG cout << var1 << " " << var2 <<  "\n";
            string ins1 = genMove(var1, "%rax");
            string ins2 = genMove(var2, "%rcx");

            string ins = "cmpq\t%rcx, %rax";
            string ins_ = relConv[t.substr(relpos, relEnd-relpos+1 )] + " .L" + t.substr(gotopos+5);
            DBG cout << ins_ << "\n";
            ans.push_back(ins1);
            ans.push_back(ins2);
            ans.push_back(ins);
            ans.push_back(ins_);
            gotoloc = t.substr(gotopos + 5);
            int gotoval = stoi(gotoloc);
            islabel[gotoval] = true;
	    
        }
        else if(instr.find("goto") != string::npos) {
            // pure goto instruction with no if
            int gotoval = stoi(instr.substr(5));
            islabel[gotoval] = true;
            string ins = string("jmp .L") + to_string(gotoval);
            ans.push_back(ins); 
        }
        else if(instr.substr(0, 5) == "print") {
            string ins1 = MOVQ + string("$0, %rax");
            string varName = instr.substr(6);
            //cout << line << "\n";
            if(DEBUG) cout << varName << " heree var name\n";
            string t;
            t=to_string(addressDes[varName]) + string("(%rbp)");
            string ins2 = MOVQ + string("$printfmt, %rdi");
            string ins3 = MOVQ + t+ string(", %rsi");
            ans.push_back(ins1);
            ans.push_back(ins2);
            ans.push_back(ins3);
            ans.push_back("call printf");

        }
        else if(instr.substr(0, 10)=="beginclass") {
            string t = "";
            int currpos = 11;
            for(auto ch : instr.substr(11)) {
                if(ch == ' ') {
                    break;
                }
                t += ch;
                currpos++;
            }
            currClassName = t;
            
        }



        
    }
    return ans;
}


vector<string> arrayAccess(string instr)
{

}

// function to translate a print statement to generate the assembly file
void print()
{
}
// generate the .data section of the assembly code
void data()
{
}

// generate the .text section of the assembly code
void textgen()
{
}

// generate the assembly code for a function
vector<string> genfunc(string funcName)
{
    // open the csv corresponding to the function name

    ifstream file(funcName + ".csv");

    vector<vector<string>> data;

    vector<string> funcCode;

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
    for (int i = 1; i < data.size(); i++)
    {
        if (DEBUG)
            cout << "hello" << data[i][2] << "var name \n";
        addressDes[data[i][2]] = -(i * 8);
        if (DEBUG)
            cout << addressDes[data[i][2]] << "\n";
    }
    int numVariables = data.size() - 1;
    if (DEBUG)
        cout << numVariables << " num var \n";
    // assuming all int variables

    int stackSpace = numVariables << 3;
    // standard code at beginning of function
    funcCode.push_back("main:");
    funcCode.push_back(string(PUSHQ) + string(RBP));
    funcCode.push_back(string(MOVQ) + string(RSP) + string(",") + string(RBP));

    funcCode.push_back(string(SUBQ) + string("$") + to_string(stackSpace) + string(",") + string(RSP));

    // opening the 3ac file
    ifstream file2(funcName + string(".txt"));

    if (!file2.is_open())
    {
        cout << "Error opening file" << endl;
        return funcCode;
    }

    // string line;
    cout << "file opened!\n";
    int linecnt = 0;
    
    while (getline(file2, line)) {
        if(DEBUG) cout << linecnt++ << "\n";
        vector<string> lines= {line};
        int isfunc = 0;
        while(line.find("pushparam") != string::npos) {
            isfunc=1;
            getline(file2, line);
            lines.push_back(line);
        }

        if(isfunc) {
            getline(file2, line);
            getline(file2, line);
            func_call(lines, funcCode);
            getline(file2, line);
            continue;
        }
        if(line.substr(0, 4)=="call") {
            func_call(lines, funcCode);
            continue;
        }
        
        vector<string> t = identifyInstr(line);
        for (auto it: t) {
            DBG cout << it <<"\n";
        }
        if(t.size()) { DBG cout << "ret\n";funcCode.insert(funcCode.end(), t.begin(), t.end()); DBG cout << "done\n";}
        // if(line.find("=") != string::npos) {
        //     // processing arithmetic ops 
        //     int flag = 0;
        //     for(auto ch: ops) {
        //         if(line.find(ch) != string::npos) {
        //             // cout << "here\n";
        //             size_t eqpos = line.find("=");
                    
        //             string res = line.substr(0,eqpos);  // variabel storing the result
        //             string t = line.substr(eqpos+1);
        //             size_t oppos = t.find(ch);
        //             string a = t.substr(0, oppos);      // first operand
                    
        //             string b = t.substr(oppos+1);       // second operand
        //             if(DEBUG) cout << a << " " << b << "\n";
        //             if(DEBUG) cout << "calling getreg\n";
        //             vector<int> rs = getreg(res, a, b, funcCode);
        //             if(DEBUG) cout << "getreg return " << rs.size() << "\n";
        //             int aReg = rs[0], bReg = rs[1];

        //             string instr = opConv[t[oppos]] + r[aReg].getname() + string(", " ) + r[bReg].getname();
        //             // string instr2 = MOVQ + 
        //             funcCode.push_back(instr);
        //             if(DEBUG) cout << "done\n";
        //             flag = 1;
        //             break;
        //         }
        //     }
        //     if(!flag) {
        //         // assuming variable initialization
        //         size_t eqpos = line.find("=");
        //         string varName = line.substr(0, eqpos);
        //         string val = line.substr(eqpos + 1);
        //         string instr = MOVQ + string("$") + val + string(", ") + to_string(addressDes[varName]) + string("(%rbp)");
        //         funcCode.push_back(instr);
        //     }


        // }
        // if(line.substr(0, 5) == "print") {
        //     string instr = MOVQ + string("$0, %rax");
        //     string varName = line.substr(6);
        //     cout << line << "\n";
        //     if(DEBUG) cout << varName << " heree var name\n";
        //     string t;
        //     if(checkRegsForVar(varName) >= 0) t=r[checkRegsForVar(varName)].getname();
        //     else t=to_string(addressDes[varName]) + string("(%rbp)");
        //     string instr2 = MOVQ + string("$printfmt, %rdi");
        //     string instr3 = MOVQ + t+ string(", %rsi");
        //     funcCode.push_back(instr);
        //     funcCode.push_back(instr2);
        //     funcCode.push_back(instr3);
        //     funcCode.push_back("call printf");

        // }
    }

    file2.close();

    // closing the function code
    funcCode.push_back("leave");
    funcCode.push_back("ret");

    return funcCode;

    // Access the data by index
    // cout << data[0][0] << endl; // Print the first cell of the first row
}

void handleClassDec(string filename)
{
    string line;
    ifstream fp(filename);
    getline(fp, line);
    if(line.substr(0, 10) == "beginclass") {
            currClassName = line.substr(11);
            getline(fp, line);
            while(line.find("endclass") != string::npos) {
                // getline(file2, line);
                // two things, integer variable or array declaration, or even class declaration
                // addressDes[currClassName + "::" + line.substr()]
                // assuming there is a csv file having variable info, type of variable and name of variable
                vector<vector<string> > data = read_csv(currClassName + ".csv");
                for(int i = 1; i < data.size(); i++) {
                    string var = data[i][2];
                    int pos = -8;
                    string typeName = data[i][3];
                    if(sizes.find(typeName) != sizes.end()) {
                        classSize[currClassName] += sizes[typeName];
                        addressDes[currClassName + "::" + var] = pos;
                        pos-=8;
                    }
                    else if(classSize.find(data[i][2]) != classSize.end()) {
                        classSize[currClassName] += classSize[data[i][3]];
                        addressDes[currClassName + "::" + var] = pos;
                        pos = pos - classSize[data[i][3]];
                    
                    }
                }
                getline(fp, line);
 
            }
        }
}

void finalCodeGen(vector<string> &funcCode)
{
    cout << ".text\n";
    cout << ".globl main\n";
    for (auto s : funcCode)
    {
        cout << s << "\n";
    }
    cout << "movq $60, %rax\n";
    cout << "xorq %rbx, %rbx\n";
    cout << "syscall\n";

    cout << "printfmt: \n";
    cout << ".string \"%d\"";
    return;
}
string consOrVar(string x){
    if(x.rfind("pushparam",0)!=0){
        cout<<"Error the instruction not start with pushparam\n";
    }
    string temp;
    int f=0;
    for(int j=0;j<x.size();j++){
        if(x[j]==' '){
            f=1;
        }
        else{
            if(f==1){
                temp.push_back(x[j]);
            }
        }
    }
    f=0;
    for(int j=0;j<temp.size();j++){
        if((x[j]<48||x[j]>59)&&x[j]!=45){
            f=1;
            break;
        }
    }
    string ans;
    if(f==1){
        ans=to_string(addressDes[x])+"(%rbp)";
    }
    else{
        ans="$"+x;
    }
    return ans;
}
string funcName(string x){
    if(x.rfind("call",0)!=0){
        cout<<"Error the instruction not start with call\n";
    }
    string temp;
    int f=0;
    for(int j=0;j<x.size();j++){
        if(f==1&&x[j]==','){
            break;
        }
        if(x[j]==' '){
            f=1;
        }
        else{
            if(f==1){
                temp.push_back(x[j]);
            }
        }
    }
    return temp;
}
string smplPush(string x){
    if(x.rfind("pushparam",0)!=0){
        cout<<"Not a push instruction";
    }
    string ans;
    int f=0;
    for(int j=0;j<x.size();j++){
        if(x[j]==' '){
            f=1;
        }
        else
        if(f==1){
            ans.push_back(x[j]);
        }
    }
    return ans;
}
vector<int> var_info(string x){
    string y;
    vector<int>ans;
    if(var.find(y)==var.end()){
        return ans;
    }
    return var[y];
}
void checkForArray(string x,vector<int>&wdt){
    vector<int>ans=var_info(x);
    if(ans.size()==0){
        cout<<"No such variable exist";
    }
    if(ans[1]==0){
        if(ans[0]>=100){
            wdt.push_back(ans[2]);
        }
        if(ans[0]>=200){
            wdt.push_back(ans[3]);
        }
        if(ans[0]>=300){
            wdt.push_back(ans[4]);
        }
    }
    else{
        
    }
}
int sz_func(){
    int ans=0;
    for(auto it:var){
        if(it.second[0]==1){//int
            ans+=8;
        }
        if(it.second[1]==0){//array def in side the func
            if(it.second[0]==100){//array
                ans+=it.second[2]*8;
            }
            if(it.second[0]==200){//2darray
                ans+=it.second[2]*it.second[3]*8;
            }
            if(it.second[0]==300){//3darray
                ans+=it.second[2]*it.second[3]*it.second[4]*8;
            }
        }
        else{   //array coming as argument
            if(it.second[0]==100){//array
                ans+=16;
            }
            if(it.second[0]==200){//2darray
                ans+=24;
            }
            if(it.second[0]==300){//3darray
                ans+=32;
            }
        }
    }
    ans+=temporary_size*8+8;
    return ans;
}
int string_to_int(string x){// replacement for stoi
    int y=1;
    if(x[0]=='-'){
        y=-1;
    }
    reverse(x.begin(),x.end());
    int ans=0;
    for(int j=0;j<x.size();j++){
        if((x[j]>=48||x[j]<=59)){
            ans+=(x[j]-'0')*y;
            y=y*10;
        }
    }
    return ans;
}
void fill_var_temp_sz(string x){
    ifstream file;
    file.open(x+".3ac");
    string line;
    getline(file, line);
    file.close();
    temporary_size=string_to_int(line);
    vector<string>v;
    file.open(x+".csv");
    getline(file, line);
    while (getline(file, line)) {
        v.push_back(line);
    }
    file.close();
    var.clear();
    for(int j=0;j<v.size();j++){
        int cnt=0;
        int k=0;
        for(k=0;k<v[j].size();k++){
            if(v[j][k]==','){
                cnt++;
            }
            if(cnt==2){
                break;
            }
        }
        k++;
        string nm;
        for(;k<v[j].size();k++){
            if(v[j][k]==',')break;
            nm.push_back(v[j][k]);
        }
        vector<int>temp;
        k++;
        string tp;
        for(;k<v[j].size();k++){
            if(v[j][k]==',')break;
            tp.push_back(v[j][k]);
        }
        if(tp=="int"){
            temp={1,0};
        }
        else{
            int w=0;
            int cnt=0;
            for(int k=0;k<tp.size();k++){
                if(tp[k]=='w'){
                    w=1;
                }
                if(tp[k]==']'){
                    cnt++;
                }
            }
            if(w==1){
                if(cnt==1){
                    temp={100,1};
                }
                if(cnt==2){
                    temp={200,1};
                }
                if(cnt==3){
                    temp={300,1};
                }
            }
            else{
                vector<int>u={cnt*100,0};
                w=0;
                string p;
                for(int k=0;k<tp.size();k++){
                    if(tp[k]=='['){
                        w=1;
                    }
                    else
                    if(w==1){
                        if(tp[k]==']'){
                            u.push_back(string_to_int(p));
                            w=0;
                            p="";
                        }
                        else{
                            p.push_back(tp[k]);
                        }
                    }
                }
                temp=u;
            }
        }
        var[nm]=temp;
    }
}
void ary_ass(string lex,int tp,string v1,string v2,string v3,string val,vector<string>&funCode){
//here lex is the array name , tp is type: 100 for 1d,200 for 2d..., v1,v2,v3 is the index you want to acess
// val is the valyue to be assigned send it as $const if it is const else send it the memory loc i.e. -8(%rbp)
    vector<int>info=var_info(lex);
    if(info.size()==0){
        cout<<"No such array exist\n";
    }
    if(info[0]!=tp){
        cout<<"There is inconsisteny in types\n";
    }
    if(tp==100){
        //put val in rdx
        string instr="movq "+val+", %rdx";
        funCode.push_back(instr);
        //put index in rax
        instr="movq "+v1+", %rax";
        funCode.push_back(instr);
        //finally assignment
        instr="movq %rdx,"+to_string(addressDes[lex]) +"(%rbp,%rax,8)";
        funCode.push_back(instr);
    }
    if(tp==200){
        //put val in rdx
        string instr="movq "+val+", %rdx";
        funCode.push_back(instr);
        //put v1 in rax
        instr="movq "+v1+", %rax";
        funCode.push_back(instr);
        //put v2 in rax
        instr="movq "+v2+", %rdi";
        funCode.push_back(instr);
        //v1*w2+v2
        if(info[1]==0){
            instr="movq $"+to_string(info[3])+", %rsi";
            funCode.push_back(instr);
        }
        else{
            instr="movq "+to_string(addressDes["_w2"+lex])+"(%rbp), %rsi";
            funCode.push_back(instr);
        }
        instr="mulq rsi, rax";
        funCode.push_back(instr);
        instr="addq rsi, rax";
        funCode.push_back(instr);
        //finally assignment
        instr="movq %rdx,"+to_string(addressDes[lex]) +"(%rbp,%rax,8)";
        funCode.push_back(instr);
    }
    if(tp==300){
        //put val in rdx
        string instr="movq "+val+", %rdx";
        funCode.push_back(instr);
        //put v1 in rax
        instr="movq "+v1+", %rax";
        funCode.push_back(instr);
        //put v2 in rdi
        instr="movq "+v2+", %rdi";
        funCode.push_back(instr);
        //put v3 in r8
        instr="movq "+v2+", %r8";
        funCode.push_back(instr);
        //put w2 in rsi
        if(info[1]==0){
            instr="movq $"+to_string(info[3])+", %rsi";
            funCode.push_back(instr);
        }
        else{
            instr="movq "+to_string(addressDes["_w2"+lex])+"(%rbp), %rsi";
            funCode.push_back(instr);
        }
        //put w3 in r9
        if(info[1]==0){
            instr="movq $"+to_string(info[4])+", %r9";
            funCode.push_back(instr);
        }
        else{
            instr="movq "+to_string(addressDes["_w3"+lex])+"(%rbp), %r9";
            funCode.push_back(instr);
        }
        //v3+v1*w2*w3+v2*w3
        instr="mulq rsi, rax";
        funCode.push_back(instr);
        instr="mulq r9, rax";
        funCode.push_back(instr);
        instr="mulq r9, rdi";
        funCode.push_back(instr);
        instr="addq rdi, rax";
        funCode.push_back(instr);
        instr="addq r8, rax";
        funCode.push_back(instr);
        //finally assignment
        instr="movq %rdx,"+to_string(addressDes[lex]) +"(%rbp,%rax,8)";
        funCode.push_back(instr);
    }
}
void ary_acc(string lex,int tp,string v1,string v2,string v3,string r,vector<string>&funCode){
//here lex is the array name , tp is type: 100 for 1d,200 for 2d..., v1,v2,v3 is the index you want to acess
//here you will get your value in register r, send r as %rdx
    vector<int>info=var_info(lex);
    if(info.size()==0){
        cout<<"No such array exist\n";
    }
    if(info[0]!=tp){
        cout<<"There is inconsisteny in types\n";
    }
    string instr;
    if(tp==100){
        //put index in rax
        instr="movq "+v1+", %rax";
        funCode.push_back(instr);
        //finally acces
        instr="movq "+to_string(addressDes[lex]) +"(%rbp,%rax,8), "+r;
        funCode.push_back(instr);
    }
    if(tp==200){
        //put v1 in rax
        instr="movq "+v1+", %rax";
        funCode.push_back(instr);
        //put v2 in rax
        instr="movq "+v2+", %rdi";
        funCode.push_back(instr);
        //v1*w2+v2
        if(info[1]==0){
            instr="movq $"+to_string(info[3])+", %rsi";
            funCode.push_back(instr);
        }
        else{
            instr="movq "+to_string(addressDes["_w2"+lex])+"(%rbp), %rsi";
            funCode.push_back(instr);
        }
        instr="mulq rsi, rax";
        funCode.push_back(instr);
        instr="addq rsi, rax";
        funCode.push_back(instr);
        //finally assignment
        instr="movq "+to_string(addressDes[lex]) +"(%rbp,%rax,8), "+r;
        funCode.push_back(instr);
    }
    if(tp==300){
        //put v1 in rax
        instr="movq "+v1+", %rax";
        funCode.push_back(instr);
        //put v2 in rdi
        instr="movq "+v2+", %rdi";
        funCode.push_back(instr);
        //put v3 in r8
        instr="movq "+v2+", %r8";
        funCode.push_back(instr);
        //put w2 in rsi
        if(info[1]==0){
            instr="movq $"+to_string(info[3])+", %rsi";
            funCode.push_back(instr);
        }
        else{
            instr="movq "+to_string(addressDes["_w2"+lex])+"(%rbp), %rsi";
            funCode.push_back(instr);
        }
        //put w3 in r9
        if(info[1]==0){
            instr="movq $"+to_string(info[4])+", %r9";
            funCode.push_back(instr);
        }
        else{
            instr="movq "+to_string(addressDes["_w3"+lex])+"(%rbp), %r9";
            funCode.push_back(instr);
        }
        //v3+v1*w2*w3+v2*w3
        instr="mulq rsi, rax";
        funCode.push_back(instr);
        instr="mulq r9, rax";
        funCode.push_back(instr);
        instr="mulq r9, rdi";
        funCode.push_back(instr);
        instr="addq rdi, rax";
        funCode.push_back(instr);
        instr="addq r8, rax";
        funCode.push_back(instr);
        //finally assignment
        instr="movq "+to_string(addressDes[lex]) +"(%rbp,%rax,8), "+r;
        funCode.push_back(instr);
    }
}
void insert_arg(vector<string>arg,vector<string>&funCode){
    int cnt=0;
    vector<string>arg_name;
    for(int j=0;j<arg.size();j++){
        arg.push_back(arg_name[j]);
        vector<int>info=var_info(arg_name[j]);
        if(info.size()==0){
            cout<<"No such variable exist\n";
        }
        if(info[0]==100){
            arg_name.push_back("_w1"+arg[j]);
        }
        if(info[0]==200){
            arg_name.push_back("_w1"+arg[j]);
            arg_name.push_back("_w2"+arg[j]);
        }
        if(info[0]==300){
            arg_name.push_back("_w1"+arg[j]);
            arg_name.push_back("_w2"+arg[j]);
            arg_name.push_back("_w3"+arg[j]);
        }
    }
    vector<string>rg={"%rdi", "%rsi", "%rdx", "%rcx", "%r8","%r9"};
    for(int j=0;j<arg_name.size();j++){
        cnt++;
        vector<int>info=var_info(arg_name[j]);
        if(info.size()==0){
            cout<<"No such variable exist\n";
        }
        else{
            if(cnt<=6){
                funCode.push_back("movq "+rg[cnt-1]+", -"+to_string(cnt*8)+"(%rbp)");
                addressDes[arg_name[j]]=-cnt*8;
            }
            else{
                funCode.push_back("movq "+to_string(16+((arg_name.size()-6)-(cnt-6))*8)+"(%rbp)"+", %rdx");
                funCode.push_back("movq %rdx, -"+to_string(cnt*8)+"(%rbp)");
                addressDes[arg_name[j]]=-cnt*8;
            }
        }
    }
}
void beg_func(string x,vector<string>&funCode){
    if(x.rfind("beginfunc",0)!=0){
        cout<<"Not a start of the function\n";
    }
    int f=0;
    string func_nm;
    int pos;
    for(int j=0;j<x.size();j++){
        if(x[j]==' '){
            f=1;
        }
        else
        if(f==1){
            if(x[j]==' '){
                pos=j+1;
                break;
            }
            func_nm.push_back(x[j]);
        }
    }
    fill_var_temp_sz(func_nm);
    vector<string>arg_name;
    string temp;
    for(int j=pos;j<x.size();j++){
        if(x[j]==','){
            arg_name.push_back(temp);
            temp="";
        }
        else{
            temp.push_back(x[j]);
        }
    }
    string instr=func_nm+":";
    funCode.push_back(instr);
    instr="pushq rbp";
    funCode.push_back(instr);
    instr="movq rsp, rbp";
    funCode.push_back(instr);
    instr="subq $"+to_string(sz_func())+", %rsp";
    funCode.push_back(instr);
    insert_arg(arg_name,funCode);
}
void func_call(vector<string>a,vector<string>&funcCode){
    vector<string>ans_reg;//keep all the register instruction
    vector<string>ans_st;//keep all the pushq instruction
    vector<string>things;
    for(int j=a.size()-2;j>=0;j--){
        string y=consOrVar(a[j]);
        if(y[0]=='$'){
            things.push_back(y);
        }
        else{
            string x=smplPush(a[j]);
            if(var_info(x).size()==0){
                cout<<"No such variable exist\n";
            }
            else{
                vector<int>info=var_info(x);
                if(info[0]==1){ //int
                    things.push_back(y);
                }
                if(info[0]>=100){   //array
                    if(info[1]==0){// the size of array is in var 
                        if(info[0]==100){//1darray
                            things.push_back('$'+to_string(info[2]));
                        }
                        if(info[0]==200){//2darray
                            things.push_back('$'+to_string(info[2]));
                            things.push_back('$'+to_string(info[3]));
                        }
                        if(info[0]==300){//3darray
                            things.push_back('$'+to_string(info[2]));
                            things.push_back('$'+to_string(info[3]));
                            things.push_back('$'+to_string(info[4]));
                        }
                    }
                    else{       // for this array we have width in stack
                        if(info[0]==100){//1darray
                            things.push_back(to_string(addressDes["_w1"+x])+"(%rbp)");
                        }
                        if(info[0]==200){//2darray
                            things.push_back(to_string(addressDes["_w1"+x])+"(%rbp)");
                            things.push_back(to_string(addressDes["_w2"+x])+"(%rbp)");
                        }
                        if(info[0]==300){//3darray
                            things.push_back(to_string(addressDes["_w1"+x])+"(%rbp)");
                            things.push_back(to_string(addressDes["_w2"+x])+"(%rbp)");
                            things.push_back(to_string(addressDes["_w3"+x])+"(%rbp)");
                        }
                    }
                }
            }
        }
    }
    vector<string>rg={"%rdi", "%rsi", "%rdx", "%rcx", "%r8","%r9"};
    for(int j=0;j<things.size();j++){
        if(j<6){
            ans_reg.push_back("movq  "+things[j]+", "+rg[j]);
        }
        else{
            ans_st.push_back("movq  "+things[j]+", %rdx");
            ans_st.push_back("pushq  %rdx");
        }
    }
    string instr;
    instr="call  "+funcName(a[a.size()-1]);
    ans_reg.push_back(instr);
    for(auto y:ans_st){
        funcCode.push_back(y);
    }
    for(auto y:ans_reg){
        funcCode.push_back(y);
    }
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
    
    

    vector<string> code = genfunc("main");
    finalCodeGen(code);
}