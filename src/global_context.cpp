#include "global_context.hpp"
#include "tables.hpp"
#include <llvm/IR/IRBuilder.h>
#include <unordered_map>

namespace {

    auto make_cast_name(const std::string& from, const std::string& to) -> std::string {
	std::string name(6 + from.size() + to.size(), 0); // cast_`FROM`_`TO`
	name = "cast_";
	name += from;
	name += '_';
	name += to;
	return name;
    }

    auto make_operation_name(const std::string& op, const std::string& type) -> std::string {
	std::string name(1 + op.size() + type.size(), 0); // `OPERATION`_`TYPE`
	name = op;
	name += '_';
	name += type;
	return name;
    }

    auto normalise_type(const std::string& type) -> std::string {
	static const type_normalization_table normalization_table = {
	    {"int", "int32"},
	    {"uint", "uint32"},
	};

	if(auto it = normalization_table.find(type); it != normalization_table.end())
	    return it->second;
	return type;
    }

}


global_context::global_context() 
    : _context{std::make_unique<llvm::LLVMContext>()}
{
    fprintf(stderr, "creating global_context\n");
    add_default_types();
    fprintf(stderr, "added default types\n");
    add_default_casts();
    fprintf(stderr, "added default casts\n");
    add_default_operations();
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
    return _types[normalise_type(type_name)].get();
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

[[nodiscard]] auto global_context::get_binary_operation(const std::string& op, types::type* type) -> const std::function<binary_operation_function>& {
    return _binary_operation_table[std::make_pair(op, type)];
}

[[nodiscard]] auto global_context::binary_operation(const std::string& op, types::type* type) -> const std::function<binary_operation_function>& {
    return instance().get_binary_operation(op, type);
}

auto global_context::add_default_types() -> void {
    _types[""]        = std::make_unique<types::type>(llvm::Type::getVoidTy(get()), "(void)");

    _types["bool"]    = std::make_unique<types::type>(llvm::Type::getInt1Ty(get()), "bool");

    _types["byte"]    = std::make_unique<types::type>(llvm::Type::getInt8Ty(get()), "byte");

    _types["int"]     = std::make_unique<types::signed_integer_type>(llvm::Type::getInt32Ty(get()), "int");
    _types["int8"]    = std::make_unique<types::signed_integer_type>(llvm::Type::getInt8Ty(get()), "int8");
    _types["int16"]   = std::make_unique<types::signed_integer_type>(llvm::Type::getInt16Ty(get()), "int16");
    _types["int32"]   = std::make_unique<types::signed_integer_type>(llvm::Type::getInt32Ty(get()), "int32");
    _types["int64"]   = std::make_unique<types::signed_integer_type>(llvm::Type::getInt64Ty(get()), "int64");
    _types["int128"]  = std::make_unique<types::signed_integer_type>(llvm::Type::getInt128Ty(get()), "int128");

    _types["uint"]    = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt32Ty(get()), "uint");
    _types["uint8"]   = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt8Ty(get()), "uint8");
    _types["uint16"]  = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt16Ty(get()), "uint16");
    _types["uint32"]  = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt32Ty(get()), "uint32");
    _types["uint64"]  = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt64Ty(get()), "uint64");
    _types["uint128"] = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt128Ty(get()), "uint128");

    _types["float"]   = std::make_unique<types::type>(llvm::Type::getFloatTy(get()), "float");
    _types["double"]  = std::make_unique<types::type>(llvm::Type::getDoubleTy(get()), "double");

    _types["char"]    = std::make_unique<types::unsigned_integer_type>(llvm::Type::getInt8Ty(get()), "char");
    _types["string"]  = std::make_unique<types::type>(llvm::Type::getInt8PtrTy(get()), "string");
}

auto global_context::add_default_casts() -> void {
    // ---------------------------------------- bool ------------------------------------------

    auto from = "bool";
    std::vector cast_to = {"byte", "int8", "int16", "int32", "int64", "int128", "uint8", "uint16", "uint32", "uint64", "uint128"};
    const std::array cast_to_fp = {"float", "double"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    // maybe add bool to float casts

    // ---------------------------------------- byte ------------------------------------------

    from = "byte";
    cast_to = {"bool", "int8", "int16", "int32", "int64", "int128", "uint8", "uint16", "uint32", "uint64", "uint128", "char"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    // ---------------------------------------- int8 ------------------------------------------

    from = "int8";
    cast_to = {"bool", "byte", "int16", "int32", "int64", "int128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- uint8 -----------------------------------------

    from = "uint8";
    cast_to = {"bool", "byte", "int16", "int32", "int64", "int128", "uint16", "uint32", "uint64", "uint128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- int16 -----------------------------------------
                                                                                              
    from = "int16";
    cast_to = {"bool", "int32", "int64", "int128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- uint16 ----------------------------------------
                                                                                              
    from = "uint16";
    cast_to = {"bool", "int32", "int64", "int128", "uint32", "uint64", "uint128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- int32 -----------------------------------------
                                                                                              
    from = "int32";
    cast_to = {"bool", "int64", "int128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);

                                                                                              
    // ---------------------------------------- uint32 ----------------------------------------
                                                                                              
    from = "uint32";
    cast_to = {"bool", "int64", "int128", "uint64", "uint128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);

                                                                                              
    // ---------------------------------------- int64 -----------------------------------------

    from = "int64";
    cast_to = {"bool", "int128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- uint64 ----------------------------------------

    from = "uint64";
    cast_to = {"bool", "int128", "uint128"};

    for(const char* to: cast_to)
	add_int_cast(from, to);

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- int128 ----------------------------------------

    from = "int128";

    add_int_cast(from, "bool");

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- uint128 ---------------------------------------

    from = "uint128";

    add_int_cast(from, "bool");

    for(const char* to: cast_to_fp)
	add_fp_cast(from, to);


    // ---------------------------------------- float -----------------------------------------

    from = "float";

    add_int_cast(from, "bool");
    add_fp_cast(from, "double");

    // ---------------------------------------- double ----------------------------------------

    add_fp_cast("double", "bool");


    // ---------------------------------------- char ------------------------------------------

    from = "char";
    cast_to = {"bool", "byte"};

    for(const char* to: cast_to)
	add_int_cast(from, to);


    // ---------------------------------------- string ----------------------------------------

    // add string to bool

}

auto global_context::add_default_operations() -> void {
    // --------------------------------------- addition ---------------------------------------
    
    for(const char* type: {"byte", "int8", "int16", "int32", "int64", "int128", "uint8", "uint16", "uint32", "uint64", "uint128"})
	_binary_operation_table[std::make_pair("+", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateAdd(lhs, rhs, "add");
	};

    for(const char* type: {"float", "double"})
	_binary_operation_table[std::make_pair("+", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateFAdd(lhs, rhs, "add");
	};


    // ------------------------------------- substruction --------------------------------------
    
    for(const char* type: {"byte", "int8", "int16", "int32", "int64", "int128", "uint8", "uint16", "uint32", "uint64", "uint128"})
	_binary_operation_table[std::make_pair("-", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateSub(lhs, rhs, "sub");
	};

    for(const char* type: {"float", "double"})
	_binary_operation_table[std::make_pair("-", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateFSub(lhs, rhs, "sub");
	};



    // ------------------------------------- mutiplication --------------------------------------
    
    for(const char* type: {"byte", "int8", "int16", "int32", "int64", "int128", "uint8", "uint16", "uint32", "uint64", "uint128"})
	_binary_operation_table[std::make_pair("*", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateMul(lhs, rhs, "mul");
	};

    for(const char* type: {"float", "double"})
	_binary_operation_table[std::make_pair("*", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateFMul(lhs, rhs, "mul");
	};



    // --------------------------------------- division ----------------------------------------
    
    for(const char* type: {"int8", "int16", "int32", "int64", "int128"})
	_binary_operation_table[std::make_pair("/", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateSDiv(lhs, rhs, "div");
	};

    for(const char* type: {"byte", "uint8", "uint16", "uint32", "uint64", "uint128"})
	_binary_operation_table[std::make_pair("/", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateUDiv(lhs, rhs, "div");
	};

    for(const char* type: {"float", "double"})
	_binary_operation_table[std::make_pair("/", get_type(type))] = [] (llvm::IRBuilderBase* b, llvm::Value* lhs, llvm::Value* rhs) {
	    return b->CreateFDiv(lhs, rhs, "div");
	};


}

auto global_context::add_int_cast(std::string&& from, std::string&& to) -> void {
    auto name = make_cast_name(from, to);

    auto from_type = get_type(std::move(from));
    auto to_type = get_type(std::move(to));
    auto key = std::make_pair(from_type, to_type);
    auto is_signed = from_type->is_signed() && to_type->is_signed();

    if(to == "bool")
	_casts[key] = [llvm_type = to_type->get(), name = std::move(name)] (llvm::IRBuilderBase* b, llvm::Value* v) {
	    return b->CreateICmpNE(v, llvm::ConstantInt::get(llvm_type, 0), name);
	};
    else
	_casts[key] = [llvm_type = to_type->get(), name = std::move(name), is_signed] (llvm::IRBuilderBase* b, llvm::Value* v) {
	    return b->CreateIntCast(v, llvm_type, is_signed, name);
	};
}

auto global_context::add_fp_cast(std::string&& from, std::string&& to) -> void {
    auto name = make_cast_name(from, to);

    auto from_type = get_type(std::move(from));
    auto to_type = get_type(std::move(to));
    auto key = std::make_pair(from_type, to_type);

    if(from == "float" && to == "double")
	_casts[key] = [llvm_type = to_type->get(), name = std::move(name)] (llvm::IRBuilderBase* b, llvm::Value* v) {
	    return b->CreateFPExt(v, llvm_type, name);
	};
    else if(to == "bool")
	_casts[key] = [&context = get(), name = std::move(name)] (llvm::IRBuilderBase* b, llvm::Value* v) {
	    return b->CreateFCmpULT(v, llvm::ConstantFP::get(context, llvm::APFloat(0.f)), name);
	};
    else if(from_type->is_signed())
	_casts[key] = [llvm_type = to_type->get(), name = std::move(name)] (llvm::IRBuilderBase* b, llvm::Value* v) {
	    return b->CreateSIToFP(v, llvm_type, name);
	};
    else
	_casts[key] = [llvm_type = to_type->get(), name = std::move(name)] (llvm::IRBuilderBase* b, llvm::Value* v) {
	    return b->CreateUIToFP(v, llvm_type, name);
	};
}
