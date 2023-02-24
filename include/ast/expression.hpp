#pragma once

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

namespace ast {

    class value_visitor;
    class type_visitor;

    class expression {
    public:
	virtual ~expression() = default;

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* = 0;
	[[nodiscard]] virtual auto accept(type_visitor*) const -> llvm::Type* = 0;
    };

}
