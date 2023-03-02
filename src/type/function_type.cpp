#include <algorithm>
#include <memory>

#include "type/function_type.hpp"

types::function_type::function_type(const std::vector<type*>& params, type* return_type) 
    : _param_type{params}
    , _return_type{return_type}
{
    std::vector<llvm::Type*> params_types;
    params_types.reserve(params.size());
    for(const auto& param: params)
	params_types.push_back(param->get());

    set_type(llvm::FunctionType::get(return_type->get(), params_types, false));
}

[[nodiscard]] auto types::function_type::is_signed() const noexcept -> bool {
    return false;
}

[[nodiscard]] auto types::function_type::begin() noexcept -> std::vector<type*>::iterator {
    return _param_type.begin();
}

[[nodiscard]] auto types::function_type::end() noexcept -> std::vector<type*>::iterator {
    return _param_type.end();
}

[[nodiscard]] auto types::function_type::get_return_type() const noexcept -> types::type* {
    return _return_type;
}

[[nodiscard]] auto types::function_type::get_num_params() const noexcept -> std::size_t {
    return _param_type.size();
}
