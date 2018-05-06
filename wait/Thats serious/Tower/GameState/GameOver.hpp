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
        Font m_font;

        Text m_over;
        Text m_exit;

        Rectangle m_rect;

        Camera m_camera;
};
