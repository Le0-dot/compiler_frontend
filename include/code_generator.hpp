#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#include "ast.hpp"

class code_generator : public ast::value_visitor {
private:
    std::unique_ptr<llvm::LLVMContext> _context;
    std::unique_ptr<llvm::Module> _module;
    std::unique_ptr<llvm::IRBuilder<>> _builder;
    std::unordered_map<std::string, llvm::Value*> _named_values{};

public:
    code_generator(const std::string&);

    code_generator()                      = delete;
    code_generator(const code_generator&) = delete;
    code_generator(code_generator&&)      = delete;
    auto operator=(const code_generator&) = delete;
    auto operator=(code_generator&&)      = delete;
    ~code_generator()                     = default;

    virtual auto visit(const ast::expression*)                   -> llvm::Value* override;
    virtual auto visit(const ast::integer_literal_expression*)   -> llvm::Value* override;
    virtual auto visit(const ast::floating_literal_expression*)  -> llvm::Value* override;
    virtual auto visit(const ast::character_literal_expression*) -> llvm::Value* override;
    virtual auto visit(const ast::string_literal_expression*)    -> llvm::Value* override;
    virtual auto visit(const ast::variable_expression*)          -> llvm::Value* override;
    virtual auto visit(const ast::binary_expression*)            -> llvm::Value* override;
    virtual auto visit(const ast::call_expression*)              -> llvm::Value* override;
    virtual auto visit(const ast::function_expression*)          -> llvm::Value* override;
    virtual auto visit(const ast::block_expression*)             -> llvm::Value* override;
    virtual auto visit(const ast::implicit_cast*)                -> llvm::Value* override;
};
