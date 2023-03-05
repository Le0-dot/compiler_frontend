#include <limits>
#include <ranges>

#include "semantic_analyzer.hpp"
#include "global_context.hpp"

namespace {

    auto cast_to(types::type* type) {
	return [type] (std::unique_ptr<ast::expression>&& expr) { 
	    return std::make_unique<ast::implicit_cast>(std::move(expr), type);
	};
    }

    template<std::ranges::forward_range Range, typename Value>
    requires std::same_as<std::ranges::range_value_t<Range>, std::tuple<Value, Value, std::string>>
    auto find_in_range_or_default(Range&& range, const Value& val, const std::string& _default) {
	auto res = std::ranges::find_if(std::forward<Range&&>(range), [&val] (auto t) {
	    const auto& [lower, upper, _] = t;
	    return lower <= val && val <= upper;
	});
	return res == std::end(range) ? _default : get<2>(*res);
    }
}

auto semantic_analyzer::visit(ast::expression* expr) -> types::type* {
    return expr->accept(this);
}

auto semantic_analyzer::visit(ast::integer_literal_expression* expr) -> types::type* {
    using namespace std::literals;
    const static std::vector<std::tuple<ast::integer_container, ast::integer_container, std::string>> unsigned_ranges {
	{"0", std::to_string(std::numeric_limits<uint8_t>::max()), "uint8"},
	{"0", std::to_string(std::numeric_limits<uint16_t>::max()), "uint16"},
	{"0", std::to_string(std::numeric_limits<uint32_t>::max()), "uint32"},
	{"0", std::to_string(std::numeric_limits<uint64_t>::max()), "uint64"},
    };
    const static std::vector<std::tuple<ast::integer_container, ast::integer_container, std::string>> signed_ranges {
	{std::to_string(std::numeric_limits<int8_t>::min()), std::to_string(std::numeric_limits<int8_t>::max()), "int8"},
	{std::to_string(std::numeric_limits<int16_t>::min()), std::to_string(std::numeric_limits<int16_t>::max()), "int16"},
	{std::to_string(std::numeric_limits<int32_t>::min()), std::to_string(std::numeric_limits<int32_t>::max()), "int32"},
	{std::to_string(std::numeric_limits<int64_t>::min()), std::to_string(std::numeric_limits<int64_t>::max()), "int64"},
    };

    types::type* final_type{};

    if(const auto& v = expr->value(); v >= "0"s)
	final_type = global_context::type(find_in_range_or_default(unsigned_ranges, v, "uint128"));
    else
	final_type = global_context::type(find_in_range_or_default(signed_ranges, v, "int128"));

    return expr->type() = final_type;
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
    if(!lhs_type || !rhs_type)
	return nullptr;
    
    types::type* common_type;
    if(lhs_type == rhs_type) {
	common_type = lhs_type;
    } else if(global_context::cast(lhs_type, rhs_type)) {
	expr->insert_lhs_cast(cast_to(rhs_type));
	common_type = rhs_type;
    } else if(global_context::cast(rhs_type, lhs_type)) {
	expr->insert_rhs_cast(cast_to(lhs_type));
	common_type = lhs_type;
    } else {
	fprintf(stderr, "error: unable to cast binary expression to common type: \"%s\" and \"%s\"", lhs_type->name().data(), rhs_type->name().data());
    }

    if(!global_context::binary_operation(expr->op(), common_type)) {
	fprintf(stderr, "error: no suitable \"%s\" operation for type \"%s\"", expr->op().data(), common_type->name().data());
	return nullptr;
    }

    return expr->type() = common_type;
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
	if(auto type = (*arg)->accept(this); !type)
	    return nullptr;
	else if(type == *arg_type)
	    continue;
	else if(!global_context::cast(type, *arg_type)) {
	    fprintf(stderr, "error: unable to cast function call argument: \"%s\" required, \"%s\" given", (*arg_type)->name().data(), type->name().data());
	    return nullptr;
	} else if(type != *arg_type)
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

    if(auto type = expr->body()->accept(this), return_type = func_type->get_return_type(); !type)
	return nullptr;
    else if(type == return_type)
	return expr->type() = func_type;
    else if(!global_context::cast(type, return_type)) {
	fprintf(stderr, "error: uncompatable return value type \"%s\" and return function type \"%s\"", type->name().data(), return_type->name().data());
	return nullptr;
    } else if(type != func_type->get_return_type())
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
