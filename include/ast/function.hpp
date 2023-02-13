#pragma once

#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"

namespace ast {

    class function_definition_expression : public expression {
    private:
	std::string _name;
	std::vector<std::string> _args;
	std::unique_ptr<expression> _body;

    public:
	function_definition_expression(std::string&& name, std::vector<std::string>&& args, std::unique_ptr<expression>&& body)
	    : _name{std::move(name)}
	    , _args{std::move(args)}
	    , _body{std::move(body)}
	    {}
    };

}
