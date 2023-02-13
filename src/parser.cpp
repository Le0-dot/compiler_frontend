#include <cstdio>
#include <memory>

#include "ast/binary.hpp"
#include "ast/call.hpp"
#include "ast/function.hpp"
#include "ast/literal.hpp"
#include "ast/variable.hpp"
#include "lexer.hpp"
#include "parser.hpp"

namespace parser {

    // literal_expression ::= literal
    auto parse_literal() -> std::unique_ptr<ast::expression> {
	ast::literal_types type;
	switch(lexer::instance().token()) {
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
	lexer::instance().consume();
	return std::make_unique<ast::literal_expression>(std::move(lexer::instance().identifier()), type);
    }

    // parenthesis ::= '(' expression ')'
    auto parse_parenthesis() -> std::unique_ptr<ast::expression> {
	lexer::instance().consume();

	auto expr = parse_expression();
	if(!expr)
	    return nullptr;

	if(lexer::instance().token() != tokens::right_parenthesis) {
	    fprintf(stderr, "error: expected ')'");
	    return nullptr;
	}

	lexer::instance().consume();
	return expr;
    }

    // indentifierExpr 
    //			::= indentifier
    //			::= indentifier '(' expression* ')'
    auto parse_indentifier() -> std::unique_ptr<ast::expression> {
	std::string identifier = lexer::instance().identifier();
	lexer::instance().consume();

	if(lexer::instance().token() != tokens::left_parenthesis)
	    return std::make_unique<ast::variable_expression>(std::move(identifier));

	std::vector<std::unique_ptr<ast::expression>> args;

	while(lexer::instance().token() != tokens::right_parenthesis) {
	    lexer::instance().consume();

	    if(auto arg = parse_expression())
		args.emplace_back(std::move(arg));
	    else
		return nullptr;

	    if(auto t = lexer::instance().token(); 
		    t != tokens::right_parenthesis && t != tokens::comma) {
		fprintf(stderr, "error: expected ')' or ','");
		return nullptr;
	    }
	}
	lexer::instance().consume();

	return std::make_unique<ast::call_expression>(std::move(identifier), std::move(args));
    }

    // primary 
    //		::= indentifierExpr
    //		::= literal
    //		::= parenthesis
    auto parse_primary() -> std::unique_ptr<ast::expression> {
	switch (lexer::instance().token()) {
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
    auto parse_expression() -> std::unique_ptr<ast::expression> {
	auto lhs = parse_primary();
	if(!lhs)
	    return nullptr;

	return parse_binary_rhs(0, std::move(lhs));
    }

    // binary ::= (op prmary)*
    auto parse_binary_rhs(uint16_t precedence, std::unique_ptr<ast::expression>&& lhs) -> std::unique_ptr<ast::expression> {
	while(true) {
	    uint16_t current_precedence = [](){return 0;}(); // placeholder

	    if(current_precedence < precedence)
		return lhs;

	    std::string op = lexer::instance().identifier();
	    lexer::instance().consume();

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

    auto parse_function_definition() -> std::unique_ptr<ast::expression> {
	if(lexer::instance().identifier() != "function") {
	    fprintf(stderr, "expected 'function' in function definition");
	    return nullptr;
	}
	lexer::instance().consume();

	std::string name{};
	if(lexer::instance().token() == tokens::identifier) {
	    name = std::move(lexer::instance().identifier());
	    lexer::instance().consume();
	}

	fprintf(stderr, "name is %s\n", name.data());
	
	if(lexer::instance().token() != tokens::left_parenthesis) {
	    fprintf(stderr, "error: expected '(' in function definition");
	    return nullptr;
	}

	fprintf(stderr, "found lp\n");

	lexer::instance().consume();
	std::vector<std::string> args;
	while(lexer::instance().token() == tokens::identifier) {
	    args.emplace_back(std::move(lexer::instance().identifier()));
	    lexer::instance().consume();
	    if(lexer::instance().token() == tokens::comma)
		lexer::instance().consume();
	}

	for(const auto& s: args)
	    fprintf(stderr, "%s ", s.data());
	fprintf(stderr, "\n");

	if(lexer::instance().token() != tokens::right_parenthesis) {
	    fprintf(stderr, "error: expected ')'");
	    return nullptr;
	}
	lexer::instance().consume();

	auto body = parse_expression();
	if(!body)
	    return nullptr;

	return std::make_unique<ast::function_definition_expression>(std::move(name), std::move(args), std::move(body));
    }

}
