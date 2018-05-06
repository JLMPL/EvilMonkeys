#pragma once
#include "GameState.hpp"
#include "Render/2D/Font.hpp"
#include "Render/2D/Text.hpp"
#include "Render/2D/Rectangle.hpp"
#include "Render/3D/Camera.hpp"

struct Message;

class GameOver : public GameState
{
    public:
        GameOver() = default;
        ~GameOver() = default;

        void init() override final;
        void update(seconds deltaTime) override final;
        void exit() override final;

        void sendMessage(const Message& message) override final;

    private:
        void initCredits();
        void initBack();

        void updateCredits();

    private:
        struct Credit
        {
            Text text;
        };

        constexpr static u32 NumCredits = 37u;

        Font m_font;
        Rectangle m_rect;

        Credit m_credits[NumCredits];
        Text m_exit;

        float m_scrollPos;

        Camera m_camera;
        seconds m_timer;
};
