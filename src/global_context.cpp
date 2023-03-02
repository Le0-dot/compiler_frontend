#include "global_context.hpp"
#include <cstdio>

global_context::global_context() 
    : _context{std::make_unique<llvm::LLVMContext>()}
{
    add_default_types();
    add_default_casts();
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

[[nodiscard]] auto global_context::get_type(const std::string& type_name) -> types::type* {
    return _types[type_name].get();
}

[[nodiscard]] auto global_context::type(const std::string& type_name) -> types::type* {
    return instance().get_type(type_name);
}

[[nodiscard]] auto global_context::get_type(const std::vector<std::string>& arg_types, const std::string& ret_type) -> types::function_type* {
    auto& func_type = _function_types[std::make_pair(arg_types, ret_type)];
    if(func_type)
	return func_type.get();

    std::vector<types::type*> param_types;
    for(const auto& arg_type: arg_types) {
	param_types.push_back(get_type(arg_type));
	if(!param_types.back())
	    return nullptr;
    }

    func_type = std::make_unique<types::function_type>(param_types, get_type(ret_type));
    return func_type.get();
}

[[nodiscard]] auto global_context::type(const std::vector<std::string>& arg_types, const std::string& ret_type) -> types::function_type* {
    return instance().get_type(arg_types, ret_type);
}

[[nodiscard]] auto global_context::get_cast(types::type* from, types::type* to) -> const std::function<cast_function>& {
    return _casts[std::make_pair(from, to)];
}

[[nodiscard]] auto global_context::cast(types::type* from, types::type* to) -> const std::function<cast_function>& {
    return instance().get_cast(from, to);
}

auto global_context::add_default_types() -> void {
    _types[""]        = std::make_unique<types::type>(llvm::Type::getVoidTy(get()));

    _types["bool"]    = std::make_unique<types::type>(llvm::Type::getInt1Ty(get()));

    _types["byte"]    = std::make_unique<types::type>(llvm::Type::getInt8Ty(get()));

    _types["int"]     = std::make_unique<types::signed_integer_type>(llvm::Type::getInt32Ty(get()));
    _types["int8"]    = std::make_unique<types::signed_integer_type>(llvm::Type::getInt8Ty(get()));
    _types["int16"]   = std::make_unique<types::signed_integer_type>(llvm::Type::getInt16Ty(get()));
    _types["int32"]   = std::make_unique<types::signed_integer_type>(llvm::Type::getInt32Ty(get()));
    _types["int64"]   = std::make_unique<types::signed_integer_type>(llvm::Type::getInt64Ty(get()));
    _types["int128"]  = std::make_unique<types::signed_integer_type>(llvm::Type::getInt128Ty(get()));

    _types["uint"]    = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt32Ty(get()));
    _types["uint8"]   = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt8Ty(get()));
    _types["uint16"]  = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt16Ty(get()));
    _types["uint32"]  = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt32Ty(get()));
    _types["uint64"]  = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt64Ty(get()));
    _types["uint128"] = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt128Ty(get()));

    _types["float"]   = std::make_unique<types::type>(llvm::Type::getFloatTy(get()));
    _types["double"]  = std::make_unique<types::type>(llvm::Type::getDoubleTy(get()));

    _types["char"]    = std::make_unique<types::type>(llvm::Type::getInt8Ty(get()));
    _types["string"]  = std::make_unique<types::type>(llvm::Type::getInt8PtrTy(get()));
}

auto global_context::add_default_casts() -> void {
    /*
    // int to bool
    _casts[std::make_pair(_types["int8"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int8"), 0, true), "cast");
    };
    _casts[std::make_pair(_types["int16"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int16"), 0, true), "cast");
    };
    _casts[std::make_pair(_types["int32"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int32"), 0, true), "cast");
    };
    _casts[std::make_pair(_types["int64"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int64"), 0, true), "cast");
    };
    _casts[std::make_pair(_types["int128"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int128"), 0, true), "cast");
    };

    // uint to bool
    _casts[std::make_pair(_types["uint8"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint8"), 0, false), "cast");
    };
    _casts[std::make_pair(_types["uint16"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint16"), 0, false), "cast");
    };
    _casts[std::make_pair(_types["uint32"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint32"), 0, false), "cast");
    };
    _casts[std::make_pair(_types["uint64"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint64"), 0, false), "cast");
    };
    _casts[std::make_pair(_types["uint128"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint128"), 0, false), "cast");
    };

    // float to double
    _casts[std::make_pair(_types["float"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateFCmpULT(v, llvm::ConstantFP::get(context(), llvm::APFloat(0.f)), "cast");
    };
    _casts[std::make_pair(_types["double"], _types["bool"])] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateFCmpULT(v, llvm::ConstantFP::get(context(), llvm::APFloat(0.f)), "cast");
    };
    */
}
