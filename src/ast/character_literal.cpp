#include "ast/character_literal.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

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


