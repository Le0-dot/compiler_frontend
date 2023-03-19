#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    /**
     * Character value literal expression
     */
    class character_literal_expression : public expression {
    private:
	char _value; ///< value of literal

    public:
	/**
	 * Constructor of character_literal_expression
	 * @param value a string of 1 element that holds value
	 */
	character_literal_expression(std::string&& value);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of value for character literal
	 * @return character of literal
	 */
	[[nodiscard]] auto value() const -> char;
    };

}
