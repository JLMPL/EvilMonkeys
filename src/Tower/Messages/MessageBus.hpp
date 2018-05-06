#pragma once
#include "Message.hpp"

namespace MessageBus
{
    void queueMessage(const Message& message);
    bool getNextMessage(Message& message);
}
