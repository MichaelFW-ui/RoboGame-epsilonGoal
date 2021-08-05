#ifndef __MOTION_H__
#define __MOTION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef enum{
  HighSpeed = 2,
  LowSpeed = 1,
  ZeroSpeed = 0
}Motion_Speed_t;

typedef enum{
  Direction_North = 0,
  Direction_South = 1,
  Direction_West = 2,
  Direction_East = 3
}Motion_Direction_t;

typedef enum{
  Rotate = 1,
  Stop = 0
}Motion_Friction_t;

typedef enum{
  UpRotate = 1,
  DownRotate = 2,
  Stop = 0
}Motion_Pushbeam_t;

typedef struct {
  Motion_Speed_t Motion_Speed;
  Motion_Direction_t Motion_Direction;
  int16_t Wheel_Speed[4];
  uint8_t Steering_Angle[6];
  int8_t Under_Error[4];
  Motion_Friction_t Motion_Friction;
  Motion_Pushbeam_t Motion_Pushbeam;
}Motion_t;

extern Motion_t Motion_Information;

#ifdef __cplusplus
}
#endif

#endif 
