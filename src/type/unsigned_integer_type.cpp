#include "type/unsigned_integer_type.hpp"

types::unsigned_integer_type::unsigned_integer_type(llvm::IntegerType* type, std::string&& name)
    : types::integer_type{type, std::move(name)}
{}

[[nodiscard]] auto types::unsigned_integer_type::is_signed() const noexcept -> bool {
    return false;
}
