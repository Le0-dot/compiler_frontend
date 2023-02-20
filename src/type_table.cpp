#include "type_table.hpp"

[[nodiscard]] auto type_table::operator[](const std::string& type_name) -> llvm::Type*& {
    return _table[type_name];
}

[[nodiscard]] auto type_table::operator[](const std::string& type_name) const -> llvm::Type* const & {
    return _table[type_name];
}
