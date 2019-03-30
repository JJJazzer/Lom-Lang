#######################################################################
# file name: Makefile
# author   : Yu Liu
# email    : <ilhanwnz@hotmail.com>
# time     : Fri 22 Feb 2019 06:14:23 PM CST
#######################################################################

OBJS := lex.yy.o y.tab.o ast.o util.o symbol.o table.o\
	errmsg.o types.o env.o util.o semantic.o translate.o \
	tmplabel.o frame_x86.o IR-tree.o escape.o assem.o codegen.o \
	main.o
CFLAGS += -Iinclude
INTERPRETER = python$(PYTHON_VERSON)
PYTHON_VERSON := 3
DIRS := Tools docs 
vpath include
.PHONY: clean Tools docs

build: lomtC Tools docs 
lomtC: $(OBJS)
	$(CC) $^ -o $@
$(OBJS): lex.yy.c ast.c y.tab.c util.c symbol.c table.c\
	 errmsg.c main.c
lex.yy.c:
	lex lexical.lex
y.tab.c:
	yacc -dv grammar.y
	chmod +x Tools/DealYaccIssue.py
	$(INTERPRETER) Tools/DealYaccIssue.py
lex.yy.o:
	cc -g -c -Iinclude lex.yy.c
y.tab.o:
	cc -g -c -Iinclude y.tab.c
ast.o:
	cc -g -c -Iinclude ast.c
AddSnytaxHi:
	./Tools/AddSyntaxHi.sh
Tools:
	cd Tools; make;
docs:
	cd Tools; make;
	cd docs; make;

clean:
	rm -rf $(OBJS) lomtC lex.yy.c y.tab.c y.tab.h y.output
	
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done 
