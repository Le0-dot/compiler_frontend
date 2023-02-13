#pragma once

#include <memory>
#include <string>

#include "expression.hpp"

namespace ast {

    class binary_expression : public expression {
    private:
	std::string _operator;
	std::unique_ptr<expression> _lhs, _rhs;

    public:
	binary_expression(std::string&& op, std::unique_ptr<expression>&& lhs, std::unique_ptr<expression>&& rhs)
	    : _operator{std::move(op)}
	    , _lhs{std::move(lhs)}
	    , _rhs{std::move(rhs)}
	    {}
    };

}
