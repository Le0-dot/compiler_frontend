#pragma once

#include <string>

#include "expression.hpp"

namespace ast {

    enum class literal_types : uint8_t {
	integer,
	floating,
	character,
	string
    };


    class literal_expression : public expression {
    private:
	std::string _value;
	literal_types _type;

    public:
	literal_expression(std::string&& literal, literal_types type) 
	    : _value{std::move(literal)} 
	    , _type{type} 
	    {}
    };

}
