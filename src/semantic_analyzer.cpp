#include "semantic_analyzer.hpp"
#include "global_context.hpp"

auto semantic_analyzer::visit(const ast::expression* expr) -> llvm::Type* {
    return expr->accept(this);
}

auto semantic_analyzer::visit(const ast::integer_literal_expression* expr) -> llvm::Type* {
    return global_context::type("int");
}

auto semantic_analyzer::visit(const ast::floating_literal_expression* expr) -> llvm::Type* {
    return global_context::type("double");
}

auto semantic_analyzer::visit(const ast::character_literal_expression* expr) -> llvm::Type* {
    return global_context::type("char");
}

auto semantic_analyzer::visit(const ast::string_literal_expression* expr) -> llvm::Type* {
    return global_context::type("string");
}

auto semantic_analyzer::visit(const ast::variable_expression* expr) -> llvm::Type* {
    return _sm.search_symbol(expr->name());
}

auto semantic_analyzer::visit(const ast::binary_expression* expr) -> llvm::Type* {
    llvm::Type* lhs_type = expr->lhs()->accept(this);
    llvm::Type* rhs_type = expr->rhs()->accept(this);
    return lhs_type; // find common type between lhs and rhs
}

auto semantic_analyzer::visit(const ast::call_expression* expr) -> llvm::Type* {
    llvm::FunctionType* func_type = _sm.search_function(expr->callee());
    if(!func_type)
	return nullptr;

    if(func_type->getNumParams() != expr->args().size())
	return nullptr;
    auto param_type = func_type->param_begin();
    for(const auto& arg: expr->args())
	if(arg->accept(this) != *param_type++)
	    return nullptr;

    return func_type->getReturnType();
}

auto semantic_analyzer::visit(const ast::function_expression* expr) -> llvm::Type* {
    llvm::FunctionType* func_type = global_context::type(expr->types(), expr->return_type());
    _sm.new_function(expr->name(), func_type);
    _sm.new_scope();

    auto param_type = func_type->param_begin();
    for(const auto& arg: expr->args())
	_sm.new_symbol(arg, *param_type++);

    if(expr->body()->accept(this) != func_type->getReturnType())
	return nullptr;

    return func_type;
}

auto semantic_analyzer::visit(const ast::block_expression* expr) -> llvm::Type* {
    llvm::Type* type{};
    for(const auto& e: expr->expressions()) {
	type = e->accept(this);
	if(!type)
	    return nullptr;
    }

    _sm.delete_scope();
    return type;
}
