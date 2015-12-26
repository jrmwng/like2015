#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\like2015\shared_ptr.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace like2015UnitTest
{		
	TEST_CLASS(UnitTest_shared_ptr)
	{
		template <typename T1>
		typename std::enable_if<std::is_base_of<like::weak_ptr<int>, T1>::value, void>::type test_weak_ptr(T1 & wp1)
		{
			like::shared_ptr<int> sp0;
			wp1.reset(wp1);
			wp1.reset(wp1, sp0.get());
			wp1.reset(std::move(wp1));
			wp1.reset(std::move(wp1), sp0.get());
			wp1.reset(sp0);
			wp1.reset(sp0, sp0.get());
			wp1.reset(std::move(sp0));
			wp1.reset(std::move(sp0), sp0.get());
			wp1.swap(wp1);
			bool b4 = wp1.expired();
			bool b5 = (wp1 == wp1);
			bool b6 = (wp1 != wp1);
			bool b7 = wp1;
			wp1 = wp1;
			wp1 = std::move(wp1);
			wp1 = sp0;
			wp1 = std::move(sp0);
		}
		template <typename T1>
		typename std::enable_if<std::is_base_of<like::shared_ptr<int>, T1>::value, void>::type test_shared_ptr(T1 & sp0)
		{
			like::weak_ptr<int> wp1;
			int *pn4 = sp0.get();
			long l5 = sp0.use_count();
			bool b6 = sp0.unique();
			bool b7 = (sp0 == sp0);
			bool b8 = (sp0 != sp0);
			bool b9 = sp0;
			int const & n10 = *sp0;
			sp0.reset();
			sp0.reset(sp0);
			sp0.reset(sp0, sp0.get());
			sp0.reset(std::move(sp0));
			sp0.reset(std::move(sp0), sp0.get());
			sp0.reset(wp1);
			sp0.reset(wp1, sp0.get());
			sp0.reset(std::move(wp1));
			sp0.reset(std::move(wp1), sp0.get());
			sp0.swap(sp0);
			sp0 = sp0;
			sp0 = std::move(sp0);
			sp0 = wp1;
			sp0 = std::move(wp1);
		}
		template <typename T1>
		void test_shared_obj(like::shared_obj<T1> & so)
		{
			T1 t1;
			std::add_const_t<std::remove_cv_t<T1>> t1c;
			so.swap(t1);
			so.cas(t1);
			so.cas(t1, t1);
			so.cas(t1, t1c);
			so.cmov(std::move(t1));
			so.cmov(std::move(t1), t1);
			so.cmov(std::move(t1), t1c);
			so.cmov(like::shared_ptr<int>(t1));
			so.cmov(like::shared_ptr<int>(t1), t1);
			so.cmov(like::shared_ptr<int>(t1), t1c);
			t1 = so.load();
			so.load([&](auto const & o)
			{
				t1 = o;
			});
		}
	public:
		
		TEST_METHOD(compile_shared_ptr)
		{
			like::shared_ptr<int> sp0;
			like::weak_ptr<int> wp1;
			like::atomic_shared_ptr<int> asp2;
			like::atomic_weak_ptr<int> awp3;

			test_shared_ptr(sp0);
			test_shared_ptr(asp2);

			test_weak_ptr(wp1);
			test_weak_ptr(awp3);

			test_shared_obj(asp2);
			test_shared_obj(awp3);
		}

	};
}