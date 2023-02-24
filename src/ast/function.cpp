#include "ast/function.hpp"
#include "ast/visitor.hpp"

ast::function_expression::function_expression(std::string&& name, std::vector<std::string>&& args,
	std::vector<std::string>&& type_list, std::string&& return_type, std::unique_ptr<expression>&& body)
    : _name{std::move(name)}
    , _args{std::move(args)}
    , _types{std::move(type_list)}
    , _ret_type{std::move(return_type)}
    , _body{std::move(body)}
{}

[[nodiscard]] auto ast::function_expression::accept(value_visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::function_expression::accept(type_visitor* v) const -> llvm::Type* {
    return v->visit(this);
}

[[nodiscard]] auto ast::function_expression::name() const -> const std::string& {
    return _name;
}

[[nodiscard]] auto ast::function_expression::args() const -> const std::vector<std::string>& {
    return _args;
}

[[nodiscard]] auto ast::function_expression::types() const -> const std::vector<std::string>& {
    return _types;
}

[[nodiscard]] auto ast::function_expression::return_type() const -> const std::string& {
    return _ret_type;
}

[[nodiscard]] auto ast::function_expression::body() const -> const ast::expression* {
    return _body.get();
}


