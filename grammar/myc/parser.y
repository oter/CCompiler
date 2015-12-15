%{
#include <stack>
#include <iostream>
#include <string>
#include <assert.h>

#include "../tree_syntax/tree_syntax.hpp"

#define YYSTYPE char*

int yyparse(void);
int yylex();
extern char* yytext;
extern int yylineno;

void yyerror(const std::string& err)
{
    std::cerr << "Error: " << err << " " << yytext << std::endl << "At line: " << yylineno << std::endl;
}

int yywrap()
{
	return 1;
}

extern FILE *yyin;

std::stack<TreeSyntaxShared> syntax_stack;
%}

%token INT FLOAT CHAR RET
%token FOR WHILE 
%token IF ELSE
%token NUM ID LE GE EQ NE LT GT AND OR NOT

%left NOT
%right '='
%left AND OR
%left '<' '>' LE GE EQ NE LT GT
%%

start : Function
    {
        TreeSyntaxShared top_level_syntax;
        if (syntax_stack.empty())
        {
            top_level_syntax = std::make_shared<TreeTopLevel>(std::vector<TreeSyntaxShared >{});
        } else if (syntax_stack.top()->type() != TreeSyntaxType::kTopLevel)
        {
            top_level_syntax = std::make_shared<TreeTopLevel>(std::vector<TreeSyntaxShared >{});
        } else
        {
            top_level_syntax = syntax_stack.top();
            syntax_stack.pop();
        }
        std::static_pointer_cast<TreeTopLevel>(top_level_syntax)->AddDeclaration(syntax_stack.top());
        syntax_stack.pop();
        syntax_stack.push(top_level_syntax);
    } 
    | 
    Declaration
    {
        TreeSyntaxShared top_level_syntax;
        if (syntax_stack.empty())
        {
            top_level_syntax = std::make_shared<TreeTopLevel>(std::vector<TreeSyntaxShared >{});
        } else if (syntax_stack.top()->type() != TreeSyntaxType::kTopLevel)
        {
            top_level_syntax = std::make_shared<TreeTopLevel>(std::vector<TreeSyntaxShared >{});
        } else
        {
            top_level_syntax = syntax_stack.top();
            syntax_stack.pop();
        }
        std::static_pointer_cast<TreeTopLevel>(top_level_syntax)->AddDeclaration(syntax_stack.top());
        syntax_stack.pop();
        syntax_stack.push(top_level_syntax);
    }
    ;

/* Declaration block */
Declaration: Type ID ';'
    {
        // TODO: Type $1
        // TODO: Check for nullptr
        std::cout << "Warning: Uninitialized variable." << std::endl;
        syntax_stack.push(std::make_shared<TreeDefineVariable>($2, nullptr));
    }
    |
    Type ID '=' Assignment ';'
    {
        auto init_value = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeDefineVariable>($2, init_value));
    }
    | Assignment ';'
    {
        // Nothing to do
    }  
    | FunctionCall ';'
    {
        // Nothing to do
    }
    | ArrayUsage ';'
    {
        // Nothing to do
    }
    ;

/* Assignment block */
Assignment: ID '=' Assignment
    {

    }
    | ID '=' FunctionCall
    | ID '=' ArrayUsage
    | ID '+' Assignment
    | ID '-' Assignment
    | ID '*' Assignment
    | ID '/' Assignment
    | NUM '+' Assignment
    | NUM '-' Assignment
    | NUM '*' Assignment
    | NUM '/' Assignment
    | '(' Assignment ')'
    {
        // Nothing to do
    }
    |   NUM
    {
        // DONE
        // TODO: float immediate
        syntax_stack.push(std::make_shared<TreeImmediate>(atoi((char*)($1))));
        free(($1));
    }
    |   ID
    {
        // DONE
        syntax_stack.push(std::make_shared<TreeVariable>($1));
    }
    ;

FunctionArgs : Assignment
    {
        // DONE
        assert(!syntax_stack.empty() && "Stack can't be empty! Internal error in processing non empty args list");
        auto assign = std::make_shared<TreeFunctionArguments>(std::vector<TreeSyntaxShared>{});
        assign->AddArgument(syntax_stack.top());
        syntax_stack.pop();
        syntax_stack.push(assign);

    }
    | FunctionArgs ',' Assignment
    {
        // DONE
        // TODO: Checks??
        // TODO: Check for correctness!
        assert(!syntax_stack.empty() && "Stack can't be empty! Internal error in processing non empty args list");
    
        auto assign = syntax_stack.top();
        syntax_stack.pop();

        std::static_pointer_cast<TreeFunctionArguments>(syntax_stack.top())->AddArgument(assign);
    }
    ;

/* Function Call Block */
FunctionCall : ID '(' ')'
    {
        // DONE
        auto args = std::make_shared<TreeFunctionArguments>(std::vector<TreeSyntaxShared>{});
        syntax_stack.push(std::make_shared<TreeFunctionCall>($1, args));
    }
    | ID '(' FunctionArgs ')'
    {
        // DONE
        auto args =  syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeFunctionCall>($1, args));
    }
    ;

/* Array Usage */
ArrayUsage : ID'['Assignment']'
    ;

/* Function block */
Function: Type ID '(' ArgList ')' CompoundStmt
    {
        std::cout << "Function block: " << $1 << " " << $2 << std::endl;
        auto current_syntax = syntax_stack.top();
        syntax_stack.pop();
        assert(current_syntax->type() == TreeSyntaxType::kStatementsBlock && "Type must be TreeSyntaxType::kStatementsBlock.");
        syntax_stack.push(std::make_shared<TreeFunction>($2, std::vector<TreeSyntaxShared>{}, current_syntax));
    }
    |
    Type ID '(' ')' CompoundStmt
    {
        std::cout << "Function block without args!" << $1 << " " << $2 << std::endl;
    }
    ;
ArgList:  ArgList ',' Arg
    | Arg
    ;
Arg:    Type ID
    ;
CompoundStmt:   '{' StmtList '}'
    ;
StmtList:   StmtList Stmt
    | 
    ;
Stmt: WhileStmt
    | Declaration
    | ForStmt
    | IfStmt
    | RetStmt
    | ';'
    {
        // TODO: Push null statement!!
    }
    ;

/* Type Identifier block */
Type : INT 
    | FLOAT
    | CHAR
    ;

CtrlStmt : Stmt
    | CompoundStmt
    ;

/* Loop Blocks */ 
WhileStmt : WHILE '(' Expr ')' CtrlStmt
    {
        // DONE
        auto body = syntax_stack.top();
        syntax_stack.pop();
        auto condition = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeWhileStatement>(condition, body));
    }
    ;

/* For Block */
ForStmt : FOR '(' Expr ';' Expr ';' Expr ')' CtrlStmt
    {
        // TODO: For statement
    }
    ;

RetStmt : RET Expr ';'
    {
        // DONE
        auto current_syntax = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeReturnStatement>(current_syntax));
    }
    ;

/* IfStmt Block */
IfStmt  : IF '(' Expr ')' CtrlStmt
    {
        // DONE
        auto then = syntax_stack.top();
        syntax_stack.pop();
        auto condition = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeIfStatement>(condition, then));
    }
    |
    IF '(' Expr ')' CtrlStmt ELSE CtrlStmt
    {
        // TODO: ELSE statement
        auto else_stmt = syntax_stack.top();
        syntax_stack.pop();

        auto then = syntax_stack.top();
        syntax_stack.pop();
        auto condition = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeIfStatement>(condition, then));
    }
    ;

/*Expression Block*/
Expr:   
    | Expr LE Expr 
    | Expr GE Expr
    | Expr NE Expr
    | Expr EQ Expr
    | Expr GT Expr
    | Expr LT Expr
    | Assignment
    | ArrayUsage
    ;
%%