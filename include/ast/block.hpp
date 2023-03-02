#pragma once

#include <vector>

#include "expression.hpp"
#include "implicit_cast.hpp"

namespace ast {

    class block_expression : public expression {
    private:
	std::vector<std::unique_ptr<expression>> _expressions;

    public:
	block_expression(std::vector<std::unique_ptr<expression>>&&);

	[[nodiscard]] virtual auto accept(value_visitor *) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor *) -> types::type* override;

	[[nodiscard]] auto expressions() const -> const std::vector<std::unique_ptr<expression>>&;

	auto insert_result_cast(cast_builder auto&& builder = {}) -> void {
	    _expressions.back() = std::invoke(builder, std::move(_expressions.back()));
	}
    };

}
