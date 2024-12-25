#include "All.h"

#include "..\CommonDefines.h"
#include "..\Bullet.h"

void RegisterBullets(){
    gBulletTable.insert({Bullets::NOTHING, {
        {0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0}, 1, 1},
        [](BULLET* bul){ }
    }});

    RegisterSnake();
    RegisterFireball();
    RegisterBubbler();
    RegisterMachineGun();
    RegisterSpur();
    RegisterBoomerangBlades();
}