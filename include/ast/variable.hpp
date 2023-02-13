#pragma once

#include <string>

#include "expression.hpp"

namespace ast {

    class variable_expression : public expression {
    private:
	std::string _name;

    public:
	variable_expression(std::string&& name) : _name{std::move(name)} {}
    };

}
