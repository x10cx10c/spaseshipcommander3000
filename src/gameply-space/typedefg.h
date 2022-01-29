#ifndef TYPEDEFG_H__
#define TYPEDEFG_H__

typedef struct{
  float x; //center x
  float y; //center y
  float rotation; //rotation !degrees
}t_spaceship_position;

typedef struct{
  float x;
  float y;
}t_camera;

#ifndef M_PI
  #define M_PI 3.14159265358979323846f
#endif

#define deg2rad(x) ((x)*(3.14159265358979323846f / 180.0f))
#define rad2deg(x) ((x)*(180.0f / 3.14159265358979323846f))

#endif /*TYPEDEFG_H__*/