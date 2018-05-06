
Spell = {
    name = "Fireball",
    type = SpellType_Projectile, --SpellType_OnSelf, SpellType_OnFocus, SpellType_Projectile, SpellType_Area
    castAnim = "Guy_quickCast.x",
    manaCost = 10,

    cast = function(me, other)
        Projectile proj;
        proj.type = PRJT_BALL;
        proj.anim = "spell_fireball.pan"; --pan -> particle animation file

        World_castProjectileSpell(proj, me);
    end,

    hit = function(one) -- when given projectile hits someone
        Actor_damage(one, 20);
        Actor_setState(one, AS_BURNING);
    end
}
