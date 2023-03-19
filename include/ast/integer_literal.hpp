#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    class integer_container : public std::string {
    public:
	template<typename T>
	integer_container(T&& o) : std::string(std::forward<T&&>(o)) {}

	[[nodiscard]] auto operator<=>(const integer_container& o) const -> std::strong_ordering;
	[[nodiscard]] auto operator<=>(const std::string& o) const -> std::strong_ordering;
    };

    /**
     * Integer value literal expression
     */
    class integer_literal_expression : public expression {
    private:
	integer_container _value; ///< value of the literal
	uint8_t _radix; ///< radix of literal

    public:
	/**
	 * Constructor of integer_literal_expression 
	 * @param value a string that represents integer value
	 * @param radix a radix of literal i.e. 2, 8, 10, 16
	 */
	integer_literal_expression(std::string&& value, uint8_t radix);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of value for integer literal
	 * @return integer_container value of literal
	 */
	[[nodiscard]] auto value() const -> const integer_container&;

	/**
	 * Accessor of radix for integer literal
	 * @return radix value
	 */
	[[nodiscard]] auto radix() const -> uint8_t;
    };

}
