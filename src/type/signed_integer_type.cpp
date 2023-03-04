#include "type/signed_integer_type.hpp"
#include "type/integer_type.hpp"

types::signed_integer_type::signed_integer_type(llvm::IntegerType* type, std::string&& name)
    : types::integer_type{type, std::move(name)}
{}

[[nodiscard]] auto types::signed_integer_type::is_signed() const noexcept -> bool {
    return true;
}
