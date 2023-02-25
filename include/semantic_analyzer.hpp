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
    
    virtual auto visit(ast::expression*)                   -> llvm::Type* override;
    virtual auto visit(ast::integer_literal_expression*)   -> llvm::Type* override;
    virtual auto visit(ast::floating_literal_expression*)  -> llvm::Type* override;
    virtual auto visit(ast::character_literal_expression*) -> llvm::Type* override;
    virtual auto visit(ast::string_literal_expression*)    -> llvm::Type* override;
    virtual auto visit(ast::variable_expression*)          -> llvm::Type* override;
    virtual auto visit(ast::binary_expression*)            -> llvm::Type* override;
    virtual auto visit(ast::call_expression*)              -> llvm::Type* override;
    virtual auto visit(ast::function_expression*)          -> llvm::Type* override;
    virtual auto visit(ast::block_expression*)             -> llvm::Type* override;
};
