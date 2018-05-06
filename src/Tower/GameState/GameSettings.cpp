#include "GameSettings.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"
#include <fstream>
#include <array>

constexpr u32 gTopOffset = -160u;
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

static u32 pint = Config::Display::Height * .5 + gTopOffset;

void GameSettings::initTitle()
{
    m_titleText.init(&m_font);
    m_titleText.setString("Settings", 1);
    m_titleText.setPosition(vec2(Config::Display::Width * .5 - m_titleText.getWidth() * .5, pint));

    m_titleBack.init(vec2(Config::Display::Width, Config::Display::Height));
    m_titleBack.setColor(vec4(0,0,0,.5));

    gRenderer2D.registerRectangle(&m_titleBack);
    gRenderer2D.registerText(&m_titleText);
}

void GameSettings::initRes()
{
    m_resText.init(&m_font);
    m_resText.setString("1280x720 Fullscreen", .5);
    m_resText.setPosition(vec2(
        Config::Display::Width * .5 - m_resText.getWidth() * .5, pint + gTitleOffset
    ));

    m_resBack.init({512, 48});
    m_resBack.setColor({0,0,0,.5});
    m_resBack.setPosition(vec2(
        Config::Display::Width * .5 - 256, pint + gTitleOffset
    ));

    gRenderer2D.registerText(&m_resText);
    gRenderer2D.registerRectangle(&m_resBack);
}

void GameSettings::initMusic()
{
    m_musicBack.init({gBarWidth + 8, gBarHeight + 8});
    m_musicBack.setColor({0,0,0,.5});
    m_musicBack.setPosition({
        (Config::Display::Width * .5 - gBarWidth * .5) - 4,
        (pint + gTitleOffset + gBarHeight + gInterBarOffset) - 4
    });

    gRenderer2D.registerRectangle(&m_musicBack);

    m_musicText.init(&m_font);
    m_musicText.setString("Music", .5);
    m_musicText.setPosition({
        Config::Display::Width * .5 - m_musicText.getWidth() * .5,
        pint + gTitleOffset + gBarHeight + gInterBarOffset
    });

    gRenderer2D.registerText(&m_musicText);

    m_musicBar.init(
        {
            Config::Display::Width * .5 - gBarWidth * .5,
            pint + gTitleOffset + gBarHeight + gInterBarOffset
        },
        {gBarWidth, gBarHeight},
        {0,0.6,0.2},
        100
    );
}

void GameSettings::initEffects()
{
    m_effectsBack.init({gBarWidth + 8, gBarHeight + 8});
    m_effectsBack.setColor({0,0,0,.5});
    m_effectsBack.setPosition({
        (Config::Display::Width * .5 - gBarWidth * .5) - 4,
        (pint + gTitleOffset + gBarHeight + gInterBarOffset + 1 * (gBarHeight + gInterBarOffset)) - 4
    });

    gRenderer2D.registerRectangle(&m_effectsBack);

    m_effectsText.init(&m_font);
    m_effectsText.setString("Music", .5);
    m_effectsText.setPosition({
        Config::Display::Width * .5 - m_effectsText.getWidth() * .5,
        pint + gTitleOffset + gBarHeight + gInterBarOffset + 1 * (gBarHeight + gInterBarOffset)
    });

    gRenderer2D.registerText(&m_effectsText);

    m_effectsBar.init(
        {
            Config::Display::Width * .5 - gBarWidth * .5,
            pint + gTitleOffset + gBarHeight + gInterBarOffset + 1 * (gBarHeight + gInterBarOffset)
        },
        {gBarWidth, gBarHeight},
        {0,0.6,0.2},
        100
    );
}

void GameSettings::initBack()
{
    m_backText.init(&m_font);
    m_backText.setString("Back", .5);
    m_backText.setPosition(vec2(
        Config::Display::Width * .5 - m_backText.getWidth() * .5,
        pint + gTitleOffset + 160 + gBarHeight + gInterBarOffset
    ));

    m_backBack.init({192,48});
    m_backBack.setColor(vec4(0,0,0,.5));
    m_backBack.setPosition(vec2(
        Config::Display::Width * .5 - 192 * .5,
        pint + gTitleOffset + 160 + gBarHeight + gInterBarOffset
    ));

    gRenderer2D.registerText(&m_backText);
    gRenderer2D.registerRectangle(&m_backBack);
}

void GameSettings::init()
{
    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_font.loadFromFile("Fonts/dis.fnt");

    pint = Config::Display::Height * .5 + gTopOffset;

    initTitle();
    initRes();
    initMusic();
    initEffects();
    initBack();

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

void GameSettings::handleSelection(const Message& message)
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
            m_selection = math::min(++m_selection, i32(Selection::Backs));
            m_timer = seconds(0);
        }
        else if (message.keyboard.key == Config::Controls::Exit)
        {
            Message message(Message::Type::GameStateSet);
            message.state = i32(GameState::Type::Menu);
            MessageBus::queueMessage(message);
        }
    }
}

void GameSettings::handleResolution(const Message& message)
{
    if (m_timer > seconds(0.2))
    {
        if (m_selection == Selection::Resolutions)
        {
            switch (message.keyboard.key)
            {
                case Config::Controls::Left:
                    m_resolution = math::max(--m_resolution, 0);
                    m_timer = seconds(0);
                    break;
                case Config::Controls::Right:
                    m_resolution = math::min(++m_resolution, i32(gResolutions.size()-1));
                    m_timer = seconds(0);
                    break;
                case Config::Controls::Return:
                {
                    writeOutConfig();

                    Message message(Message::Type::EngineChangeResolution);
                    MessageBus::queueMessage(message);

                    Message messagee(Message::Type::GameStateSet);
                    messagee.state = i32(GameState::Type::Menu);
                    MessageBus::queueMessage(messagee);

                    Message message3(Message::Type::GameStateSet);
                    message3.state = i32(GameState::Type::Settings);
                    MessageBus::queueMessage(message3);

                    m_timer = seconds(0);
                }
                break;
            }
        }
    }
}

void GameSettings::handleMusic(const Message& message)
{
    if (m_timer > seconds(0.025) && m_selection == Selection::Musics)
    {
        if (message.keyboard.key == Config::Controls::Left)
        {
            Config::Settings::MusicVolume = math::max(--Config::Settings::MusicVolume, 0);
            m_timer = seconds(0);
        }
        else if (message.keyboard.key == Config::Controls::Right)
        {
            Config::Settings::MusicVolume = math::min(++Config::Settings::MusicVolume, 100);
            m_timer = seconds(0);
        }
    }
}

void GameSettings::handleEffects(const Message& message)
{
    if (m_timer > seconds(0.025) && m_selection == Selection::Effects)
    {
        if (message.keyboard.key == Config::Controls::Left)
        {
            Config::Settings::EffectsVolume = math::max(--Config::Settings::EffectsVolume, 0);
            m_timer = seconds(0);
        }
        else if (message.keyboard.key == Config::Controls::Right)
        {
            Config::Settings::EffectsVolume = math::min(++Config::Settings::EffectsVolume, 100);
            m_timer = seconds(0);
        }
    }
}

void GameSettings::handleBack(const Message& message)
{
    if (m_timer > seconds(0.2))
    {
        if (m_selection == Selection::Backs &&
            message.keyboard.key == Config::Controls::Return)
        {
            Message message(Message::Type::GameStateSet);
            message.state = i32(GameState::Type::Menu);
            MessageBus::queueMessage(message);

            m_timer = seconds(0);
        }
    }
}

void GameSettings::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        handleSelection(message);
        handleResolution(message);
        handleMusic(message);
        handleEffects(message);
        handleBack(message);
    }
}

void GameSettings::updateResolutions()
{
    std::string shit = (gResolutions[m_resolution].fullscreen) ? " Fullscreen" : " Windowed";

    m_resText.setString(
        std::to_string(gResolutions[m_resolution].width) + "x" +
        std::to_string(gResolutions[m_resolution].height) +
        shit
    , .5);

    if (m_selection == Selection::Resolutions)
        m_resBack.setColor({0,0,0,.5});
    else
        m_resBack.setColor({0,0,0,0});
}

void GameSettings::updateMusic()
{
    m_musicBar.setValue(Config::Settings::MusicVolume);

    if (m_selection == Selection::Musics)
        m_musicBack.setColor({0,0,0,.5});
    else
        m_musicBack.setColor({0,0,0,0});
}

void GameSettings::updateEffects()
{
    m_effectsBar.setValue(Config::Settings::EffectsVolume);

    if (m_selection == Selection::Effects)
        m_effectsBack.setColor({0,0,0,.5});
    else
        m_effectsBack.setColor({0,0,0,0});
}

void GameSettings::updateBack()
{
    if (m_selection == Selection::Backs)
        m_backBack.setColor({0,0,0,.5});
    else
        m_backBack.setColor({0,0,0,0});
}

void GameSettings::update(seconds deltaTime)
{
    m_timer += deltaTime;

    updateResolutions();
    updateMusic();
    updateEffects();
    updateBack();
}
