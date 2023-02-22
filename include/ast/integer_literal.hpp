#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    class integer_literal_expression : public expression {
    private:
	int _value;

    public:
	integer_literal_expression(std::string&&, uint8_t);

	[[nodiscard]] virtual auto type() const -> llvm::Type* override;
	[[nodiscard]] virtual auto accept(visitor*) const -> llvm::Value* override;

	[[nodiscard]] auto value() const -> int;
    };

}
