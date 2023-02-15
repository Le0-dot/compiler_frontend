#pragma once

#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"

namespace ast {

    class call_expression : public expression {
    private:
	std::string _callee;
	std::vector<std::unique_ptr<expression>> _args;

    public:
	call_expression(std::string&&, std::vector<std::unique_ptr<expression>>&&);

	virtual auto accept(visitor*) const -> llvm::Value* override;

	auto callee() const -> const std::string&;
	auto args() const -> const std::vector<std::unique_ptr<expression>>&;
    };

}
