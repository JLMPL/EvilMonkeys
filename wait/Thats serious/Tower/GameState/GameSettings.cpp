#include "GameSettings.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"
#include <fstream>
#include <array>

constexpr u32 gTopOffset = -120u;
constexpr u32 gTitleOffset = 100u;
constexpr u32 gBarWidth = 512u;
constexpr u32 gBarHeight = 48u;
constexpr u32 gInterBarOffset = 32u;

struct Resolution
{
    u32 width, height;
    bool fullscreen;
};

constexpr static std::array<Resolution, 16> gResolutions =
{{
    {640u, 480u, false},
    {640u, 480u, true},
    {800u, 600u, false},
    {800u, 600u, true},
    {1024u, 600u, false},
    {1024u, 600u, true},
    {1024u, 768u, false},
    {1024u, 768u, true},
    {1280u, 720u, false},
    {1280u, 720u, true},
    {1366u, 768u, false},
    {1366u, 768u, true},
    {1600u, 900u, false},
    {1600u, 900u, true},
    {1920u, 1080u, false},
    {1920u, 1080u, true}
}};

static u32 pint = Config::Display::Height * 0.5 + gTopOffset;

void GameSettings::initOptions()
{
    for (u32 i = 0; i < Option::Count; i++)
    {
        Option& opt = m_options[i];

        gRenderer2D.registerRectangle(&opt.rect);

        opt.text.init(&m_font);
        opt.bar.init(
            {
                Config::Display::Width * 0.5 - gBarWidth * 0.5,
                pint + gTitleOffset + gBarHeight + gInterBarOffset + i * (gBarHeight + gInterBarOffset)
            },
            {gBarWidth, gBarHeight},
            {0,0.6,0.2},
            100
        );

        gRenderer2D.registerText(&opt.text);
    }

    m_options[Option::Music].text.setString("Music", 0.5);
    m_options[Option::Music].bar.setValue(Config::Settings::MusicVolume);
    m_options[Option::Effects].text.setString("Effects", 0.5);
    m_options[Option::Music].bar.setValue(Config::Settings::EffectsVolume);

    for (u32 i = 0; i < Option::Count; i++)
    {
        Option& opt = m_options[i];

        opt.rect.init({gBarWidth + 8, gBarHeight + 8});
        opt.rect.setColor({0,0,0,0.5});
        opt.rect.setPosition({
            (Config::Display::Width * 0.5 - gBarWidth * 0.5) - 4,
            (pint + gTitleOffset + gBarHeight + gInterBarOffset + i * (gBarHeight + gInterBarOffset)) - 4
        });

        opt.text.setPosition({
            Config::Display::Width * 0.5 - opt.text.getWidth() * 0.5,
            pint + gTitleOffset + gBarHeight + gInterBarOffset + i * (gBarHeight + gInterBarOffset)
        });
    }
}

void GameSettings::initBack()
{
    m_optBack.init(&m_font);
    m_optBack.setString("Back", 0.5);
    m_optBack.setPosition(vec2(
        Config::Display::Width * 0.5 - m_optBack.getWidth() * 0.5,
        pint + gTitleOffset + 160 + gBarHeight + gInterBarOffset
    ));

    m_optBackRect.init({192,48});
    m_optBackRect.setColor(vec4(0,0,0,0.5));
    m_optBackRect.setPosition(vec2(
        Config::Display::Width * 0.5 - 192 * 0.5,
        pint + gTitleOffset + 160 + gBarHeight + gInterBarOffset
    ));

    gRenderer2D.registerText(&m_optBack);
    gRenderer2D.registerRectangle(&m_optBackRect);
}

void GameSettings::init()
{
    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_font.loadFromFile("Fonts/dis.fnt");

    m_text.init(&m_font);
    m_text.setString("Settings", 1);
    m_text.setPosition(vec2(Config::Display::Width * 0.5 - m_text.getWidth() * 0.5, pint));

    m_back.init(vec2(Config::Display::Width, Config::Display::Height));
    m_back.setColor(vec4(0,0,0,0.5));

    gRenderer2D.registerRectangle(&m_back);
    gRenderer2D.registerText(&m_text);

    initOptions();
    initBack();

    m_resText.init(&m_font);
    m_resText.setString("1280x720 Fullscreen", 0.5);
    m_resText.setPosition(vec2(
        Config::Display::Width * .5 - m_resText.getWidth() * .5, pint + gTitleOffset
    ));

    m_resBack.init({512, 48});
    m_resBack.setColor({0,0,0,0.5});
    m_resBack.setPosition(vec2(
        Config::Display::Width * .5 - 256, pint + gTitleOffset
    ));

    gRenderer2D.registerText(&m_resText);
    gRenderer2D.registerRectangle(&m_resBack);

    m_timer = seconds(0);
}

void GameSettings::exit()
{
    gRenderer2D.clearAllData();
}

void GameSettings::writeOutConfig()
{
    std::ofstream file("config.ini");

    std::string width = "DisplayWidth = " + std::to_string(gResolutions[m_resolution].width);
    std::string height = "DisplayHeight = " + std::to_string(gResolutions[m_resolution].height);
    std::string oneorzero = (gResolutions[m_resolution].fullscreen) ? "1" : "0";
    std::string full = "DisplayFullscreen = " + oneorzero;

    file << width << "\n";
    file << height << "\n";
    file << full;

    file.close();
}

void GameSettings::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        if (m_timer > seconds(0.1))
        {
            if (message.keyboard.key == Config::Controls::Up)
            {
                m_selection = math::max(--m_selection, 0);
                m_timer = seconds(0);
            }
            else if (message.keyboard.key == Config::Controls::Down)
            {
                m_selection = math::min(++m_selection, 3);
                m_timer = seconds(0);
            }
            else if (message.keyboard.key == Config::Controls::Left)
            {
                if (m_selection == 0)
                    m_resolution = math::max(--m_resolution, 0);
                m_timer = seconds(0);

            }
            else if (message.keyboard.key == Config::Controls::Right)
            {
                if (m_selection == 0)
                    m_resolution = math::min(++m_resolution, i32(gResolutions.size()-1));
                m_timer = seconds(0);
            }

        }
        if (m_timer > seconds(0.025))
        {
            if (message.keyboard.key == Config::Controls::Left)
            {
                if (m_selection == 1)
                    m_options[m_selection].bar.setValue(--Config::Settings::MusicVolume);
                else if (m_selection == 2)
                    m_options[m_selection].bar.setValue(--Config::Settings::EffectsVolume);

                m_timer = seconds(0);
            }
            else if (message.keyboard.key == Config::Controls::Right)
            {
                if (m_selection == 1)
                    m_options[m_selection].bar.setValue(++Config::Settings::MusicVolume);
                else if (m_selection == 2)
                    m_options[m_selection].bar.setValue(++Config::Settings::EffectsVolume);

                if (Config::Settings::MusicVolume > 100)
                    Config::Settings::MusicVolume = 100;

                if (Config::Settings::EffectsVolume > 100)
                    Config::Settings::EffectsVolume = 100;

                m_timer = seconds(0);
            }
        }
        if (m_timer > seconds(0.2))
        {
            if (message.keyboard.key == Config::Controls::Exit)
            {
                Message message(Message::Type::GameStateSet);
                message.state = i32(GameState::Type::Menu);
                MessageBus::queueMessage(message);
            }
            else if (m_selection == 3 && message.keyboard.key == Config::Controls::Return)
            {
                Message message(Message::Type::GameStateSet);
                message.state = i32(GameState::Type::Menu);
                MessageBus::queueMessage(message);
            }
            else if (m_selection == 0 && message.keyboard.key == Config::Controls::Return)
            {
                writeOutConfig();
            }
        }
    }
}

void GameSettings::update(seconds deltaTime)
{
    m_timer += deltaTime;

    if (m_selection == 0)
        m_resBack.setColor({0,0,0,0.5});
    else
        m_resBack.setColor(vec4(0));

    for (u32 i = 0; i < Option::Count; i++)
    {
        if (i == m_selection - 1)
            m_options[i].rect.setColor({0,0,0,0.5});
        else
            m_options[i].rect.setColor(vec4(0));
    }

    m_options[Option::Music].bar.setValue(Config::Settings::MusicVolume);
    m_options[Option::Effects].bar.setValue(Config::Settings::EffectsVolume);

    if (m_selection == 3)
        m_optBackRect.setColor(vec4(0,0,0,0.5));
    else
        m_optBackRect.setColor(vec4(0));

    std::string shit = (gResolutions[m_resolution].fullscreen) ? " Fullscreen" : " Windowed";

    m_resText.setString(
        std::to_string(gResolutions[m_resolution].width) + "x" +
        std::to_string(gResolutions[m_resolution].height) +
        shit
    , .5);
}
