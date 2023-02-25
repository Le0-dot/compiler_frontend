#pragma once

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

#include "binary.hpp"
#include "call.hpp"
#include "function.hpp"
#include "integer_literal.hpp"
#include "floating_literal.hpp"
#include "character_literal.hpp"
#include "string_literal.hpp"
#include "variable.hpp"
#include "block.hpp"
#include "implicit_cast.hpp"

namespace ast {

    class value_visitor {
    public:
	virtual ~value_visitor() = default;
	virtual auto visit(const expression*)                   -> llvm::Value* = 0;
	virtual auto visit(const integer_literal_expression*)   -> llvm::Value* = 0;
	virtual auto visit(const floating_literal_expression*)  -> llvm::Value* = 0;
	virtual auto visit(const character_literal_expression*) -> llvm::Value* = 0;
	virtual auto visit(const string_literal_expression*)    -> llvm::Value* = 0;
	virtual auto visit(const variable_expression*)          -> llvm::Value* = 0;
	virtual auto visit(const binary_expression*)            -> llvm::Value* = 0;
	virtual auto visit(const call_expression*)              -> llvm::Value* = 0;
	virtual auto visit(const function_expression*)          -> llvm::Value* = 0;
	virtual auto visit(const block_expression*)             -> llvm::Value* = 0;
	virtual auto visit(const implicit_cast*)                -> llvm::Value* = 0;
    };

    class type_visitor {
    public:
	virtual ~type_visitor() = default;
	virtual auto visit(expression*)                   -> llvm::Type* = 0;
	virtual auto visit(integer_literal_expression*)   -> llvm::Type* = 0;
	virtual auto visit(floating_literal_expression*)  -> llvm::Type* = 0;
	virtual auto visit(character_literal_expression*) -> llvm::Type* = 0;
	virtual auto visit(string_literal_expression*)    -> llvm::Type* = 0;
	virtual auto visit(variable_expression*)          -> llvm::Type* = 0;
	virtual auto visit(binary_expression*)            -> llvm::Type* = 0;
	virtual auto visit(call_expression*)              -> llvm::Type* = 0;
	virtual auto visit(function_expression*)          -> llvm::Type* = 0;
	virtual auto visit(block_expression*)             -> llvm::Type* = 0;
    };

}
