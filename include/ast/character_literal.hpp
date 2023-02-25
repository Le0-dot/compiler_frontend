#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    class character_literal_expression : public expression {
    private:
	char _value;

    public:
	character_literal_expression(std::string&&);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> llvm::Type* override;

	[[nodiscard]] auto value() const -> char;
    };

}
