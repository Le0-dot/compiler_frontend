#include "type/integer_type.hpp"

types::integer_type::integer_type(llvm::IntegerType* type, std::string&& name) 
    : types::type{type, std::move(name)}
{}
