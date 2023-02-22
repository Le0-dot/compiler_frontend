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

	[[nodiscard]] virtual auto type() const -> llvm::Type* override;
	[[nodiscard]] virtual auto accept(visitor*) const -> llvm::Value* override;

	[[nodiscard]] auto value() const -> char;
    };

}
