#include "GameOver.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"

constexpr const char* gCredits[] =
{
    "Evil Monkey Fencing",
    "",
    "Project Lead Director",
    "Dominik Korotkiewicz",
    "",
    "Lead Designer",
    "Dominik Korotkiewicz",
    "",

    "Lead Programmer",
    "Dominik Korotkiewicz",
    "",
    "Lead Artist",
    "Dominik Korotkiewicz",
    "",
    "Chief of Staff",
    "Dominik Korotkiewicz",

    "",
    "QA Lead",
    "Dominik Korotkiewicz",
    "",
    "Chief of Marketing",
    "Dominik Korotkiewicz",
    "",
    "Special Thanks",

    "Textures:",
    "textures.org",
    "Some stolen from Gothic II",
    "",
    "Sound:",
    "Music is \"New Magincia\" from Ultima IX: Ascension",
    "Strike sound: https://tinyurl.com/y7326vrn",
    "Swoosh sound: https://tinyurl.com/yaqqm5ph",
    "",
    "This is non-commercial project.",
    "I claim no ownership of copyright",
    "of items listed above.",
    "They only serve placeholder role anyway."
};

void GameOver::initCredits()
{
    for (u32 i = 0; i < NumCredits; i++)
    {
        Text& credit = m_credits[i].text;

        credit.init(&m_font);
        credit.setString(gCredits[i], 0.5);
        credit.setPosition(vec2(
            Config::Display::Width * 0.5 - credit.getWidth() * 0.5,
            m_scrollPos + i * 48
        ));

        gRenderer2D.registerText(&credit);
    }
}

void GameOver::initBack()
{
    m_exit.init(&m_font);
    m_exit.setString("[Any Key To Exit]", 0.5);
    m_exit.setPosition(vec2(
        32,
        Config::Display::Height - m_exit.getHeight() - 32
    ));

    gRenderer2D.registerText(&m_exit);
}

void GameOver::init()
{
    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_scrollPos = Config::Display::Height * 0.5;

    m_font.loadFromFile("Fonts/dis.fnt");

    m_rect.init(vec2(Config::Display::Width, Config::Display::Height));
    m_rect.setColor(vec4(0,0,0,1));
    m_rect.setPosition(vec2(0));

    gRenderer2D.registerRectangle(&m_rect);

    initCredits();
    initBack();

    m_timer = seconds(0);
}

void GameOver::exit()
{
    gRenderer2D.clearAllData();
}

void GameOver::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed &&
        m_timer > seconds(1))
    {
        Message message(Message::Type::GameStateExit);
        MessageBus::queueMessage(message);
    }
}

void GameOver::updateCredits()
{
    for (u32 i = 0; i < NumCredits; i++)
    {
        m_credits[i].text.setPosition(vec2(
            Config::Display::Width * 0.5 - m_credits[i].text.getWidth() * 0.5,
            m_scrollPos + i * 48
        ));
    }

    if (m_credits[36].text.getPosition().y < -50)
    {
        Message message(Message::Type::GameStateExit);
        MessageBus::queueMessage(message);
    }
}

void GameOver::update(seconds deltaTime)
{
    m_timer += deltaTime;
    m_scrollPos -= deltaTime.count() * 32;

    updateCredits();
}
