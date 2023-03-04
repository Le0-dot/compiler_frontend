#pragma once

#include <llvm/IR/Type.h>

namespace types {

    class type {
    private:
	llvm::Type* _type{};
	std::string _name{};

    public:
	type(llvm::Type*, std::string&&);
	
	type()                               = default;
	type(const type&)                    = default;
	type(type&&)                         = default;
	auto operator=(const type&) -> type& = default;
	auto operator=(type&&)      -> type& = default;
	virtual ~type()                      = default;

	auto operator==(const type&) const noexcept -> bool;
	auto operator==(llvm::Type*) const noexcept -> bool;
	explicit operator bool() const noexcept;

	[[nodiscard]] auto get() const noexcept -> llvm::Type*;
	[[nodiscard]] auto name() const noexcept -> const std::string&;

	[[nodiscard]] virtual auto is_signed() const noexcept -> bool;
	[[nodiscard]] auto is_integral()       const noexcept -> bool;
	[[nodiscard]] auto is_floating_point() const noexcept -> bool;
	[[nodiscard]] auto is_boolean()        const noexcept -> bool;
	[[nodiscard]] auto is_byte()           const noexcept -> bool;
	[[nodiscard]] auto is_function()       const noexcept -> bool;

    protected:
	auto set_type(llvm::Type*) -> void;
    };

}
