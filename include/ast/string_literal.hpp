#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    class string_literal_expression : public expression {
    private:
	std::string _value;

    public:
	string_literal_expression(std::string&&);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	[[nodiscard]] auto value() const -> const std::string&;
    };

}
