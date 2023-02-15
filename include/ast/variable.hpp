#pragma once

#include <string>

#include "expression.hpp"

namespace ast {

    class variable_expression : public expression {
    private:
	std::string _name;

    public:
	variable_expression(std::string&&);

	virtual auto accept(visitor*) const -> llvm::Value* override;

	auto name() const -> const std::string&;
    };

}
