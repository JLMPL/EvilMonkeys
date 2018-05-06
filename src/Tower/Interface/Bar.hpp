#pragma once
#include "Render/2D/Rectangle.hpp"

class Bar
{
    public:
        Bar() = default;
        ~Bar() = default;

        void init(const vec2& pos, const vec2& size, const vec3& color, u32 maxValue, bool leftie = false);

        void show();
        void hide();

        void setMaxValue(u32 value);
        u32 getMaxValue();

        void setValue(i32 value);
        u32 getValue();

    private:
        void update();

    private:
        Rectangle m_rect;
        Rectangle m_back;

        bool m_isShown = true;
        bool m_leftie = false;

        u32 m_width = 0;
        u32 m_height = 0;

        u32 m_maxValue = 1;
        u32 m_value = 1;
};
