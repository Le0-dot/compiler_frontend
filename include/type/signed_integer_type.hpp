#pragma once

#include "integer_type.hpp"

namespace types {

    class signed_integer_type : public integer_type {
    public:
	signed_integer_type(llvm::IntegerType*);

	signed_integer_type()                                              = default;
	signed_integer_type(const signed_integer_type&)                    = default;
	signed_integer_type(signed_integer_type&&)                         = default;
	auto operator=(const signed_integer_type&) -> signed_integer_type& = default;
	auto operator=(signed_integer_type&&)      -> signed_integer_type& = default;
	virtual ~signed_integer_type()                                     = default;

	[[nodiscard]] virtual auto is_signed() const noexcept -> bool override final;
    };

}
