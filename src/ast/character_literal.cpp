#include "ast/character_literal.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

ast::character_literal_expression::character_literal_expression(std::string&& value)
    : _value{value[0]}
{}

[[nodiscard]] auto ast::character_literal_expression::accept(value_visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::character_literal_expression::accept(type_visitor* v) -> llvm::Type* {
    return v->visit(this);
}

[[nodiscard]] auto ast::character_literal_expression::value() const -> char {
    return _value;
}


