#pragma once

#include <forward_list>

#include "tables.hpp"

class scope {
private:
    symbol_table _symbols{};
    function_symbol_table _functions{};

public:
    auto new_symbol(const std::string&, llvm::Type*) -> void;
    auto new_function(const std::string&, llvm::FunctionType*) -> void;

    [[nodiscard]] auto search_symbol(const std::string&) const -> llvm::Type*;
    [[nodiscard]] auto search_function(const std::string&) const -> llvm::FunctionType*;

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

    auto new_symbol(const std::string&, llvm::Type*) -> void;
    auto new_function(const std::string&, llvm::FunctionType*) -> void;

    [[nodiscard]] auto search_symbol(const std::string&) const -> llvm::Type*;
    [[nodiscard]] auto search_function(const std::string&) const -> llvm::FunctionType*;
};
