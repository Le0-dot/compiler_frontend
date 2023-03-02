#include "type/signed_integer_type.hpp"
#include "type/integer_type.hpp"

types::signed_integer_type::signed_integer_type(llvm::IntegerType* type)
    : types::integer_type{type}
{}

[[nodiscard]] auto types::signed_integer_type::is_signed() const noexcept -> bool {
    return true;
}
