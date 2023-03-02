#include "semantic_analyzer.hpp"
#include "global_context.hpp"

namespace {

    auto cast_to(types::type* type) {
	return [type] (std::unique_ptr<ast::expression>&& expr) { 
	    return std::make_unique<ast::implicit_cast>(std::move(expr), type);
	};
    }

}

auto semantic_analyzer::visit(ast::expression* expr) -> types::type* {
    return expr->accept(this);
}

auto semantic_analyzer::visit(ast::integer_literal_expression* expr) -> types::type* {
    return expr->type() = global_context::type("int");
}

auto semantic_analyzer::visit(ast::floating_literal_expression* expr) -> types::type* {
    return expr->type() = global_context::type("double");
}

auto semantic_analyzer::visit(ast::character_literal_expression* expr) -> types::type* {
    return expr->type() = global_context::type("char");
}

auto semantic_analyzer::visit(ast::string_literal_expression* expr) -> types::type* {
    return expr->type() = global_context::type("string");
}

auto semantic_analyzer::visit(ast::variable_expression* expr) -> types::type* {
    return expr->type() = _sm.search_symbol(expr->name());
}

auto semantic_analyzer::visit(ast::binary_expression* expr) -> types::type* {
    types::type* lhs_type = expr->lhs()->accept(this);
    types::type* rhs_type = expr->rhs()->accept(this);
    
    /*
    types::type* common_type;
    if(global_context::cast(lhs_type, rhs_type)) {
	expr->insert_lhs_cast(cast_to(rhs_type));
	common_type = rhs_type;
    } else if(global_context::cast(rhs_type, lhs_type)) {
	expr->insert_rhs_cast(cast_to(lhs_type));
	common_type = lhs_type;
    }

    return expr->type() = common_type;
    */
    return expr->type() = lhs_type;
}

auto semantic_analyzer::visit(ast::call_expression* expr) -> types::type* {
    types::function_type* func_type = _sm.search_function(expr->callee());
    if(!func_type)
	return nullptr;

    if(func_type->get_num_params() != expr->args().size())
	return nullptr;

    auto arg_type = func_type->begin();
    auto arg = expr->args().begin();
    for(; arg != expr->args().end(); ++arg, ++arg_type) {
	if(auto type = (*arg)->accept(this); !type /*|| !global_context::cast(type, *arg_type)*/)
	    return nullptr;
	else if(type != *arg_type)
	    expr->insert_arg_cast(arg, cast_to(*arg_type));
    }

    return expr->type() = func_type->get_return_type();
}

auto semantic_analyzer::visit(ast::function_expression* expr) -> types::type* {
    types::function_type* func_type = global_context::type(expr->types(), expr->return_type());
    _sm.new_function(expr->name(), func_type);
    _sm.new_scope();

    auto param_type = func_type->begin();
    for(const auto& arg: expr->args())
	_sm.new_symbol(arg, *param_type++);

    if(auto type = expr->body()->accept(this), return_type = func_type->get_return_type();
	    !type /*|| !global_context::cast(type, return_type)*/) 
	return nullptr;
    else if(type != func_type->get_return_type())
	expr->body()->insert_result_cast(cast_to(return_type));

    return expr->type() = func_type;
}

auto semantic_analyzer::visit(ast::block_expression* expr) -> types::type* {
    types::type* t{};
    for(const auto& e: expr->expressions()) {
	t = e->accept(this);
	if(!t)
	    return nullptr;
    }

    _sm.delete_scope();
    return expr->type() = t;
}
