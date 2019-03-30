/*
 * file name: main.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Fri 22 Feb 2019 03:10:15 PM CST
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
#include "semantic.h"
#include "y.tab.h"
#include "terms_color.h"
#include "escape.h"
#include "codegen.h"
#include "assem.h"


#define Try_help() 		fprintf(stderr, \
		"--help                 View the help manual.\n" \
		"    -S                 Assembly the target files.\n")
FILE *yyin;
int yyparse();
extern Ast_decList root;
extern bool anyError;
static void Convert2Asm(char *arg);
static char *strtrunc_suffix(char *name, char c);
static void SelectItems(int argc, char *argv[]);

static void Exception(char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(-1);
}

static void Convert2Asm(char *arg)
{
	char *out_file = strcat(strtrunc_suffix(arg, '.'), ".S");
	int ret;
	if ((yyin = fopen(arg, "r+")) == NULL) 
		Exception("%s\n", strerror(errno));
	
	Cgen_Init(out_file);
	ret = yyparse();
	if (ret != 0) 
		Exception(PURPLE(LIGHT) "Warning: Compile failed");
	Esc_SearchEscape(root);	
	Sem_TransProg(root);
	/*
	if (anyError) {
		fclose(yyin);
		fclose(out);
		Exception(PURPLE(LIGHT) "Warning: Compile failed");
	}
	Cgen_CodeGen(Assem_Label("tmp", "int", "0", NULL));
	fclose(yyin);
	fclose(out);
	free(root);
	*/
	return;
}
	
static char *strtrunc_suffix(char *name, char c)
{
	int i = 0;
	int len = strlen(name);
	char *s = malloc(sizeof(*s) * len);
	while (*name != c && i != len) {
		s[i] = *name++;
		i++;
	}
	return s;
}

static void SelectItems(int argc, char *argv[])
{
	int opt;
	
	static struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{NULL, required_argument, NULL, 'S'},
		{0, 0, 0, 0}
	};
	while ((opt = getopt_long(argc, argv, ":S:h:", long_options, 0)) != -1) {
		switch (opt) {
		case 'S':
			/* Execute translate operate */

			Convert2Asm(optarg);
			break;
		case 'h':
			Try_help();
			exit(0);	/* Normal exited */
		case '?':
			fprintf(stderr, "Unknown arguments, please try to use --help command.\n");
			exit(-1);
		}	
	}	
}

int main(int argc, char *argv[])
{
	
	if (argc < 2)  
		Exception(RED(LIGHT) "error: " WHITE(LIGHT)
				"No input file.");
	SelectItems(argc, argv);
	return 0;
}
