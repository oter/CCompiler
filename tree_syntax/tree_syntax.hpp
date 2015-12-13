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

class TreeImmediate : public TreeSyntax
{
public:
    explicit TreeImmediate(int value) : TreeSyntax(TreeSyntaxType::kImmediate), value_(value) {}
    virtual ~TreeImmediate() = default;
    int value() const noexcept { return this->value_; }
private:
    int value_;
};

class TreeVariable : public TreeSyntax
{
public:
    explicit TreeVariable(const std::string& name) : TreeSyntax(TreeSyntaxType::kVariable), name_(name) {}
    virtual ~TreeVariable() = default;
    const std::string name() const noexcept { return this->name_; }
private:
    std::string name_;
};

class TreeDefineVariable : public TreeSyntax
{
public:
    explicit TreeDefineVariable(const std::string& var_name, TreeSyntaxShared init_value) :
            TreeSyntax(TreeSyntaxType::kDefineVariable),
            var_name_(var_name),
            init_value_(init_value) {}
    virtual ~TreeDefineVariable() = default;
    const std::string& condition() const noexcept { return this->var_name_; }
    const TreeSyntaxShared& init_value() const noexcept { return this->init_value_; }
private:
    std::string var_name_;
    TreeSyntaxShared init_value_;
};

class TreeUnaryExpression : public TreeSyntax
{
public:
    explicit TreeUnaryExpression(TreeUnaryExpressionType expr_type, TreeSyntaxShared expr) :
            TreeSyntax(TreeSyntaxType::kUnaryOperator),
            expr_type_(expr_type),
            expression_(expr) {}
    virtual ~TreeUnaryExpression() = default;
    TreeUnaryExpressionType expr_type() const noexcept { return this->expr_type_; }
    TreeSyntaxShared expression() const noexcept { return this->expression_; }
private:
    TreeUnaryExpressionType expr_type_;
    TreeSyntaxShared expression_;
};

class TreeBinaryExpression : public TreeSyntax
{
public:
    explicit TreeBinaryExpression(TreeBinaryExpressionType expr_type, TreeSyntaxShared lhs, TreeSyntaxShared rhs) :
            TreeSyntax(TreeSyntaxType::kBinaryOperator),
            expr_type_(expr_type),
            lhs_(lhs),
            rhs_(rhs){}
    virtual ~TreeBinaryExpression() = default;
    TreeBinaryExpressionType expr_type() const noexcept { return this->expr_type_; }
    const TreeSyntaxShared& lhs() const noexcept { return this->lhs_; }
    const TreeSyntaxShared& rhs() const noexcept { return this->rhs_; }
private:
    TreeBinaryExpressionType expr_type_;
    TreeSyntaxShared lhs_;
    TreeSyntaxShared rhs_;
};

class TreeFunctionArguments : public TreeSyntax
{
public:
    explicit TreeFunctionArguments(const std::vector<TreeSyntaxShared>& args) :
            TreeSyntax(TreeSyntaxType::kFunctionArguments),
            args_(args) {}
    virtual ~TreeFunctionArguments() = default;
    const std::vector<TreeSyntaxShared>& args() const noexcept { return this->args_; }
private:
    std::vector<TreeSyntaxShared> args_;
};

class TreeFunctionCall : public TreeSyntax
{
public:
    explicit TreeFunctionCall(const std::string&name, TreeSyntaxShared args) :
            TreeSyntax(TreeSyntaxType::kFunctionCall),
            args_(args),
            name_(name){}
    virtual ~TreeFunctionCall() = default;
    const TreeSyntaxShared& args() const noexcept { return this->args_; }
    const std::string& name() const noexcept { return this->name_; }
private:
    TreeSyntaxShared args_;
    std::string name_;
};

class TreeAssignment : public TreeSyntax
{
public:
    explicit TreeAssignment(const std::string& var_name, TreeSyntaxShared expr) :
            TreeSyntax(TreeSyntaxType::kAssignment),
            var_name_(var_name),
            expr_(expr) {}
    virtual ~TreeAssignment() = default;
    const std::string& var_name() const noexcept { return this->var_name_; }
    const TreeSyntaxShared& expr() const noexcept { return this->expr_; }
private:
    std::string var_name_;
    TreeSyntaxShared expr_;
};

class TreeIfStatement : public TreeSyntax
{
public:
    explicit TreeIfStatement(TreeSyntaxShared condition, TreeSyntaxShared then) :
            TreeSyntax(TreeSyntaxType::kIfStatement),
            condition_(condition),
            then_(then) {}
    virtual ~TreeIfStatement() = default;
    const TreeSyntaxShared& condition() const noexcept { return this->condition_; }
    const TreeSyntaxShared& then() const noexcept { return this->then_; }
private:
    TreeSyntaxShared condition_;
    TreeSyntaxShared then_;
};

class TreeWhileStatement : public TreeSyntax
{
public:
    explicit TreeWhileStatement(TreeSyntaxShared condition, TreeSyntaxShared body) :
            TreeSyntax(TreeSyntaxType::kWhileStatement),
            condition_(condition),
            body_(body) {}
    virtual ~TreeWhileStatement() = default;
    const TreeSyntaxShared& condition() const noexcept { return this->condition_; }
    const TreeSyntaxShared& body() const noexcept { return this->body_; }
private:
    TreeSyntaxShared condition_;
    TreeSyntaxShared body_;
};

class TreeReturnStatement : public TreeSyntax
{
public:
    explicit TreeReturnStatement(TreeSyntaxShared expr) :
            TreeSyntax(TreeSyntaxType::kReturnStatement),
            expr_(expr) {}
    virtual ~TreeReturnStatement() = default;
    const TreeSyntaxShared& expression() const noexcept { return this->expr_; }
private:
    TreeSyntaxShared expr_;
};

class TreeStatementsBlock : public TreeSyntax
{
public:
    explicit TreeStatementsBlock(const std::vector<TreeSyntaxShared>& stmts) :
            TreeSyntax(TreeSyntaxType::kStatementsBlock),
            stmts_(stmts) {}
    virtual ~TreeStatementsBlock() = default;
    const std::vector<TreeSyntaxShared>& stmts() const noexcept { return this->stmts_; }
private:
    std::vector<TreeSyntaxShared> stmts_;
};


class TreeFunction : public TreeSyntax
{
public:
    explicit TreeFunction(const std::string& name, const std::vector<TreeSyntaxShared>& args, TreeSyntaxShared root) :
            TreeSyntax(TreeSyntaxType::kFunction),
            name_(name),
            args_(args) {}
    virtual ~TreeFunction() = default;
    const std::string& name() const noexcept { return this->name_; }
    const std::vector<TreeSyntaxShared>& args() const noexcept { return this->args_; }
    const TreeSyntaxShared& root() const noexcept { return this->root_; }
private:
    std::string name_;
    std::vector<TreeSyntaxShared> args_;
    TreeSyntaxShared root_;
};


// TODO: Function argument type
class TreeFunctionArgument : public TreeSyntax
{
public:
    explicit TreeFunctionArgument(const std::string& name) :
            TreeSyntax(TreeSyntaxType::kFunctionArgument),
            name_(name) {}
    virtual ~TreeFunctionArgument() = default;
    const std::string& name() const noexcept { return this->name_; }
private:
    const std::string name_;
};

// TODO: Rename
class TreeTopLevel : public TreeSyntax
{
public:
    explicit TreeTopLevel(const std::vector<TreeSyntaxShared>& decls) :
            TreeSyntax(TreeSyntaxType::kTopLevel),
            decls_(decls) {}
    virtual ~TreeTopLevel() = default;
    const std::vector<TreeSyntaxShared>& decls() const noexcept { return this->decls_; }
private:
    std::vector<TreeSyntaxShared> decls_;
};



#endif //CCOMPILER_TREE_SYNTAX_HPP
