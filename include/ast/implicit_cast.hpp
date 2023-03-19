#pragma once

#include <memory>

#include "expression.hpp"
#include "types.hpp"

namespace ast {

    class implicit_cast;

    /**
     * Concept that represents callable object that creates
     * new implicit_cast based on previous expression
     * @tparam F type of object to test
     */
    template<typename F>
    concept cast_builder = std::invocable<F, std::unique_ptr<expression>&&> &&
    requires(F f, std::unique_ptr<expression>&& e) {
	{ std::invoke(f, std::move(e)) } -> std::same_as<std::unique_ptr<implicit_cast>>;
    };


    /**
     * Implicit cast expression that cannot be created externaly
     */
    class implicit_cast : public expression {
    private:
	std::unique_ptr<expression> _subject; ///< expression to cast

    public:
	/**
	 * Constructor of implicit cast
	 * @param subj an expression to cast
	 * @param to a type of resulting expression
	 */
	implicit_cast(std::unique_ptr<expression>&& subj, types::type* to);

	[[nodiscard]] virtual auto accept(value_visitor*) const -> llvm::Value* override;
	[[nodiscard]] virtual auto accept(type_visitor*) -> types::type* override;

	/**
	 * Accessor of subject for const implicit_cast
	 * @return internal expression before cast
	 */
	[[nodiscard]] auto subject() const -> const expression*;
    };

}
