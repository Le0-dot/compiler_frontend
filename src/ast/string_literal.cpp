#include "ast/string_literal.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

ast::string_literal_expression::string_literal_expression(std::string&& value)
    : _value{std::move(value)}
{}

[[nodiscard]] auto ast::string_literal_expression::accept(value_visitor* v) const -> llvm::Value* {
    fprintf(stderr, "string accept value\n");
    return v->visit(this);
}

[[nodiscard]] auto ast::string_literal_expression::accept(type_visitor* v) -> types::type* {
    fprintf(stderr, "string accept type\n");
    return v->visit(this);
}

[[nodiscard]] auto ast::string_literal_expression::value() const -> const std::string& {
    return _value;
}
