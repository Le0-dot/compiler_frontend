#include "ast/binary.hpp"
#include "ast/expression.hpp"
#include "ast/visitor.hpp"

ast::binary_expression::binary_expression(std::string&& op, std::unique_ptr<expression>&& lhs, std::unique_ptr<expression>&& rhs)
    : _operator{std::move(op)}
    , _lhs{std::move(lhs)}
    , _rhs{std::move(rhs)}
{}

[[nodiscard]] auto ast::binary_expression::accept(value_visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::binary_expression::accept(type_visitor* v) -> llvm::Type* {
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

[[nodiscard]] auto ast::binary_expression::lhs() -> ast::expression* {
    return _lhs.get();
}

[[nodiscard]] auto ast::binary_expression::rhs() -> ast::expression* {
    return _rhs.get();
}
