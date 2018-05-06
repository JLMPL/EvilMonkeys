#ifndef BASIC_ALLOCATOR_HPP
#define BASIC_ALLOCATOR_HPP
#include <stdlib.h>

template <typename T>
class BasicAllocator
{
    public:
        using value_type = T;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        BasicAllocator() = default;
        ~BasicAllocator() = default;

        T* allocate(std::size_t n)
        {
            return static_cast<T*>(malloc(sizeof(T) * n));
        }

        void deallocate(T* p, std::size_t n)
        {
            free(p);
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
};

#endif
