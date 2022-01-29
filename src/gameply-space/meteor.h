#ifndef METEOR_H__
#define METEOR_H__

#include "typedefg.h"
#include "../../../inc/raylib.h"
#include "drawingEngine.h"

typedef struct{
  t_atomic_figure body;
  float angle;
  float rotationSpeed;
  Color color;
  Vector2 speed;
  float mass;
  int health; //healthy meteor; hah
  float radius;
  bool shouldDie;
  int dying_process;
}t_meteor_v2;

#define METEOR_MAX_SPEED 10
void meteorField_Update(void);
void meteorDraw_v2(void);
void meterCreateNew(void);
void meteo_init(void);

// void meteorDraw (void);
// void create_meteor(void);

#endif /*METEOR_H__*/