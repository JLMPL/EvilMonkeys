#pragma once
#include "ActorState.hpp"

BEGIN_ACTOR_STATE(PlayerMovement)
    bool m_controls[4] = {false};
    bool m_dodge = false;
END_ACTOR_STATE()
