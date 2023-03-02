#pragma once

#include <concepts>
#include <memory>

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

#include "types.hpp"

namespace ast {

    class value_visitor;
    class type_visitor;

    class expression {
    private:
	types::type* _type{};

    public:
	virtual ~expression() = default;

	[[nodiscard]] auto type()       -> types::type*&;
	[[nodiscard]] auto type() const -> types::type* const &;
	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* = 0;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* = 0;
    };

}
