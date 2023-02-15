#include <cstdio>
#include <memory>

#include "ast.hpp"
#include "lexer.hpp"
#include "operator_table.hpp"
#include "parser.hpp"

parser::parser(lexer&& _lexer, operator_table&& _table) 
    : _lexer{std::move(_lexer)}
    , _table{std::move(_table)}
{}

// literal_expression ::= literal
[[nodiscard]] auto parser::parse_literal() -> std::unique_ptr<ast::expression> {
    ast::literal_types type;
    switch(_lexer.token()) {
	case tokens::decimal: [[fallthrough]];
	case tokens::hexadecimal: [[fallthrough]];
	case tokens::octal: [[fallthrough]];
	case tokens::binary:
	    type = ast::literal_types::integer;
	case tokens::floating:
	    type = ast::literal_types::floating;
	case tokens::character:
	    type = ast::literal_types::character;
	case tokens::string:
	    type = ast::literal_types::string;
	default:
	    fprintf(stderr, "error: unrecognised literal type");
	    return nullptr;
    }
    _lexer.consume();
    return std::make_unique<ast::literal_expression>(std::move(_lexer.identifier()), type);
}

// parenthesis ::= '(' expression ')'
[[nodiscard]] auto parser::parse_parenthesis() -> std::unique_ptr<ast::expression> {
    _lexer.consume();

    auto expr = parse_expression();
    if(!expr)
	return nullptr;

    if(_lexer.token() != tokens::right_parenthesis) {
	fprintf(stderr, "error: expected ')'");
	return nullptr;
    }

    _lexer.consume();
    return expr;
}

// indentifierExpr 
//			::= indentifier
//			::= indentifier '(' expression* ')'
[[nodiscard]] auto parser::parse_indentifier() -> std::unique_ptr<ast::expression> {
    std::string identifier = _lexer.identifier();
    _lexer.consume();

    if(_lexer.token() != tokens::left_parenthesis)
	return std::make_unique<ast::variable_expression>(std::move(identifier));

    std::vector<std::unique_ptr<ast::expression>> args;

    while(_lexer.token() != tokens::right_parenthesis) {
	_lexer.consume();

	if(auto arg = parse_expression())
	    args.emplace_back(std::move(arg));
	else
	    return nullptr;

	if(auto t = _lexer.token(); 
		t != tokens::right_parenthesis && t != tokens::comma) {
	    fprintf(stderr, "error: expected ')' or ','");
	    return nullptr;
	}
    }
    _lexer.consume();

    return std::make_unique<ast::call_expression>(std::move(identifier), std::move(args));
}

// primary 
//		::= indentifierExpr
//		::= literal
//		::= parenthesis
[[nodiscard]] auto parser::parse_primary() -> std::unique_ptr<ast::expression> {
    switch (_lexer.token()) {
	case tokens::identifier:
	    return parse_indentifier();
	case tokens::decimal: [[fallthrough]];
	case tokens::hexadecimal: [[fallthrough]];
	case tokens::octal: [[fallthrough]];
	case tokens::binary: [[fallthrough]];
	case tokens::character: [[fallthrough]];
	case tokens::string: 
	    return parse_literal();
	case tokens::left_parenthesis:
	    return parse_parenthesis();
	default:
	    fprintf(stderr, "error: unknown token in expression");
	    return nullptr;
    }
}

// expression ::= primary rhsExpr
[[nodiscard]] auto parser::parse_expression() -> std::unique_ptr<ast::expression> {
    auto lhs = parse_primary();
    if(!lhs)
	return nullptr;

    return parse_binary_rhs(0, std::move(lhs));
}

// binary ::= (op prmary)*
[[nodiscard]] auto parser::parse_binary_rhs(uint8_t precedence, std::unique_ptr<ast::expression>&& lhs) -> std::unique_ptr<ast::expression> {
    while(true) {
	uint16_t current_precedence = [](){return 0;}(); // placeholder

	if(current_precedence < precedence)
	    return lhs;

	std::string op = _lexer.identifier();
	_lexer.consume();

	auto rhs = parse_primary();
	if(!rhs)
	    return nullptr;

	uint16_t next_precedence = [](){return 0;}(); // placeholder
	if(current_precedence < next_precedence) {
	    rhs = parse_binary_rhs(current_precedence + 1, std::move(rhs));
	    if(!rhs)
		return nullptr;
	}

	lhs = std::make_unique<ast::binary_expression>(std::move(op), std::move(lhs), std::move(rhs));
    }
}

[[nodiscard]] auto parser::parse_function() -> std::unique_ptr<ast::expression> {
    if(_lexer.identifier() != "function") {
	fprintf(stderr, "expected 'function' in function definition");
	return nullptr;
    }
    _lexer.consume();

    std::string name{};
    if(_lexer.token() == tokens::identifier) {
	name = std::move(_lexer.identifier());
	_lexer.consume();
    }

    fprintf(stderr, "name is %s\n", name.data());
    
    if(_lexer.token() != tokens::left_parenthesis) {
	fprintf(stderr, "error: expected '(' in function definition");
	return nullptr;
    }

    fprintf(stderr, "found lp\n");

    _lexer.consume();
    std::vector<std::string> args;
    while(_lexer.token() == tokens::identifier) {
	args.emplace_back(std::move(_lexer.identifier()));
	_lexer.consume();
	if(_lexer.token() == tokens::comma)
	    _lexer.consume();
    }

    for(const auto& s: args)
	fprintf(stderr, "%s ", s.data());
    fprintf(stderr, "\n");

    if(_lexer.token() != tokens::right_parenthesis) {
	fprintf(stderr, "error: expected ')'");
	return nullptr;
    }
    _lexer.consume();

    auto body = parse_expression();
    if(!body)
	return nullptr;

    return std::make_unique<ast::function_expression>(std::move(name), std::move(args), std::move(body));
}
