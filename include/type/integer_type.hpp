#pragma once

#include <llvm/IR/DerivedTypes.h>

#include "type.hpp"

namespace types {

    class integer_type : public type {
    public:
	integer_type(llvm::IntegerType*, std::string&&);

	virtual ~integer_type() = default;

	[[nodiscard]] virtual auto is_signed() const noexcept -> bool override = 0;
    };

}
