#include <cstdio>
#include <memory>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

parser::parser(lexer&& _lexer, operator_table&& _table) 
    : _lexer{std::move(_lexer)}
    , _table{std::move(_table)}
{}

// literal_expression ::= literal
[[nodiscard]] auto parser::parse_literal() -> std::unique_ptr<ast::expression> {
    ast::literal_types type;
    switch(_lexer.token()) {
	case tokens::binary:
	    type = ast::literal_types::binary;
	    break;
	case tokens::octal: 
	    type = ast::literal_types::octal;
	    break;
	case tokens::decimal: 
	    type = ast::literal_types::decimal;
	    break;
	case tokens::hexadecimal:
	    type = ast::literal_types::hexadecimal;
	    break;
	case tokens::floating:
	    type = ast::literal_types::floating;
	    break;
	case tokens::character:
	    type = ast::literal_types::character;
	    break;
	case tokens::string:
	    type = ast::literal_types::string;
	    break;
	default:
	    fprintf(stderr, "error: unrecognised literal type");
	    return nullptr;
    }
    auto literal = std::move(_lexer.identifier());
    _lexer.consume();
    return std::make_unique<ast::literal_expression>(std::move(literal), type);
}

// parenthesis ::= '(' expression ')'
[[nodiscard]] auto parser::parse_parenthesis() -> std::unique_ptr<ast::expression> {
    fprintf(stderr, "parsing parenthesis with token = \"%s\"\n", _lexer.identifier().data());
    _lexer.consume();

    auto expr = parse_expression();
    if(!expr)
	return nullptr;

    if(_lexer.token() != tokens::right_parenthesis) {
	fprintf(stderr, "error: expected ')', found \"%s\"", _lexer.identifier().data());
	return nullptr;
    }

    _lexer.consume();
    fprintf(stderr, "finished parsing parenthesis with token = \"%s\"\n", _lexer.identifier().data());
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
    fprintf(stderr, "parsing primary exprssion\n");
    switch (_lexer.token()) {
	case tokens::identifier:
	    return parse_indentifier();
	case tokens::decimal: [[fallthrough]];
	case tokens::hexadecimal: [[fallthrough]];
	case tokens::octal: [[fallthrough]];
	case tokens::binary: [[fallthrough]];
	case tokens::floating: [[fallthrough]];
	case tokens::character: [[fallthrough]];
	case tokens::string: 
	    return parse_literal();
	case tokens::left_parenthesis:
	    return parse_parenthesis();
	default:
	    fprintf(stderr, "error: unknown token in expression: %d - \"%s\"", _lexer.token(), _lexer.identifier().data());
	    return nullptr;
    }
}

// expression ::= primary binary
[[nodiscard]] auto parser::parse_expression() -> std::unique_ptr<ast::expression> {
    fprintf(stderr, "parsing expression with token = \"%s\"\n", _lexer.identifier().data());
    auto lhs = parse_primary();
    if(!lhs)
	return nullptr;

    return parse_binary_rhs(1, std::move(lhs));
}

// binary ::= (op prmary)*
[[nodiscard]] auto parser::parse_binary_rhs(uint8_t precedence, std::unique_ptr<ast::expression>&& lhs) -> std::unique_ptr<ast::expression> {
    fprintf(stderr, "parsing binary rhs with token = \"%s\"\n", _lexer.identifier().data());
    while(_lexer.token() == tokens::identifier) {
	uint16_t current_precedence = _table[_lexer.identifier()];

	if(current_precedence < precedence)
	    return lhs;

	std::string op = std::move(_lexer.identifier());
	_lexer.consume();

	auto rhs = parse_primary();
	if(!rhs)
	    return nullptr;

	uint16_t next_precedence = _table[_lexer.identifier()];
	if(current_precedence < next_precedence) {
	    rhs = parse_binary_rhs(current_precedence + 1, std::move(rhs));
	    if(!rhs)
		return nullptr;
	}

	lhs = std::make_unique<ast::binary_expression>(std::move(op), std::move(lhs), std::move(rhs));
    }
    fprintf(stderr, "finished parsing binary rhs with token = \"%s\"\n", _lexer.identifier().data());
    return lhs;
}

[[nodiscard]] auto parser::parse_function() -> std::unique_ptr<ast::expression> {
    // check if function definition starts with 'function' key word
    if(_lexer.identifier() != "function") {
	fprintf(stderr, "expected 'function' in function definition");
	return nullptr;
    }
    _lexer.consume();

    // parse name if there is one
    std::string name{};
    if(_lexer.token() == tokens::identifier) {
	name = std::move(_lexer.identifier());
	_lexer.consume();
    }

    // check for '(' before argument list
    if(_lexer.token() != tokens::left_parenthesis) {
	fprintf(stderr, "error: expected '(' in function definition");
	return nullptr;
    }
    _lexer.consume();

    // parse argument list in form of: arg_name arg_type
    std::vector<std::pair<std::string, std::string>> args;
    while(_lexer.token() == tokens::identifier) {
	auto arg_name = std::move(_lexer.identifier());
	_lexer.consume();

	if(_lexer.token() != tokens::identifier) {
	    fprintf(stderr, "error: expected argument type after argument name, found: \"%s\"", _lexer.identifier().data());
	    return nullptr;
	}

	args.emplace_back(std::move(arg_name), std::move(_lexer.identifier()));
	_lexer.consume();

	if(_lexer.token() == tokens::comma)
	    _lexer.consume();
    }

    // check for ')' after argument list
    if(_lexer.token() != tokens::right_parenthesis) {
	fprintf(stderr, "error: expected ')'");
	return nullptr;
    }
    _lexer.consume();

    std::string return_type{};
    if(_lexer.token() == tokens::identifier) {
	return_type = std::move(_lexer.identifier());
	_lexer.consume();
    }


    // parse body of a function
    auto body = parse_block();
    if(!body)
	return nullptr;
    fprintf(stderr, "finished parsing block\n");

    return std::make_unique<ast::function_expression>(std::move(name), std::move(return_type), std::move(args), std::move(body));
}

[[nodiscard]] auto parser::parse_block() -> std::unique_ptr<ast::expression> {
    if(_lexer.token() != tokens::eol && _lexer.token() != tokens::left_curly_brace) {
	fprintf(stderr, "error: expected new line or '{' in the beginning of the block");
	return nullptr;
    }

    std::vector<std::unique_ptr<ast::expression>> expressions;
    if(_lexer.token() == tokens::eol) { // found eol
	_lexer.consume();
	fprintf(stderr, "found eol, creating return expression\n");
	auto expr = parse_expression();
	if(!expr)
	    return nullptr;
	expressions.emplace_back(std::move(expr));
    } else {                            // found '{'
	_lexer.consume();
	if(_lexer.token() != tokens::eol) {
	    fprintf(stderr, "error: expected new line after '{'");
	    return nullptr;
	}
	_lexer.consume();
	bool is_return = false;
	while(_lexer.token() != tokens::right_curly_brace && !is_return) {
	    fprintf(stderr, "found token = \"%s\"\n", _lexer.identifier().data());
	    if(_lexer.token() == tokens::return_token) {
		is_return = true;
		_lexer.consume();
	    }
	    auto expr = parse_expression();
	    if(!expr)
		return nullptr;
	    expressions.emplace_back(std::move(expr));
	    _lexer.consume();
	}
    }

    return std::make_unique<ast::block_expression>(std::move(expressions));
}
