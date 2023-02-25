#include "ast/expression.hpp"

[[nodiscard]] auto ast::expression::type() const -> llvm::Type* {
    return _type;
}

[[nodiscard]] auto ast::expression::type() -> llvm::Type*& {
    return _type;
}
