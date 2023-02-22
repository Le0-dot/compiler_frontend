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

	[[nodiscard]] virtual auto type() const -> llvm::Type* override;
	[[nodiscard]] virtual auto accept(visitor*) const -> llvm::Value* override;

	[[nodiscard]] auto value() const -> double;
    };

}
