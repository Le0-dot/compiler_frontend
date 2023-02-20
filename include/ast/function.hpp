#pragma once

#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"

namespace ast {

    class function_expression : public expression {
    private:
	std::string _name;
	std::vector<std::pair<std::string, std::string>> _args;
	std::unique_ptr<expression> _body;

    public:
	function_expression(std::string&&, std::vector<std::pair<std::string, std::string>>&&, std::unique_ptr<expression>&&);

	virtual auto accept(visitor* v) const -> llvm::Value* override;

	auto name() const -> const std::string&;
	auto args() const -> const std::vector<std::pair<std::string, std::string>>&;
	auto body() const -> const expression*;
    };

}
