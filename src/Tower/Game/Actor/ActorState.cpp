#include "ActorState.hpp"
#include "Messages/MessageBus.hpp"
#include "../Actor.hpp"

void ActorState::setState(Type state, u32 flags)
{
    Message message(Message::Type::ActorSetState);
    message.state = i32(state);
    message.stateFlags = flags;
    message.recieverEntity = m_actor->_id;

    MessageBus::queueMessage(message);
}
