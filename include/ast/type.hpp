#pragma once

#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

namespace ast {

    class type {
    private:
	llvm::Type* _type{};
	bool _signed{false}, _function{false};

    public:
	type(llvm::Type*);
	type(llvm::IntegerType*, bool);
	type(llvm::FunctionType*);
	
	type()                               = default;
	type(const type&)                    = default;
	type(type&&)                         = default;
	auto operator=(const type&) -> type& = default;
	auto operator=(type&&)      -> type& = default;
	~type()                              = default;

	[[nodiscard]] auto get_type() const -> llvm::Type*;

	[[nodiscard]] auto is_signed()   const -> bool;
	[[nodiscard]] auto is_int()      const -> bool;
	[[nodiscard]] auto is_uint()     const -> bool;
	[[nodiscard]] auto is_float()    const -> bool;
	[[nodiscard]] auto is_bool()     const -> bool;
	[[nodiscard]] auto is_byte()     const -> bool;
	[[nodiscard]] auto is_function() const -> bool;
    };

}
