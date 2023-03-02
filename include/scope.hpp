#pragma once

#include <forward_list>

#include "tables.hpp"
#include "types.hpp"

class scope {
private:
    symbol_table _symbols{};
    function_symbol_table _functions{};

public:
    auto new_symbol(const std::string&, types::type*) -> void;
    auto new_function(const std::string&, types::function_type*) -> void;

    [[nodiscard]] auto search_symbol(const std::string&) -> types::type*;
    [[nodiscard]] auto search_function(const std::string&) -> types::function_type*;

    scope()                      = default;
    scope(const scope&)          = delete;
    scope(scope&&)               = delete;
    auto operator=(const scope&) = delete;
    auto operator=(scope&)       = delete;
    ~scope()                     = default;
};

class scope_manager {
private:
    std::forward_list<scope> _scopes{};

public:
    scope_manager();
    scope_manager(const scope_manager&)  = delete;
    scope_manager(scope_manager&&)       = delete;
    auto operator=(const scope_manager&) = delete;
    auto operator=(scope_manager&)       = delete;
    ~scope_manager()                     = default;

    auto new_scope() -> scope&;
    auto delete_scope() -> void;

    auto new_symbol(const std::string&, types::type*) -> void;
    auto new_function(const std::string&, types::function_type*) -> void;

    [[nodiscard]] auto search_symbol(const std::string&) -> types::type*;
    [[nodiscard]] auto search_function(const std::string&) -> types::function_type*;
};
