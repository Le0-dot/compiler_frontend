#include <cstdio>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/Constants.h>

#include "code_generator.hpp"

code_generator::code_generator(const std::string& module_name)
    : _context{std::make_unique<llvm::LLVMContext>()}
    , _module{std::make_unique<llvm::Module>(module_name, *_context)}
    , _builder{std::make_unique<llvm::IRBuilder<>>(*_context)}
{}

auto code_generator::visit(const ast::expression* expr) -> llvm::Value* {
    return expr->accept(this);
}

auto code_generator::visit(const ast::literal_expression* expr) -> llvm::Value* {
    return llvm::ConstantFP::get(*_context, llvm::APFloat((float)(std::stoi(expr->value()))));
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
    // TODO
}
