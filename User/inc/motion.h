/**
 * @file motion.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 运动模式封装，和自主修正偏差
 * 
 * 
 * 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MOTION_H
#define __MOTION_H

#include "motor.h"

#define MOTION_HIGH_SPEED 57

#define MOTION_LOW_SPEED 28

typedef enum {
    Node_0 = 0u,
    Node_1 = 1u,
    Node_2 = 2u,
    Node_3 = 3u,
    Node_4 = 4u,
    Node_5 = 5u,
    Node_6 = 6u,
    Node_7 = 7u,
    Node_8 = 8u,
    Node_9 = 9u,
    Node_10 = 10u,
    Node_11 = 11u,
    Node_12 = 12u,
    Node_13 = 13u,
    Node_14 = 14u,
    Node_15 = 15u,
    Node_16 = 16u,
    Node_17 = 17u,
    Node_18 = 18u,
    Node_19 = 19u,
    Node_InValid = 20u
} node_t;

extern node_t CurrentNode;

void __STATIC_INLINE Motion_MoveForward(MotorInput_t speed) {
    Motor_Decode(0, speed, 0);
}

void __STATIC_INLINE Motion_MoveBackward(MotorInput_t speed) {
    Motor_Decode(0, -speed, 0);
}

void __STATIC_INLINE Motion_MoveToLeft(MotorInput_t speed) {
    Motor_Decode(speed, 0, 0);
}

void __STATIC_INLINE Motion_MoveToRight(MotorInput_t speed) {
    Motor_Decode(-speed, 0, 0);
}

void __STATIC_INLINE Motion_RotateClockwise(MotorInput_t speed) {
    Motor_Decode(0, 0, speed);
}

void __STATIC_INLINE Motion_RotateCounterClockwise(MotorInput_t speed) {
    Motor_Decode(0, 0, -speed);
}

void Motion_CurrentNodeUpdate(void);

void Motion_CurrentNodeDecreaseUpdate(void);

void Motion_CorrectWhenMovingAtY(void);

void Motion_CorrectWhenMovingAtX(void);

void Motion_CorrectAtCross(void);

void Motion_CorrectInPickingArea(void);

void Motion_CorrectWhenThrowing(void);


void Motion_MoveLeftStable(uint8_t num);

void Motion_MoveRightStable(uint8_t num);

void Motion_MoveForwardStable(uint8_t num);

void Motion_MoveBackwardStable(uint8_t num);



void Motion_MoveForwardCrossing(uint8_t num);

void Motion_MoveBackwardCrossing(uint8_t num);


void Motion_MoveLeftStableInPickingArea(uint8_t num);

void Motion_MoveRightStableInPickingArea(uint8_t num);



uint8_t Motion_PickUpBallForward(void);

uint8_t Motion_PickUpBallBackward(void);


/*                    API                                                     */

void Motion_MoveFromNodeToNode(node_t from, node_t to);

void Motion_MoveFromBeginningArea(node_t to);


#endif // !__MOTION_H

//合理的伪代码

// {
//     SetSpeed()
//     while (1) {
//         Motion_CorrectWhenMoving()
//         getTraceInfo()
//         if (between node) {
//             node++
//         }
//         if (node == targetnode) {
//             SetSpeed(less)
//             delay()
//             SetSpeed(stop)
//             break
//         }
//     }
//     Motion_CorrectAtCross()
// }

