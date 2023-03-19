#pragma once

#include <vector>

#include "expression.hpp"
#include "implicit_cast.hpp"

namespace ast {

    /**
     * A block expression class that can hold any number of expressions
     */
    class block_expression : public expression {
    private:
	std::vector<std::unique_ptr<expression>> _expressions; ///< expressions of block

    public:
	/*
	 * Constructor of block expression
	 * @param expressions a vector of expressions that will be held by block
	 */
	block_expression(std::vector<std::unique_ptr<expression>>&& expressions);

	[[nodiscard]] virtual auto accept(value_visitor *) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor *) -> types::type* override;

	/**
	 * Accessor of expression for block_expression
	 * @return a vector of pointers to expressions
	 */
	[[nodiscard]] auto expressions() const -> const std::vector<std::unique_ptr<expression>>&;

	/**
	 * Insert new expression, instead of last one (return expression), that will hold old expression
	 * @tparam cast_builder a function-like type of object that creates new expression node from old expression
	 * @param builder an instance of cast_builder
	 */
	auto insert_result_cast(cast_builder auto&& builder = {}) -> void {
	    _expressions.back() = std::invoke(builder, std::move(_expressions.back()));
	}
    };

}
