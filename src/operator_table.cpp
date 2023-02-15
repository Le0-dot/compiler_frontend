#include "operator_table.hpp"

auto operator_table::add(const std::string& op, uint8_t precedence) -> void {
    _table[op] = precedence;
}

[[nodiscard]] auto operator_table::get(const std::string& op) -> uint8_t {
    uint8_t precedence = _table[op];
    // all user-defined operators have precedence level 1 higher than '='
    return precedence ? precedence : _table["="] + 1;
}
