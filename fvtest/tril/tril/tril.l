%{
    #include "ast.h"
    #include "tril.parser.h"
    #include <stdio.h>

    void yyerror(char *s);
%}

%%

@?[a-zA-Z][a-zA-Z0-9]* {
                          char* name = (char*)malloc(yyleng*sizeof(char) + 1);
                          snprintf(name, yyleng + 1, "%s", yytext);
                          yylval.str = name;
                          return IDENTIFIER;
                       };

[-]?[0-9]+      {
                yylval.integer = atol(yytext);
                return INTEGER;
            }

[-]?[0-9]+[.][0-9]+      {
                yylval.f64 = strtod(yytext, NULL);
                return DOUBLE;
            }

;[^\n]*     ; /* ignore comment */

\"[^"]*\"   {
                char* val = (char*)malloc(yyleng*sizeof(char) + 1);
                snprintf(val, yyleng - 1, "%s", yytext + 1); // remove surrounding "
                yylval.str = val;
                return STRING;
            };

[()[\]=,]      return *yytext;

[ \t\n]     ; /* ignore white space */

.           fprintf(stderr, "Invalid character '%s'\n", yytext);

%%

void set_input_string(const char* in) {
    yy_scan_string(in);
}

void end_lexical_scan(void) {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

int yywrap(void) {
    return 1;
}
