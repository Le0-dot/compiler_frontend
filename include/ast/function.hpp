#pragma once

#include <memory>
#include <string>
#include <vector>

#include <llvm/IR/Function.h>

#include "expression.hpp"
#include "block.hpp"

namespace ast {

    class function_expression : public expression {
    private:
	std::string _name;
	std::vector<std::string> _args;
	std::vector<std::string> _types;
	std::string _ret_type;
	std::unique_ptr<block_expression> _body;

    public:
	function_expression(std::string&&, std::vector<std::string>&&,
		std::vector<std::string>&&, std::string&&, std::unique_ptr<block_expression>&&);

	[[nodiscard]] virtual auto accept(value_visitor* v) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor* v) -> llvm::Type* override;

	[[nodiscard]] auto name()        const -> const std::string&;
	[[nodiscard]] auto args()        const -> const std::vector<std::string>&;
	[[nodiscard]] auto types()       const -> const std::vector<std::string>&;
	[[nodiscard]] auto return_type() const -> const std::string&;
	[[nodiscard]] auto body()        const -> const block_expression*;
	[[nodiscard]] auto body()              ->       block_expression*;
    };

}
