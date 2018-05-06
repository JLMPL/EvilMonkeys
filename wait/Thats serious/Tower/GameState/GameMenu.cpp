#include "GameMenu.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"

constexpr u32 gOptionWidth = 192u;
constexpr u32 gOptionHeight = 48u;
constexpr u32 gInterOptionOffset = 2u;
constexpr u32 gTopOffset = -150u;
constexpr u32 gTitleOffset = 100u;

void GameMenu::init()
{
    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_font.loadFromFile("Fonts/dis.fnt");

    u32 pint = Config::Display::Height * 0.5 + gTopOffset;

    m_text.init(&m_font);
    m_text.setString("Some Demo Stuff", 1);
    m_text.setPosition(vec2(Config::Display::Width * 0.5 - m_text.getWidth() * 0.5, pint));

    m_back.init(vec2(Config::Display::Width, Config::Display::Height));
    m_back.setColor(vec4(0,0,0,0.5));
    m_back.setPosition(vec2(0));

    gRenderer2D.registerRectangle(&m_back);
    gRenderer2D.registerText(&m_text);

    for (u32 i = 0; i < Option::Count; i++)
    {
        Option& opt = m_options[i];

        opt.text.init(&m_font);

        gRenderer2D.registerRectangle(&opt.back);
        gRenderer2D.registerText(&opt.text);
    }

    m_options[Option::Start].text.setString("Start", 0.5);
    m_options[Option::Help].text.setString("Help", 0.5);
    m_options[Option::Settings].text.setString("Settings", 0.5);
    m_options[Option::Exit].text.setString("Exit", 0.5);

    for (u32 i = 0; i < Option::Count; i++)
    {
        Option& opt = m_options[i];

        opt.text.setPosition(vec2(
            Config::Display::Width * 0.5 - opt.text.getWidth() * 0.5,
            pint + gTitleOffset + (i * (gOptionHeight + gInterOptionOffset))
        ));

        opt.back.init(vec2(gOptionWidth, gOptionHeight));
        opt.back.setColor(vec4(0,0,0,0.5));
        opt.back.setPosition(vec2(
            Config::Display::Width * 0.5 - gOptionWidth * 0.5,
            pint + gTitleOffset + (i * (gOptionHeight + gInterOptionOffset))
        ));
    }

    m_timer = seconds(0);
}

void GameMenu::exit()
{
    gRenderer2D.clearAllData();
}

void GameMenu::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        if (m_timer > seconds(0.15))
        {
            if (message.keyboard.key == Config::Controls::Up)
            {
                m_selection = math::max(--m_selection, 0);
                m_timer = seconds(0);
            }
            else if (message.keyboard.key == Config::Controls::Down)
            {
                m_selection = math::min(++m_selection, Option::Count - 1);
                m_timer = seconds(0);
            }
        }
        if (m_timer > seconds(0.2))
        {
            if (message.keyboard.key == Config::Controls::Return)
            {
                Message message(Message::Type::GameStateSet);

                switch (m_selection)
                {
                    case Option::Start:
                    {
                        message.state = i32(GameState::Type::Playing);
                    }
                    break;
                    case Option::Help:
                    {
                        message.state = i32(GameState::Type::Help);
                    }
                    break;
                    case Option::Settings:
                    {
                        message.state = i32(GameState::Type::Settings);
                    }
                    break;
                    case Option::Exit:
                    {
                        message.type = Message::Type::GameStateExit;
                    }
                    break;
                }

                MessageBus::queueMessage(message);
            }
            else if (message.keyboard.key == Config::Controls::Exit)
            {
                Message message(Message::Type::GameStateExit);
                MessageBus::queueMessage(message);
            }
        }
    }
}

void GameMenu::update(seconds deltaTime)
{
    m_timer += deltaTime;

    for (u32 i = 0; i < Option::Count; i++)
    {
        if (i == m_selection)
            m_options[i].back.setColor(vec4(0,0,0,0.5));
        else
            m_options[i].back.setColor(vec4(0));
    }
}
