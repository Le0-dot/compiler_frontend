#include "ast/string_literal.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

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
