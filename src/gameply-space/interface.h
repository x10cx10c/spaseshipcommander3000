#ifndef INTERFACE_H__
#define INTERFACE_H__
#include "typedefg.h"
#include "../../../inc/raylib.h"

void XP_FOR_KILL(void);
void HEALTH_Init(void);
void HEALTH_Hit(void);
void HEALTH_Update(void);
typedef struct {
  int healthMAX;
  int health;
  float health_regen_cur;
  float health_regen_delta;
  int level;
  int exp;
  int exp_req;
  int distance;
  bool gameover;
  float xp_mult;

  bool level_up;
}t_interface;

#endif /*INTERFACE_H__*/