#include<string>
#include<map>
#include<vector>
using namespace std;

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

} ;

// vector<int> genregs = {0, 1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15};

