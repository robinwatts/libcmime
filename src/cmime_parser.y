%{
	#define _GNU_SOURCE
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>
	
	#include "cmime_string.h"
	#include "cmime_message.h"
	#include "cmime_list.h"
	#include "cmime_internal.h"
	
	extern int yylex();
	void yyerror(CMimeMessage_T *msg,char *s,...);
	extern int yylineno; 
%}

%parse-param {CMimeMessage_T *msg}

%union {
	char *string;
}

%token <string> HEADERNAME HEADERBODY EMPTY_LINE LINE CONTINUATION 
%type <string> body
%type <string> headerline

%type <string> continuations
%type <m> message
%type <h> header

%%

message:
		header EMPTY_LINE body { 
			/* printf("BODY\n"); */
		}
	;
	
header: 
		headerline { /* nothing */	}
	|	header headerline { /* nothing */ } 
	;
	
headerline:
		HEADERNAME HEADERBODY {
			char *t = $2;
			if (msg->linebreak == NULL) 
				msg->linebreak = _cmime_internal_determine_linebreak($2);
			
			t = strsep(&t,msg->linebreak);
//			printf("1: HEADERNAME [%s] HEADERBODY [%s]\n",$1,t);
			_cmime_internal_set_linked_header_value(msg->headers,$1,t); 
		}
	| HEADERNAME HEADERBODY continuations {	
//			printf("2: HEADERNAME [%s] HEADERBODY [%s] continuations [%s]\n",$1,$2,$3);	
			char *t;
			t = cmime_string_strsep_last($2,msg->linebreak);
			_cmime_internal_set_linked_header_value(msg->headers,$1,t);
			free(t);
		}
	;

continuations:
		CONTINUATION { 
//			printf("3: CONTINUATION [%s]\n",$1);
//			$1 = cmime_string_chomp($1);
//			$$ = $1; 
		}
	| continuations CONTINUATION {
//			printf("4: continuations [%s] CONTINUATION [%s]\n",$1,$2);
			
//			char *t;
//			$2 = cmime_string_chomp($2);
//			asprintf(&t,"%s%s%s",$1,msg->linebreak,$2); 
//			$$ = t;
		}
	;
	
body:
		LINE {
			
		}
	|	body LINE {  }
	;
	
%%

void yyerror(CMimeMessage_T *msg,char *s,...) {
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}