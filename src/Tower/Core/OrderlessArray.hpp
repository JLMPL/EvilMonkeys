#pragma once
#include "Core/Types.hpp"

template <typename T, u32 MaxSize>
class OrderlessArray
{
    public:
        OrderlessArray() = default;

        T& operator[](u32 index)
        {
            return m_data[index];
        }

        void add(const T& stuff)
        {
            m_data[m_size++] = stuff;
        }

        void remove(u32 index)
        {
            std::swap(m_data[index], m_data[--m_size]);
        }

        void remove(const T& stuff)
        {
            for (u32 i = 0; i < m_size; i++)
            {
                if (m_data[i] == stuff)
                {
                    remove(i);
                    return;
                }
            }
        }

        void clear()
        {
            m_size = 0;
        }

        u32 size()
        {
            return m_size;
        }

    private:
        T m_data[MaxSize];
        u32 m_size = 0u;
};
