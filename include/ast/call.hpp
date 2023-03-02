#pragma once

#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"
#include "implicit_cast.hpp"

namespace ast {

    class call_expression : public expression {
    private:
	std::string _callee;
	std::vector<std::unique_ptr<expression>> _args;

    public:
	call_expression(std::string&&, std::vector<std::unique_ptr<expression>>&&);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	[[nodiscard]] auto callee() const -> const std::string&;
	[[nodiscard]] auto args() const -> const std::vector<std::unique_ptr<expression>>&;
	[[nodiscard]] auto args()       ->       std::vector<std::unique_ptr<expression>>&;

	auto insert_arg_cast(
		typename std::vector<std::unique_ptr<expression>>::const_iterator it,
		cast_builder auto&& builder = {}) 
	    -> void {
	    auto iter = std::ranges::next(_args.begin(), it);
	    *iter = std::invoke(builder, std::move(*iter));
	}
    };

}
