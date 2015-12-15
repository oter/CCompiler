//
// Created by Maxym on 12/13/2015.
//

#include <assert.h>
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
                { TreeSyntaxType::kArrayIndexer,        "ARRAY_INDEXER"},
                { TreeSyntaxType::kEmptyStatement,      "EMPTY_STATEMENT"},
                { TreeSyntaxType::kArrayAssignment,     "ARRAY_ELEMENT_ASSIGN"},
                { TreeSyntaxType::kTreeForStatement,    "FOR_STATEMENT"},
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
                { TreeBinaryExpressionType::kDivision,          "BINARY_DIVISION" },
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

void TreeSyntaxHelper::OutputTreeSyntax(std::ostream &out, TreeSyntaxShared root, size_t indent)
{
    const std::string indent_str(indent, ' ');
    out << indent_str;

    auto& type_str = GetSyntaxTypeName(root);

    switch (root->type())
    {
        case TreeSyntaxType::kArrayAssignment:
            out << type_str << " ARRAY_VAR\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeArrayAssignment>(root)->var(), indent + kDefaultIndent);
            out << indent_str << type_str << " ASSIGN\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeArrayAssignment>(root)->expr(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kArrayIndexer:
            out << type_str << " \"" << std::static_pointer_cast<TreeArrayIndexer>(root)->var_name() << "\"[->]\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeArrayIndexer>(root)->expression(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kImmediate:
            out << type_str << " \"" << std::static_pointer_cast<TreeImmediate>(root)->value() << "\"\n";
            break;
        case TreeSyntaxType::kVariable:
            out << type_str << " \"" << std::static_pointer_cast<TreeVariable>(root)->name() << "\"\n";
            break;
        case TreeSyntaxType::kUnaryOperator:
            out << type_str << "\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeUnaryExpression>(root)->expression(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kBinaryOperator:
            out << type_str << " LEFT\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeBinaryExpression>(root)->lhs(), indent + kDefaultIndent);
            out << indent_str << type_str << " RIGHT\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeBinaryExpression>(root)->rhs(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kStatementsBlock:
        {
            out << type_str << "\n";
            auto block = std::static_pointer_cast<TreeStatementsBlock>(root);
            for (auto stmt : block->stmts())
            {
                OutputTreeSyntax(out, stmt, indent + kDefaultIndent);
            }
            break;
        }
        case TreeSyntaxType::kIfStatement:
            out << type_str << " CONDITION\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeIfStatement>(root)->condition(), indent + kDefaultIndent);
            out << indent_str << type_str << " THEN\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeIfStatement>(root)->then(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kReturnStatement:
            out << type_str << "\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeReturnStatement>(root)->expression(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kDefineVariable:
        {
            auto def = std::static_pointer_cast<TreeDefineVariable>(root);
            out << type_str << " \"" << def->name() << "\"\n";
            if (def->init_value()->type() != TreeSyntaxType::kEmptyStatement)
            {
                out << indent_str << " \"" << def->name() << "\" INITIAL_VALUE\n";
                OutputTreeSyntax(out, std::static_pointer_cast<TreeDefineVariable>(root)->init_value(), indent + kDefaultIndent);
            } else
            {
                // Handle uninitialized variable
            }

            break;
        }
        case TreeSyntaxType::kFunction:
            out << type_str << " \"" << std::static_pointer_cast<TreeFunction>(root)->name() << "\"\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeFunction>(root)->args(), indent + kDefaultIndent);
            OutputTreeSyntax(out, std::static_pointer_cast<TreeFunction>(root)->compound(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kFunctionCall:
        {
            auto func = std::static_pointer_cast<TreeFunctionCall>(root);
            out << type_str << " \"" << func->name() << "()\"\n";
            OutputTreeSyntax(out, func->args(), indent + kDefaultIndent);
            break;
        }
        case TreeSyntaxType::kFunctionArgument:
        {
            auto args = std::static_pointer_cast<TreeFunctionArgument>(root);
            out << type_str << " \"" << args->name() << "\"\n";

            break;
        }
        case TreeSyntaxType::kFunctionArguments:
        {
            out << type_str << "\n";
            const auto &args = std::static_pointer_cast<TreeFunctionArguments>(root);
            for (auto& arg : args->args())
            {
                out << indent_str << " ARGUMENT\n";
                OutputTreeSyntax(out, arg, indent + kDefaultIndent);
            }
            break;
        }
        case TreeSyntaxType::kAssignment:
            out << type_str << " \"" << std::static_pointer_cast<TreeAssignment>(root)->var_name() << "\"\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeAssignment>(root)->expr(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kWhileStatement:
            out << type_str << " CONDITION\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeWhileStatement>(root)->condition(), indent + kDefaultIndent);
            out  << indent_str << type_str << " BODY\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeWhileStatement>(root)->body(), indent + kDefaultIndent);
            break;
        case TreeSyntaxType::kTopLevel:
            out << type_str << "\n";
            for (auto declaration : std::static_pointer_cast<TreeTopLevel>(root)->decls())
            {
                OutputTreeSyntax(out, declaration, indent + kDefaultIndent);
            }
            break;
        case TreeSyntaxType::kEmptyStatement:
            out << type_str << "\n";
            break;
        case TreeSyntaxType::kTreeForStatement:
        {
            out << type_str << " INITIAL\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeForStatement>(root)->assign(), indent + kDefaultIndent);
            out  << indent_str << type_str << " CONDITION\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeForStatement>(root)->condition(), indent + kDefaultIndent);
            out  << indent_str << type_str << " INCREMENT\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeForStatement>(root)->inc(), indent + kDefaultIndent);
            out  << indent_str << type_str << " BODY\n";
            OutputTreeSyntax(out, std::static_pointer_cast<TreeForStatement>(root)->body(), indent + kDefaultIndent);

            break;
        }
    }
}
