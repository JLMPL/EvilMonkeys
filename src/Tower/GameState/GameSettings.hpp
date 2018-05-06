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
        void initTitle();
        void initRes();
        void initMusic();
        void initEffects();
        void initBack();

        void writeOutConfig();

        void handleSelection(const Message& message);
        void handleResolution(const Message& message);
        void handleMusic(const Message& message);
        void handleEffects(const Message& message);
        void handleBack(const Message& message);

        void updateResolutions();
        void updateMusic();
        void updateEffects();
        void updateBack();

    private:
        enum Selection
        {
            Resolutions = 0,
            Musics,
            Effects,
            Backs
        };

        Font         m_font;

        Text         m_titleText;
        Rectangle    m_titleBack;

        Text         m_resText;
        Rectangle    m_resBack;

        Text         m_musicText;
        Bar          m_musicBar;
        Rectangle    m_musicBack;

        Text         m_effectsText;
        Bar          m_effectsBar;
        Rectangle    m_effectsBack;

        Text         m_backText;
        Rectangle    m_backBack;

        i32          m_selection = 0;
        i32          m_resolution = 0;

        Camera       m_camera;
        seconds      m_timer;
};
