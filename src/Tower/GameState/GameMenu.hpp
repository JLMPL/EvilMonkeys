#pragma once
#include "GameState.hpp"
#include "Render/2D/Font.hpp"
#include "Render/2D/Text.hpp"
#include "Render/2D/Rectangle.hpp"
#include "Render/3D/Camera.hpp"

struct Message;

class GameMenu : public GameState
{
    public:
        GameMenu() = default;
        ~GameMenu() = default;

        void init() override final;
        void update(seconds deltaTime) override final;
        void exit() override final;

        void sendMessage(const Message& message) override final;

    private:
        struct Option
        {
            enum
            {
                Start,
                Help,
                Settings,
                Exit,

                Count
            };

            Text text;
            Rectangle back;
        };

        Font m_font;
        Text m_text;
        Rectangle m_back;

        Option m_options[Option::Count];

        i32 m_selection = 0;

        Camera m_camera;
        seconds m_timer;
};
