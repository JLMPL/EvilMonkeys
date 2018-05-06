

function onDamage(sender, value)

    roll = math.random(1, 100);

    Actor_setFocus(sender);

    if roll <= 66 then
        Actor_damage(value);
        Actor_setState(AS_HIT);
    else
        Actor_setState(AS_DODGE);
    end
end
