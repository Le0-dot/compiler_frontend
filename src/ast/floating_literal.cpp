#include "ast/floating_literal.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

ast::floating_literal_expression::floating_literal_expression(std::string&& value)
    : _value{std::stod(std::move(value))}
{}

[[nodiscard]] auto ast::floating_literal_expression::accept(value_visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::floating_literal_expression::accept(type_visitor* v) const -> llvm::Type* {
    return v->visit(this);
}

[[nodiscard]] auto ast::floating_literal_expression::value() const -> double {
    return _value;
}


