#include "ast/function.hpp"
#include "ast/visitor.hpp"

ast::function_expression::function_expression(std::string&& name, std::vector<std::string>&& args,
	std::vector<llvm::Type*>&& type_list, llvm::Type* return_type, std::unique_ptr<expression>&& body)
    : _name{std::move(name)}
    , _args{std::move(args)}
    , _type{llvm::FunctionType::get(return_type, type_list, false)}
    , _body{std::move(body)}
{}

[[nodiscard]] auto ast::function_expression::type() const -> llvm::Type* {
    return _type;
}

[[nodiscard]] auto ast::function_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::function_expression::name() const -> const std::string& {
    return _name;
}

[[nodiscard]] auto ast::function_expression::args() const -> const std::vector<std::string>& {
    return _args;
}

[[nodiscard]] auto ast::function_expression::body() const -> const ast::expression* {
    return _body.get();
}


