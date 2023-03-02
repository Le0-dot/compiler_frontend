#include "ast/variable.hpp"
#include "ast/visitor.hpp"

ast::variable_expression::variable_expression(std::string&& name) 
    : _name{std::move(name)} 
{}

[[nodiscard]] auto ast::variable_expression::accept(value_visitor* v) const -> llvm::Value* {
    fprintf(stderr, "var accept value\n");
    return v->visit(this);
}

[[nodiscard]] auto ast::variable_expression::accept(type_visitor* v) -> types::type* {
    fprintf(stderr, "var accept type\n");
    return v->visit(this);
}

[[nodiscard]] auto ast::variable_expression::name() const -> const std::string& {
    return _name;
}

