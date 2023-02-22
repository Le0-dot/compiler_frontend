#pragma once

#include <memory>
#include <string>
#include <vector>

#include <llvm/IR/Function.h>

#include "expression.hpp"

namespace ast {

    class function_expression : public expression {
    private:
	std::string _name;
	std::vector<std::string> _args;
	llvm::FunctionType* _type;
	std::unique_ptr<expression> _body;

    public:
	function_expression(std::string&&, std::vector<std::string>&&, std::vector<llvm::Type*>&&, llvm::Type*, std::unique_ptr<expression>&&);

	[[nodiscard]] virtual auto type() const -> llvm::Type* override;
	[[nodiscard]] virtual auto accept(visitor* v) const -> llvm::Value* override;

	[[nodiscard]] auto name() const -> const std::string&;
	[[nodiscard]] auto args() const -> const std::vector<std::string>&;
	[[nodiscard]] auto body() const -> const expression*;
    };

}
