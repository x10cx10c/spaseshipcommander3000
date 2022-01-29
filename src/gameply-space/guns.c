#include "guns.h"
// #include "gp_space.h"
#include "malloc.h"
#include "math.h"
#include "ship.h"
#include <stdio.h>
#include "meteor.h"

extern t_spaceship_position spaceship_position;
extern t_camera camera_delta;
extern t_meteor_v2* meteor_field;
extern int meteor_field_size;


t_gun_v1 support_gun_1;
t_gun_v1 support_gun_2;

void __GUN_BULLET_Create_by_gun(t_gun_v1* gun);

float testangle = 0.0f;

#define GUN_LOCK_RADIUS
Sound fxShot = { 0 };

void __GUN_body_init(t_atomic_figure* body, Vector2* delta) {
  fxShot = LoadSound("resources/blust.wav");


  body->center.x = spaceship_position.x + delta->x;
  body->center.y = spaceship_position.y + delta->y;
  body->sec_amnt = 8;
  body->sections = malloc(sizeof(t_atomic_section)*body->sec_amnt);

  body->sections[0].line_length = 10.0f;
  body->sections[0].angle = 45.0f;

  body->sections[1].line_length = 10.0f;
  body->sections[1].angle = 135.0f;

  body->sections[2].line_length = 10.0f;
  body->sections[2].angle = 225.0f;

  body->sections[3].line_length = 10.0f;
  body->sections[3].angle = 315.0f;

  body->sections[4].line_length = 12.5f;
  body->sections[4].angle = 350.0f;

  body->sections[5].line_length = 16.0f;
  body->sections[5].angle = 351.0f;

  body->sections[6].line_length = 16.0f;
  body->sections[6].angle = 9.0f;
  
  body->sections[7].line_length = 12.5f;
  body->sections[7].angle = 10.0f;
}
//here we create guns positions and size
void GUNS_Init(void) {
//test gun init
  support_gun_1.pos.x = 0;
  support_gun_1.pos.y = 38;
  __GUN_body_init(&support_gun_1.body, &support_gun_1.pos);
  support_gun_1.rotationSpeed = 4.0f;
  support_gun_1.bullets_amount = 0; //bullets
  support_gun_1.targetIsLocked = false;

  support_gun_2.pos.x = 0;
  support_gun_2.pos.y = -32;
  __GUN_body_init(&support_gun_2.body, &support_gun_2.pos);
  support_gun_2.rotationSpeed = 4.0f;
  support_gun_2.bullets_amount = 0; //bullets
  support_gun_2.targetIsLocked = false;
}

float dbg_guncheck;
float dbg_shipang;
int ctr = 0;
void DBG_GUN_print(void) {
  printf("Shipangle: %.6f, rotate Gun: %.6f\n", dbg_shipang, dbg_guncheck);
}

int __GUN_update_one(t_gun_v1* gun_obj, int forb_ind) {
  //testgun update
  float gep = sqrt(pow(gun_obj->pos.x,2)+pow(gun_obj->pos.y,2));
  
  gun_obj->body.center.y = spaceship_position.y;
  gun_obj->body.center.x = spaceship_position.x;
  
  if ( gun_obj->pos.y > 0) {
    gun_obj->body.center.y += ( gep* cos(deg2rad(spaceship_position.rotation)));
    gun_obj->body.center.x -= ( gep* sin(deg2rad(spaceship_position.rotation)));
  } else {
    gun_obj->body.center.y -= ( gep* cos(deg2rad(spaceship_position.rotation)));
    gun_obj->body.center.x += ( gep* sin(deg2rad(spaceship_position.rotation)));
  }

  float gun_angle;
  gun_angle = spaceship_position.rotation + gun_obj->angle;

  int target_index = -1; 
  float target_angle;
  if (SHIPsensors_FindNearestTarget(&target_angle, &target_index, forb_ind, &gun_obj->body.center)) {
    // start_angle = (int)start_angle%360;
    testangle = target_angle;

    while (gun_angle > 360.0f) gun_angle -= 360.0f;
    while (gun_angle < 0.0f) gun_angle += 360.0f;

    float deltaRotation = gun_angle - target_angle;
    float rotationDist;
    float newRotation = 0.0f;

    float guncheck; 
    if (gun_angle > 180.0f) {
        guncheck = gun_angle - 180.0f;
        if ( (target_angle > guncheck)&&(target_angle<gun_angle) ) {//clickwise
          newRotation = -gun_obj->rotationSpeed;
        } else {//acticlickwisenewRotation = 
          newRotation = gun_obj->rotationSpeed;
        }

    } else {
        guncheck = gun_angle + 180.0f;
        if ( (target_angle > gun_angle)&&(target_angle<guncheck) ) {//clickwise
          newRotation = gun_obj->rotationSpeed;
        } else {//acticlickwisenewRotation = 
          newRotation = -gun_obj->rotationSpeed;
        }
    }
    dbg_guncheck = newRotation;
    dbg_shipang = gun_angle;

    /*the target is close*/
    rotationDist = gun_angle - target_angle;
    if (rotationDist<0) rotationDist = -rotationDist; 
    if (rotationDist<gun_obj->rotationSpeed){
      //aimed OK!
      gun_obj->targetIsLocked = true;
      // printf("Locked\n");
      if (newRotation>0) newRotation = rotationDist;
      else newRotation = -rotationDist;
      if (--ctr<0) {
        // printf("Close %.f\n", newRotation);
        ctr = 60;
      }
    } else {
      //distance is huge
      gun_obj->targetIsLocked = false;
    }

    gun_obj->angle += newRotation;
  } else {
    gun_obj->targetIsLocked = false;
  }

  return target_index;
}

int blust1_to = 15;
int blust2_to = 15;
int x_MAX_BULLETS_PER_SHOT = 1;
void GUNS_Update(void) {
  /*after update gun return index of found enemy*/
  bool s1 = false;
  bool s2 = false;
  if (--blust1_to< 0) {
    s1 = true;
    blust1_to = 15;
  }
  if (--blust2_to< 0) {
    s2 = true;
    blust2_to = 15;
  }

  int target_ind = __GUN_update_one(&support_gun_1, -1);
  __GUN_update_one(&support_gun_2, target_ind);

  /*check if the gun is locked on target*/
  if (support_gun_1.targetIsLocked){
    // fxShot = LoadSound("resources/blust.wav");
    for (int pp = 0; pp < (x_MAX_BULLETS_PER_SHOT); pp++ ){
      __GUN_BULLET_Create_by_gun(&support_gun_1);
      if (s1) PlaySound(fxShot);
    }
  }
  if (support_gun_2.targetIsLocked){
    // fxShot = LoadSound("resources/blust.wav");
    for (int pp = 0; pp < (x_MAX_BULLETS_PER_SHOT); pp++ ){
      __GUN_BULLET_Create_by_gun(&support_gun_2);
      if (s2) PlaySound(fxShot);
    }
  }
}


//here we draw guns
void GUNS_Draw(void) {
  Color col = (Color){ 70, 70, 81, 255 } ;
  DrawAtomicFigure(&support_gun_1.body, &col, spaceship_position.rotation+support_gun_1.angle);
  DrawAtomicFigure(&support_gun_2.body, &col, spaceship_position.rotation+support_gun_2.angle);
}

float x_bullet_speed = 12.0f;
#define BULLET_INACURACY_ANGLE 5
#define BULLET_HEIGHT 10.0f
#define BULLET_WIDTH  0.8f
void __GUN_BULLET_Create_by_gun(t_gun_v1* gun) {
  //memory
  if (gun->bullets_amount==0) {
    gun->bullets = malloc(sizeof(t_bullet_v1));
    gun->bullets_amount++;
  } else {
    gun->bullets_amount++;
    gun->bullets = realloc(gun->bullets,sizeof(t_bullet_v1)*gun->bullets_amount);
  }

  //angle of bullet direction
  float bullet_angle = gun->angle + spaceship_position.rotation + 180.0f;//180, but why?
  //bullet_angle += ( (float) GetRandomValue(-BULLET_INACURACY_ANGLE*5, BULLET_INACURACY_ANGLE*5 ))/10.0f;
  //better rangom generation
  float addAngle = pow((((float)GetRandomValue(0,100))/100.0f),2)*BULLET_INACURACY_ANGLE;
  int sign = GetRandomValue(0,1);
  if (sign) addAngle = -addAngle;
  bullet_angle += addAngle;

  //find three Vertexes : top
  Vector2 center = gun->body.center;
  center.x += camera_delta.x;//camera correction
  center.y += camera_delta.y;
  
  // printf("Gun: x: %.6f, y: %.6f\n", center.x, center.y);
  Vector2 vertA, vertB, vertC;
  float gep = BULLET_HEIGHT;
  vertC.y = center.y + ( gep* cos(deg2rad(bullet_angle)));
  vertC.x = center.x - ( gep* sin(deg2rad(bullet_angle)));
  // printf("C: x: %.6f, y: %.6f\n", vertC.x, vertC.y);

  //find three Vertexes : left
  float angle_A = bullet_angle - 90;
  gep = BULLET_WIDTH;
  vertA.y = center.y + ( gep* cos(deg2rad(angle_A)));
  vertA.x = center.x - ( gep* sin(deg2rad(angle_A)));
  // printf("A: x: %.6f, y: %.6f\n", vertA.x, vertA.y);

  //find three Vertexes : right
  float angle_B = bullet_angle + 90;
  gep = BULLET_WIDTH;
  vertB.y = center.y + ( gep* cos(deg2rad(angle_B)));
  vertB.x = center.x - ( gep* sin(deg2rad(angle_B)));
  // printf("B: x: %.6f, y: %.6f\n", vertB.x, vertB.y);

  Vector2 speed;
  float speedee = ((float)GetRandomValue(x_bullet_speed*5,x_bullet_speed*10))/10.0f;

  speed.y = speedee * cos(deg2rad(bullet_angle));
  speed.x = - speedee * sin(deg2rad(bullet_angle));

  gun->bullets[ gun->bullets_amount -1 ].speed = speed;
  gun->bullets[ gun->bullets_amount -1 ].Vertex[0] = vertB;
  gun->bullets[ gun->bullets_amount -1 ].Vertex[1] = vertC;
  gun->bullets[ gun->bullets_amount -1 ].Vertex[2] = vertA;
  gun->bullets[ gun->bullets_amount -1 ].cameraBorn = camera_delta;
  gun->bullets[ gun->bullets_amount -1 ].life_ticks = 10000;

  //color
  int red = GetRandomValue(250, 255);
  int green = GetRandomValue(200, 255);
  Color bul_col = (Color){ red, green, 0, 255 };
  gun->bullets[ gun->bullets_amount -1 ].col = bul_col;
}

void GUN_BULLET_Create(void) {
  __GUN_BULLET_Create_by_gun(&support_gun_1);
  __GUN_BULLET_Create_by_gun(&support_gun_2);
}

void __GUN_BULLET_Update_one(t_gun_v1* gun) {
  int /*dist, */deltaX_cam, deltaY_cam;
  // int destoyDist = GetScreenWidth();
  t_bullet_v1* bullet_sort = malloc(gun->bullets_amount*sizeof(t_bullet_v1));
  int bulletsort_index = 0;
  for ( int i = 0; i < gun->bullets_amount; i++)  {
    if (gun->bullets[i].destroyRequired) continue;
    // dist = sqrt( pow((gun->body.center.x - gun->bullets[i].Vertex[1].x),2)+
    //              pow((gun->body.center.y - gun->bullets[i].Vertex[1].y),2) );
    // dist = sqrt( pow((camera_delta.x - gun->bullets[i].Vertex[1].x),2)+
    //              pow((camera_delta.y - gun->bullets[i].Vertex[1].y),2) );

    int ticks = gun->bullets[i].life_ticks - 1;
    if (ticks > 0) {
      bullet_sort[ bulletsort_index ] = gun->bullets[i];

      deltaX_cam = camera_delta.x - bullet_sort[ bulletsort_index ].cameraBorn.x;
      deltaY_cam = camera_delta.y - bullet_sort[ bulletsort_index ].cameraBorn.y;

      bullet_sort[ bulletsort_index ].Vertex[0].x = gun->bullets[i].Vertex[0].x + gun->bullets[i].speed.x + deltaX_cam;
      bullet_sort[ bulletsort_index ].Vertex[0].y = gun->bullets[i].Vertex[0].y + gun->bullets[i].speed.y + deltaY_cam;
      bullet_sort[ bulletsort_index ].Vertex[1].x = gun->bullets[i].Vertex[1].x + gun->bullets[i].speed.x + deltaX_cam;
      bullet_sort[ bulletsort_index ].Vertex[1].y = gun->bullets[i].Vertex[1].y + gun->bullets[i].speed.y + deltaY_cam;
      bullet_sort[ bulletsort_index ].Vertex[2].x = gun->bullets[i].Vertex[2].x + gun->bullets[i].speed.x + deltaX_cam;
      bullet_sort[ bulletsort_index ].Vertex[2].y = gun->bullets[i].Vertex[2].y + gun->bullets[i].speed.y + deltaY_cam;

      bullet_sort[ bulletsort_index ].cameraBorn.x = camera_delta.x;
      bullet_sort[ bulletsort_index ].cameraBorn.y = camera_delta.y;

      bullet_sort[ bulletsort_index ].life_ticks = ticks;
      bullet_sort[ bulletsort_index ].destroyRequired = false;
      bulletsort_index++;
    }
  }
  bullet_sort = realloc(bullet_sort, bulletsort_index*sizeof(t_bullet_v1));
  free(gun->bullets);
  gun->bullets = bullet_sort;
  gun->bullets_amount = bulletsort_index;
}

void GUN_BULLET_Update(void) {
  __GUN_BULLET_Update_one(&support_gun_1);
  __GUN_BULLET_Update_one(&support_gun_2);
}

// int ccctr = 0;
void __GUN_BULLET_Draw_one(t_gun_v1* gun) {
  for ( int i = 0; i < gun->bullets_amount; i++)  {
    Vector2 tmp_vert0, tmp_vert1, tmp_vert2;
    tmp_vert0.x = gun->bullets[i].Vertex[0].x;// + camera_delta.x;
    tmp_vert0.y = gun->bullets[i].Vertex[0].y;// + camera_delta.y;
    tmp_vert1.x = gun->bullets[i].Vertex[1].x;// + camera_delta.x;
    tmp_vert1.y = gun->bullets[i].Vertex[1].y;// + camera_delta.y;
    tmp_vert2.x = gun->bullets[i].Vertex[2].x;// + camera_delta.x;
    tmp_vert2.y = gun->bullets[i].Vertex[2].y;// + camera_delta.y;

    DrawTriangle(tmp_vert0, tmp_vert1, tmp_vert2, gun->bullets[i].col);
    // if (--ccctr < 0 ) {
      // printf("draw bullet\n");
      // printf("Amount %d\n",gun->bullets_amount);
      // printf("0: x: %.6f, y: %.6f\n", gun->bullets[i].Vertex[0].x, gun->bullets[i].Vertex[0].y);
      // printf("1: x: %.6f, y: %.6f\n", gun->bullets[i].Vertex[1].x, gun->bullets[i].Vertex[1].y);
      // printf("2: x: %.6f, y: %.6f\n", gun->bullets[i].Vertex[2].x, gun->bullets[i].Vertex[2].y);
      // ccctr = 60;
    // }
  }
}

void GUN_BULLET_Draw(void) {
  __GUN_BULLET_Draw_one(&support_gun_1);
  __GUN_BULLET_Draw_one(&support_gun_2);
}

void bullets_checks_collisions(void){
  //foreach bullet

  //foreach meteor
  
}

#define BULLETS_DAMAGE           15
#define BULLET2METEOR_MULTIPLIER 0.005f
void BULLETS_and_METEORS_interceptions_check(void) {
  int sg1_max = support_gun_1.bullets_amount;
  int sg2_max = support_gun_2.bullets_amount;
  bool collision_found = false;
  Vector2 spd_inc, realMIcenter;

  for (int mi = 0; mi < meteor_field_size; mi++){
    realMIcenter.x = meteor_field[ mi ].body.center.x + camera_delta.x;
    realMIcenter.y = meteor_field[ mi ].body.center.y + camera_delta.y;
    for (int sg1i = 0; sg1i < sg1_max; sg1i++){
      collision_found = CheckCollisionPointCircle(
        support_gun_1.bullets[ sg1i ].Vertex[1],
        realMIcenter,
        meteor_field[ mi ].radius
        );
      if (collision_found) {
        // printf("colision");
        meteor_field[ mi ].health -= BULLETS_DAMAGE;
        support_gun_1.bullets[ sg1i ].destroyRequired = true;
        spd_inc.x = support_gun_1.bullets[ sg1i ].speed.x * BULLET2METEOR_MULTIPLIER;
        spd_inc.y = support_gun_1.bullets[ sg1i ].speed.y * BULLET2METEOR_MULTIPLIER;
        meteor_field[ mi ].speed.x += spd_inc.x;
        meteor_field[ mi ].speed.y += spd_inc.y;
        collision_found = false;
      } 
    }

    for (int sg2i = 0; sg2i < sg2_max; sg2i++){
      collision_found = CheckCollisionPointCircle(
        support_gun_2.bullets[ sg2i ].Vertex[1],
        realMIcenter,
        meteor_field[ mi ].radius
        );
      if (collision_found) {
        // printf("colision");
        meteor_field[ mi ].health -= BULLETS_DAMAGE;
        support_gun_2.bullets[ sg2i ].destroyRequired = true;
        spd_inc.x = support_gun_2.bullets[ sg2i ].speed.x * BULLET2METEOR_MULTIPLIER;
        spd_inc.y = support_gun_2.bullets[ sg2i ].speed.y * BULLET2METEOR_MULTIPLIER;
        meteor_field[ mi ].speed.x += spd_inc.x;
        meteor_field[ mi ].speed.y += spd_inc.y;
        collision_found = false;
      } 
    }
  }
}

void guns_meteor_debug(void){
  printf("bullets 1 %d\n", support_gun_1.bullets_amount);
  printf("bullets 2 %d\n", support_gun_2.bullets_amount);
  printf("meteors %d\n", meteor_field_size);
}