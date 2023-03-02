#pragma once

#include <memory>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

#include "tables.hpp"
#include "types.hpp"

class global_context {
private:
    std::unique_ptr<llvm::LLVMContext> _context;
    type_table _types{};
    function_type_table _function_types{};
    cast_table _casts{};

public:
    static auto instance() -> global_context&;

    [[nodiscard]] auto get() -> llvm::LLVMContext&;
    [[nodiscard]] static auto context() -> llvm::LLVMContext&;

    [[nodiscard]] auto get_type(const std::string&) -> types::type*;
    [[nodiscard]] static auto type(const std::string&) -> types::type*;

    [[nodiscard]] auto get_type(const std::vector<std::string>&, const std::string&) -> types::function_type*;
    [[nodiscard]] static auto type(const std::vector<std::string>&, const std::string&) -> types::function_type*;

    [[nodiscard]] auto get_cast(types::type*, types::type*) -> const std::function<cast_function>&;
    [[nodiscard]] static auto cast(types::type*, types::type*) -> const std::function<cast_function>&;

private:
    global_context();
    global_context(const global_context&) = delete;
    global_context(global_context&&)      = delete;
    auto operator=(const global_context&) = delete;
    auto operator=(global_context&&)      = delete;
    ~global_context()                     = default;

    auto add_default_types() -> void;
    auto add_default_casts() -> void;
};
