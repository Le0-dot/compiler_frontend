#pragma once

#include <concepts>
#include <memory>

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

namespace ast {

    class value_visitor;
    class type_visitor;

    class expression {
    private:
	llvm::Type* _type{};

    public:
	virtual ~expression() = default;

	[[nodiscard]] auto type() const -> llvm::Type*;
	[[nodiscard]] auto type() -> llvm::Type*&;
	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* = 0;
	[[nodiscard]] virtual auto accept(type_visitor*) -> llvm::Type* = 0;
    };

}
