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

	[[nodiscard]] auto callee() const -> const std::string&;
	[[nodiscard]] auto args() const -> const std::vector<std::unique_ptr<expression>>&;
    };

}
