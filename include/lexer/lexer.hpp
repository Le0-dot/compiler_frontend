#pragma once

#include <map>
#include <array>
#include <string>
#include <string_view>
#include <memory>
#include <cstdio>

namespace tokens {
    
    enum : uint8_t {
	error,
	eof,
	eol,
	identifier,
	decimal,
	hexadecimal,
	octal,
	binary,
	floating,
	dot,
	comma,
	left_parenthesis,
	right_parenthesis,
	left_square_bracket,
	right_square_bracket,
	left_curly_brace,
	right_curly_brace,
	last_defined
    };

}

class lexer {
    static constexpr std::array SPECIAL_SYMBOL_TOKENS{'!', '@', '#', '$', '%', '^', '&', '*', '-', '+', '/', '~', '?', '=', '<', '>', ';', ':'};

    std::map<std::string, uint8_t> _tokens{};
    std::unique_ptr<std::FILE, decltype(&std::fclose)> _file{stdin, std::fclose};
    uint8_t _current_token{0};
    std::string _identifier{};

public:
    static auto instance() -> lexer&;

    auto for_file(std::string_view filename) -> lexer&;
    [[nodiscard]] auto register_token(std::string_view token, uint8_t token_id) noexcept -> bool;
    auto token() noexcept -> uint8_t;
    auto identifier() noexcept -> const std::string&;
    auto consume() noexcept -> void;

private:
    lexer() = default;
    ~lexer() = default;
    lexer(const lexer&) = delete;
    lexer(lexer&&) = delete;
    auto operator=(const lexer&) = delete;
    auto operator=(lexer&&) = delete;

    auto read_token() noexcept -> uint8_t;
    auto peek() noexcept -> char;
    constexpr auto isspecial(char ch) noexcept -> bool;
};
