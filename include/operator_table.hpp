#pragma once

#include <string>
#include <unordered_map>

class operator_table {
private:
    std::unordered_map<std::string, uint8_t> _table{};

public:
    operator_table()                                    = default;
    operator_table(const operator_table&)               = delete;
    operator_table(operator_table&&)                    = default;
    auto operator=(const operator_table&)               = delete;
    auto operator=(operator_table&&) -> operator_table& = default;
    ~operator_table()                                   = default;

    auto add(const std::string&, uint8_t) -> void;
    [[nodiscard]] auto get(const std::string&) -> uint8_t;
};
