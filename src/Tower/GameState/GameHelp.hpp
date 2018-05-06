#pragma once
#include "GameState.hpp"
#include "Render/2D/Font.hpp"
#include "Render/2D/Text.hpp"
#include "Render/2D/Rectangle.hpp"
#include "Render/3D/Camera.hpp"

struct Message;

class GameHelp : public GameState
{
    public:
        GameHelp() = default;
        ~GameHelp() = default;

        void init() override final;
        void update(seconds deltaTime) override final;
        void exit() override final;

        void sendMessage(const Message& message) override final;

    private:
        Rectangle m_back;

        Font m_font;
        Text m_text;

        Text m_help[6];
        Rectangle m_rect;

        Camera m_camera;
        seconds m_timer;
};
