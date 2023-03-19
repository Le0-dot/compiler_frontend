#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    /**
     * String value literal expression
     */
    class string_literal_expression : public expression {
    private:
	std::string _value; ///< value of literal

    public:
	/**
	 * Constructor of string_literal_expression
	 * @param value a string that represents value of literal
	 */
	string_literal_expression(std::string&& value);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of value for string literal
	 * @return value of literal
	 */
	[[nodiscard]] auto value() const -> const std::string&;
    };

}
