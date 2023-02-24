#pragma once

#include "ast/visitor.hpp"
#include "scope.hpp"

class semantic_analyzer : public ast::type_visitor {
private:
    scope_manager _sm;

public:
    semantic_analyzer()                         = default;
    semantic_analyzer(const semantic_analyzer&) = delete;
    semantic_analyzer(semantic_analyzer&&)      = delete;
    auto operator=(const semantic_analyzer&)    = delete;
    auto operator=(semantic_analyzer&&)         = delete;
    ~semantic_analyzer()                        = default;
    
    virtual auto visit(const ast::expression*)                   -> llvm::Type* override;
    virtual auto visit(const ast::integer_literal_expression*)   -> llvm::Type* override;
    virtual auto visit(const ast::floating_literal_expression*)  -> llvm::Type* override;
    virtual auto visit(const ast::character_literal_expression*) -> llvm::Type* override;
    virtual auto visit(const ast::string_literal_expression*)    -> llvm::Type* override;
    virtual auto visit(const ast::variable_expression*)          -> llvm::Type* override;
    virtual auto visit(const ast::binary_expression*)            -> llvm::Type* override;
    virtual auto visit(const ast::call_expression*)              -> llvm::Type* override;
    virtual auto visit(const ast::function_expression*)          -> llvm::Type* override;
    virtual auto visit(const ast::block_expression*)             -> llvm::Type* override;
};
