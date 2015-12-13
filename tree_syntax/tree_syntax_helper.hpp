//
// Created by Maxym on 12/13/2015.
//

#ifndef CCOMPILER_TREE_SYNTAX_HELPER_HPP
#define CCOMPILER_TREE_SYNTAX_HELPER_HPP

#include <map>
#include <ostream>

#include "tree_syntax.hpp"

class TreeSyntaxHelper
{
    enum
    {
        kDefaultIndent = 4
    };
public:

    static const std::string& GetSyntaxTypeName(TreeSyntaxShared node);
private:
    const static std::map<TreeSyntaxType, std::string> tree_syntax_types_;
    const static std::map<TreeUnaryExpressionType, std::string> tree_unary_types_;
    const static std::map<TreeBinaryExpressionType, std::string> tree_binary_types_;
};

#endif //CCOMPILER_TREE_SYNTAX_HELPER_HPP
