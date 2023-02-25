#pragma once

#include <string>

#include "expression.hpp"

namespace ast {

    class variable_expression : public expression {
    private:
	std::string _name;

    public:
	variable_expression(std::string&&);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> llvm::Type* override;

	[[nodiscard]] auto name() const -> const std::string&;
    };

}
