//
// Created by Maxym on 12/13/2015.
//

#ifndef CCOMPILER_TREE_SYNTAX_HPP
#define CCOMPILER_TREE_SYNTAX_HPP

#include <string>
#include <memory>
#include <vector>

enum class TreeSyntaxType
{
    kImmediate,
    kVariable,
    kUnaryOperator,
    kBinaryOperator,
    kStatementsBlock,
    kIfStatement,
    kReturnStatement,
    kDefineVariable,
    kFunction,
    kFunctionCall,
    kFunctionArgument,
    kFunctionArguments,
    kAssignment,
    kWhileStatement,
    kTopLevel
};

enum class TreeUnaryExpressionType
{
    kBitwiseNegation,
    kLogicalNegation
};

enum class TreeBinaryExpressionType
{
    kAddition,
    kSubtraction,
    kMultiplication,
    kLessThan,
    kLessThanOrEqual
};

class TreeSyntax : public std::enable_shared_from_this<TreeSyntax>
{
public:
    explicit TreeSyntax(TreeSyntaxType type) : type_(type) {}
    virtual ~TreeSyntax() = default;
    TreeSyntaxType type() const noexcept { return this->type_; }
private:
    TreeSyntaxType type_;
};

using TreeSyntaxShared = std::shared_ptr<TreeSyntax>;



#endif //CCOMPILER_TREE_SYNTAX_HPP
