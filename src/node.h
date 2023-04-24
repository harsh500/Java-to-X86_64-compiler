#pragma once
#include <bits/stdc++.h>
#include "irtype.h"
#include "types.h"
using namespace std;

extern vector<Quadruple *> ircode;

typedef struct Node
{
    vector<struct Node *> children;
    int id;
    struct Node *parent;
    int type = VOID_TYPE;
    string label;
    string attr;
    string varName;
    bool reduced; // true if the expression is broken down
    bool useful;
    bool isCond = false;
    string width1;
    string width2;
    string width3;
    int _width1 = 0;
    int _width2 = 0;
    int _width3 = 0;
    int arrayType = 0;
    int len = 0;          // for strings
    int cnt = 0;          // used to count dimensions in array access
    int last = -1;        // last index of global IR code
    string next = "fall"; // stores the label of next statement to jump
    string True = "fall"; // label of next statement to jump to if evaluated to true
    string False;
    string ir_label;
    vector<Quadruple *> code;
    vector<int> truelist, falselist, nextlist;

    Node(string label, string attr);
    Node(string label, string attr, int id);
    Node(string label, vector<struct Node *> children);
    Node(string label, string attr, vector<struct Node *> children);
    Node(string label, string attr, vector<struct Node *> children, int id);

    Node(string attr);
    void addChild(struct Node *n);
    void addChildToLeft(struct Node *n);
    void changeLabel(string newLabel);
    void useless();
} Node;

void printTree(Node *root);
