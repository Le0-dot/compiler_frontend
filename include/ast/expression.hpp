#pragma once

#include <llvm/IR/Value.h>

namespace ast {

    class visitor;

    class expression {
    public:
	virtual ~expression() = default;

	[[nodiscard]] virtual auto type() const -> llvm::Type* = 0;
	[[nodiscard]] virtual auto accept(visitor*) const -> llvm::Value* = 0;
    };

}
