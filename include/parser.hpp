#pragma once

#include <memory>

#include "ast.hpp"
#include "lexer.hpp"
#include "generic_table.hpp"

class parser {
private:
    lexer _lexer;
    operator_table _table;

public:
    parser(lexer&&, operator_table&&);

    parser()                      = delete;
    parser(const parser&)         = delete;
    parser(parser&&)              = delete;
    auto operator=(const parser&) = delete;
    auto operator=(parser&&)      = delete;
    ~parser()                     = default;

    [[nodiscard]] auto parse_literal()                                               -> std::unique_ptr<ast::expression>;
    [[nodiscard]] auto parse_parenthesis()                                           -> std::unique_ptr<ast::expression>;
    [[nodiscard]] auto parse_indentifier()                                           -> std::unique_ptr<ast::expression>;
    [[nodiscard]] auto parse_primary()                                               -> std::unique_ptr<ast::expression>;
    [[nodiscard]] auto parse_expression()                                            -> std::unique_ptr<ast::expression>;
    [[nodiscard]] auto parse_binary_rhs(uint8_t, std::unique_ptr<ast::expression>&&) -> std::unique_ptr<ast::expression>;
    [[nodiscard]] auto parse_function()                                              -> std::unique_ptr<ast::expression>;
    [[nodiscard]] auto parse_block()                                                 -> std::unique_ptr<ast::expression>;
};
