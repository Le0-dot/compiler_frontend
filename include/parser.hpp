#pragma once

#include <memory>

#include "ast.hpp"
#include "ast/expression.hpp"

namespace parser {

    auto parse_literal()                                                -> std::unique_ptr<ast::expression>;
    auto parse_parenthesis()                                            -> std::unique_ptr<ast::expression>;
    auto parse_indentifier()                                            -> std::unique_ptr<ast::expression>;
    auto parse_primary()                                                -> std::unique_ptr<ast::expression>;
    auto parse_expression()                                             -> std::unique_ptr<ast::expression>;
    auto parse_binary_rhs(uint16_t, std::unique_ptr<ast::expression>&&) -> std::unique_ptr<ast::expression>;
    auto parse_function_definition()                                    -> std::unique_ptr<ast::expression>;

}
