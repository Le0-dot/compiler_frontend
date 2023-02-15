#include "lexer.hpp"

#include <algorithm>
#include <cstdint>

auto lexer::for_file(std::string_view filename) -> lexer& {
    _file = {std::fopen(filename.data(), "r"), std::fclose};
    if(!_file)
	throw std::runtime_error("Unable to read from file");
    consume();
    return *this;
}

[[nodiscard]] auto lexer::register_token(std::string_view token, uint8_t token_id) noexcept -> bool {
    std::string token_str{token};
    if(token_id <= tokens::last_defined || _tokens[token_str])
	return false;
    _tokens[token_str] = token_id;
    return true;
}

[[nodiscard]] auto lexer::token() noexcept -> uint8_t {
    return _current_token;
}

[[nodiscard]] auto lexer::identifier() noexcept -> std::string& {
    return _identifier;
}

auto lexer::consume() noexcept -> void {
    _current_token = read_token();
}

[[nodiscard]] auto lexer::read_token() noexcept -> uint8_t {
    static char last = ' ';
    _identifier = {};


    // skip spaces and emit End Of Line tokens
    if((last == '\n' || last == '\r') && _current_token != tokens::eol)
	return tokens::eol;
    while(isspace(last)) {
	last = std::fgetc(_file.get());
	if(last == '\n' || last == '\r')
	    return tokens::eol;
    }

    // special symbols that cannot be overriden
    static const std::map<char, uint8_t> special_non_overridable {
	{'(', tokens::left_parenthesis},
	{')', tokens::right_parenthesis},
	{'[', tokens::left_square_bracket},
	{']', tokens::right_square_bracket},
	{'{', tokens::left_curly_brace},
	{'}', tokens::right_curly_brace},
	{',', tokens::comma},
	{'.', tokens::dot},
    };
    if(auto _last = last; last != '.' && special_non_overridable.contains(last) || last == '.' && !isdigit(peek()) ) {
	_identifier = last;
	last = std::fgetc(_file.get());
	return special_non_overridable.at(_last);
    }

    // other special symbols and their versions with '=', e.g. '<='
    char next = peek();
    bool is_singleline_comment = (last == '/' && next == '/');
    bool is_multiline_comment = (last == '/' && next == '*');
    bool is_comment = is_singleline_comment || is_multiline_comment;
    if(!is_comment && isspecial(last)) {
	_identifier.push_back(last);
	last = std::fgetc(_file.get());
	if(isspecial(last)) {
	    _identifier.push_back(last);
	    last = std::fgetc(_file.get());
	}
	if(last == '=') {
	    _identifier.push_back(last);
	    last = std::fgetc(_file.get());
	}
	auto token_id = _tokens[_identifier];
	return token_id ? token_id : tokens::identifier;
    }

    // identifier [a-zA-Z_][a-zA-Z0-9_]*
    if(isalpha(last) || last == '_') {
	_identifier.push_back(last);
	while(std::isalnum(last = std::fgetc(_file.get())) || last == '_')
	    _identifier.push_back(last);

	auto token_id = _tokens[_identifier];
	return token_id ? token_id : tokens::identifier;
    }

    // character literals
    if(last == '\'') {
	last = std::fgetc(_file.get());
	_identifier.push_back(last);
	last = std::fgetc(_file.get());
	if(last != '\'')
	    return tokens::error;
	return tokens::character;
    }

    // string literals
    if(last == '\"') {
	last = std::fgetc(_file.get());
	while(last != '\"' && last != EOF) {
	    _identifier.push_back(last);
	    last = std::fgetc(_file.get());
	}
	if(last == EOF)
	    return tokens::error;
	return tokens::string;
    }

    // number (0x|0b|.|[0-9])[0-9._]*
    if(std::isdigit(last) || last == '.') {
	_identifier.push_back(last);
	uint8_t number_type = (last == '.') ? tokens::floating : tokens::decimal;

	// handle integer numbers
	if(last == '0') {
	    last = std::fgetc(_file.get());
	    _identifier.push_back(last);
	    if(last == 'x')
		number_type = tokens::hexadecimal;
	    else if(last == 'b')
		number_type = tokens::binary;
	    else if(last >= '0' && last <= '7')
		number_type = tokens::octal;
	    else
		return tokens::error;
	}

	// consume types
	while(std::isdigit(last = std::fgetc(_file.get())) || 
		(last >= 'a' && last <= 'f' || last >= 'A' && last <= 'F') && number_type == tokens::hexadecimal ||
		last == '.' || last == '_') {
	    if(last == '_')
		continue;
	    if(number_type == tokens::octal && last >= '8')
		return tokens::error;
	    if(number_type == tokens::binary && last >= '2')
		return tokens::error;
	    _identifier.push_back(last);
	    if(last == '.' && number_type == tokens::decimal)
		number_type = tokens::floating;
	    else if(last == '.')
		return tokens::error;
	}

	return number_type;
    }

    // comment // and /* ... */
    if(last == '/') {
	last = std::fgetc(_file.get());
	if(last == '/')
	    while(last != '\n' && last != '\r' && last != EOF)
		last = std::fgetc(_file.get());
	if(last == '*') {
	    char prev = 0;
	    while(!(prev == '*' && last == '/') && last != EOF) {
		prev = last;
		last = std::fgetc(_file.get());
	    }
	    last = std::fgetc(_file.get());
	}
	if(last == EOF)
	    return tokens::eof;
	return read_token();
    }

    if(last == EOF)
	return tokens::eof;

    return tokens::error;
}

[[nodiscard]] auto lexer::peek() noexcept -> char {
    char last = std::fgetc(_file.get());
    std::ungetc(last, _file.get());
    return last;
}

constexpr auto lexer::isspecial(char ch) noexcept -> bool {
    return std::ranges::find(SPECIAL_SYMBOL_TOKENS, ch) != SPECIAL_SYMBOL_TOKENS.end();
}
