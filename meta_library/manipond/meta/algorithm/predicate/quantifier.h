// Manipond
// Copyright (c) 2026 Frog Singing (@frog-singing)
// SPDX-License-Identifier: MIT

#pragma once
#include <manipond/meta/constraint/construct.h> //用于 meta::constructible_to
#include <concepts> //用于 std::derived_from, std::integral，C++20标准
#include <cstddef> //用于 std::size_t


//演化塘::元工具::谓词::量词
namespace manipond::meta::predicate::quantifier
{
	//算法::量词 algorithm::quantifier================================================================================

	//量词标签
	struct quantifier_tag {};

	//量词约束
	template<typename TargetQuantifier>
	concept Quantifier = std::derived_from<TargetQuantifier, quantifier_tag>;

	//--------------------------------------------------------------------------------

	struct always_true : quantifier_tag
	{
		static constexpr bool solve(constructible_to<bool> auto...) { return true; }

		struct solver
		{
			constexpr bool solved() const noexcept { return true; }
			constexpr bool result() const noexcept { return true; }
			constexpr void step(bool) noexcept {}
		};
	};

	struct always_false : quantifier_tag
	{
		static constexpr bool solve(constructible_to<bool> auto...) { return false; }

		struct solver
		{
			constexpr bool solved() const noexcept { return true; }
			constexpr bool result() const noexcept { return false; }
			constexpr void step(bool) noexcept {}
		};
	};

	//--------------------------------------------------------------------------------

	struct all_of : quantifier_tag
	{
		static constexpr bool solve(constructible_to<bool> auto... condition)
		{
			return (condition && ...);
		}

		struct solver
		{
			bool state{ true };

			solver() = default; //显式声明默认构造函数，禁用聚合初始化

			constexpr bool solved() const noexcept { return !state; }
			constexpr bool result() const noexcept { return state; }
			constexpr void step(bool condition) noexcept { state &= condition; }
		};
	};

	struct any_of : quantifier_tag
	{
		static constexpr bool solve(constructible_to<bool> auto... condition)
		{
			return (condition || ...);
		}

		struct solver
		{
			bool state{ false };

			solver() = default; //显式声明默认构造函数，禁用聚合初始化

			constexpr bool solved() const noexcept { return state; }
			constexpr bool result() const noexcept { return state; }
			constexpr void step(bool condition) noexcept { state |= condition; }
		};
	};

	struct none_of : quantifier_tag
	{
		static constexpr bool solve(constructible_to<bool> auto... condition)
		{
			return !(condition || ...);
		}

		struct solver
		{
			bool state{ true };

			solver() = default; //显式声明默认构造函数，禁用聚合初始化

			constexpr bool solved() const noexcept { return !state; }
			constexpr bool result() const noexcept { return state; }
			constexpr void step(bool condition) noexcept { state &= !condition; }
		};
	};

	struct not_every : quantifier_tag
	{
		static constexpr bool solve(constructible_to<bool> auto... condition)
		{
			return !(condition && ...);
		}

		struct solver
		{
			bool state{ false };

			solver() = default; //显式声明默认构造函数，禁用聚合初始化

			constexpr bool solved() const noexcept { return state; }
			constexpr bool result() const noexcept { return state; }
			constexpr void step(bool condition) noexcept { state |= !condition; }
		};
	};

	//--------------------------------------------------------------------------------

	template<std::size_t Target>
	struct exactly_impl : quantifier_tag
	{
		using type = exactly_impl;

		static constexpr bool solve(constructible_to<bool> auto... condition)
		{
			return (static_cast<std::size_t>(condition) + ...) == Target;
		}

		struct solver
		{
			std::size_t count{ 0 };

			solver() = default; //显式声明默认构造函数，禁用聚合初始化

			constexpr bool solved() const noexcept { return count > Target; }
			constexpr bool result() const noexcept { return count == Target; }

			constexpr void step(bool condition) noexcept
			{ count += static_cast<std::size_t>(condition); }
		};
	};

	template<std::size_t Threshold>
	struct at_most_impl : quantifier_tag
	{
		using type = at_most_impl;

		static constexpr bool solve(constructible_to<bool> auto... condition)
		{
			return (static_cast<std::size_t>(condition) + ...) <= Threshold;
		}

		struct solver
		{
			std::size_t count{ 0 };

			solver() = default; //显式声明默认构造函数，禁用聚合初始化

			constexpr bool solved() const noexcept { return count > Threshold; }
			constexpr bool result() const noexcept { return count <= Threshold; }

			constexpr void step(bool condition) noexcept
			{ count += static_cast<std::size_t>(condition); }
		};
	};

	template<std::size_t Threshold>
	struct more_than_impl : quantifier_tag
	{
		using type = more_than_impl;

		static constexpr bool solve(constructible_to<bool> auto... condition)
		{
			return (static_cast<std::size_t>(condition) + ...) > Threshold;
		}

		struct solver
		{
			std::size_t count{ 0 };

			solver() = default; //显式声明默认构造函数，禁用聚合初始化

			constexpr bool solved() const noexcept { return count > Threshold; }
			constexpr bool result() const noexcept { return count > Threshold; }

			constexpr void step(bool condition) noexcept
			{ count += static_cast<std::size_t>(condition); }
		};
	};

	template<> struct exactly_impl<0>	{ using type = none_of; };
	template<> struct at_most_impl<0>	{ using type = none_of; };
	template<> struct more_than_impl<0>	{ using type = any_of; };


	template<std::integral auto N> requires (N >= 0)
	using exactly = typename exactly_impl<static_cast<std::size_t>(N)>::type;

	template<std::integral auto N> requires (N >= 0)
	using at_most = typename at_most_impl<static_cast<std::size_t>(N)>::type;

	template<std::integral auto N> requires (N >= 0)
	using more_than = typename more_than_impl<static_cast<std::size_t>(N)>::type;

	//--------------------------------------------------------------------------------

	template<std::size_t Threshold>
	struct at_least_impl
	{
		using type = more_than<Threshold - 1>;
	};

	template<std::size_t Threshold>
	struct less_than_impl
	{
		using type = at_most<Threshold - 1>;
	};

	template<> struct at_least_impl<0>	{ using type = always_true; };
	template<> struct less_than_impl<0>	{ using type = always_false; };


	template<std::integral auto N> requires (N >= 0)
	using at_least = typename at_least_impl<static_cast<std::size_t>(N)>::type;

	template<std::integral auto N> requires (N >= 0)
	using less_than = typename less_than_impl<static_cast<std::size_t>(N)>::type;

}


//演化塘::元工具::谓词
namespace manipond::meta::predicate
{
	using quantifier::quantifier_tag;
	using quantifier::Quantifier;

	using quantifier::always_true;	// ⊤
	using quantifier::always_false;	// ⊥

	using quantifier::all_of;		// ∀
	using quantifier::any_of;		// ∃
	using quantifier::none_of;		// ¬∃
	using quantifier::not_every;	// ¬∀
	
	using quantifier::exactly;		// =
	using quantifier::at_least;		// ≥
	using quantifier::at_most;		// ≤

	using quantifier::more_than;	// >
	using quantifier::less_than;	// <

}
