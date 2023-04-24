#pragma once
#include "node.h"
#include "symbols.h"
#include "irtype.h"

void defineCastNames();
string GetCurrentWorkingDir( void );
string append_scope_level(string s);

void ir_gen(vector<Quadruple *> ircode, string fln);

void backpatch(vector<int> &lst, int n);

void processFieldDec(Node *n, Node *n1, int type);

int getmethodtype(SymNode *n);

// n is the VariableDeclarator node,
void init1DArray(Node *n, string type);

void init2DArray(Node *n, string type);

void init3DArray(Node *n, string type);

void processPostIncre(Node *n);

void processAssignment(Node *n1, Node *n2, Node *n3, Node *n);

int spacestripind(string s);

string spacestrip(string s);

void processRelational(vector<Node *> nodes, string op);

void processArithmetic(vector<Node *> nodes, string op);

void processWhile(Node *n, Node *n1, Node *n2);
void processDoWhile(Node *n, Node *n1, Node *n2);

int generateArgumentList(vector<Node *> nodes, Node *n);

void verbose(int v, string h);

void processUninitDec(Node *, Node *, int);
