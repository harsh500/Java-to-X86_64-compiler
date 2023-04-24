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
