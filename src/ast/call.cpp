#include "ast/call.hpp"
#include "ast/visitor.hpp"

ast::call_expression::call_expression(std::string&& callee, std::vector<std::unique_ptr<expression>>&& args)
    : _callee{std::move(callee)}
    , _args{std::move(args)}
{}

[[nodiscard]] auto ast::call_expression::accept(value_visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::call_expression::accept(type_visitor* v) -> llvm::Type* {
    return v->visit(this);
}

[[nodiscard]] auto ast::call_expression::callee() const -> const std::string& {
    return _callee;
}

[[nodiscard]] auto ast::call_expression::args() const -> const std::vector<std::unique_ptr<expression>>& {
    return _args;
}

[[nodiscard]] auto ast::call_expression::args() -> std::vector<std::unique_ptr<expression>>& {
    return _args;
}
