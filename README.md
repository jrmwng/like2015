# like2015
like c++ template

<pre>
namespace like
{
  template &lt;typename T&gt; shared_ptr&lt;T&gt;;
  template &lt;typename T&gt; weak_ptr&lt;T&gt;;
  
  template &lt;std::uint8_t uCount16&gt; shared_lock;
  
  template &lt;typename T            &gt; void atomic_store(T       volatile *, T);
  template &lt;typename T            &gt; T    atomic_load (T const volatile *);
  template &lt;typename T            &gt; void atomic_swap (T       volatile *, T *);
  template &lt;typename T, typename U&gt; bool atomic_cas  (T       volatile *, T *, U);
}
</pre>
