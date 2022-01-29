#ifndef GUNS_H__
#define GUNS_H__
#include "../../../inc/raylib.h"
#include "drawingEngine.h"
#include "typedefg.h"

typedef struct{
  Vector2 speed;
  Vector2 Vertex[3];
  Color col;
  bool destroyRequired;
  t_camera cameraBorn;
  int life_ticks;
}t_bullet_v1;

typedef struct{
  t_atomic_figure body;
  Vector2 pos; // deltaposition from ship center
  float rotationSpeed;
  float angle; //relative angle (ship)
  t_bullet_v1* bullets;
  int bullets_amount;
  bool targetIsLocked;
}t_gun_v1;

void DBG_GUN_print(void);
void GUNS_Init(void);
void GUNS_Update(void);
void GUNS_Draw(void);

void GUN_BULLET_Create(void);
void GUN_BULLET_Update(void);
void GUN_BULLET_Draw(void);

void guns_meteor_debug(void);
void BULLETS_and_METEORS_interceptions_check(void);

#endif /*GUNS_H__*/