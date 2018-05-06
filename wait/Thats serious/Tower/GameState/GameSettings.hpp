#pragma once
#include "GameState.hpp"
#include "Render/2D/Font.hpp"
#include "Render/2D/Text.hpp"
#include "Render/2D/Rectangle.hpp"
#include "Render/3D/Camera.hpp"
#include "Interface/Bar.hpp"

struct Message;

class GameSettings : public GameState
{
    public:
        GameSettings() = default;
        ~GameSettings() = default;

        void init() override final;
        void update(seconds deltaTime) override final;
        void exit() override final;

        void sendMessage(const Message& message) override final;

    private:
        void initOptions();
        void initBack();
        void writeOutConfig();

    private:
        struct Option
        {
            enum
            {
                Music,
                Effects,

                Count
            };

            Text text;
            Bar bar;
            Rectangle rect;
        };

        Font m_font;
        Text m_text;
        Rectangle m_back;

        Option m_options[Option::Count];

        Text m_optBack;
        Rectangle m_optBackRect;

        Rectangle m_resBack;
        Text m_resText;

        i32 m_selection = 0;
        i32 m_resolution = 0;

        Camera m_camera;
        seconds m_timer;
};
