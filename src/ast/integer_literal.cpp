#include "ast/integer_literal.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

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
