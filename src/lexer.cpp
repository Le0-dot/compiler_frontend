#include <algorithm>
#include <cstdint>
#include <cstdio>

#include "lexer.hpp"

std::map<std::string, uint8_t> lexer::_tokens = {
    {"return", tokens::return_token},
};

lexer::lexer(std::string_view filename) 
    : _file{std::fopen(filename.data(), "r"), std::fclose}
{
    if(!_file)
	throw std::runtime_error("Unable to read from file");
    consume();
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
    if(iseol(last)) {
	last = read_char();
	return tokens::eol;
    }
    while(isspace(last)) {
	last = read_char();
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
    if(last != '.' && special_non_overridable.contains(last) ||
       last == '.' && !isdigit(peek())) {
	_identifier = last;
	uint8_t token = special_non_overridable.at(last);
	last = read_char();
	return token;
    }

    // special symbols that serve as operators
    if(!iscomment(last, peek()) && isspecial(last)) {
	_identifier.push_back(last);
	while(isspecial(last = read_char()))
	    _identifier.push_back(last);
	return tokens::identifier;
    }

    // identifier [a-zA-Z_][a-zA-Z0-9_]*
    if(isalpha(last) || last == '_') {
	_identifier.push_back(last);
	while(std::isalnum(last = read_char()) || last == '_')
	    _identifier.push_back(last);

	auto token_id = _tokens[_identifier];
	return token_id ? token_id : tokens::identifier;
    }

    // character literals
    if(last == '\'') {
	_identifier.push_back(read_char());
	last = read_char();
	if(last != '\'')
	    return tokens::error;
	last = read_char();
	return tokens::character;
    }

    // string literals
    if(last == '\"') {
	while((last = read_char()) != '\"' && last != EOF)
	    _identifier.push_back(last);
	if(last == EOF)
	    return tokens::error;
	last = read_char();
	return tokens::string;
    }

    // number (0x|0b|.|[0-9])[0-9._]*
    if(isfloatingdigit(last)) {
	uint8_t number_type;
	bool (*validator)(char);

	// handle types of numbers
	if(ishexadecimalprefix(last, peek())) {   // found "0x" - hexadecimal number
	    last = read_char(); // eat 'x'
	    last = read_char();
	    number_type = tokens::hexadecimal;
	    validator = ishexadecimaldigit;
	} else if(isoctalprefix(last, peek())) {  // found '0' and number in range 0-7 - octal number
	    last = read_char();
	    number_type = tokens::octal;
	    validator = isoctaldigit;
	} else if(isbinaryprefix(last, peek())) { // found "0b" - binary number
	    last = read_char(); // eat 'b'
	    last = read_char();
	    number_type = tokens::binary;
	    validator = isbinarydigit;
	} else if(last == '0') {                  // found '0' and some other number or character that is not prefix of a number type - decimal '0'
	    _identifier.push_back(last);
	    last = read_char();
	    return tokens::decimal;
	} else if(last == '.') {                  // found '.' - floating point number
	    number_type = tokens::floating;
	    validator = isfloatingdigit;
	} else {                                  // found number in range 0-9 - decimal
	    number_type = tokens::decimal;
	    validator = isdecimaldigit;
	}

	// read characters until validator is false
	do {
	    _identifier.push_back(last);
	    last = read_char();
	    if(last == '_') // skip '_'
		continue;
	    if(last == '.' && number_type == tokens::decimal) { // found first '.' in decimal - fall back to floating point
		number_type = tokens::floating;
		validator = isfloatingdigit;
	    } else if(last == '.')                              // found '.' in hexadecimal, octal or floating point - return with error
		return tokens::error;
	} while(validator(last));

	if(isfloatingdigit(last) || ishexadecimaldigit(last)) // found digit right after another digit without separation 
	    return tokens::error;

	return number_type;
    }

    // skip comment // and /* ... */
    if(iscomment(last, peek())) {
	char next = read_char();
	if(issinglelinecomment(last, next)) {
	    do
		last = read_char();
	    while(last != '\n' && last != '\r' && last != EOF);
	}
	if(ismultilinecomment(last, next)) {
	    char prev;
	    do {
		prev = last;
		last = read_char();
	    } while(!(prev == '*' && last == '/') && last != EOF);
	}

	last = read_char();
	if(last == EOF)
	    return tokens::eof;
	return read_token();
    }

    if(last == EOF)
	return tokens::eof;

    return tokens::error;
}

[[nodiscard]] auto lexer::read_char() noexcept -> char {
    return std::fgetc(_file.get());
}

[[nodiscard]] auto lexer::peek() noexcept -> char {
    char last = std::fgetc(_file.get());
    std::ungetc(last, _file.get());
    return last;
}

[[nodiscard]] constexpr auto isspecial(char ch) noexcept -> bool {
    constexpr std::array SPECIAL_SYMBOL_TOKENS{'!', '#', '$', '%', '&', '*', '+', '-', '/', ':', ';', '<', '=', '>', '?', '@', '^', '~'};
    return std::ranges::binary_search(SPECIAL_SYMBOL_TOKENS, ch);
}

[[nodiscard]] constexpr auto iseol(char ch) noexcept -> bool {
    return ch == '\n' || ch == '\r';
}

[[nodiscard]] constexpr auto iscomment(char first, char second) noexcept -> bool {
    return first == '/' && (second == '/' || second == '*');
}

[[nodiscard]] constexpr auto issinglelinecomment(char first, char second) noexcept -> bool {
    return first == '/' && second == '/';
}

[[nodiscard]] constexpr auto ismultilinecomment(char first, char second) noexcept -> bool {
    return first == '/' && second == '*';
}

[[nodiscard]] constexpr auto isbinarydigit(char ch) noexcept -> bool {
    return ch == '0' || ch == '1' || ch == '_';
}

[[nodiscard]] constexpr auto isoctaldigit(char ch) noexcept -> bool {
    return ch >= '0' && ch <= '7' || ch == '_';
}

[[nodiscard]] constexpr auto isdecimaldigit(char ch) noexcept -> bool {
    return ch >= '0' && ch <= '9' || ch == '_';
}

[[nodiscard]] constexpr auto ishexadecimaldigit(char ch) noexcept -> bool {
    return ch >= '0' && ch <= '9' || 
	   ch >= 'a' && ch <= 'f' ||
	   ch >= 'A' && ch <= 'F' ||
	   ch == '_';
}

[[nodiscard]] constexpr auto isfloatingdigit(char ch) noexcept -> bool {
    return isdecimaldigit(ch) || ch == '.';
};

[[nodiscard]] constexpr auto isbinaryprefix(char first, char second) noexcept -> bool {
    return first == '0' && second == 'b';
}

[[nodiscard]] constexpr auto isoctalprefix(char first, char second) noexcept -> bool {
    return first == '0' && isoctaldigit(second);
}

[[nodiscard]] constexpr auto ishexadecimalprefix(char first, char second) noexcept -> bool {
    return first == '0' && second == 'x';
}
