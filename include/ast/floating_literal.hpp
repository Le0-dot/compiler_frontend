#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    /**
     * Floating point value literal expression
     */
    class floating_literal_expression : public expression {
    private:
	double _value; ///< value of literal

    public:
	/**
	 * Constructor of floating_literal_expression
	 * @param value a string that represents floating point number
	 */
	floating_literal_expression(std::string&&);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of value for floating point literal
	 * @return double value of literal
	 */
	[[nodiscard]] auto value() const -> double;
    };

}
