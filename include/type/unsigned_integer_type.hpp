#pragma once

#include "integer_type.hpp"

namespace types {

    class unsigned_integer_type : public integer_type {
    public:
	unsigned_integer_type(llvm::IntegerType*);

	unsigned_integer_type()                                                = default;
	unsigned_integer_type(const unsigned_integer_type&)                    = default;
	unsigned_integer_type(unsigned_integer_type&&)                         = default;
	auto operator=(const unsigned_integer_type&) -> unsigned_integer_type& = default;
	auto operator=(unsigned_integer_type&&)      -> unsigned_integer_type& = default;
	virtual ~unsigned_integer_type()                                       = default;

	[[nodiscard]] virtual auto is_signed() const noexcept -> bool override final;
    };

}
