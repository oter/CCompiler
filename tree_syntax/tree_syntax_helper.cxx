//
// Created by Maxym on 12/13/2015.
//

#include "tree_syntax_helper.hpp"

decltype(TreeSyntaxHelper::tree_syntax_types_) TreeSyntaxHelper::tree_syntax_types_ =
        {
                { TreeSyntaxType::kImmediate,           "IMMEDIATE"},
                { TreeSyntaxType::kVariable,            "VARIABLE"},
                { TreeSyntaxType::kUnaryOperator,       "UNARY_OPERATOR"},
                { TreeSyntaxType::kBinaryOperator,      "BINARY_OPERATOR"},
                { TreeSyntaxType::kStatementsBlock,     "STATEMENTS_BLOCK"},
                { TreeSyntaxType::kIfStatement,         "IF_STATEMENT"},
                { TreeSyntaxType::kReturnStatement,     "RETURN_STATEMENT"},
                { TreeSyntaxType::kDefineVariable,      "DEFINE_VARIABLE"},
                { TreeSyntaxType::kFunction,            "FUNCTION"},
                { TreeSyntaxType::kFunctionCall,        "FUNCTION_CALL"},
                { TreeSyntaxType::kFunctionArgument,    "FUNCTION_ARGUMENT"},
                { TreeSyntaxType::kFunctionArguments,   "FUNCTION_ARGUMENTS"},
                { TreeSyntaxType::kAssignment,          "ASSIGNMENT"},
                { TreeSyntaxType::kWhileStatement,      "WHILE_STATEMENT"},
                { TreeSyntaxType::kTopLevel,            "TOP_LEVEL"},
        };

decltype(TreeSyntaxHelper::tree_unary_types_) TreeSyntaxHelper::tree_unary_types_ =
        {
                { TreeUnaryExpressionType::kBitwiseNegation, "UNARY_BITWISE_NEGATION" },
                { TreeUnaryExpressionType::kLogicalNegation, "UNARY_LOGICAL_NEGATION" }
        };

decltype(TreeSyntaxHelper::tree_binary_types_) TreeSyntaxHelper::tree_binary_types_ =
        {
                { TreeBinaryExpressionType::kAddition,          "BINARY_ADDITION" },
                { TreeBinaryExpressionType::kSubtraction,       "BINARY_SUBTRACTION" },
                { TreeBinaryExpressionType::kMultiplication,    "BINARY_MULTIPLICATION" },
                { TreeBinaryExpressionType::kLessThan,          "LESS_THAN" },
                { TreeBinaryExpressionType::kLessThanOrEqual,   "LESS_THAN_OR_EQUAL" },
        };

const std::string &TreeSyntaxHelper::GetSyntaxTypeName(TreeSyntaxShared node)
{
    if (node->type() == TreeSyntaxType::kUnaryOperator)
    {
        return tree_unary_types_.at(std::static_pointer_cast<TreeUnaryExpression>(node)->expr_type());
    } else if (node->type() == TreeSyntaxType::kBinaryOperator)
    {
        return tree_binary_types_.at(std::static_pointer_cast<TreeBinaryExpression>(node)->expr_type());
    } else
    {
       return tree_syntax_types_.at(node->type());
    }
}
