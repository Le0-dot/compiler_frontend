#pragma once

#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"

namespace ast {

    class function_expression : public expression {
    private:
	std::string _name;
	std::string _return_type;
	std::vector<std::pair<std::string, std::string>> _args;
	std::unique_ptr<expression> _body;

    public:
	function_expression(std::string&&, std::string&&, std::vector<std::pair<std::string, std::string>>&&, std::unique_ptr<expression>&&);

	virtual auto accept(visitor* v) const -> llvm::Value* override;

	[[nodiscard]] auto name() const -> const std::string&;
	[[nodiscard]] auto return_type() const -> const std::string&;
	[[nodiscard]] auto args() const -> const std::vector<std::pair<std::string, std::string>>&;
	[[nodiscard]] auto body() const -> const expression*;
    };

}
