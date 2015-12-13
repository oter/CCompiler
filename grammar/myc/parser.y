%{
#include <stack>
#include <iostream>
#include <string>
#include <assert.h>

#include "../tree_syntax/tree_syntax.hpp"

#define YYSTYPE char*

int yyparse(void);
int yylex();

void yyerror(const std::string& err)
{
    std::cerr << "Error: " << err << std::endl;
}

int yywrap()
{
	return 1;
}

extern FILE *yyin;

std::stack<TreeSyntaxShared> syntax_stack;
%}

%token INCLUDE HEADER_NAME
%token TYPE IDENTIFIER RETURN NUMBER
%token OPEN_BRACE CLOSE_BRACE
%token IF WHILE
%token LESS_OR_EQUAL

/* Operator associativity, least precedence first.
 * See http://en.cppreference.com/w/c/language/operator_precedence
 */
%left '='
%left '<'
%left '+'
%left '-'
%left '*'
%nonassoc '!'
%nonassoc '~'

%%

program:
        function program
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
        ;

function:
	TYPE IDENTIFIER '(' parameter_list ')' OPEN_BRACE block CLOSE_BRACE
        {
            auto current_syntax = syntax_stack.top();
            syntax_stack.pop();
            assert(current_syntax->type() == TreeSyntaxType::kStatementsBlock && "Type must be TreeSyntaxType::kStatementsBlock.");
            syntax_stack.push(std::make_shared<TreeFunction>($2, std::vector<TreeSyntaxShared>{}, current_syntax));
        }
        ;

parameter_list:
        nonempty_parameter_list
        |
        ;

nonempty_parameter_list:
        TYPE IDENTIFIER ',' parameter_list
        |
        TYPE IDENTIFIER
        ;

block:
        statement block
        {
            TreeSyntaxShared block_syntax;
            if (syntax_stack.empty())
            {
                block_syntax = std::make_shared<TreeStatementsBlock>(std::vector<TreeSyntaxShared>{});// block_new(list_new());
            } else if (syntax_stack.top()->type() != TreeSyntaxType::kStatementsBlock)
            {
                block_syntax = std::make_shared<TreeStatementsBlock>(std::vector<TreeSyntaxShared>{});
            } else
            {
                block_syntax = syntax_stack.top();
                syntax_stack.pop();
            }
            std::static_pointer_cast<TreeStatementsBlock>(block_syntax)->AddStatement(syntax_stack.top());
            syntax_stack.pop();
            syntax_stack.push(block_syntax);
        }
        |
        ;

argument_list:
        nonempty_argument_list
        |
        {
            // TODO: Arguments
            syntax_stack.push(std::make_shared<TreeFunctionArguments>(std::vector<TreeSyntaxShared>{}));
        }
        ;

nonempty_argument_list:
        expression ',' nonempty_argument_list
        {
            TreeSyntaxShared arguments_syntax;
            assert(!syntax_stack.empty() && "Stack can't be empty! Internal error in processing non empty args list");
            if (syntax_stack.top()->type() != TreeSyntaxType::kFunctionArguments)
            {
                arguments_syntax =  std::make_shared<TreeFunctionArguments>(std::vector<TreeSyntaxShared>{});//function_arguments_new();
            } else
            {
                arguments_syntax = syntax_stack.top();
                syntax_stack.pop();
            }
            std::static_pointer_cast<TreeFunctionArguments>(arguments_syntax)->AddArgument(syntax_stack.top());
            syntax_stack.pop();
            syntax_stack.push(arguments_syntax);
        }
        |
        expression
        {
            assert(!syntax_stack.empty() && "Stack can't be empty! Internal error in processing non empty args list");
            auto arguments_syntax = std::make_shared<TreeFunctionArguments>(std::vector<TreeSyntaxShared>{});
            arguments_syntax->AddArgument(syntax_stack.top());
            syntax_stack.pop();
            syntax_stack.push(arguments_syntax);
        }
        ;

statement:
        RETURN expression ';'
        {
            auto current_syntax = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeReturnStatement>(current_syntax));
        }
        |
        IF '(' expression ')' OPEN_BRACE block CLOSE_BRACE
        {
            // TODO: else statements.
            auto then = syntax_stack.top();
            syntax_stack.pop();
            auto condition = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeIfStatement>(condition, then));
        }
        |
        WHILE '(' expression ')' OPEN_BRACE block CLOSE_BRACE
        {
            auto body = syntax_stack.top();
            syntax_stack.pop();
            auto condition = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeWhileStatement>(condition, body));
        }
        |
        TYPE IDENTIFIER '=' expression ';'
        {
            auto init_value = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeDefineVariable>($2, init_value));
        }
        |
        expression ';'
        {
            // Nothing to do
        }
        ;

expression:
	NUMBER
        {
            // TODO: float immediate
            syntax_stack.push(std::make_shared<TreeImmediate>(atoi((char*)($1))));
            free(($1));
        }
        |
	IDENTIFIER
        {
            syntax_stack.push(std::make_shared<TreeVariable>($1));
        }
        |
	IDENTIFIER '=' expression
        {
            auto expression = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeAssignment>($1, expression));
        }
        |
        '~' expression
        {
            auto current_syntax = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeUnaryExpression>(TreeUnaryExpressionType::kBitwiseNegation, current_syntax));//  bitwise_negation_new(current_syntax));
        }
        |
        '!' expression
        {
            auto current_syntax = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeUnaryExpression>(TreeUnaryExpressionType::kLogicalNegation, current_syntax));
        }
        |
        expression '+' expression
        {
            auto right = syntax_stack.top();
            syntax_stack.pop();
            auto left = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kAddition, left, right));
        }
        |
        expression '-' expression
        {
            auto right = syntax_stack.top();
            syntax_stack.pop();
            auto left = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kSubtraction, left, right));
        }
        |
        expression '*' expression
        {
            auto right = syntax_stack.top();
            syntax_stack.pop();
            auto left = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kMultiplication, left, right));
        }
        |
        expression '<' expression
        {
            auto right = syntax_stack.top();
            syntax_stack.pop();
            auto left = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kLessThan, left, right));
        }
        |
        expression LESS_OR_EQUAL expression
        {
            auto right = syntax_stack.top();
            syntax_stack.pop();
            auto left = syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeBinaryExpression>(TreeBinaryExpressionType::kLessThanOrEqual, left, right));
        }
        |
        IDENTIFIER '(' argument_list ')'
        {
            auto arguments =  syntax_stack.top();
            syntax_stack.pop();
            syntax_stack.push(std::make_shared<TreeFunctionCall>($1, arguments));
        }
        ;
