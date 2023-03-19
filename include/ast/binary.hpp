#pragma once

#include <string>

#include "expression.hpp"
#include "implicit_cast.hpp"

namespace ast {

    /**
     * Binary expression class that holds
     * left hand side expression, right hand side expression
     * and operator
     */
    class binary_expression : public expression {
    private:
	std::string _operator; ///< Operator between 2 expressions
	std::unique_ptr<expression> _lhs, _rhs; ///< Expression of binary expression

    public:
	/**
	 * Constructor of binary_expression from operator and 2 expressions
	 * @param op an operator between 2 expressions
	 * @param lhs an left hand side expression node
	 * @param rhs a right hand side expression node
	 */
	binary_expression(std::string&& op, std::unique_ptr<expression>&& lhs, std::unique_ptr<expression>&& rhs);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of operator for binary_expression
	 */
	[[nodiscard]] auto op()  const -> const std::string&;

	/**
	 * Accessor of lhs expression for const binary_expression
	 */
	[[nodiscard]] auto lhs() const -> const expression*;

	/**
	 * Accessor of lhs expression for non-const binary_expression
	 */
	[[nodiscard]] auto lhs()       ->       expression*;

	/**
	 * Accessor of rhs expression for const binary_expression
	 */
	[[nodiscard]] auto rhs() const -> const expression*;

	/**
	 * Accessor of rhs expression for non-const binary_expression
	 */
	[[nodiscard]] auto rhs()       ->       expression*;

	/**
	 * Insert new cast node that will replace lhs and hold within itself old lhs
	 * @tparam cast_builder a function-like type of object that creates new expression node from old expression
	 * @param builder an instance of cast_builder
	 */
	auto insert_lhs_cast(cast_builder auto&& builder = {}) -> void {
	    _lhs = std::invoke(builder, std::move(_lhs));
	}

	/**
	 * Insert new cast node that will replace rhs and hold within itself old rhs
	 * @tparam cast_builder a function-like type of object that creates new expression node from old expression
	 * @param builder an instance of cast_builder
	 */
	auto insert_rhs_cast(cast_builder auto&& builder = {}) -> void {
	    _rhs = std::invoke(builder, std::move(_rhs));
	}
    };

}
