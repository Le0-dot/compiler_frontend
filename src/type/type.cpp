#include "type/type.hpp"

types::type::type(llvm::Type* type)
    : _type{type}
{}

auto types::type::operator==(const type& other) const noexcept -> bool {
    return _type == other._type;
}

auto types::type::operator==(llvm::Type* other) const noexcept -> bool {
    return _type == other;
}

types::type::operator bool() const noexcept {
    return _type;
}

[[nodiscard]] auto types::type::get() const noexcept -> llvm::Type* {
    return _type;
}

[[nodiscard]] auto types::type::is_signed() const noexcept -> bool {
    return true;
}

[[nodiscard]] auto types::type::is_integral() const noexcept -> bool {
    return _type->isIntegerTy();
}

[[nodiscard]] auto types::type::is_floating_point() const noexcept -> bool {
    return _type->isFloatingPointTy();
}

[[nodiscard]] auto types::type::is_boolean() const noexcept -> bool {
    return _type->isIntegerTy(1);
}

[[nodiscard]] auto types::type::is_byte() const noexcept -> bool {
    return _type->isIntegerTy(8);
}

[[nodiscard]] auto types::type::is_function() const noexcept -> bool {
    return _type->isFunctionTy();
}

auto types::type::set_type(llvm::Type* type) -> void {
    _type = type;
}
