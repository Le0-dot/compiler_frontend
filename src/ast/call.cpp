#include "ast/call.hpp"
#include "ast/visitor.hpp"

ast::call_expression::call_expression(std::string&& callee, std::vector<std::unique_ptr<expression>>&& args)
    : _callee{std::move(callee)}
    , _args{std::move(args)}
{}

[[nodiscard]] auto ast::call_expression::type() const -> llvm::Type* {

}

[[nodiscard]] auto ast::call_expression::accept(visitor* v) const -> llvm::Value* {
    return v->visit(this);
}

[[nodiscard]] auto ast::call_expression::callee() const -> const std::string& {
    return _callee;
}

[[nodiscard]] auto ast::call_expression::args() const -> const std::vector<std::unique_ptr<expression>>& {
    return _args;
}

