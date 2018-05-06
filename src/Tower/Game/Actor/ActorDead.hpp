#pragma once
#include "ActorState.hpp"

BEGIN_ACTOR_STATE(ActorDead)
    bool m_sent = false;
    seconds m_timer;
END_ACTOR_STATE()
