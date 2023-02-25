#include "semantic_analyzer.hpp"
#include "global_context.hpp"

namespace {

    auto cast_to(llvm::Type* type) {
	return [type] (std::unique_ptr<ast::expression>&& expr) { 
	    return std::make_unique<ast::implicit_cast>(std::move(expr), type);
	};
    }

}

auto semantic_analyzer::visit(ast::expression* expr) -> llvm::Type* {
    return expr->accept(this);
}

auto semantic_analyzer::visit(ast::integer_literal_expression* expr) -> llvm::Type* {
    return expr->type() = global_context::type("int");
}

auto semantic_analyzer::visit(ast::floating_literal_expression* expr) -> llvm::Type* {
    return expr->type() = global_context::type("double");
}

auto semantic_analyzer::visit(ast::character_literal_expression* expr) -> llvm::Type* {
    return expr->type() = global_context::type("char");
}

auto semantic_analyzer::visit(ast::string_literal_expression* expr) -> llvm::Type* {
    return expr->type() = global_context::type("string");
}

auto semantic_analyzer::visit(ast::variable_expression* expr) -> llvm::Type* {
    return expr->type() = _sm.search_symbol(expr->name());
}

auto semantic_analyzer::visit(ast::binary_expression* expr) -> llvm::Type* {
    llvm::Type* lhs_type = expr->lhs()->accept(this);
    llvm::Type* rhs_type = expr->rhs()->accept(this);
    return expr->type() = lhs_type; // find common type between lhs and rhs
}

auto semantic_analyzer::visit(ast::call_expression* expr) -> llvm::Type* {
    llvm::FunctionType* func_type = _sm.search_function(expr->callee());
    if(!func_type)
	return nullptr;

    if(func_type->getNumParams() != expr->args().size())
	return nullptr;
    auto arg_type = func_type->param_begin();
    auto arg = expr->args().begin();
    for(; arg != expr->args().end(); ++arg, ++arg_type) {
	if(auto type = (*arg)->accept(this); !type /* || check if conversion is impossible*/)
	    return nullptr;
	else if(type != *arg_type)
	    expr->insert_arg_cast(arg, cast_to(*arg_type));
    }

    return expr->type() = func_type->getReturnType();
}

auto semantic_analyzer::visit(ast::function_expression* expr) -> llvm::Type* {
    llvm::FunctionType* func_type = global_context::type(expr->types(), expr->return_type());
    _sm.new_function(expr->name(), func_type);
    _sm.new_scope();

    auto param_type = func_type->param_begin();
    for(const auto& arg: expr->args())
	_sm.new_symbol(arg, *param_type++);

    if(auto type = expr->body()->accept(this); !type /* || check if conversion is impossible*/) 
	return nullptr;
    else if(type != func_type->getReturnType())
	expr->body()->insert_result_cast(cast_to(func_type->getReturnType()));

    return expr->type() = func_type;
}

auto semantic_analyzer::visit(ast::block_expression* expr) -> llvm::Type* {
    llvm::Type* type{};
    for(const auto& e: expr->expressions()) {
	type = e->accept(this);
	if(!type)
	    return nullptr;
    }

    _sm.delete_scope();
    return expr->type() = type;
}
