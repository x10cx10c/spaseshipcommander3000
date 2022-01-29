#include "ship.h"
#include "typedefg.h"
#include "meteor.h"
#include "typedefg.h"
#include "math.h"
#include "drawingEngine.h"
#include "malloc.h"
#include "guns.h"
#include <stdio.h>
#include "interface.h"

extern t_gun_v1 support_gun_1;
extern t_gun_v1 support_gun_2;

extern t_meteor_v2* meteor_field;
extern int meteor_field_size;
float len2;
float cosVal;

typedef struct {
  t_atomic_figure hull;
  t_atomic_figure no1;
  t_atomic_figure no2;
  t_atomic_figure no3;
  Vector2 pos_no1;
  Vector2 pos_no2;
  Vector2 pos_no3;
}t_spaceship;

extern t_camera camera_delta;
extern t_spaceship_position spaceship_position;
t_spaceship spaceship;
/*drawing*/

/*AIMING STUFF*/
_Bool SHIPsensors_FindNearestTarget(float* angle, int* index, int frbd, Vector2* from) {
  _Bool found = false;
  float y = 0;
  float x;
  float len2m = 200.0f;
  float x_m, y_m, tmp_len;
  int ind;
  for (int i = 0; i < meteor_field_size; i++) {
    if (i==frbd) continue;
    if (meteor_field[i].shouldDie) continue;
    x_m = meteor_field[i].body.center.x;
    y_m = meteor_field[i].body.center.y;
    // tmp_len = sqrt(pow((fabsf(spaceship_position.x-x_m)),2)+pow((fabsf(spaceship_position.y-y_m)),2));
    tmp_len = sqrt(pow((fabsf(from->x-x_m)),2)+pow((fabsf(from->y-y_m)),2));
    
    if (len2m > tmp_len) {
      len2m = tmp_len;
      y = from->y - y_m;
      x = x_m;
      ind = i;
      found = true;
    }
  }
    // x_m = 200.0f;
    // y_m = 200.0f;
    // len2m = sqrt(pow((fabsf(spaceship_position.x-x_m)),2)+pow((fabsf(spaceship_position.y-y_m)),2));
    // found = true;
    // y = spaceship_position.y - y_m;

  len2 = len2m;
  if (found) {
    *index = ind;
    *angle = rad2deg( acos( (y/len2m) ) );
    // if (spaceship_position.x > x) *angle = 360.0f - *angle;
    if (from->x > x) *angle = 360.0f - *angle;
    // *angle = 360.0f - *angle;//reverse
  }
  

  return found;
}

#define THE_BIG_BONE 25.0f
#define THE_SMALL_BONE 22.0f
#define BONE_MULTI 1.5f

#define DETAILS_MILT 0.5f
void SHIP_hull_init(void) {

  spaceship.hull.sec_amnt = 8;
  spaceship.hull.sections = malloc(sizeof(t_atomic_section)*spaceship.hull.sec_amnt);

  spaceship.hull.sections[0].line_length = THE_BIG_BONE*BONE_MULTI;
  spaceship.hull.sections[0].angle = 15.0f;

  spaceship.hull.sections[1].line_length = THE_SMALL_BONE*BONE_MULTI;
  spaceship.hull.sections[1].angle = 30.0f;

  spaceship.hull.sections[2].line_length = THE_SMALL_BONE*BONE_MULTI;
  spaceship.hull.sections[2].angle = 150.0f;

  spaceship.hull.sections[3].line_length = THE_BIG_BONE*BONE_MULTI;
  spaceship.hull.sections[3].angle = 165.0f;

  spaceship.hull.sections[4].line_length = THE_BIG_BONE*BONE_MULTI;
  spaceship.hull.sections[4].angle = 195.0f;

  spaceship.hull.sections[5].line_length = THE_SMALL_BONE*BONE_MULTI;
  spaceship.hull.sections[5].angle = 210.0f;

  spaceship.hull.sections[6].line_length = THE_SMALL_BONE*BONE_MULTI;
  spaceship.hull.sections[6].angle = 330.0f;

  spaceship.hull.sections[7].line_length = THE_BIG_BONE*BONE_MULTI;
  spaceship.hull.sections[7].angle = 345.0f;

  // new ship design
  // DETAIL ONE
  spaceship.no1.sec_amnt = 8;
  spaceship.no1.sections = malloc(sizeof(t_atomic_section)*spaceship.no1.sec_amnt);

  spaceship.no1.sections[0].line_length = 42.4f*DETAILS_MILT;
  spaceship.no1.sections[0].angle = 45.0f;

  spaceship.no1.sections[1].line_length = 41.23f*DETAILS_MILT;
  spaceship.no1.sections[1].angle = 75.97f;

  spaceship.no1.sections[2].line_length = 46.0f*DETAILS_MILT;
  spaceship.no1.sections[2].angle = 119.805f;

  spaceship.no1.sections[3].line_length = 50.0f*DETAILS_MILT;
  spaceship.no1.sections[3].angle = 165.0f;

  spaceship.no1.sections[4].line_length = 50.0f*DETAILS_MILT;
  spaceship.no1.sections[4].angle = 195.0f;

  spaceship.no1.sections[5].line_length = 46.0f*DETAILS_MILT;
  spaceship.no1.sections[5].angle = 240.195f;

  spaceship.no1.sections[6].line_length = 41.23f*DETAILS_MILT;
  spaceship.no1.sections[6].angle = 284.03f;

  spaceship.no1.sections[7].line_length = 42.4f*DETAILS_MILT;
  spaceship.no1.sections[7].angle = 315.0f;
  spaceship.pos_no1.x = 0;//80*DETAILS_MILT;
  spaceship.pos_no1.y = 80*DETAILS_MILT;

  //DETAIL TWO
  spaceship.no2.sec_amnt = 4;
  spaceship.no2.sections = malloc(sizeof(t_atomic_section)*spaceship.no2.sec_amnt);

  spaceship.no2.sections[0].line_length = 65.08f*DETAILS_MILT;
  spaceship.no2.sections[0].angle = 32.11f;
  spaceship.no2.sections[1].line_length = 65.08f*DETAILS_MILT;
  spaceship.no2.sections[1].angle = 147.83f;
  spaceship.no2.sections[2].line_length = 65.08f*DETAILS_MILT;
  spaceship.no2.sections[2].angle = 212.11f;
  spaceship.no2.sections[3].line_length = 65.08f*DETAILS_MILT;
  spaceship.no2.sections[3].angle = 327.89f;
  spaceship.pos_no2.x = 0;
  spaceship.pos_no2.y = 10*DETAILS_MILT;

  //DETAIL THREE
  spaceship.no3.sec_amnt = 6;
  spaceship.no3.sections = malloc(sizeof(t_atomic_section)*spaceship.no1.sec_amnt);

  spaceship.no3.sections[0].line_length = 46.0f*DETAILS_MILT*1.1f;
  spaceship.no3.sections[0].angle = 240.195f + 180.0f;

  spaceship.no3.sections[1].line_length = 41.23f*DETAILS_MILT*1.1f;
  spaceship.no3.sections[1].angle = 284.03f + 180.0f;

  spaceship.no3.sections[2].line_length = 42.4f*DETAILS_MILT*1.1f;
  spaceship.no3.sections[2].angle = 315.0f + 180.0f;

  spaceship.no3.sections[3].line_length = 42.4f*DETAILS_MILT*1.1f;
  spaceship.no3.sections[3].angle = 45.0f + 180.0f;

  spaceship.no3.sections[4].line_length = 41.23f*DETAILS_MILT*1.1f;
  spaceship.no3.sections[4].angle = 75.97f + 180.0f;

  spaceship.no3.sections[5].line_length = 46.0f*DETAILS_MILT*1.1f;
  spaceship.no3.sections[5].angle = 119.805f + 180.0f;

  spaceship.pos_no3.x = 0;//80*DETAILS_MILT;
  spaceship.pos_no3.y = -70*DETAILS_MILT;
}

void __DETAIL_update_one(t_atomic_figure* fig, Vector2* position){
  float gep = sqrt(pow(position->x,2)+pow(position->y,2));
  
  fig->center.y = spaceship_position.y;
  fig->center.x = spaceship_position.x;
  
  if ( position->y > 0) {
    fig->center.x += ( gep* sin(deg2rad(spaceship_position.rotation)));
    fig->center.y -= ( gep* cos(deg2rad(spaceship_position.rotation)));
  } else {
    fig->center.x -= ( gep* sin(deg2rad(spaceship_position.rotation)));
    fig->center.y += ( gep* cos(deg2rad(spaceship_position.rotation)));
  }
}

void shipsh_colisions_check(void){
  for (int i=0; i< meteor_field_size; i++ ){
    if (meteor_field[i].shouldDie) continue;
    Vector2 meteor_cent;
    meteor_cent.x = meteor_field[i].body.center.x;
    meteor_cent.y = meteor_field[i].body.center.y + camera_delta.y;
    Vector2 center1;
    center1.x = support_gun_2.body.center.x;
    center1.y = support_gun_2.body.center.y+camera_delta.y;
    float radius1 = 20;
    Vector2 center2;
    center2.x = support_gun_1.body.center.x;
    center2.y = support_gun_1.body.center.y+camera_delta.y;
    float radius2 = 16;
    if ( CheckCollisionCircles(meteor_cent, meteor_field[i].radius, center1, radius1 )){
      meteor_field[i].health = -1;
      meteor_field[i].rotationSpeed = ((float)GetRandomValue(2,40))/5.0f;

      Vector2 speed;
      speed.x = ((float)GetRandomValue(-METEOR_MAX_SPEED*5,METEOR_MAX_SPEED*5))/10.0f;
      speed.y = ((float)GetRandomValue(-METEOR_MAX_SPEED*5,METEOR_MAX_SPEED*5))/10.0f;
      meteor_field[i].speed = speed;
      HEALTH_Hit();
      continue;
    }
    if ( CheckCollisionCircles(meteor_cent, meteor_field[i].radius, center2, radius2 )){
      meteor_field[i].health = -1;      
      meteor_field[i].rotationSpeed = ((float)GetRandomValue(2,40))/5.0f;

      Vector2 speed;
      speed.x = ((float)GetRandomValue(METEOR_MAX_SPEED*5/2,METEOR_MAX_SPEED*5/2))/10.0f;
      speed.y = ((float)GetRandomValue(METEOR_MAX_SPEED*5/4,METEOR_MAX_SPEED*5))/10.0f;
      meteor_field[i].speed = speed;
      HEALTH_Hit();
      continue;
    }
    Vector2 cencen;
    cencen.x = spaceship_position.x;
    cencen.y = spaceship_position.y+camera_delta.y;
    float cenrad = 16;
    if ( CheckCollisionCircles(meteor_cent, meteor_field[i].radius, cencen, cenrad )){
      meteor_field[i].health = -1;      
      meteor_field[i].rotationSpeed = ((float)GetRandomValue(2,40))/5.0f;

      Vector2 speed;
      speed.x = ((float)GetRandomValue(METEOR_MAX_SPEED*5/2,METEOR_MAX_SPEED*5/2))/10.0f;
      speed.y = ((float)GetRandomValue(METEOR_MAX_SPEED*5/4,METEOR_MAX_SPEED*5))/10.0f;
      meteor_field[i].speed = speed;
      HEALTH_Hit();
      continue;
    }
  }
}


void SHIP_DETAILS_Update(void){
  shipsh_colisions_check();
  __DETAIL_update_one(&spaceship.no1, &spaceship.pos_no1);
  __DETAIL_update_one(&spaceship.no2, &spaceship.pos_no2);
  __DETAIL_update_one(&spaceship.no3, &spaceship.pos_no3);

  // camera_delta.y = spaceship_position.y;
}

void SHIP_draw(void) {
  Color col = GRAY;
  spaceship.hull.center.x = spaceship_position.x;
  spaceship.hull.center.y = spaceship_position.y;
  Color col2 = (Color){ 120, 120, 120, 255 } ;
  Color col3 = (Color){ 170, 170, 170, 255 } ;
  Color col4 = (Color){ 150, 140, 140, 255 } ;
  // Dra
  DrawAtomicFigure(&spaceship.hull, &col, spaceship_position.rotation );
  DrawAtomicFigure(&spaceship.no2, &col3, spaceship_position.rotation );
  DrawAtomicFigure(&spaceship.no1, &col2, spaceship_position.rotation );
  DrawAtomicFigure(&spaceship.no3, &col2, spaceship_position.rotation );

  // DrawCircle(support_gun_2.body.center.x,support_gun_2.body.center.y+camera_delta.y, 20, BLUE);
  // DrawCircle(support_gun_1.body.center.x,support_gun_1.body.center.y+camera_delta.y, 16, BLUE);
  // DrawCircle(spaceship_position.x,spaceship_position.y+camera_delta.y, 16, BLUE);
}
