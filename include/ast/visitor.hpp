#pragma once

#include <llvm/IR/Value.h>

#include "binary.hpp"
#include "call.hpp"
#include "function.hpp"
#include "literal.hpp"
#include "variable.hpp"
#include "block.hpp"

namespace ast {

    class visitor {
    public:
	virtual ~visitor() = default;
	virtual auto visit(const expression*)          -> llvm::Value* = 0;
	virtual auto visit(const literal_expression*)  -> llvm::Value* = 0;
	virtual auto visit(const variable_expression*) -> llvm::Value* = 0;
	virtual auto visit(const binary_expression*)   -> llvm::Value* = 0;
	virtual auto visit(const call_expression*)     -> llvm::Value* = 0;
	virtual auto visit(const function_expression*) -> llvm::Value* = 0;
	virtual auto visit(const block_expression*)    -> llvm::Value* = 0;
    };

}
