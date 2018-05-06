#include "MessageBus.hpp"

namespace MessageBus
{
    namespace
    {
        constexpr u32 MaxMessages = 128u;

        Message m_queue[MaxMessages];

        u32 m_numMessages = 0;
        u32 m_messageIndex = 0;
    }

    void queueMessage(const Message& message)
    {
        m_queue[m_numMessages++] = message;
    }

    bool getNextMessage(Message& message)
    {
        if (m_numMessages > 0)
        {
            if (m_messageIndex < m_numMessages)
            {
                message = m_queue[m_messageIndex];
                m_messageIndex++;
                return true;
            }
            else
            {
                m_numMessages = 0;
                m_messageIndex = 0;
                return false;
            }
        }

        return false;
    }
}


