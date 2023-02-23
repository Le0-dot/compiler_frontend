#include "ast/block.hpp"
#include "ast/visitor.hpp"
#include "global_context.hpp"

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
