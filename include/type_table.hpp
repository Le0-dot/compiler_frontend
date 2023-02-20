#pragma once

#include "operator_table.hpp"
#include <llvm/IR/Type.h>

#include <map>
#include <string>
#include <string_view>

class type_table {
private:
    mutable std::map<std::string, llvm::Type*> _table{};

public:
    type_table()                      = default;
    type_table(const type_table&)     = delete;
    type_table(type_table&&)          = delete;
    auto operator=(const type_table&) = delete;
    auto operator=(type_table&&)      = delete;
    ~type_table()                     = default;

    [[nodiscard]] auto operator[](const std::string&) -> llvm::Type*&;
    [[nodiscard]] auto operator[](const std::string&) const -> llvm::Type* const &;
};
