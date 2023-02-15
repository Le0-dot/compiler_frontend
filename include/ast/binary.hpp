#pragma once

#include <memory>
#include <string>

#include "expression.hpp"

namespace ast {

    class binary_expression : public expression {
    private:
	std::string _operator;
	std::unique_ptr<expression> _lhs, _rhs;

    public:
	binary_expression(std::string&&, std::unique_ptr<expression>&&, std::unique_ptr<expression>&&);

	virtual auto accept(visitor*) const -> llvm::Value* override;

	auto op() const -> const std::string&;
	auto lhs() const -> const expression*;
	auto rhs() const -> const expression*;
    };

}
