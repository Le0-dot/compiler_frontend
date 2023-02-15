#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    enum class literal_types : uint8_t {
	integer,
	floating,
	character,
	string
    };


    class literal_expression : public expression {
    private:
	std::string _value;
	literal_types _type;

    public:
	literal_expression(std::string&&, literal_types);

	virtual auto accept(visitor*) const -> llvm::Value* override;

	auto value() const -> const std::string&;
	auto type() const -> literal_types;
    };

}
