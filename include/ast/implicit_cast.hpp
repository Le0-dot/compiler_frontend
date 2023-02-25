#pragma once

#include <memory>

#include "expression.hpp"

namespace ast {

    class implicit_cast;

    template<typename F>
    concept cast_builder = std::invocable<F, std::unique_ptr<expression>&&> &&
    requires(F f, std::unique_ptr<expression>&& e) {
	{ std::invoke(f, std::move(e)) } -> std::same_as<std::unique_ptr<implicit_cast>>;
    };


    class implicit_cast : public expression {
    private:
	std::unique_ptr<expression> _subject;

    public:
	implicit_cast(std::unique_ptr<expression>&&, llvm::Type*);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> llvm::Type* override;

	[[nodiscard]] auto subject() const -> const expression*;
    };

}
