#include "ast/implicit_cast.hpp"
#include "ast/visitor.hpp"

ast::implicit_cast::implicit_cast(std::unique_ptr<expression>&& subj, types::type* to)
    : _subject{std::move(subj)}
{
    type() = to;
}

[[nodiscard]] auto ast::implicit_cast::accept(ast::value_visitor* v) const -> llvm::Value* {
    fprintf(stderr, "cast accept value\n");
    return v->visit(this);
}

[[nodiscard]] auto ast::implicit_cast::accept(ast::type_visitor*) -> types::type* {
    assert("called accept(type_visitor*) on implicit_cast");
    return nullptr;
}

[[nodiscard]] auto ast::implicit_cast::subject() const -> const expression* {
    return _subject.get();
}
