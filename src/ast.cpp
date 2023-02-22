#include "ast.hpp"
#include "global_context.hpp"
#include <llvm/IR/DerivedTypes.h>
#include <stdexcept>

ast::integer_literal_expression::integer_literal_expression(std::string&& value, uint8_t base)
    : _value{std::stoi(std::move(value), {}, base)}
{}

[[nodiscard]] auto ast::integer_literal_expression::type() const -> llvm::Type* {
    return global_context::type("int");
}

[[nodiscard]] auto ast::integer_literal_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::integer_literal_expression::value() const -> int {
    return _value;
}


ast::floating_literal_expression::floating_literal_expression(std::string&& value)
    : _value{std::stod(std::move(value))}
{}

[[nodiscard]] auto ast::floating_literal_expression::type() const -> llvm::Type* {
    return global_context::type("double");
}

[[nodiscard]] auto ast::floating_literal_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::floating_literal_expression::value() const -> double {
    return _value;
}


ast::character_literal_expression::character_literal_expression(std::string&& value)
    : _value{value[0]}
{}

[[nodiscard]] auto ast::character_literal_expression::type() const -> llvm::Type* {
    return global_context::type("char");
}

[[nodiscard]] auto ast::character_literal_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::character_literal_expression::value() const -> char {
    return _value;
}


ast::string_literal_expression::string_literal_expression(std::string&& value)
    : _value{std::move(value)}
{}

[[nodiscard]] auto ast::string_literal_expression::type() const -> llvm::Type* {
    return global_context::type("string");
}

[[nodiscard]] auto ast::string_literal_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::string_literal_expression::value() const -> const std::string& {
    return _value;
}


ast::variable_expression::variable_expression(std::string&& name) : _name{std::move(name)} {}

[[nodiscard]] auto ast::variable_expression::type() const -> llvm::Type* {

}

[[nodiscard]] auto ast::variable_expression::accept(visitor* v) const -> llvm::Value* {
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

[[nodiscard]] auto ast::binary_expression::type() const -> llvm::Type* {

}

[[nodiscard]] auto ast::binary_expression::accept(visitor* v) const -> llvm::Value* {
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

[[nodiscard]] auto ast::call_expression::type() const -> llvm::Type* {

}

[[nodiscard]] auto ast::call_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::call_expression::callee() const -> const std::string& {
    return _callee;
}

[[nodiscard]] auto ast::call_expression::args() const -> const std::vector<std::unique_ptr<expression>>& {
    return _args;
}


ast::function_expression::function_expression(std::string&& name, std::vector<std::string>&& args,
	std::vector<llvm::Type*>&& type_list, llvm::Type* return_type, std::unique_ptr<expression>&& body)
    : _name{std::move(name)}
    , _args{std::move(args)}
    , _type{llvm::FunctionType::get(return_type, type_list, false)}
    , _body{std::move(body)}
{}

[[nodiscard]] auto ast::function_expression::type() const -> llvm::Type* {
    return _type;
}

[[nodiscard]] auto ast::function_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::function_expression::name() const -> const std::string& {
    return _name;
}

[[nodiscard]] auto ast::function_expression::args() const -> const std::vector<std::string>& {
    return _args;
}

[[nodiscard]] auto ast::function_expression::body() const -> const ast::expression* {
    return _body.get();
}


ast::block_expression::block_expression(std::vector<std::unique_ptr<ast::expression>>&& expressions)
    : _expressions{std::move(expressions)}
{}

[[nodiscard]] auto ast::block_expression::type() const -> llvm::Type* {
    if(_expressions.empty())
	return global_context::type("");
    return _expressions.back()->type(); // May be not a return expression so should change it to void
}

[[nodiscard]] auto ast::block_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::block_expression::expressions() const -> const std::vector<std::unique_ptr<ast::expression>>& {
    return _expressions;
}
