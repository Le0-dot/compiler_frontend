#pragma once

#include <string>

#include "expression.hpp"
#include "implicit_cast.hpp"

namespace ast {

    class binary_expression : public expression {
    private:
	std::string _operator;
	std::unique_ptr<expression> _lhs, _rhs;

    public:
	binary_expression(std::string&&, std::unique_ptr<expression>&&, std::unique_ptr<expression>&&);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	[[nodiscard]] auto op()  const -> const std::string&;
	[[nodiscard]] auto lhs() const -> const expression*;
	[[nodiscard]] auto lhs()       ->       expression*;
	[[nodiscard]] auto rhs() const -> const expression*;
	[[nodiscard]] auto rhs()       ->       expression*;

	auto insert_lhs_cast(cast_builder auto&& builder = {}) -> void {
	    _lhs = std::invoke(builder, std::move(_lhs));
	}

	auto insert_rhs_cast(cast_builder auto&& builder = {}) -> void {
	    _rhs = std::invoke(builder, std::move(_rhs));
	}
    };

}
