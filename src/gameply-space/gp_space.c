#include "../../../inc/raylib.h"
#include "gp_space.h"
#include <stdio.h>
#include <math.h>
#include "meteor.h"
#include "typedefg.h"
#include "guns.h"
#include "interface.h"

/*************************************************************** PROTOTYPES **/
void mainJetEngineBoost(void);
// void GrowGuns(float ship_center_x, float ship_center_y);

/********************************************************************* DATA **/
t_spaceship_position spaceship_position = {0.0f,0.0f,0.0f};
extern t_interface interface;

t_camera camera_delta = {0,0};
float death_cam_added = 0;
float death_cam = 0;

Vector2 acceleration = {0,0};
float axel_rotation;
extern float testangle;
extern float len2;
extern float dbg_guncheck;
extern float dbg_shipang;
bool create = false;
void create_dbg(void){
    Vector2 A,B,C;
    B.x = 430.0f;
    B.y = 261.0f;
    C.x = 454.0f;
    C.y = 309.0f;
    A.x = 469.0f;
    A.y = 258.0f;
    DrawTriangle(B, C, A, YELLOW);
}
  
float death_range = 0;

float x_MAIN_ENGINE_POWER =  0.15f;
float x_axel_rotationMAX = 0.05f;
/********************************************************************* FUNC **/
#define CAMERA_SPEED 5.0f
void ShipControl(void) {
  if (IsKeyDown(KEY_W)) mainJetEngineBoost();
  if (IsKeyDown(KEY_D)) axel_rotation += x_axel_rotationMAX;
  if (IsKeyDown(KEY_A)) axel_rotation -= x_axel_rotationMAX;
  // if (IsKeyPressed(KEY_J)) printf("%.6f\n",spaceship_position.rotation );

  //DBG camera
  // if (IsKeyDown(KEY_UP)) camera_delta.y += CAMERA_SPEED;
  // if (IsKeyDown(KEY_DOWN)) camera_delta.y -= CAMERA_SPEED;
  // if (IsKeyDown(KEY_RIGHT)) camera_delta.x -= CAMERA_SPEED;
  // if (IsKeyDown(KEY_LEFT)) camera_delta.x += CAMERA_SPEED;

  //DBG meteor
  // if (IsKeyPressed(KEY_M)) meterCreateNew();
  // if (IsKeyPressed(KEY_H)) create = true;

  // if (IsKeyDown(KEY_G)) {
  //   GUN_BULLET_Create();
  // } 
  // //DBG meteor
  // if (IsKeyPressed(KEY_F)) {
  //   printf("Angle: %.6f , ", testangle);
  //   printf("Shipangle: %.6f, rotate Gun: %.6f\n", dbg_shipang, dbg_guncheck);
  //   printf("Distance: %.6f\n", len2);
  // }

  // if (IsKeyPressed(KEY_B)){
  //   // guns_meteor_debug();
  //   printf("CAM %.6f\n",camera_delta.y);
  //   printf("SHIP %.6f\n",spaceship_position.y);
  //   printf("DEATH %.6f\n",death_range);

  // }


}

float x_FRICTION_FORCE = 0.02f;

void mainJetEngineBoost(void){
  /*calc direction*/

  /*calc amount*/
  float axy = x_MAIN_ENGINE_POWER * cos(deg2rad(spaceship_position.rotation)); // cos(spaceship.rotation * (M_PI /180.0f));
  float axx = x_MAIN_ENGINE_POWER * sin(deg2rad(spaceship_position.rotation)); // sin(spaceship.rotation * (M_PI /180.0f));
  // printf("BOOST");
  // printf("y: %.6f, x: %.6f\n", axy, axx);
  acceleration.y -= axy;
  acceleration.x += axx;
}

double last_time = 0;
void ShipPositionInit(void){
  last_time = GetTime();

  spaceship_position.x = GetScreenWidth()/2;
  spaceship_position.y = GetScreenHeight()/2;
}

void CamUp(float val){
  //move camera
  camera_delta.y -= val;
}

bool start_death = false;
// _Bool fst = true;
/* @brief ship moves, forces works*/
int meteor_timeout = 10;
int mt = 5;
float posWhen_last_created = 0;
int presaved_level = 1;;
void ShipMove(void) {
  float diff_multiplier = 0.5 + ((float)interface.level / 4);
  /*meteor autogen*/
  if (presaved_level != interface.level){
    meteor_timeout = 10 - (interface.level/2);
    presaved_level = interface.level;
  }
  if (spaceship_position.y< 0) {
    if (--mt<0) {
      if ( posWhen_last_created - spaceship_position.y > 10){
        meterCreateNew();
        mt = meteor_timeout;
        posWhen_last_created = spaceship_position.y;
      }
    }
  }
  /*death conditions and death camera!!!*/
  if (start_death) {
    double tim = GetTime() - last_time;
    last_time = GetTime();

    if (death_range< spaceship_position.y) {
      CamUp(-tim*20*diff_multiplier);
      death_cam_added += tim*20*diff_multiplier;
      death_range -= tim*20*diff_multiplier;
    }
    else {
      if (death_cam_added>0){
        CamUp(1.5f);
        death_cam_added -= 1.5f;
      } 
      death_range -= tim*200*diff_multiplier;
    }
  } else {
    if (spaceship_position.y< 0) start_death = true;
    last_time = GetTime();
  }

  spaceship_position.x += acceleration.x;
  spaceship_position.y += acceleration.y;

  if (spaceship_position.y< GetScreenHeight()/2) CamUp(acceleration.y);

  spaceship_position.rotation += axel_rotation;

  //acceleration decrease 
  float acceleration_total = sqrt( pow(acceleration.x, 2) + pow(acceleration.y, 2) );

  if (acceleration_total!=0) {
    float acc_new = acceleration_total * (1.0f - x_FRICTION_FORCE);
    float new_x = ( acceleration.x / acceleration_total ) * acc_new;
    float new_y = ( acceleration.y / acceleration_total ) * acc_new;

    // if (fst) {
    //   fst = false;
    //   printf("total: %.6f\n", acceleration_total);
    //   printf("y: %.6f, x: %.6f\n", new_y, new_x);
    // }
    acceleration.x = new_x;
    acceleration.y = new_y;
  }

  //rotation acceleration decrease
  axel_rotation = axel_rotation * (1.0f-(x_FRICTION_FORCE/4));
}

void DrawSpaceShip(void) {
  Rectangle tmp_rect, tmp_rect2, tmp_rect3, start_line;
  tmp_rect.height = 85;
  tmp_rect.width = 35;
  tmp_rect.x = spaceship_position.x + camera_delta.x;
  tmp_rect.y = spaceship_position.y + camera_delta.y;

  Vector2 center, center2, center3;
  center.x = (tmp_rect.width/2) ;
  center.y = (tmp_rect.height/2) ;

  //top part
  tmp_rect2.x = tmp_rect.x;// + (center.x)/2;
  tmp_rect2.y = tmp_rect.y;// + (center.y)/2;
  tmp_rect2.height = 10;
  tmp_rect2.width = 20;

  center2.x = tmp_rect2.width/2;
  center2.y = tmp_rect2.height + tmp_rect.height/2 + 10;

 //btm part
  tmp_rect3.x = tmp_rect.x;
  tmp_rect3.y = tmp_rect.y;
  tmp_rect3.height = 20;
  tmp_rect3.width = 20;

  center3.x = tmp_rect3.width/2;
  center3.y = tmp_rect3.height - tmp_rect.height/2 - 25;

  // DrawRectanglePro(tmp_rect,center, spaceship_position.rotation, WHITE);
  // DrawRectanglePro(tmp_rect2,center2, spaceship_position.rotation, RED);
  // DrawRectanglePro(tmp_rect3,center3, spaceship_position.rotation, DARKBLUE);
  
  // GrowGuns(tmp_rect2.x,tmp_rect2.y);

  start_line.x = 0;
  start_line.y = -10 + camera_delta.y;

  start_line.width = GetScreenWidth()+100;
  start_line.height = 20;

  Color cc = (Color){ 245, 245, 245, 200 }; 
  DrawRectangleRec(start_line, cc);

  if (spaceship_position.y>0){
    DrawText("Use W,A,S,D to control spaceship", 600, 400, 20, RAYWHITE);
    DrawText("Avoid asteroids", 600, 450, 20, RAYWHITE);
    DrawText("Avoid screen borders", 600, 500, 20, RAYWHITE);
  }
}

#define SOCKET_Y_HEIGHT 10.0f
void relativeCenterFinf(Vector2* center, Vector2* rCenter) {
  rCenter->y = center->y + ( SOCKET_Y_HEIGHT* cos(deg2rad(spaceship_position.rotation)));
  rCenter->x = center->x - (SOCKET_Y_HEIGHT* sin(deg2rad(spaceship_position.rotation)));
}

#define GUN_SOCKET_LENGTH 10
#define GUN_SOCKET_SIZE 10
void calc_soc_pos( Vector2* point, Vector2* center, float angle ) {
  float dx,dy;
  dy = GUN_SOCKET_LENGTH * cos(deg2rad(angle));
  dx = GUN_SOCKET_LENGTH * sin(deg2rad(angle));

  point->x = center->x + dx;
  point->y = center->y + dy;
}

// /*
// /* GUNS */
// #define GUN_SOCKET_THICK 1.0f
// void GrowGuns(float ship_center_x, float ship_center_y){
//   Vector2 top,bot,lft,rght;
//   float initAngle = 45.0f;
//   initAngle -= spaceship_position.rotation;
//   Vector2 centerO, center;
//   centerO.x = ship_center_x;
//   centerO.y = ship_center_y;

//   relativeCenterFinf(&centerO,&center );

//   calc_soc_pos(&rght,&center, initAngle);
//   initAngle+=90;
//   calc_soc_pos(&bot,&center, initAngle);
//   initAngle+=90;
//   calc_soc_pos(&lft,&center, initAngle);
//   initAngle+=90;
//   calc_soc_pos(&top,&center, initAngle);

//   // rght.x = ship_center_x + dx;
//   // rght.y = ship_center_y + dy;

//   // top.x = ship_center_x - GUN_SOCKET_SIZE/2;
//   // top.y = ship_center_y + GUN_SOCKET_SIZE/2;
  
//   // rght.x = ship_center_x + GUN_SOCKET_SIZE/2;
//   // rght.y = ship_center_y + GUN_SOCKET_SIZE/2;

//   // bot.x = ship_center_x + GUN_SOCKET_SIZE/2;
//   // bot.y = ship_center_y - GUN_SOCKET_SIZE/2;
  
//   // lft.x = ship_center_x - GUN_SOCKET_SIZE/2;
//   // lft.y = ship_center_y - GUN_SOCKET_SIZE/2;

//   DrawTriangle(top, rght, bot, DARKGRAY);
  
//   DrawTriangle(top, bot, lft, RED);
//   // DrawLineEx(top,rght,GUN_SOCKET_THICK,BLACK);
//   // DrawLineEx(rght,bot,GUN_SOCKET_THICK,BLACK);
//   // DrawLineEx(bot,lft,GUN_SOCKET_THICK,BLACK);
//   // DrawLineEx(lft,top,GUN_SOCKET_THICK,BLACK);
// }*/