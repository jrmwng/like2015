#pragma once

/* Author: jrmwng @ 2015 */

#include <malloc.h>
#include <algorithm>

namespace like
{
	struct heap
	{
		virtual ~heap(void)
		{}

		virtual void * malloc(size_t uRequestSize)
		{
			return ::malloc(uRequestSize);
		}

		virtual void * calloc(size_t uN, size_t uElementSize)
		{
			return ::calloc(uN, uElementSize);
		}

		virtual void * realloc(void *pOld, size_t uNewSize)
		{
			return ::realloc(pOld, uNewSize);
		}

		virtual void  free(void *p)
		{
			::free(p);
		}
	};

	static thread_local heap *g_pHeap = nullptr;

	heap * heap_threadlocal(heap *pHeap)
	{
		return std::exchange(g_pHeap, pHeap);
	}
	template <typename TF>
	typename std::enable_if<std::is_void<typename TF::result_type>::value, void>::type heap_threadlocal(heap *pHeap, TF const & tF)
	{
		std::swap(g_pHeap, pHeap);
		tF();
		g_pHeap = pHeap;
	}
	template <typename TF>
	typename std::enable_if<!std::is_void<typename TF::result_type>::value, typename TF::result_type>::type heap_threadlocal(heap *pHeap, TF const & tF)
	{
		std::swap(g_pHeap, pHeap);
		typename TF::result_type tR(std::move(tF()));
		g_pHeap = pHeap;
		return std::move(tR);
	}

	void * heap_malloc(size_t uRequestSize)
	{
		return g_pHeap->malloc(uRequestSize);
	}
	void * heap_calloc(size_t uN, size_t uElementSize)
	{
		return g_pHeap->calloc(uN, uElementSize);
	}
	void * heap_realloc(void *pOld, size_t uNewSize)
	{
		return g_pHeap->realloc(pOld, uNewSize);
	}
	void heap_free(void *p)
	{
		g_pHeap->free(p);
	}
}