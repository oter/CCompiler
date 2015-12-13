#include <stack>
#include <iostream>

#include "tree_syntax/tree_syntax.hpp"
#include "tree_syntax/tree_syntax_helper.hpp"
#include "AstProxy.hpp"
//#include "assembly.hpp"

//
//Disable warnings when use deprecated features like auto_ptr in boost.
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
//
//#include <boost/regex.hpp>
//
//#pragma GCC diagnostic pop

static const std::string help_message = R"msg(
Welcome to My C compiler.
    usage:
    myc [OPTION] [file]

    Options     Description
    --c         Compile
    --ast       Print ast
    --help      Print this help message
)msg";

enum class Operation
{
    kCompile,
    kPrintAst,
    kPrintHelp
};

void PrintHelp()
{
    std::cout << help_message;
}

Operation HandleConsoleArgs(int argc, char *argv[], std::string& file_to_compile)
{
    ++argv, --argc; // Ignore compiler executable path
    switch (argc)
    {
        case 2:
        {
            if (std::string(argv[0]).compare("--ast") == 0)
            {
                file_to_compile = argv[1];
                return Operation::kPrintAst;
            } else if (std::string(argv[0]).compare("--c") == 0)
            {
                file_to_compile = argv[1];
                return Operation::kCompile;
            }
        }
        default:
            PrintHelp();
            return Operation::kPrintHelp;
    }
};

int main(int argc, char *argv[])
{
    std::string file_to_compile;
    auto operation = HandleConsoleArgs(argc, argv, file_to_compile);
    if (operation == Operation::kPrintHelp) { return 1; }

    AstProxy ast;
    int result = ast.ProcessFile(file_to_compile);
    if (result != 0) { return result; }
    auto root = ast.root();

    if (operation == Operation::kPrintAst)
    {
        TreeSyntaxHelper::OutputTreeSyntax(std::cout, root, 0);
    } else
    {
        // TODO: Write assembly
    }
    return 0;
}