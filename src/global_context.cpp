#include "global_context.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

global_context::global_context() 
    : _context{std::make_unique<llvm::LLVMContext>()}
{
    add_default_types();
}

auto global_context::instance() -> global_context& {
    static global_context _global_context{};
    return _global_context;
}

[[nodiscard]] auto global_context::get() -> llvm::LLVMContext& {
    return *_context;
}

[[nodiscard]] auto global_context::context() -> llvm::LLVMContext& {
    return instance().get();
}

[[nodiscard]] auto global_context::get_type(const std::string& type_name) -> llvm::Type* {
    return _types[type_name];
}


[[nodiscard]] auto global_context::type(const std::string& type_name) -> llvm::Type* {
    return instance().get_type(type_name);
}

auto global_context::add_default_types() -> void {
    _types[""]        = llvm::Type::getVoidTy(get());
    _types["bool"]    = llvm::Type::getInt1Ty(get());

    _types["int"]     = llvm::Type::getInt32Ty(get());
    _types["int8"]    = llvm::Type::getInt8Ty(get());
    _types["int16"]   = llvm::Type::getInt16Ty(get());
    _types["int32"]   = llvm::Type::getInt32Ty(get());
    _types["int64"]   = llvm::Type::getInt64Ty(get());
    _types["int128"]  = llvm::Type::getInt128Ty(get());

    _types["uint"]    = llvm::Type::getInt32Ty(get());
    _types["uint8"]   = llvm::Type::getInt8Ty(get());
    _types["uint16"]  = llvm::Type::getInt16Ty(get());
    _types["uint32"]  = llvm::Type::getInt32Ty(get());
    _types["uint64"]  = llvm::Type::getInt64Ty(get());
    _types["uint128"] = llvm::Type::getInt128Ty(get());

    _types["float"]   = llvm::Type::getFloatTy(get());
    _types["bfloat"]  = llvm::Type::getBFloatTy(get());
    _types["double"]  = llvm::Type::getDoubleTy(get());

    _types["char"]    = llvm::Type::getInt8Ty(get());
    _types["string"]  = llvm::Type::getInt8PtrTy(get());

    _types["byte"]    = llvm::Type::getInt8Ty(get());
}