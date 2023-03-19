#pragma once

#include <memory>
#include <string>
#include <vector>

#include <llvm/IR/Function.h>

#include "expression.hpp"
#include "block.hpp"

namespace ast {

    /**
     * Function definition expression class
     */
    class function_expression : public expression {
    private:
	std::string _name;			 ///< name of function
	std::vector<std::string> _args;          ///< names of arguments
	std::vector<std::string> _types;         ///< types of arguments
	std::string _ret_type;                   ///< return type of function
	std::unique_ptr<block_expression> _body; ///< body of function that is block expression

    public:
	/**
	 * Costructor of function expression
	 * @param name a name of a function
	 * @param args a vector of arguments' names
	 * @param type_list a vector of arguments' types
	 * @param return_type a return type of a function
	 * @param body a block expression that is a body of a function
	 */
	function_expression(std::string&& name, std::vector<std::string>&& args,
		std::vector<std::string>&& type_list, std::string&& return_type,
		std::unique_ptr<block_expression>&& body);

	[[nodiscard]] virtual auto accept(value_visitor* v) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor* v) -> types::type* override;

	/**
	 * Accessor of name for function
	 * @return name of a function
	 */
	[[nodiscard]] auto name()        const -> const std::string&;

	/**
	 * Accessor of arguments for const function
	 * @return arguments' names of a function
	 */
	[[nodiscard]] auto args()        const -> const std::vector<std::string>&;

	/**
	 * Accessor of types for const function
	 * @return arguments' types of a function
	 */
	[[nodiscard]] auto types()       const -> const std::vector<std::string>&;

	/**
	 * Accessor of return type for const function
	 * @return return type of a function
	 */
	[[nodiscard]] auto return_type() const -> const std::string&;

	/**
	 * Accessor of body for cosnt function
	 * @return block of body of function
	 */
	[[nodiscard]] auto body()        const -> const block_expression*;

	/**
	 * Accessor of body for non-cosnt function
	 * @return block of body of function
	 */
	[[nodiscard]] auto body()              ->       block_expression*;
    };

}
