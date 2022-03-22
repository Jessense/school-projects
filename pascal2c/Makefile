LEX=flex
YACC=bison
CC=gcc

all: codeAnalysis.tab.o lex.yy.o symbolTable.o util.o
	$(CC) -o transform.out codeAnalysis.tab.o lex.yy.o symbolTable.o util.o

codeAnalysis.tab.c: codeAnalysis.y
	$(YACC) -d codeAnalysis.y

codeAnalysis.tab.o: codeAnalysis.tab.c
	$(CC) -c codeAnalysis.tab.c

lex.yy.c: lex.l
	$(LEX) lex.l

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

symbolTable.o: symbolTable.c
	$(CC) -c symbolTable.c

util.o: util.c
	$(CC) -c util.c

clean:
	rm util.o lex.yy.* codeAnalysis.tab.* symbolTable.o

output:
	$(YACC) -v codeAnalysis.y

debug:
	$(YACC) --defines --debug codeAnalysis.y
	make codeAnalysis.tab.o
	make all
