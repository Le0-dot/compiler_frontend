#include "ast/expression.hpp"

[[nodiscard]] auto ast::expression::type() const -> types::type* const & {
    return _type;
}

[[nodiscard]] auto ast::expression::type() -> types::type*& {
    return _type;
}
