//
// Created by Maxym on 12/13/2015.
//

#include "AstProxy.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "tree_syntax/tree_syntax_helper.hpp"
#include "utils/CommentsCleaner.hpp"


// Interface with flex-bison parser and lexer
extern std::stack<TreeSyntaxShared> syntax_stack;
extern int yyparse(void);
extern FILE *yyin;

std::string AstProxy::RemoveFilenameExtension(const std::string& file_name)
{
    auto found = file_name.find_last_of('.');
    return file_name.substr(0, found);
}

int AstProxy::ProcessFile(const std::string &file_name)
{
    // Clean comments
    std::ifstream file_with(file_name); // File with comments
    file_with.seekg(0, std::ifstream::end);
    auto with_size = file_with.tellg();
    file_with.seekg(std::ifstream::beg);

    std::string src_with;
    src_with.reserve(static_cast<unsigned long long int>(with_size));
    src_with.assign((std::istreambuf_iterator<char>(file_with)), std::istreambuf_iterator<char>());
    file_with.close();

    CommentsCleaner cleaner(src_with);
    auto src_without = cleaner.ToString();

    std::string file_path_base = RemoveFilenameExtension(file_name);
    std::string file_path_cleaned = file_path_base + ".cleaned";

    std::ofstream o_file_without(file_path_cleaned);
    if (o_file_without.is_open())
    {
        o_file_without << src_without;
    } else
    {
        std::cerr << "Cannot open file for writing: " << file_path_cleaned << std::endl;
        return -3;
    }
    o_file_without.close();

    yyin = nullptr;
    yyin = fopen(file_path_cleaned.c_str(), "r");
    if (yyin == nullptr) {
        std::cerr << "Could not open the file: " << file_path_cleaned << std::endl;
        fclose(yyin);
        yyin = nullptr;
        return -2;
    }

    // Empty the working stack
    while(!syntax_stack.empty()) { syntax_stack.pop(); }
    // Parse
    int parse_result = yyparse();
    if (parse_result != 0) {
        std::cerr << "Error while processing file: " << file_path_cleaned << std::endl;
        return parse_result;
    }

    if (syntax_stack.size() == 0)
    {
        std::cout << "Internal error" << std::endl;
        return -5;
    }

        this->root_ = syntax_stack.top();
        syntax_stack.pop();
        if (syntax_stack.size() > 0) {
            std::cerr << "WARNING: Remaining objects are left in the stack after parsing:\n";
            while(syntax_stack.size() > 0) {
                std::cerr << "====TREE_ROOT====\n\t";
                std::cerr << TreeSyntaxHelper::GetSyntaxTypeName(syntax_stack.top()) << std::endl;
                syntax_stack.pop();
            }
        }

    fclose(yyin);
    yyin = nullptr;

    return 0;
}
