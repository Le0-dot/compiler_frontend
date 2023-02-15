#pragma once

#include <llvm/IR/Value.h>

#include "ast/binary.hpp"
#include "ast/call.hpp"
#include "ast/function.hpp"
#include "ast/literal.hpp"
#include "ast/variable.hpp"

namespace ast {

    class visitor {
    public:
	virtual ~visitor() = 0;
	virtual auto visit(const expression*)          -> llvm::Value* = 0;
	virtual auto visit(const literal_expression*)  -> llvm::Value* = 0;
	virtual auto visit(const variable_expression*) -> llvm::Value* = 0;
	virtual auto visit(const binary_expression*)   -> llvm::Value* = 0;
	virtual auto visit(const call_expression*)     -> llvm::Value* = 0;
	virtual auto visit(const function_expression*) -> llvm::Value* = 0;
    };

}
