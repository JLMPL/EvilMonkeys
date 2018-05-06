#pragma once
#include "Music.hpp"
#include "Sound.hpp"

struct Message;

class Audio
{
    public:
        Audio() = default;
        ~Audio();

        void init();
        void refresh();

        void sendMessage(const Message& message);

    private:
        Music m_newMagincia;

        Sound m_hurt;
        Sound m_swing;
};
