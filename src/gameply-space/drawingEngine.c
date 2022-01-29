#include "drawingEngine.h"
#include "typedefg.h"
#include "math.h"
#include "malloc.h"
#include <stdio.h>

extern t_camera camera_delta;
extern t_spaceship_position spaceship_position;
_Bool logging = false;

t_atomic_figure test;
void InitTestFigure(void) {
  test.center.x = 200;
  test.center.y = 200;
  test.sec_amnt = 5;
  test.sections = malloc(sizeof(t_atomic_section)*test.sec_amnt);

  test.sections[0].line_length = 10.0f*4;
  test.sections[0].angle = 45.0f;

  test.sections[1].line_length = 10.0f*4;
  test.sections[1].angle = 135.0f;

  test.sections[2].line_length = 10.0f*4;
  test.sections[2].angle = 225.0f;

  test.sections[3].line_length = 10.0f*4;
  test.sections[3].angle = 315.0f;

  test.sections[4].line_length = 20.0f*4;
  test.sections[4].angle = 360.0f;
}

void relativeCenterFind(Vector2* center, Vector2* rCenter, Vector2* delta) {
  // float g = sqrt (  pow(delta->x, 2) + pow(delta->y, 2) );
  // float initAngle = acos(delta->y / g );
  // rCenter->y = center->y + ( SOCKET_Y_HEIGHT* cos(deg2rad(spaceship_position.rotation)));
  // rCenter->x = center->x - (SOCKET_Y_HEIGHT* sin(deg2rad(spaceship_position.rotation)));
}

void DrawTestFigure(void) {
  // Color testcol = PINK;
  // DrawAtomicFigure(&test, &testcol, spaceship_position.rotation);
}

/*draws only one layer*/
void DrawAtomicFigure(t_atomic_figure* fig, Color* color, float rotation) {
  Vector2 center;
  center.y = fig->center.y + camera_delta.y;
  center.x = fig->center.x + camera_delta.x;
  Vector2 first, last, current;
  float dx,dy;
  int i = 0;
  dx = ( fig->sections[i].line_length * sin(deg2rad(fig->sections[i].angle + rotation)) );
  dy = -( fig->sections[i].line_length * cos(deg2rad(fig->sections[i].angle + rotation)) );
  if (logging) printf("(%d) y: %.6f, x: %.6f\n", i, dy, dx);
  if (logging) printf("(%d) ll: %.6f, angle: %.6f, rads: %.6f\n", 
                        i, 
                        fig->sections[i].line_length, 
                        (fig->sections[i].angle + rotation),
                        (deg2rad(fig->sections[i].angle + rotation)));

  last.y = center.y + dy;
  last.x = center.x + dx;
  first = last;
  if (logging) printf("(%d)[last] V2x: %.6f, V2y: %.6f\n", i, last.y, last.x );
  if (logging) printf("(%d)[first] V2x: %.6f, V2y: %.6f\n", i, first.y, first.x );
  if (logging) printf("(%d)[center] V2x: %.6f, V2y: %.6f\n", i, center.y, center.x );

  for ( i=1; i < fig->sec_amnt; i++ ) {
    dx = ( fig->sections[i].line_length * sin(deg2rad(fig->sections[i].angle + rotation)) );
    dy = -( fig->sections[i].line_length * cos(deg2rad(fig->sections[i].angle + rotation)) );
    if (logging) printf("(%d) y: %.6f, x: %.6f\n", i, dy, dx);
    if (logging) printf("(%d) ll: %.6f, angle: %.6f, rads: %.6f\n", i, fig->sections[i].line_length, (fig->sections[i].angle + rotation),(deg2rad(fig->sections[i].angle + rotation)));
    current.y = center.y + dy;
    current.x = center.x + dx;
    // if (i==1)
    //   DrawTriangle(current, last, fig->center, ORANGE );
    // if (i==2)
    //   DrawTriangle(current, last, fig->center, BLACK );
    // if (i==3)
    //   DrawTriangle(current, last, fig->center, DARKGRAY );
    DrawTriangle(current, last, center, *color );
      
    if (logging) printf("(%d)[last] V2x: %.6f, V2y: %.6f\n", i, last.y, last.x );
    if (logging) printf("(%d)[current] V2x: %.6f, V2y: %.6f\n", i, current.y, current.x );
    if (logging) printf("(%d)[center] V2x: %.6f, V2y: %.6f\n", i, center.y, center.x );
    last = current;
  }
  // DrawTriangle( first, last, fig->center, WHITE );
  DrawTriangle( first, last, center, *color );
  
  if (logging) printf("(%d)[last] V2x: %.6f, V2y: %.6f\n", 4, last.y, last.x );
  if (logging) printf("(%d)[first] V2x: %.6f, V2y: %.6f\n", 4, first.y, first.x );
  if (logging) printf("(%d)[center] V2x: %.6f, V2y: %.6f\n", 4, center.y, center.x );

  if (logging) logging = false;
}
