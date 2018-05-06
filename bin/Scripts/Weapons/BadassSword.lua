
Weapon = {
    name = "The Badass Sword",
    type = WeaponType_OneHanded,
    mesh = "wp_badass_sword.obj",

    damage = 12,
    range = 10,

    reqStrength = 10,
    value = 200,

    attack = function(me, other)
        Actor_damage(other, damage);
    end,

    block = function(me, other)
        Actor_heal(me, damage * 0.25);
    end
}