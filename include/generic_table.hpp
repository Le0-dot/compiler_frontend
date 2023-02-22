#pragma once

#include <map>
#include <string>

#include <llvm/IR/Type.h>

template<typename Key, typename Value, Value DefaultValue = Value{}>
class generic_table {
private:
    mutable std::map<Key, Value> _table{};

public:
    generic_table()                                        = default;
    generic_table(const generic_table&)                    = default;
    generic_table(generic_table&&)                         = default;
    auto operator=(const generic_table&) -> generic_table& = default;
    auto operator=(generic_table&&)      -> generic_table& = default;
    ~generic_table()                                       = default;

    auto operator[](Key&& key) -> Value& {
	auto found = _table.find(std::forward<Key&&>(key));
	if(found == _table.end())
	    return _table[key] = DefaultValue;
	return found->second;
    }

    auto operator[](Key&& key) const -> const Value& {
	auto found = _table.find(std::forward<Key&&>(key));
	if(found == _table.end())
	    return _table[key] = DefaultValue;
	return found->second;
    }

    auto operator[](const Key& key) -> Value& {
	auto found = _table.find(key);
	if(found == _table.end())
	    return _table[key] = DefaultValue;
	return found->second;
    }

    auto operator[](const Key& key) const -> const Value& {
	auto found = _table.find(key);
	if(found == _table.end())
	    return _table[key] = DefaultValue;
	return found->second;
    }
};

using type_table = generic_table<std::string, llvm::Type*>;
using operator_table = generic_table<std::string, uint8_t, 1>;
