#pragma once
#include "ActorState.hpp"

BEGIN_ACTOR_STATE(PlayerIdle)
    bool m_controls[4];
    bool m_prep = false;
END_ACTOR_STATE()
