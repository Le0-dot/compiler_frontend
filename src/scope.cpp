#include "scope.hpp"

auto scope::new_symbol(const std::string& sym, types::type* type) -> void {
    _symbols[sym] = type;
}

auto scope::new_function(const std::string& func, types::function_type* type) -> void {
    _functions[func] = type;
}

[[nodiscard]] auto scope::search_symbol(const std::string& sym) -> types::type* {
    return _symbols[sym];
}

[[nodiscard]] auto scope::search_function(const std::string& func) -> types::function_type* {
    return _functions[func];
}


scope_manager::scope_manager() {
    _scopes.emplace_front();
}

auto scope_manager::new_scope() -> scope& {
    return _scopes.emplace_front();
}

auto scope_manager::delete_scope() -> void {
    _scopes.pop_front();
}

auto scope_manager::new_symbol(const std::string& sym, types::type* type) -> void {
    _scopes.front().new_symbol(sym, type);
}

auto scope_manager::new_function(const std::string& func, types::function_type* type) -> void {
    _scopes.front().new_function(func, type);
}

[[nodiscard]] auto scope_manager::search_symbol(const std::string& sym) -> types::type* {
    for(auto& s: _scopes)
	if(auto type = s.search_symbol(sym))
	    return type;
    return nullptr;
}

[[nodiscard]] auto scope_manager::search_function(const std::string& func) -> types::function_type* {
    for(auto& s: _scopes)
	if(auto type = s.search_function(func))
	    return type;
    return nullptr;
}
