#pragma once

#include <memory>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

#include "generic_table.hpp"

class global_context {
private:
    std::unique_ptr<llvm::LLVMContext> _context;
    type_table _types{};

public:
    static auto instance() -> global_context&;

    [[nodiscard]] auto get() -> llvm::LLVMContext&;
    [[nodiscard]] static auto context() -> llvm::LLVMContext&;

    [[nodiscard]] auto get_type(const std::string&)    -> llvm::Type*;
    [[nodiscard]] static auto type(const std::string&) -> llvm::Type*;

private:
    global_context();
    global_context(const global_context&) = delete;
    global_context(global_context&&)      = delete;
    auto operator=(const global_context&) = delete;
    auto operator=(global_context&&)      = delete;
    ~global_context()                     = default;

    auto add_default_types() -> void;
};