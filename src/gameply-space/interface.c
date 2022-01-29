#include "interface.h"
#include "gp_space.h"

extern t_camera camera_delta;
extern t_spaceship_position spaceship_position;

t_interface interface;

void HEALTH_Init(void) {
  interface.healthMAX = 3;
  interface.health = 3;
  interface.health_regen_delta = 0.2f;
  interface.health_regen_cur = 0.0f;

  interface.exp = 0;
  interface.exp_req = 2500;

  interface.gameover = false;
  interface.level = 1;

  interface.level_up = false;
  interface.distance = 0;
  interface.xp_mult = 1.0f;
}

void XP_FOR_KILL(void){
  interface.exp += 10*interface.xp_mult;
}

void HEALTH_Hit(void){
  interface.health--;
  interface.health_regen_cur = 0;
}

int old_dist=0;
void HEALTH_Update(void){
  if (interface.health < interface.healthMAX) {
    if (interface.health_regen_cur < 100){
      interface.health_regen_cur += interface.health_regen_delta;
    } else {
      interface.health++;
      interface.health_regen_cur = 0;
    }
  }
  //distance
  if (spaceship_position.y<0){
    interface.distance = -spaceship_position.y;

    //exp
    int deltaDist = interface.distance - old_dist;
    if (deltaDist > 0) {
      interface.exp += (deltaDist*interface.xp_mult);
      old_dist = interface.distance;
    }
    //exp_check
    if ( interface.exp > interface.exp_req ){
      interface.level_up = true;
      interface.exp = 0;
      interface.exp_req = interface.exp_req + GetRandomValue(100*interface.level, 1000*interface.level);
      interface.level++;
    }
  }
  
  //checking gameover conditions
  if ((spaceship_position.x < 0 )|| (spaceship_position.x > GetScreenWidth())) {
    interface.gameover = true;
  }

  if (interface.health <= 0) interface.gameover = true;

  if ((spaceship_position.y+camera_delta.y) > GetScreenHeight()) interface.gameover = true;
}