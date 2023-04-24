#include "node.h"

using namespace std;

vector<Quadruple *> ircode;

Node::Node(string label, string attr)
{
    this->label = label;
    this->attr = attr;
    this->useful = true;
    this->varName = attr;
    this->arrayType = 0;
    this->last = ircode.size() - 1;
    this->isCond = false;
    this->width1 = "w";
    this->width2 = "w";
    this->width3 = "w";
}
Node::Node(string label, string attr, int id)
{
    this->label = label;
    this->attr = attr;
    this->useful = true;
    this->id = id;
    this->varName = attr;
    this->arrayType = 0;
    this->last = ircode.size() - 1;
    this->isCond = false;
    this->width1 = "w";
    this->width2 = "w";
    this->width3 = "w";
}
Node::Node(string label, string attr, vector<struct Node *> children)
{
    this->label = label;
    this->attr = attr;
    this->useful = true;
    this->children = children;
    this->isCond = false;

    for (auto it : children)
    {
        if (it->code.size() > 0)
            this->code.insert(this->code.end(), it->code.begin(), it->code.end());
        this->last = max(this->last, it->last);
        this->nextlist = it->nextlist;
    }
    this->varName = attr;
    this->arrayType = 0;
    this->last = ircode.size() - 1;
    this->width1 = "w";
    this->width2 = "w";
    this->width3 = "w";
}
Node::Node(string label, vector<struct Node *> children)
{
    this->label = label;
    // this->attr = attr;
    this->useful = true;
    this->children = children;
    this->isCond = false;
    for (auto it : children)
    {
        if (it->code.size() > 0)
            this->code.insert(this->code.end(), it->code.begin(), it->code.end());
        this->last = max(this->last, it->last);
        this->nextlist = it->nextlist;
        this->varName = this->attr = this->attr +  it->varName;
    }
    this->arrayType = 0;
    this->last = ircode.size() - 1;
    this->width1 = "w";
    this->width2 = "w";
    this->width3 = "w";
    // this->varName= attr;
}
Node::Node(string label, string attr, vector<struct Node *> children, int id)
{
    this->label = label;
    this->attr = attr;
    this->useful = true;
    this->id = id;
    this->children = children;
    this->isCond = false;
    for (auto it : children)
    {
        if (it->code.size() > 0)
            this->code.insert(this->code.end(), it->code.begin(), it->code.end());
        this->last = max(this->last, it->last);
        this->nextlist = it->nextlist;
        this->varName = this->attr = this->attr +  it->varName;
    }
    this->varName = attr;
    this->arrayType = 0;
    this->last = ircode.size() - 1;
    this->width1 = "w";
    this->width2 = "w";
    this->width3 = "w";
}
Node::Node(string attr)
{
    this->type = 0;
    this->attr = attr;
    this->useful = true;
    this->varName = attr;
    this->arrayType = 0;
    this->last = ircode.size() - 1;
    this->isCond = false;
    this->width1 = "w";
    this->width2 = "w";
    this->width3 = "w";
}

void Node::addChild(struct Node *n)
{
    if (n == NULL)
        return;
    if (n->useful)
    {
        this->children.push_back(n);
        this->last = max(this->last, n->last);
    }
    else
    {
        for (auto it : n->children)
        {
            this->children.push_back(it);
            this->last = max(this->last, it->last);
        }
    }
    if (n->code.size() > 0)
        this->code.insert(this->code.end(), n->code.begin(), n->code.end());
    this->nextlist = n->nextlist;
}

void Node::addChildToLeft(struct Node *n)
{
    if (n == NULL)
        return;
    cout << "calles\n";
    int m = this->children.size();
    cout << "m = " << m << "\n";
    this->last = max(this->last, n->last);
    this->children.resize(m + 1);
    cout << "resizes\n";
    for (int i = m; i > 0; i--)
    {
        this->children[i] = this->children[i - 1];
    }
        cout << "resizes\n";

    this->children[0] = n;
        cout << "resizes\n";
    
    if (n->code.size() > 0)
    {
        cout << "her\n";
        if(this->code.size() > 0 ) n->code.insert(n->code.end(), this->code.begin(), this->code.end());
        cout << "done\n";
        this->code.clear();
        for(auto it: n->code) {
            this->code.push_back(it);
        }
        cout << "here\n";
    }
    else {
        cout << "eks\n";
    }
    cout << "done\n";
    return;
}

void Node::changeLabel(string newLabel)
{
    this->label = newLabel;
}

void Node::useless()
{
    this->useful = false;
}

void printTree(Node *root)
{
    queue<pair<Node *, int>> q;
    q.push({root, 0});
    int c = 0;
    while (!q.empty())
    {
        pair<Node *, int> p = q.front();

        if (p.second > c)
        {
            c++;
        }

        q.pop();
        for (auto it : p.first->children)
        {
            q.push({it, p.second + 1});
        }
    }
}