# like2015

like c++ template, see [like2015 wiki page](https://github.com/jrmwng/like2015/wiki) for description...

## Smart pointers with lock-free performance

Please start from [shared_ptr.h](https://github.com/jrmwng/like2015/blob/master/like2015/shared_ptr.h)

    namespace like
    {
      template <typename T> shared_ptr<T>;
      template <typename T> weak_ptr<T>;
      template <typename T> atomic_shared_ptr<T>;
      template <typename T> atomic_weak_ptr<T>;
    }

`like::atomic_shared_ptr<T>` could be your good friend in lock-free programming, if you are friend with it.

## Supplements of &lt;functional&gt;

Please start from [functional.h](https://github.com/jrmwng/like2015/blob/master/like2015/functional.h)

    struct coord_t
    {
        int x;
        int y;
    } acoPosition[10];
    
    // ...
    
    std::sort(std::begin(acoPosition), std::end(acoPosition), like::order_by(&coord_t::x)); // sort coordinates by x
    std::sort(std::begin(acoPosition), std::end(acoPosition), like::order_by(&coord_t::x, std::greater<int>()));

## Sudoku solver without back-tracking in 0.3ms

Please start from [sudoku](https://github.com/jrmwng/like2015/blob/master/sudoku)
