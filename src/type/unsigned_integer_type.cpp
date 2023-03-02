#include "type/unsigned_integer_type.hpp"

types::unsigned_integer_type::unsigned_integer_type(llvm::IntegerType* type)
    : types::integer_type(type)
{}

[[nodiscard]] auto types::unsigned_integer_type::is_signed() const noexcept -> bool {
    return false;
}
