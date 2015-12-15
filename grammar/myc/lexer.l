%{
#define YYSTYPE char*
#include <string>
#include "parser.hxx"

void comment();
void yyerror(const std::string& err);
extern "C" int yywrap()
{
    return 1;
}

%}


alpha [a-zA-Z_]
digit [0-9]

%%
#[^\n]*       				{ /* Discard preprocessor comments. */ }
"//"[^\n]*    				{ /* Discard c99 comments. */ }
"/*"          				{ comment(); }
[ \t]                       ;
[ \n]                       { yylineno = yylineno + 1; }
"int"                       { yylval = strdup("int"); return INT; }
"float"                     { yylval = strdup("float"); return FLOAT; }
"for"                       { return FOR; }
"while"                     { return WHILE; }
"if"                        { return IF; }
"else"                      { return ELSE; }
"return"					{ return RET; }
{digit}+                    { yylval = strdup(yytext); return NUM; }
{alpha}({alpha}|{digit})*   { yylval = strdup(yytext); return ID; }
"<="                        { return LE; }
">="                        { return GE; }
"=="                        { return EQ; }
"!="                        { return NE; }
">"                         { return GT; }
"<"                         { return LT; }
"&&"                        { return AND; }
"||"                        { return OR; }
"!"                         { return NOT; }
\/\*(.*\n)*.*\*\/           ;
.						{ return yytext[0]; };
%%

#define INPUT_EOF 0

void comment(void) {
    /* Consume characters up to the closing comment marker. */
    char c, prev = 0;
  
    while ((c = yyinput()) != INPUT_EOF) {
        if (c == '/' && prev == '*')
            return;
        prev = c;
    }
    yyerror("unterminated comment");
}
