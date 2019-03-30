#######################################################################
# file name: Makefile
# author   : Yu Liu
# email    : <ilhanwnz@hotmail.com>
# time     : Sat 16 Mar 2019 10:12:43 AM CST
#######################################################################

PROGS := basic lexical
all: $(PROGS)

%: %.im
	../Tools/Im2Html $@.im $@.html
clean:
	rm *.html -rf
