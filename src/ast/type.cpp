#include "ast/type.hpp"

ast::type::type(llvm::Type* type)
    : _type{type}
    , _signed{true}
{}

ast::type::type(llvm::IntegerType* type, bool issigned)
    : _type{type}
    , _signed{issigned}
{}

ast::type::type(llvm::FunctionType* type)
    : _type{type}
    , _function{true}
{}

[[nodiscard]] auto ast::type::get_type() const -> llvm::Type* {
    return _type;
}

[[nodiscard]] auto ast::type::is_signed() const -> bool {
    return _signed;
}

[[nodiscard]] auto ast::type::is_int() const -> bool {
    return _type->isIntegerTy() && _signed;
}

[[nodiscard]] auto ast::type::is_uint() const -> bool {
    return _type->isIntegerTy() && !_signed;
}

[[nodiscard]] auto ast::type::is_float() const -> bool {
    return _type->isFloatingPointTy();
}

[[nodiscard]] auto ast::type::is_bool() const -> bool {
    return _type->isIntegerTy(1);
}

[[nodiscard]] auto ast::type::is_byte() const -> bool {
    return _type->isIntegerTy(8) && !_signed;
}

[[nodiscard]] auto ast::type::is_function() const -> bool {
    return _type->isFunctionTy();
}
