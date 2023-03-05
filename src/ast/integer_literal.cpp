#include "ast/integer_literal.hpp"
#include "ast/visitor.hpp"
#include <compare>

ast::integer_literal_expression::integer_literal_expression(std::string&& value, uint8_t base)
    : _value{std::move(value)}
    , _radix{base}
{}

[[nodiscard]] auto ast::integer_literal_expression::accept(value_visitor* v) const -> llvm::Value* {
    fprintf(stderr, "int accept value\n");
    return v->visit(this);
}

[[nodiscard]] auto ast::integer_literal_expression::accept(type_visitor* v) -> types::type* {
    fprintf(stderr, "int accept type\n");
    return v->visit(this);
}

[[nodiscard]] auto ast::integer_literal_expression::value() const -> const integer_container& {
    return _value;
}

[[nodiscard]] auto ast::integer_literal_expression::radix() const -> uint8_t {
    return _radix;
}

[[nodiscard]] auto ast::integer_container::operator<=>(const ast::integer_container& o) const -> std::strong_ordering {
    const auto* s1 = this, * s2 = &o;
    if((*this)[0] == '-' && o[0] == '-') // both negative
	std::swap(s1, s2);
    else if((*this)[0] == '-') // this negative
	return std::strong_ordering::less;
    else if(o[0] == '-') // o negative
	return std::strong_ordering::greater;
    
    int res = s1->compare(*s2);
    if(res > 0 || (res == 0 && s1->size() > s2->size())) // this greater
	return std::strong_ordering::greater;
    else if(res < 0 || (res == 0 && s1->size() < s2->size())) // o greater
	return std::strong_ordering::less;
    // equals
    return std::strong_ordering::equal;
}

[[nodiscard]] auto ast::integer_container::operator<=>(const std::string& o) const -> std::strong_ordering {
    return this->operator<=>(integer_container(o));
}
