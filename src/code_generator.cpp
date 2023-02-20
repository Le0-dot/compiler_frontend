#include <cstdio>
#include <algorithm>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <ranges>

#include "ast/literal.hpp"
#include "code_generator.hpp"

code_generator::code_generator(const std::string& module_name)
    : _context{std::make_unique<llvm::LLVMContext>()}
    , _module{std::make_unique<llvm::Module>(module_name, *_context)}
    , _builder{std::make_unique<llvm::IRBuilder<>>(*_context)}
{
    add_default_types();
}

auto code_generator::visit(const ast::expression* expr) -> llvm::Value* {
    return expr->accept(this);
}

auto code_generator::visit(const ast::literal_expression* expr) -> llvm::Value* {
    switch (expr->type()) {
	case ast::literal_types::binary:
	    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*_context), expr->value(), 2);
	case ast::literal_types::octal:
	    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*_context), expr->value(), 8);
	case ast::literal_types::decimal:
	    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*_context), expr->value(), 10);
	case ast::literal_types::hexadecimal:
	    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*_context), expr->value(), 16);
	case ast::literal_types::floating:
	    return llvm::ConstantFP::get(*_context, llvm::APFloat(std::stod(expr->value())));
	case ast::literal_types::character:
	    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*_context), static_cast<uint8_t>(expr->value()[0]));
	case ast::literal_types::string:
	    return nullptr;
	default:
	    fprintf(stderr, "error: unrecognised constant type");
	    return nullptr;
    }
}

auto code_generator::visit(const ast::variable_expression* expr) -> llvm::Value* {
    llvm::Value* v = _named_values[expr->name()];
    if(!v) {
	fprintf(stderr, "error: unknown variable name");
	return nullptr;
    }
    return v;
}

auto code_generator::visit(const ast::binary_expression* expr) -> llvm::Value* {
    llvm::Value *lhs = visit(expr->lhs()), *rhs = visit(expr->rhs());
    if(!lhs || !rhs)
	return nullptr;

    switch (expr->op()[0]) {
	case '+':
	    return _builder->CreateFAdd(lhs, rhs, "addtmp");
	case '-':
	    return _builder->CreateFSub(lhs, rhs, "subtmp");
	case '*':
	    return _builder->CreateFMul(lhs, rhs, "multmp");
	case '/':
	    return _builder->CreateFDiv(lhs, rhs, "divtmp");
	default:
	    fprintf(stderr, "error: to be done");
	    return nullptr;
    }
}

auto code_generator::visit(const ast::call_expression* expr) -> llvm::Value* {
    llvm::Function* callee = _module->getFunction(expr->callee());
    if(!callee) {
	fprintf(stderr, "error: unknown function reference");
	return nullptr;
    }

    if(callee->arg_size() != expr->args().size()) {
	fprintf(stderr, "error: incorrect number of argument expected %zu, given %zu", callee->arg_size(), expr->args().size());
	return nullptr;
    }

    std::vector<llvm::Value*> arg_values;
    for(const auto& arg: expr->args()) {
	arg_values.emplace_back(visit(arg.get()));
	if(!arg_values.back())
	    return nullptr;
    }

    return _builder->CreateCall(callee, arg_values, "calltmp");
}

auto code_generator::visit(const ast::function_expression* expr) -> llvm::Value* {
    // set up argument list types
    std::vector<llvm::Type*> args_type;
    args_type.reserve(expr->args().size());
    for(const auto& [_, type_str]: expr->args()) {
	llvm::Type* type = _type_table[type_str];
	if(!type) {
	    fprintf(stderr, "error: unknown type \"%s\"", type_str.data());
	    return nullptr;
	}
	args_type.push_back(type);
    }

    // set up return type and function type
    llvm::Type* return_type = llvm::Type::getDoubleTy(*_context);
    llvm::FunctionType* function_type = llvm::FunctionType::get(return_type, args_type, false);

    // create function
    llvm::Function* function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, expr->name(), _module.get());

    // set arguments names, types and add fuction arguments to named values
    _named_values.clear();
    std::ranges::for_each(expr->args(), [this, arg = function->args().begin()] (const auto arg_pair) mutable {
	arg->setName(arg_pair.first);
	_named_values[arg_pair.first] = &*arg++;
	fprintf(stderr, "found argument with name = \"%s\"; type = \"%s\";\n", arg_pair.first.data(), arg_pair.second.data());
    });

    // create basic block to write to
    llvm::BasicBlock* block = llvm::BasicBlock::Create(*_context, "entry", function);
    _builder->SetInsertPoint(block);

    if(llvm::Value* return_value = visit(expr->body())) {
	_builder->CreateRet(return_value);

	llvm::verifyFunction(*function);

	return function;
    }

    function->eraseFromParent();
    return nullptr;
}

auto code_generator::add_default_types() -> void {
    _type_table["int"] = llvm::Type::getInt32Ty(*_context);
    _type_table["int8"] = llvm::Type::getInt8Ty(*_context);
    _type_table["int16"] = llvm::Type::getInt16Ty(*_context);
    _type_table["int32"] = llvm::Type::getInt32Ty(*_context);
    _type_table["int64"] = llvm::Type::getInt64Ty(*_context);
    _type_table["int128"] = llvm::Type::getInt128Ty(*_context);

    _type_table["uint"] = llvm::Type::getInt32Ty(*_context);
    _type_table["uint8"] = llvm::Type::getInt8Ty(*_context);
    _type_table["uint16"] = llvm::Type::getInt16Ty(*_context);
    _type_table["uint32"] = llvm::Type::getInt32Ty(*_context);
    _type_table["uint64"] = llvm::Type::getInt64Ty(*_context);
    _type_table["uint128"] = llvm::Type::getInt128Ty(*_context);

    _type_table["float"] = llvm::Type::getFloatTy(*_context);
    _type_table["bfloat"] = llvm::Type::getBFloatTy(*_context);
    _type_table["double"] = llvm::Type::getDoubleTy(*_context);
}
