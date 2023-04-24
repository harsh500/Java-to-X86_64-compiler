#include "dotgen.h"
using namespace std;

int r = 1;
void dotgen(Node *head, vector<string> &dec, vector<string> &edg, int cnt)
{
  string lb(head->label);
  if (head->attr.size() != 0)
  {
    string t(head->attr);
    string typ(to_string(head->type));
    lb = lb + "(" + t + ")" + "(" + typ + ")";
  }
  string declaration = to_string(r) + " [label=\"" + lb + "\"]";
  dec.push_back(declaration);
  int par = r;
  for (auto y : head->children)
  {
    r++;
    string edges = to_string(par) + "->" + to_string(r);
    edg.push_back(edges);
    dotgen(y, dec, edg, cnt);
  }
}
void call_dotgen(Node *head, string fln)
{
  ofstream MyFile(fln);
  vector<string> dec, edg;
  dotgen(head, dec, edg, 1);
  MyFile << "digraph D {\n";
  for (auto y : dec)
  {
    MyFile << y << "\n";
  }
  cout << "\n";
  for (auto y : edg)
  {
    MyFile << y << "\n";
  }
  MyFile << "}\n";
  MyFile.close();
}