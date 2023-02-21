#pragma once

#include <vector>
#include <memory>

#include "expression.hpp"

namespace ast {

    class block_expression : public expression {
    private:
	std::vector<std::unique_ptr<expression>> _expressions;

    public:
	block_expression(std::vector<std::unique_ptr<expression>>&&);

	virtual auto accept(visitor *) const -> llvm::Value* override;

	[[nodiscard]] auto expressions() const -> const std::vector<std::unique_ptr<expression>>&;
    };

}
