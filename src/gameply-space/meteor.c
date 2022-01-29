#include "meteor.h"
#include <stdio.h>
#include "malloc.h"
#include <stdio.h>
#include <math.h>
#include "interface.h"

extern t_camera camera_delta;
extern t_spaceship_position spaceship_position;

void __meteor_mass_calc(t_meteor_v2* meteor);

// typedef struct{
//   int center_x;
//   int center_y;
//   float radius;
//   int speed_x;
//   int speed_y;
//   bool active;
// }t_meteor;

t_meteor_v2* meteor_field;
int meteor_field_size = 0;

Sound fxBoom = { 0 };
void meteo_init(void){
  fxBoom = LoadSound("resources/boom.wav");
}

void meterCreateNew(void) {
  if (meteor_field_size==0) {
    meteor_field = malloc(sizeof(t_meteor_v2));
    meteor_field_size++;
  } else {
    meteor_field_size++;
    meteor_field = realloc(meteor_field, sizeof(t_meteor_v2)*meteor_field_size);
  }
  //center create
  int height = GetScreenHeight();
  meteor_field[meteor_field_size-1].body.center.x = GetRandomValue(0, GetScreenWidth());
  meteor_field[meteor_field_size-1].body.center.y = GetRandomValue(spaceship_position.y - height, spaceship_position.y-((2*height)/3));

  int sides = GetRandomValue(8,12);
  meteor_field[meteor_field_size-1].body.sec_amnt = sides;
  t_atomic_section* sections = malloc(sizeof(t_atomic_section)*sides);
  meteor_field[meteor_field_size-1].body.sections = sections;

  //sticks
  int upperLimit_tick = 360/sides;
  int lastAngle = 0;
  float bones_summ = 0.0f;
  for (int i=0; i<sides; i++) {
    sections[i].line_length = ((float)GetRandomValue(100,200))/10.0f;
    bones_summ += sections[i].line_length;
    sections[i].angle = GetRandomValue(lastAngle, upperLimit_tick*(i+1));
    lastAngle = sections[i].angle;
  }
  //delta raduis
  meteor_field[meteor_field_size-1].radius = bones_summ/sides;

  //rotation speed, angle, color
  meteor_field[meteor_field_size-1].angle = 0.0f;
  meteor_field[meteor_field_size-1].rotationSpeed = ((float)GetRandomValue(0,2))/10.0f;
  int colStrength = GetRandomValue(60,140);
  Color color = {colStrength,colStrength,colStrength,250};
  meteor_field[meteor_field_size-1].color = color;
  Vector2 speed;
  speed.x = ((float)GetRandomValue(-METEOR_MAX_SPEED/2,METEOR_MAX_SPEED/2))/10.0f;
  speed.y = ((float)GetRandomValue(METEOR_MAX_SPEED/4,METEOR_MAX_SPEED))/10.0f;
  meteor_field[meteor_field_size-1].speed = speed;

  //mass
  __meteor_mass_calc(&meteor_field[meteor_field_size-1]);
  //health==mass
  meteor_field[meteor_field_size-1].health = (int)meteor_field[meteor_field_size-1].mass;

  meteor_field[meteor_field_size-1].shouldDie = false;

  // printf("meteor created\n");
  // printf("mass %.6f\n", meteor_field[meteor_field_size-1].mass);
}

void meteorField_Update(void) {
  if (meteor_field_size==0) return;
  int dist;
  int destoyDist = GetScreenWidth()*4;//here! free memory [TODO]
  t_meteor_v2* meteor_field_sort = malloc(sizeof(t_meteor_v2)*meteor_field_size);
  int nmet_ind = 0;

  for (int i = 0; i< meteor_field_size; i++){
    if (meteor_field[i].health < 0) {
      if (meteor_field[i].shouldDie){
        meteor_field[i].dying_process -= 5;
        if(meteor_field[i].dying_process == 0 ) {
          XP_FOR_KILL();
          PlaySound(fxBoom);
          goto die_meteor;
        } 
      }else {
        meteor_field[i].shouldDie = true;
        meteor_field[i].dying_process = 255;
      }
    }
    dist = sqrt( pow((spaceship_position.x - meteor_field[i].body.center.x),2)+
                pow((spaceship_position.y - meteor_field[i].body.center.y),2) );
    if (dist < destoyDist) {
      meteor_field_sort[nmet_ind].angle = meteor_field[i].angle + meteor_field[i].rotationSpeed;
      meteor_field_sort[nmet_ind].body.center.x = meteor_field[i].body.center.x + meteor_field[i].speed.x;
      meteor_field_sort[nmet_ind].body.center.y = meteor_field[i].body.center.y + meteor_field[i].speed.y;
      meteor_field_sort[nmet_ind].health = meteor_field[i].health;
      meteor_field_sort[nmet_ind].color = meteor_field[i].color;
      meteor_field_sort[nmet_ind].radius = meteor_field[i].radius;
      meteor_field_sort[nmet_ind].rotationSpeed = meteor_field[i].rotationSpeed;
      meteor_field_sort[nmet_ind].speed = meteor_field[i].speed;

      meteor_field_sort[nmet_ind].dying_process = meteor_field[i].dying_process;
      meteor_field_sort[nmet_ind].shouldDie = meteor_field[i].shouldDie;

      int tmp_sec = meteor_field[i].body.sec_amnt;
      meteor_field_sort[nmet_ind].body.sec_amnt = tmp_sec;
      meteor_field_sort[nmet_ind].body.sections = malloc(sizeof(t_atomic_section)*tmp_sec);
      for (int section = 0; section < tmp_sec; section++ ) {
        meteor_field_sort[nmet_ind].body.sections[ section ].angle = meteor_field[i].body.sections[ section ].angle;
        meteor_field_sort[nmet_ind].body.sections[ section ].line_length = meteor_field[i].body.sections[ section ].line_length;
        meteor_field_sort[nmet_ind].body.sections[ section ].bline_length = meteor_field[i].body.sections[ section ].bline_length;
      }
      free(meteor_field[i].body.sections);
      nmet_ind++;
      continue;
    } else {
die_meteor:
      free(meteor_field[i].body.sections);
      continue;
    }
  } 
  meteor_field_sort = realloc(meteor_field_sort, nmet_ind*sizeof(t_meteor_v2));
  free(meteor_field);
  meteor_field = meteor_field_sort;
  meteor_field_size = nmet_ind;
  // printf("METEORS Q %d\n", meteor_field_size);
}

void meteorDraw_v2(void) {
  for (int i = 0; i< meteor_field_size; i++){
    if (meteor_field[i].shouldDie){
      Color c = meteor_field[i].color;
      c.a = meteor_field[i].dying_process;
      DrawAtomicFigure( &meteor_field[i].body, &c, meteor_field[i].angle );
    } else {
      DrawAtomicFigure( &meteor_field[i].body, &meteor_field[i].color, meteor_field[i].angle );
    }
  }
}

static inline float part_mass(Vector2* a, Vector2* b, Vector2* c);
void __meteor_mass_calc(t_meteor_v2* meteor) {
  Vector2 center,first, last, current;
  t_atomic_figure* fig = &meteor->body;
  float dx,dy;
  int i = 0;
  center.y = 0;
  center.x = 0;
  meteor->mass = 0.0f;

  dx = fig->sections[i].line_length * sin(deg2rad(fig->sections[i].angle));
  dy = -fig->sections[i].line_length * cos(deg2rad(fig->sections[i].angle));

  last.y = center.y + dy;
  last.x = center.x + dx;
  first = last;

  for ( i=1; i < fig->sec_amnt; i++ ) {
    dx = fig->sections[i].line_length * sin(deg2rad(fig->sections[i].angle));
    dy = -fig->sections[i].line_length * cos(deg2rad(fig->sections[i].angle));
    current.y = center.y + dy;
    current.x = center.x + dx;

    meteor->mass += part_mass(&current, &last, &center);
    // DrawTriangle(current, last, center, *color );

    last = current;
  }
  meteor->mass += part_mass(&first, &last, &center);
  // DrawTriangle( first, last, center, *color );
}

static inline float part_mass(Vector2* a, Vector2* b, Vector2* c) {
  float s = 0.5f * fabsf( ((b->x - a->x)*(c->y - a->y)) - ((c->x - a->x)*(b->y - a->y)) );
  return s;
}

// void meteorDraw (void) {
//   for ( int i =0; i < 100; i++ ) {
//     if (meteors[i].active) {
//       DrawCircle(meteors[i].center_x+camera_delta.x,meteors[i].center_y+camera_delta.y,meteors[i].radius,BLACK );
//     }
//   }
// }



// t_meteor meteors[100];
// int meteor_amount = 0;+;
// }
// void create_meteor(void) {
//   meteors[meteor_amount].center_x = GetRandomValue(0, GetScreenWidth());
//   meteors[meteor_amount].center_y = GetRandomValue(0, GetScreenHeight());
//   meteors[meteor_amount].radius = (float) GetRandomValue(4, 20);
//   meteors[meteor_amount].active = true;
//   meteor_amount+