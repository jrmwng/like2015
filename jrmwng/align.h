#pragma once

/* Author: jrmwng @ 2016 */

#include <intrin.h>
#include <type_traits>

namespace jrmwng
{
	template <unsigned uAlign, typename T>
	struct align_t
		: T
	{
		template <typename... TArgs>
		align_t(TArgs... Args)
			: T(std::forward<TArgs>(Args...))
		{
			if (reinterpret_cast<uintptr_t>(this) % uAlign)
				__debugbreak();
		}
	};

#pragma region align_page
	template <typename T>
	struct alignas(4096) align_page_t
		: align_t<4096, T>
	{
		template <typename... TArgs>
		align_page_t(TArgs... Args)
			: align_t<4096, T>(std::forward<TArgs>(Args...))
		{}
	};
	template <typename T>
	align_page_t<T> align_page(T t)
	{
		return align_page_t<T>(t);
	}
#pragma endregion

#pragma region align_section
	template <typename T>
	struct alignas(128) align_section_t
		: align_t<128, T>
	{
		template <typename... TArgs>
		align_section_t(TArgs... Args)
			: align_t<128, T>(std::forward<TArgs>(Args...))
		{}
	};
	template <typename T>
	align_section_t<T> align_section(T t)
	{
		return align_section_t<T>(t);
	}
#pragma endregion

#pragma region align_line
	template <typename T>
	struct alignas(64) align_line_t
		: align_t<64, T>
	{
		template <typename... TArgs>
		align_line_t(TArgs... Args)
			: align_t<64, T>(std::forward<TArgs>(Args...))
		{}
	};
	template <typename T>
	align_line_t<T> align_line(T t)
	{
		return align_line_t<T>(t);
	}
#pragma endregion

#pragma region align_lock
	template <typename T>
	struct alignas(16) align_lock_t
		: align_t<16, T>
	{
		template <typename... TArgs>
		align_lock_t(TArgs... Args)
			: align_t<16, T>(std::forward<TArgs>(Args...))
		{}
	};
	template <typename T>
	align_lock_t<T> align_lock(T t)
	{
		return align_lock_t<T>(t);
	}
#pragma endregion

}