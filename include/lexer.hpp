#pragma once

#include <cstdint>
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
	character,
	string,
	dot,
	comma,
	left_parenthesis,
	right_parenthesis,
	left_square_bracket,
	right_square_bracket,
	left_curly_brace,
	right_curly_brace,
	return_token,
    };

}

class lexer {
    static std::map<std::string, uint8_t> _tokens;
    std::unique_ptr<std::FILE, decltype(&std::fclose)> _file{stdin, std::fclose};
    uint8_t _current_token{};
    std::string _identifier{};

public:
    lexer(std::string_view);

    lexer()                           = default;
    lexer(const lexer&)               = delete;
    lexer(lexer&&)                    = default;
    auto operator=(const lexer&)      = delete;
    auto operator=(lexer&&) -> lexer& = default;
    ~lexer()                          = default;

    [[nodiscard]] auto token() noexcept -> uint8_t;
    [[nodiscard]] auto identifier() noexcept -> std::string&;
    auto consume() noexcept -> void;

private:
    [[nodiscard]] auto read_token() noexcept -> uint8_t;
    [[nodiscard]] auto read_char() noexcept -> char;
    [[nodiscard]] auto peek() noexcept -> char;

    auto fill_tokens() noexcept -> void;
};

[[nodiscard]] constexpr auto isspecial(char) noexcept -> bool;
[[nodiscard]] constexpr auto iseol(char) noexcept -> bool;
[[nodiscard]] constexpr auto iscomment(char, char) noexcept -> bool;
[[nodiscard]] constexpr auto issinglelinecomment(char, char) noexcept -> bool;
[[nodiscard]] constexpr auto ismultilinecomment(char, char) noexcept -> bool;

[[nodiscard]] constexpr auto isbinarydigit(char) noexcept -> bool;
[[nodiscard]] constexpr auto isoctaldigit(char) noexcept -> bool;
[[nodiscard]] constexpr auto isdecimaldigit(char) noexcept -> bool;
[[nodiscard]] constexpr auto ishexadecimaldigit(char) noexcept -> bool;
[[nodiscard]] constexpr auto isfloatingdigit(char) noexcept -> bool;

[[nodiscard]] constexpr auto isbinaryprefix(char, char) noexcept -> bool;
[[nodiscard]] constexpr auto isoctalprefix(char, char) noexcept -> bool;
[[nodiscard]] constexpr auto ishexadecimalprefix(char, char) noexcept -> bool;
