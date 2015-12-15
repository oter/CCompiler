#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include "tree_syntax/tree_syntax.hpp"
#include "tree_syntax/tree_syntax_helper.hpp"
#include "AstProxy.hpp"
#include "asm/asm_generator.hpp"
//#include "assembly.hpp"

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

    auto ast = std::make_shared<AstProxy>();
    int result = ast->ProcessFile(file_to_compile);
    if (result != 0) { return result; }
    auto root = ast->root();

    if (operation == Operation::kPrintAst)
    {
        TreeSyntaxHelper::OutputTreeSyntax(std::cout, root, 0);
    } else
    {
        TreeSyntaxHelper::OutputTreeSyntax(std::cout, root, 0);
        const std::string file_name = ast->file_base_name() + ".asm_";
        std::ofstream out(file_name);

        if (!out.is_open())
        {
            std::cerr << "Can't access output file " << file_name << std::endl;
            return -10;
        }

        AsmGenerator asm_generator(out, ast->root());
        return asm_generator.status();
    }
    return 0;
}