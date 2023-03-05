#include <cstdio>
#include <algorithm>
#include <ranges>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include "ast.hpp"
#include "code_generator.hpp"
#include "global_context.hpp"
#include "tables.hpp"

code_generator::code_generator(const std::string& module_name)
    : _module{std::make_unique<llvm::Module>(module_name, global_context::context())}
    , _builder{std::make_unique<llvm::IRBuilder<>>(global_context::context())}
{}

auto code_generator::visit(const ast::expression* expr) -> llvm::Value* {
    return expr->accept(this);
}

auto code_generator::visit(const ast::integer_literal_expression* expr) -> llvm::Value* {
    fprintf(stderr, "%s - %d - %s", expr->value().data(), expr->radix(), expr->type()->name().data());
    auto type = static_cast<llvm::IntegerType*>(expr->type()->get());
    return llvm::ConstantInt::get(type, expr->value(), expr->radix());
}

auto code_generator::visit(const ast::floating_literal_expression* expr) -> llvm::Value* {
    return llvm::ConstantFP::get(global_context::context(), llvm::APFloat(expr->value()));
}

auto code_generator::visit(const ast::character_literal_expression* expr) -> llvm::Value* {
    return llvm::ConstantInt::get(expr->type()->get(), expr->value());
}

auto code_generator::visit(const ast::string_literal_expression* expr) -> llvm::Value* {
    return nullptr;
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

    auto op_func = global_context::binary_operation(expr->op(), expr->type());
    return op_func(_builder.get(), lhs, rhs);
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
    llvm::FunctionType* func_type = static_cast<llvm::FunctionType*>(expr->type()->get());
    fprintf(stderr, "got function type\n");
    // create function
    llvm::Function* function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, expr->name(), _module.get());
    fprintf(stderr, "created function\n");

    // set arguments names, types and add fuction arguments to named values
    _named_values.clear();
    std::ranges::for_each(expr->args(), [this, farg = function->arg_begin()] (const auto& arg) mutable {
	farg->setName(arg);
	_named_values[arg] = farg++;
    });

    // create basic block to write to
    llvm::BasicBlock* block = llvm::BasicBlock::Create(global_context::context(), "entry", function);
    _builder->SetInsertPoint(block);

    if(llvm::Value* return_value = visit(expr->body())) {
	_builder->CreateRet(return_value);

	llvm::verifyFunction(*function);

	return function;
    }

    function->eraseFromParent();
    return nullptr;
}

auto code_generator::visit(const ast::block_expression* expr) -> llvm::Value* {
    if(expr->expressions().size() == 1)
	return visit(expr->expressions().front().get());

    for(auto it = expr->expressions().begin(); it != expr->expressions().end() - 1; ++it) {
	if(!visit(it->get()))
	    return nullptr;
    }
    return visit(expr->expressions().back().get());
}

auto code_generator::visit(const ast::implicit_cast* cast) -> llvm::Value* {
    llvm::Value* subject_value = cast->subject()->accept(this);
    std::function<cast_function> cast_func = global_context::cast(cast->subject()->type(), cast->type());
    return cast_func(_builder.get(), subject_value);
}
