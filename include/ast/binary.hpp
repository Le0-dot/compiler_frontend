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

	[[nodiscard]] virtual auto type() const -> llvm::Type* override;
	[[nodiscard]] virtual auto accept(visitor*) const -> llvm::Value* override;

	[[nodiscard]] auto op() const -> const std::string&;
	[[nodiscard]] auto lhs() const -> const expression*;
	[[nodiscard]] auto rhs() const -> const expression*;
    };

}
