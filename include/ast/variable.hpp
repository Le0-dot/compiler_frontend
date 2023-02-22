#pragma once

#include <string>

#include "expression.hpp"

namespace ast {

    class variable_expression : public expression {
    private:
	std::string _name;

    public:
	variable_expression(std::string&&);

	[[nodiscard]] virtual auto type() const -> llvm::Type* override;
	[[nodiscard]] virtual auto accept(visitor*) const -> llvm::Value* override;

	[[nodiscard]] auto name() const -> const std::string&;
    };

}
