#pragma once

#include <llvm/IR/Value.h>

namespace ast {

    class visitor;

    class expression {
    public:
	virtual ~expression() = default;

	virtual auto accept(visitor*) const -> llvm::Value* = 0;
    };

}
