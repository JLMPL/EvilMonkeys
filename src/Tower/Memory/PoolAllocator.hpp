#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP
// #include "Core/Types.hpp"
#include <stdlib.h>

template <typename T>
class PoolAllocator
{
    public:
        using value_type = T;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        PoolAllocator(u32 number)
        {

        }

        ~PoolAllocator()
        {

        }

        T* allocate(std::size_t n)
        {

        }

        void deallocate(T* p, std::size_t n)
        {

        }

        template <typename... TArgs>
        void construct(T* p, TArgs&&... args)
        {
            *p = T(args...);
        }

        void destroy(T* p)
        {
            p->~T();
        }

    private:

};

#endif
