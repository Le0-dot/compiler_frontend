#pragma once

#include <cstdint>
#include <string>

#include "expression.hpp"

namespace ast {

    class integer_container : public std::string {
    public:
	template<typename T>
	integer_container(T&& o) : std::string(std::forward<T&&>(o)) {}

	[[nodiscard]] auto operator<=>(const integer_container& o) const -> std::strong_ordering;
	[[nodiscard]] auto operator<=>(const std::string& o) const -> std::strong_ordering;
    };

    class integer_literal_expression : public expression {
    private:
	integer_container _value;
	uint8_t _radix;

    public:
	integer_literal_expression(std::string&&, uint8_t);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	[[nodiscard]] auto value() const -> const integer_container&;
	[[nodiscard]] auto radix() const -> uint8_t;
    };

}
