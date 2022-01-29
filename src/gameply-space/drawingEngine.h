#ifndef DRAWINGENGINE_H__
#define DRAWINGENGINE_H__

#include "../../../inc/raylib.h"

typedef struct {
  float line_length;
  float bline_length;
  float angle;
}t_atomic_section;

typedef struct {
  t_atomic_section* sections;
  Vector2 center; /*the object position for parent, the delta from parent center for children*/
  int sec_amnt;
}t_atomic_figure;

void InitTestFigure(void);
void DrawTestFigure(void);
void DrawAtomicFigure(t_atomic_figure* fig, Color* color, float rotation);

#endif /*DRAWINGENGINE_H__*/