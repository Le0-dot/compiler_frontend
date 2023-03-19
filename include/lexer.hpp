#pragma once

#include <cstdint>
#include <map>
#include <array>
#include <string>
#include <string_view>
#include <memory>
#include <cstdio>

/**
 * Enumeration of all lexer tokens
 */
enum class tokens : uint8_t {
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

/**
 * Lexer that reads given file and produces toknes
 */
class lexer {
    static std::map<std::string, tokens> _tokens;                                 ///< a dictionary that maps key words to the tokens
    std::unique_ptr<std::FILE, decltype(&std::fclose)> _file{stdin, std::fclose}; ///< a file object to read
    tokens _current_token{};                                                      ///< previously read token
    std::string _identifier{};                                                    ///< previously read identifier

public:
    /**
     * Constructor of lexer with file to read
     * @param filename a name of a file to read
     */
    lexer(std::string_view filename);

    lexer()                           = default;
    lexer(const lexer&)               = delete;
    lexer(lexer&&)                    = default;
    auto operator=(const lexer&)      = delete;
    auto operator=(lexer&&) -> lexer& = default;
    ~lexer()                          = default;

    /**
     * Get previously read token
     * @return token that was just read
     */
    [[nodiscard]] auto token() noexcept -> tokens;

    /**
     * Get previously read identifier
     * @return identifier that was just read
     */
    [[nodiscard]] auto identifier() noexcept -> std::string&;

    /**
     * Read new token from a file
     */
    auto consume() noexcept -> void;

private:
    /**
     * Logic of reading a token from file
     * @return token that was read
     */
    [[nodiscard]] auto read_token() noexcept -> enum tokens;

    /**
     * Read char from file
     * @return character that was read
     */
    [[nodiscard]] auto read_char() noexcept -> char;

    /**
     * Get next character without reading it
     * @return character that was accessed
     */
    [[nodiscard]] auto peek() noexcept -> char;

};

/**
 * Check if character is a special symbol
 */
[[nodiscard]] constexpr auto isspecial(char) noexcept -> bool;

/**
 * Check if character is a '\n' or '\r'
 */
[[nodiscard]] constexpr auto iseol(char) noexcept -> bool;

/**
 * Check if characters are a comment i.e. "//" or "/*" 
 */
[[nodiscard]] constexpr auto iscomment(char, char) noexcept -> bool;

/**
 * Check if characters are a single list comment i.e. "//"
 */
[[nodiscard]] constexpr auto issinglelinecomment(char, char) noexcept -> bool;

/**
 * Check if characters are a single multi comment i.e. "/*"
 */
[[nodiscard]] constexpr auto ismultilinecomment(char, char) noexcept -> bool;


/**
 * Check if character is a binary digit
 */
[[nodiscard]] constexpr auto isbinarydigit(char) noexcept -> bool;

/**
 * Check if character is a octal digit
 */
[[nodiscard]] constexpr auto isoctaldigit(char) noexcept -> bool;

/**
 * Check if character is a decimal digit
 */
[[nodiscard]] constexpr auto isdecimaldigit(char) noexcept -> bool;

/**
 * Check if character is a hexadecimal digit
 */
[[nodiscard]] constexpr auto ishexadecimaldigit(char) noexcept -> bool;

/**
 * Check if character is a floating point digit or '.'
 */
[[nodiscard]] constexpr auto isfloatingdigit(char) noexcept -> bool;


/**
 * Check if characters form a binary prefix i.e. "0b"
 */
[[nodiscard]] constexpr auto isbinaryprefix(char, char) noexcept -> bool;

/**
 * Check if characters form a octal prefix i.e. '0' and 0-7
 */
[[nodiscard]] constexpr auto isoctalprefix(char, char) noexcept -> bool;

/**
 * Check if characters form a hexadecimal prefix i.e. "0x"
 */
[[nodiscard]] constexpr auto ishexadecimalprefix(char, char) noexcept -> bool;
