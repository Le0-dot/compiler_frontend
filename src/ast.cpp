#include "ast.hpp"
#include "ast/binary.hpp"
#include "ast/call.hpp"
#include "ast/expression.hpp"
#include "ast/function.hpp"
#include "ast/literal.hpp"
#include "ast/variable.hpp"
#include <memory>

ast::literal_expression::literal_expression(std::string&& literal, ast::literal_types type) 
    : _value{std::move(literal)}
    , _type{type}
{}

auto ast::literal_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

auto ast::literal_expression::value() const -> const std::string& {
    return _value;
}

auto ast::literal_expression::type() const -> ast::literal_types {
    return _type;
}


ast::variable_expression::variable_expression(std::string&& name) : _name{std::move(name)} {}

auto ast::variable_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

auto ast::variable_expression::name() const -> const std::string& {
    return _name;
}


ast::binary_expression::binary_expression(std::string&& op, std::unique_ptr<expression>&& lhs, std::unique_ptr<expression>&& rhs)
    : _operator{std::move(op)}
    , _lhs{std::move(lhs)}
    , _rhs{std::move(rhs)}
{}

auto ast::binary_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

auto ast::binary_expression::op() const -> const std::string& {
    return _operator;
}

auto ast::binary_expression::lhs() const -> const ast::expression* {
    return _lhs.get();
}

auto ast::binary_expression::rhs() const -> const ast::expression* {
    return _rhs.get();
}


ast::call_expression::call_expression(std::string&& callee, std::vector<std::unique_ptr<expression>>&& args)
    : _callee{std::move(callee)}
    , _args{std::move(args)}
{}

auto ast::call_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

auto ast::call_expression::callee() const -> const std::string& {
    return _callee;
}

auto ast::call_expression::args() const -> const std::vector<std::unique_ptr<expression>>& {
    return _args;
}


ast::function_expression::function_expression(std::string&& name, std::vector<std::string>&& args, std::unique_ptr<expression>&& body)
    : _name{std::move(name)}
    , _args{std::move(args)}
    , _body{std::move(body)}
{}

auto ast::function_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

auto ast::function_expression::name() const -> const std::string& {
    return _name;
}

auto ast::function_expression::args() const -> const std::vector<std::string>& {
    return _args;
}

auto ast::function_expression::body() const -> const ast::expression* {
    return _body.get();
}
