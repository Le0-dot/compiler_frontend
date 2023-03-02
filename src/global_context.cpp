#include "global_context.hpp"
#include <llvm/IR/IRBuilder.h>

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

    _types["char"]    = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt8Ty(get()));
    _types["string"]  = std::make_unique<types::type>(llvm::Type::getInt8PtrTy(get()));
}

auto global_context::add_default_casts() -> void {
    // ---------------------------------------- bool ------------------------------------------
    
    // byte
    _casts[std::make_pair(type("byte"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("byte")->get(), 0), "cast_byte_bool");
    };

    // int
    _casts[std::make_pair(type("int8"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int8")->get(), 0, true), "cast_int8_bool");
    };
    _casts[std::make_pair(type("int16"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int16")->get(), 0, true), "cast_int16_bool");
    };
    _casts[std::make_pair(type("int32"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int32")->get(), 0, true), "cast_int32_bool");
    };
    _casts[std::make_pair(type("int64"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int64")->get(), 0, true), "cast_int64_bool");
    };
    _casts[std::make_pair(type("int128"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("int128")->get(), 0, true), "cast_int128_bool");
    };

    // uint
    _casts[std::make_pair(type("uint8"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint8")->get(), 0), "cast_uint8_bool");
    };
    _casts[std::make_pair(type("uint16"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint16")->get(), 0), "cast_uint16_bool");
    };
    _casts[std::make_pair(type("uint32"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint32")->get(), 0), "cast_uint32_bool");
    };
    _casts[std::make_pair(type("uint64"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint64")->get(), 0), "cast_uint64_bool");
    };
    _casts[std::make_pair(type("uint128"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("uint128")->get(), 0), "cast_uint128_bool");
    };

    // float
    _casts[std::make_pair(type("float"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateFCmpULT(v, llvm::ConstantFP::get(context(), llvm::APFloat(0.f)), "cast_float_bool");
    };
    _casts[std::make_pair(type("double"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateFCmpULT(v, llvm::ConstantFP::get(context(), llvm::APFloat(0.f)), "cast_double_bool");
    };

    // string
    _casts[std::make_pair(type("char"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateICmpNE(v, llvm::ConstantInt::get(type("char")->get(), 0), "cast_char_bool");
    };
    _casts[std::make_pair(type("string"), type("bool"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return nullptr; // placeholder
    };


    // ---------------------------------------- byte ------------------------------------------

    // bool
    _casts[std::make_pair(type("bool"), type("byte"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("byte")->get(), false, "cast_bool_byte");
    };

    // int8
    _casts[std::make_pair(type("int8"), type("byte"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("byte")->get(), true, "cast_int8_byte");
    };

    // uint8
    _casts[std::make_pair(type("uint8"), type("byte"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("byte")->get(), false, "cast_uint8_byte");
    };

    // char
    _casts[std::make_pair(type("char"), type("byte"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("byte")->get(), false, "cast_char_byte");
    };


    // ---------------------------------------- int8 ------------------------------------------

    // bool
    _casts[std::make_pair(type("bool"), type("int8"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("int8")->get(), false, "cast_bool_int8");
    };


    // ---------------------------------------- uint8 -----------------------------------------

    // bool
    _casts[std::make_pair(type("bool"), type("uint8"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("uint8")->get(), false, "cast_bool_uint8");
    };

    // byte
    _casts[std::make_pair(type("byte"), type("uint8"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("uint8")->get(), false, "cast_byte_uint8");
    };

    // char
    _casts[std::make_pair(type("char"), type("uint8"))] = [] (llvm::IRBuilderBase* b, llvm::Value* v) {
	return b->CreateIntCast(v, type("uint8")->get(), false, "cast_char_uint8");
    };


    // ---------------------------------------- int16 -----------------------------------------
                                                                                              
                                                                                              
                                                                                              
    // ---------------------------------------- uint16 ----------------------------------------
                                                                                              
                                                                                              
                                                                                              
    // ---------------------------------------- int32 -----------------------------------------
                                                                                              
                                                                                              
                                                                                              
    // ---------------------------------------- uint32 ----------------------------------------
                                                                                              
                                                                                              
                                                                                              
    // ---------------------------------------- int64 -----------------------------------------



    // ---------------------------------------- uint64 ----------------------------------------



    // ---------------------------------------- int128 ----------------------------------------



    // ---------------------------------------- uint128 ---------------------------------------



    // ---------------------------------------- float -----------------------------------------



    // ---------------------------------------- double ----------------------------------------



    // ---------------------------------------- char ------------------------------------------



    // ---------------------------------------- string ----------------------------------------



}
