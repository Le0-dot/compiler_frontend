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

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) const -> llvm::Type* override;

	[[nodiscard]] auto value() const -> int;
    };

}
