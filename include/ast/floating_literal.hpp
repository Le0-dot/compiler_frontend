#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    class floating_literal_expression : public expression {
    private:
	double _value;

    public:
	floating_literal_expression(std::string&&);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> llvm::Type* override;

	[[nodiscard]] auto value() const -> double;
    };

}
