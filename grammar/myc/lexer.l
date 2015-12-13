
D			[0-9]
L			[a-zA-Z_]

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


%%
"#include"    { return INCLUDE; }
#[^\n]*       { /* Discard preprocessor comments. */ }
"//"[^\n]*    { /* Discard c99 comments. */ }
"/*"          { comment(); }
[ \t\n]+      { /* Ignore whitespace */ }

"{"           { return OPEN_BRACE; }
"}"           { return CLOSE_BRACE; }
"("           { return '('; }
")"           { return ')'; }
"~"           { return '~'; }
"!"           { return '!'; }
"+"           { return '+'; }
"-"           { return '-'; }
"*"           { return '*'; }
"<"           { return '<'; }
"<="          { return LESS_OR_EQUAL; }
"="           { return '='; }
";"           { return ';'; }
","           { return ','; }
[0-9]+        {
                  /* TODO: Binary values, hex values; Add checks  */
                  yylval = strdup(yytext); return NUMBER;
              }
"if"          { return IF; }
"while"       { return WHILE; }
"return"      { return RETURN; }

"int"         { return TYPE; }
{L}({L}|{D})* { yylval = strdup(yytext); return IDENTIFIER; }

"<"[a-z.]+">" { return HEADER_NAME; }
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
