#include "ast/block.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

ast::block_expression::block_expression(std::vector<std::unique_ptr<ast::expression>>&& expressions)
    : _expressions{std::move(expressions)}
{}

[[nodiscard]] auto ast::block_expression::accept(value_visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::block_expression::accept(type_visitor* v) const -> llvm::Type* {
    return v->visit(this);
}

[[nodiscard]] auto ast::block_expression::expressions() const -> const std::vector<std::unique_ptr<ast::expression>>& {
    return _expressions;
}
