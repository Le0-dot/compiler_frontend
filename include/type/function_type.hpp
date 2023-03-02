#pragma once

#include <vector>
#include <memory>

#include <llvm/IR/Function.h>

#include "type.hpp"

namespace types {

    class function_type : public type {
    private:
	std::vector<type*> _param_type;
	type* _return_type;

    public:
	function_type(const std::vector<type*>&, type*);
	
	function_type()                                        = default;
	function_type(const function_type&)                    = delete;
	function_type(function_type&&)                         = default;
	auto operator=(const function_type&) -> function_type& = delete;
	auto operator=(function_type&&)      -> function_type& = default;
	virtual ~function_type()                               = default;

	[[nodiscard]] virtual auto is_signed() const noexcept -> bool override final;

	[[nodiscard]] auto begin()                 noexcept -> std::vector<type*>::iterator;
	[[nodiscard]] auto end()                   noexcept -> std::vector<type*>::iterator;
	[[nodiscard]] auto get_return_type() const noexcept -> type*;
	[[nodiscard]] auto get_num_params()  const noexcept -> std::size_t;
    };

}
