#pragma once

#include <map>
#include <unordered_map>
#include <concepts>
#include <string>

#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>

#include "types.hpp"

/*
template<typename K, typename V>
class table_base : public std::map<K, V> {};

template<typename K, typename V>
requires std::default_initializable<V>
class table_base<K, V> : public std::unordered_map<K, V> {};
*/

template<typename K, typename V>
using table_base = std::map<K, V>;

template<typename Key, typename Value, Value DefaultValue = Value{}>
class defaulted_table {
private:
    mutable table_base<Key, Value> _table{};

public:
    defaulted_table()                                          = default;
    defaulted_table(const defaulted_table&)                    = default;
    defaulted_table(defaulted_table&&)                         = default;
    auto operator=(const defaulted_table&) -> defaulted_table& = default;
    auto operator=(defaulted_table&&)      -> defaulted_table& = default;
    ~defaulted_table()                                         = default;

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

using operator_table        = defaulted_table<std::string, uint8_t, 1>;

using type_table               = table_base<std::string, std::unique_ptr<types::type>>;
using function_type_table      = table_base<std::pair<std::vector<std::string>, std::string>, std::unique_ptr<types::function_type>>;
using symbol_table             = table_base<std::string, types::type*>;
using function_symbol_table    = table_base<std::string, types::function_type*>;
using type_normalization_table = table_base<std::string, std::string>;

using cast_function = llvm::Value*(llvm::IRBuilderBase*, llvm::Value*);
using cast_table    = table_base<std::pair<types::type*, types::type*>, std::function<cast_function>>;

using binary_operation_function = llvm::Value*(llvm::IRBuilderBase*, llvm::Value*, llvm::Value*);
using binary_operation_table    = table_base<std::pair<std::string, types::type*>, std::function<binary_operation_function>>;
