#include "ast.hpp"

ast::literal_expression::literal_expression(std::string&& literal, ast::literal_types type) 
    : _value{std::move(literal)}
    , _type{type}
{}

auto ast::literal_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::literal_expression::value() const -> const std::string& {
    return _value;
}

[[nodiscard]] auto ast::literal_expression::type() const -> ast::literal_types {
    return _type;
}


ast::variable_expression::variable_expression(std::string&& name) : _name{std::move(name)} {}

auto ast::variable_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::variable_expression::name() const -> const std::string& {
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

[[nodiscard]] auto ast::binary_expression::op() const -> const std::string& {
    return _operator;
}

[[nodiscard]] auto ast::binary_expression::lhs() const -> const ast::expression* {
    return _lhs.get();
}

[[nodiscard]] auto ast::binary_expression::rhs() const -> const ast::expression* {
    return _rhs.get();
}


ast::call_expression::call_expression(std::string&& callee, std::vector<std::unique_ptr<expression>>&& args)
    : _callee{std::move(callee)}
    , _args{std::move(args)}
{}

auto ast::call_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::call_expression::callee() const -> const std::string& {
    return _callee;
}

[[nodiscard]] auto ast::call_expression::args() const -> const std::vector<std::unique_ptr<expression>>& {
    return _args;
}


ast::function_expression::function_expression(std::string&& name, std::string&& return_type
	, std::vector<std::pair<std::string, std::string>>&& args, std::unique_ptr<expression>&& body)
    : _name{std::move(name)}
    , _return_type{std::move(return_type)}
    , _args{std::move(args)}
    , _body{std::move(body)}
{}

auto ast::function_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::function_expression::name() const -> const std::string& {
    return _name;
}

[[nodiscard]] auto ast::function_expression::return_type() const -> const std::string& {
    return _return_type;
}

[[nodiscard]] auto ast::function_expression::args() const -> const std::vector<std::pair<std::string, std::string>>& {
    return _args;
}

[[nodiscard]] auto ast::function_expression::body() const -> const ast::expression* {
    return _body.get();
}


ast::block_expression::block_expression(std::vector<std::unique_ptr<ast::expression>>&& expressions)
    : _expressions{std::move(expressions)}
{}

auto ast::block_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::block_expression::expressions() const -> const std::vector<std::unique_ptr<ast::expression>>& {
    return _expressions;
}
