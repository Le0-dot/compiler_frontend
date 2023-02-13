#pragma once

#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"

namespace ast {

    class call_expression : public expression {
    private:
	std::string _callee;
	std::vector<std::unique_ptr<expression>> _args;

    public:
	call_expression(std::string&& callee, std::vector<std::unique_ptr<expression>>&& args)
	    : _callee{std::move(callee)}
	    , _args{std::move(args)}
	    {}
    };

}
