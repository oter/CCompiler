%{
#include <stack>
#include <iostream>
#include <string>
#include <assert.h>

#include "../tree_syntax/tree_syntax.hpp"
#include "../tree_syntax/tree_syntax_helper.hpp"

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
%token NUM ID LE GE EQ NE LT GT AND OR NOT XOR

%right NOT XOR
%right '='
%left AND OR
%left '<' '>' LE GE EQ NE LT GT
%%

start : GlobalList
    ;

GlobalList:
    GlobalList GlblDecl
    | 
    GlblDecl
    ;

GlblDecl:
    Function
    {
        assert(!syntax_stack.empty() && "Stack can't be empty after function Declaration.");
        //std::cout << "Func1" << std::endl;

        auto top = syntax_stack.top();
        syntax_stack.pop();

        if (syntax_stack.empty())
        {
            syntax_stack.push(std::make_shared<TreeTopLevel>(std::vector<TreeSyntaxShared >{top}));
        } else
        {
            std::cout << syntax_stack.size() << std::endl;
            if (syntax_stack.size() == 1 && syntax_stack.top()->type() == TreeSyntaxType::kTopLevel) {}else{

                if (syntax_stack.size() > 0) {
                    std::cerr << "WARNING: Remaining objects are left in the stack after parsing:\n";
                    while(syntax_stack.size() > 0) {
                        std::cerr << "====TREE_ROOT====\n\t";
                        std::cerr << TreeSyntaxHelper::GetSyntaxTypeName(syntax_stack.top()) << std::endl;
                        syntax_stack.pop();
                    }
                }
            }
            assert(syntax_stack.size() == 1 && syntax_stack.top()->type() == TreeSyntaxType::kTopLevel && "Stack must contain kTopLevel");
            std::static_pointer_cast<TreeTopLevel>(syntax_stack.top())->AddDeclaration(top);
        }
    } 
    | 
    Declaration
    {
        assert(!syntax_stack.empty() && "Stack can't be empty after function Declaration.");
        //std::cout << "Func2" << std::endl;

        auto top = syntax_stack.top();
        syntax_stack.pop();

        if (syntax_stack.empty())
        {
            syntax_stack.push(std::make_shared<TreeTopLevel>(std::vector<TreeSyntaxShared >{top}));
        } else
        {
            assert(syntax_stack.size() == 1 && syntax_stack.top()->type() == TreeSyntaxType::kTopLevel && "Stack must contain kTopLevel");
            std::static_pointer_cast<TreeTopLevel>(syntax_stack.top())->AddDeclaration(top);
        }
    }
    
    ;

/* Declaration block */
Declaration: Type ID ';'
    {
        // TODO: Type $1
        std::cout << "Warning: Uninitialized variable: " << $1 << " " << $2 << std::endl;
        syntax_stack.push(std::make_shared<TreeDefineVariable>($2, std::make_shared<TreeEmptyStatement>()));
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
    | 
    Type ArrayUsage ';'
    {
        // TODO: Declare array
    }
    ;

/* Assignment block */
Assignment:
    ID '=' Assignment
    {
        // DONE
        auto expression = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeAssignment>($1, expression));
    }
    |
    ID '=' FunctionCall
    {
        // DONE
        auto expression = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeAssignment>($1, expression));
    }
    |
    ID '=' ArrayUsage
    {
        // DONE
        auto expression = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeAssignment>($1, expression));
    }
    |
    ArrayUsage '=' Assignment
    {
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        auto var = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeArrayAssignment>(var, assign));
    }
    | 
    ID '+' Assignment
    {
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        auto lhs = std::make_shared<TreeVariable>($1);
        syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kAddition, lhs, assign));
    }
    | 
    ID '-' Assignment
    {
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        auto lhs = std::make_shared<TreeVariable>($1);
        syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kSubtraction, lhs, assign));
    }
    | 
    ID '*' Assignment
    {
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        auto lhs = std::make_shared<TreeVariable>($1);
        syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kMultiplication, lhs, assign));
    }
    | 
    ID '/' Assignment
    {
        // TODO: Division
    }
    | 
    NUM '+' Assignment
    {
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        auto lhs = std::make_shared<TreeImmediate>($1);
        free($1);
        syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kAddition, lhs, assign));
    }
    | 
    NUM '-' Assignment
    {
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        auto lhs = std::make_shared<TreeImmediate>($1);
        free($1);
        syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kSubtraction, lhs, assign));
    }
    | 
    NUM '*' Assignment
    {
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        auto lhs = std::make_shared<TreeImmediate>($1);
        free($1);
        syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kMultiplication, lhs, assign));
    }
    | 
    NUM '/' Assignment
    {
        // TODO: Division
    }
    | 
    '(' Assignment ')'
    {
        // Nothing to do
    }
    |   NUM
    {
        // DONE
        // TODO: float immediate
        syntax_stack.push(std::make_shared<TreeImmediate>($1));
        free(($1));
    }
    |
    ID
    {
        // DONE
        syntax_stack.push(std::make_shared<TreeVariable>($1));
    }
    |
    NOT Assignment
    {
        // DONE
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeUnaryExpression>(TreeUnaryExpressionType::kLogicalNegation, assign));
    }
    |
    '~' Assignment
    {
        // DONE
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeUnaryExpression>(TreeUnaryExpressionType::kBitwiseNegation, assign));
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
ArrayUsage :
    ID'['Assignment']'
    {
        // DONE
        auto assign = syntax_stack.top();
        syntax_stack.pop();
        syntax_stack.push(std::make_shared<TreeArrayIndexer>($1, assign));
    }
    ;

/* Function block */
Function:
    Type ID '(' ArgListOpt ')' CompoundStmt
    {
        assert(syntax_stack.size() > 1 && "Internal error. Invalid stack size!");

        //std::cout << TreeSyntaxHelper::GetSyntaxTypeName(syntax_stack.top()) << std::endl;
        //std::cout << "Size: " << syntax_stack.size() << std::endl;
        // DONE
        // TODO: Check empty function
        //std::cout << "Function block: " << $1 << " " << $2 << std::endl;
        auto compound = syntax_stack.top();
        syntax_stack.pop();
        //std::cout << TreeSyntaxHelper::GetSyntaxTypeName(syntax_stack.top()) << std::endl;
        auto args = syntax_stack.top();
        syntax_stack.pop();
        assert(compound->type() == TreeSyntaxType::kStatementsBlock && "");
        syntax_stack.push(std::make_shared<TreeFunction>($2, args, compound));
    }  
    ;
ArgListOpt: ArgList
    |
    {
        //std::cout << "Add empty argument!" << std::endl;
        syntax_stack.push(std::make_shared<TreeEmptyStatement>());
    }
    ;
ArgList:
    ArgList ',' Arg
    | 
    Arg
    ;
Arg: Type ID
    {
        //std::cout << "ARG: " << $1 << " " << $2 << std::endl;
        if (syntax_stack.size() > 0)
        {       
            if (syntax_stack.top()->type() == TreeSyntaxType::kFunctionArguments)
            {
                std::static_pointer_cast<TreeFunctionArguments>(syntax_stack.top())->AddArgument(std::make_shared<TreeFunctionArgument>($2));
            } else if (syntax_stack.top()->type() == TreeSyntaxType::kFunctionArgument)
            {
                auto top = syntax_stack.top();
                syntax_stack.pop();
                syntax_stack.push(std::make_shared<TreeFunctionArguments>(std::vector<TreeSyntaxShared>{top}));
            }
        } else 
        {
            syntax_stack.push(std::make_shared<TreeFunctionArguments>(std::vector<TreeSyntaxShared>{std::make_shared<TreeFunctionArgument>($2)}));
        }
    }
    ;
CompoundStmt: '{' StmtList '}'
    {
        //std::cout << "CompoundStmt" << std::endl; 
        assert(syntax_stack.size() > 0 && "Invalid stack size after compound addition!");
        if (syntax_stack.top()->type() != TreeSyntaxType::kStatementsBlock)
        {
            //std::cout << "Add empty statement block" << std::endl;
            auto empty = std::make_shared<TreeEmptyStatement>();
            syntax_stack.push(std::make_shared<TreeStatementsBlock>(std::vector<TreeSyntaxShared>{empty}));
        }
    }
    ;
StmtList: StmtList Stmt
    {
        assert(syntax_stack.size() > 0 && "Stack size have to be at least 1");
        assert(syntax_stack.top()->type() != TreeSyntaxType::kStatementsBlock && "Top can't be kStatementsBlock");

        auto top = syntax_stack.top();
        syntax_stack.pop();

        if (syntax_stack.size() > 0)
        {
            if (syntax_stack.top()->type() == TreeSyntaxType::kStatementsBlock)
            {
                std::static_pointer_cast<TreeStatementsBlock>(syntax_stack.top())->AddStatement(top);
            } else 
            {
                syntax_stack.push(std::make_shared<TreeStatementsBlock>(std::vector<TreeSyntaxShared>{top}));
            }
        } else 
        {
            syntax_stack.push(std::make_shared<TreeStatementsBlock>(std::vector<TreeSyntaxShared>{top}));
        }
    }
    |
    ;
Stmt:
    WhileStmt
    |
    Declaration
    |
    ForStmt
    |
    IfStmt
    |
    RetStmt
    |
    ';'
    {
        // DONE
        // TODO: Check
        auto compound = std::make_shared<TreeEmptyStatement>();
        syntax_stack.push(compound);
    }
    ;

/* Type Identifier block */
Type :
    INT
    |
    FLOAT
    |
    CHAR
    ;

CtrlStmt :
    Stmt
    |
    CompoundStmt
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
        std::cout << "size: " << syntax_stack.size() << std::endl;
        assert(syntax_stack.size() >= 4 && "Not all expressions parsed.");
        auto body = syntax_stack.top();
        syntax_stack.pop();

        auto inc = syntax_stack.top();
        syntax_stack.pop();

        auto condition = syntax_stack.top();
        syntax_stack.pop();

        auto assign = syntax_stack.top();
        syntax_stack.pop();

        syntax_stack.push(std::make_shared<TreeForStatement>(assign, condition, inc, body));
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