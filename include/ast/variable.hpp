#pragma once

#include <string>

#include "expression.hpp"

namespace ast {

    /**
     * Expression for variable name instances
     */
    class variable_expression : public expression {
    private:
	std::string _name; ///< name of variable

    public:
	/**
	 * Constructor for variable
	 * @param name a name of variable
	 */
	variable_expression(std::string&& name);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of name for const variable_expression
	 * @return const reference to the name
	 */
	[[nodiscard]] auto name() const -> const std::string&;
    };

}
