#pragma once

#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"
#include "implicit_cast.hpp"

namespace ast {

    /**
     * Call expression class that holds callee and arguments
     */
    class call_expression : public expression {
    private:
	std::string _callee; ///< callee i.e. function name
	std::vector<std::unique_ptr<expression>> _args; ///< arguments that are passed to the function

    public:
	/**
	 * Constructor of call expression
	 * @param callee a function name
	 * @param args a vector of expressions that are passed to the function
	 */
	call_expression(std::string&& callee, std::vector<std::unique_ptr<expression>>&& args);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of callee for call expression
	 * @return function name
	 */
	[[nodiscard]] auto callee() const -> const std::string&;

	/**
	 * Accessor of args for const call expression
	 * @return const reference to the vector of arguments
	 */
	[[nodiscard]] auto args() const -> const std::vector<std::unique_ptr<expression>>&;

	/**
	 * Accessor of args for non-const call expression
	 * @return reference to the vector of arguments
	 */
	[[nodiscard]] auto args()       ->       std::vector<std::unique_ptr<expression>>&;

	/**
	 * Insert new expressions, instead of specified argument, that will hold old expression
	 * @tparam cast_builder a function-like type of object that creates new expression node from old expression
	 * @param it an iterator of args which value will be replaced
	 * @param builder an instance of cast_builder
	 */
	auto insert_arg_cast(
		typename std::vector<std::unique_ptr<expression>>::const_iterator it,
		cast_builder auto&& builder = {}) 
	    -> void {
	    auto iter = std::ranges::next(_args.begin(), it);
	    *iter = std::invoke(builder, std::move(*iter));
	}
    };

}
