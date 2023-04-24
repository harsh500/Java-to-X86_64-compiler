parser:		y.tab.o	lex.yy.o node.o dotgen.o symbols.o irtype.o types.o utils.o
			g++ lex.yy.o y.tab.o node.o dotgen.o symbols.o irtype.o types.o utils.o -o parser 
y.tab.o:	parser.yacc
			bison -d -t -y parser.yacc
			g++ -c y.tab.c
lex.yy.o:	tokenizer.l 
			flex tokenizer.l
			g++ -c lex.yy.c 

node.o:		node.h node.cpp
			g++ -c node.cpp
dotgen.o:	dotgen.h dotgen.cpp 
			g++ -c dotgen.cpp
symbols.o:	symbols.h symbols.cpp
			g++ -c symbols.cpp
irtype.o:	irtype.h irtype.cpp 
			g++ -c irtype.cpp	
types.o:	types.h types.cpp 
			g++ -c types.cpp

utils.o:	utils.h utils.cpp
			g++ -c utils.cpp 	
asmgen.o:	asmgen.cpp
			g++ -c asmgen.cpp