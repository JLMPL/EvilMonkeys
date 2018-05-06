#pragma once
#include "Bar.hpp"
#include "Render/2D/Font.hpp"
#include "Render/2D/Text.hpp"

struct Message;

class GameInterface
{
    public:
        GameInterface() = default;
        ~GameInterface() = default;

        void init();
        void exit();

        void sendMessage(const Message& message);

    private:
        Bar m_health;
        Bar m_enemyHealth;
        Rectangle m_back;

        Font m_font;
        Text m_text;
};
