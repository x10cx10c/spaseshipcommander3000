#ifndef SHIP_H__
#define SHIP_H__

#include "../../../inc/raylib.h"
#include <stdint.h>
#include <stdbool.h>

void SHIP_Init(void);
void SHIP_Update(void);
void SHIP_Draw(void);
_Bool SHIPsensors_FindNearestTarget(float* angle, int* index, int frbd, Vector2* from);

void SHIP_draw(void);
void SHIP_hull_init(void);
void SHIP_DETAILS_Update(void);

#endif /*SHIP_H__*/