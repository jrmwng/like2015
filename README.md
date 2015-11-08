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

## Sudoku solver using pre-emptive set techique

Please start from [sudoku](https://github.com/jrmwng/like2015/blob/master/sudoku)
