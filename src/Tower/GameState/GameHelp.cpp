#include "GameHelp.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"

constexpr const char* gHelps[6] =
{
    "Movement: Arrow Keys",
    "Strong Attack: Ctrl + Up",
    "Quick Attack: Ctrl + Down",
    "Block: Alt + Down",
    "Dodge Left: Alt + Left",
    "Dodge Right: Alt + Right"
};

void GameHelp::init()
{
    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_font.loadFromFile("Fonts/dis.fnt");

    m_text.init(&m_font);
    m_text.setString("Back", 0.5);
    m_text.setPosition(vec2(
        Config::Display::Width * 0.5 - m_text.getWidth() * 0.5,
        Config::Display::Height - 100
    ));

    m_rect.init({192,48});
    m_rect.setColor({0,0,0,0.5});
    m_rect.setPosition(vec2(
        Config::Display::Width * 0.5 - 192 * 0.5,
        Config::Display::Height - 100
    ));

    for (u32 i = 0; i < 6; i++)
    {
        m_help[i].init(&m_font);
        m_help[i].setString(gHelps[i], 0.5);
        m_help[i].setPosition(vec2(
            Config::Display::Width * 0.5 - m_help[i].getWidth() * 0.5,
            (Config::Display::Height * 0.5 - 200) + i * 64
        ));

        gRenderer2D.registerText(&m_help[i]);
    }

    m_back.init({Config::Display::Width, Config::Display::Height});
    m_back.setColor({0,0,0,0.5});

    gRenderer2D.registerRectangle(&m_back);
    gRenderer2D.registerRectangle(&m_rect);
    gRenderer2D.registerText(&m_text);

    m_timer = seconds(0);
}

void GameHelp::exit()
{
    gRenderer2D.clearAllData();
}

void GameHelp::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        if (m_timer > seconds(0.2))
        {
            if (message.keyboard.key == Config::Controls::Exit ||
                message.keyboard.key == Config::Controls::Return)
            {
                Message message(Message::Type::GameStateSet);
                message.state = i32(GameState::Type::Menu);
                MessageBus::queueMessage(message);
                m_timer = seconds(0);
            }
        }
    }
}

void GameHelp::update(seconds deltaTime)
{
    m_timer += deltaTime;
}
