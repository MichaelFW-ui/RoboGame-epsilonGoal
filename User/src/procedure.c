/**
 * @file procedure.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 全局过程，进行当前过程
 * 
 * 
 * 每一个Procedure分为三个部分：
 *  Procedure Sensor，读取要求的传感器数据
 *  Procedure Node，控制底盘运动到相应Node
 *  Procedure Action，控制舵机，推杆，摩擦轮之类，进行动作
 * 
 * 每一个Procedure参照CurrentNode处理相应事物。
 * 
 *  检测到新Node之后，试图将其调整至中心位置，或者仅在中心位置检测新Node。
 * 使用相应的NodeUpdate()来根据Node确认相应的执行任务。
 * 利用预编译，每次烧写只能包含一种Procedure，即如下的四种之一，
 * CrossOver_Overall, CrossOver, BareRun_Overall, BareRun.
 *  具体介绍下文有解释。
 * 
 * 事实上，数组只是为了方便编程时参考，并没有实际作用。
 * 数组的编号参考doc目录下面的一个图片。
 * 
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "procedure.h"

#include "com.h"
#include "motion.h"
#include "position.h"
#include "sensor.h"
#include "cannon.h"
#include "pushrod.h"

Procedure_t CurrentProcedure = eProcedure_Default;
uint8_t CurrentBallCnt = 0;
volatile BallTypeDef BallStatus[40];

#define CROSSOVER_OVERALL
/*
 *      CROSSOVER_OVERALL ：在取球区直接跑完全程，检测到全部的篮球；越障
 *      CROSSOVER         ：在取球区随遇取球，取到即返回；越障
 *      BARERUN_OVERALL   ：在取球区直接跑完全程，检测到全部的篮球；不越障
 *      BARERUN           ：在取球区随遇取球，取到即返回；不越障
 *
 */


// 压制 enumerated type mixed with another type 188-D Warning。
#pragma diag_suppress 188

#ifdef CROSSOVER_OVERALL

node_t ProcedureNodeInitial[]     = {0, 1,  2,  3,  4,  5,  6,  7, 8,
                                 9, 10, 11, 12, 13, 14, 15, 16};
node_t ProcedureNodeInitialBack[] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};

node_t ProcedureNodeSubprogress[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
node_t ProcedureNodeSubprogressBack[] = {16, 15, 14, 13, 12, 11, 10,
                                         9,  8,  7,  6,  5,  4};

#endif // CROSSOVER_OVERALL


#ifdef CROSSOVER

node_t ProcedureNodeInitial[]     = {0, 1,  2,  3,  4,  5,  6,  7, 8,
                                 9, 10, 11, 12, 13, 14, 15, 16};
node_t ProcedureNodeInitialBack[] = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};

node_t ProcedureNodeSubprogress[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
node_t ProcedureNodeSubprogressBack[] = {16, 15, 14, 13, 12, 11, 10,
                                         9,  8,  7,  6,  5,  4};
// node_t ProcedureNodeInitial[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
// // 进入取球区后轨迹不确定
// node_t ProcedureNodeInitialBack[] = {8, 7, 6, 5, 4};

// node_t ProcedureNodeSubprogress[] = {5, 6, 7, 8};
// // 进入取球区后轨迹不确定
// node_t ProcedureNodeSubprogressBack[] = {8, 7, 6, 5, 4};

#endif // CROSSOVER

#ifdef BARERUN_OVERALL

node_t ProcedureNodeInitial[] = {0,  1,  2,  17, 19, 7,  8, 9,
                                 10, 11, 12, 13, 14, 15, 16};
// 中间无法循线，看造化
node_t ProcedureNodeInitialBack[] = {16, 15, 14, 13, 12, 11, 10, 9,
                                     8,  7,  19, 17, 2,  3,  4};

node_t ProcedureNodeSubprogress[] = {4,  3,  2,  17, 19, 7,  8, 9,
                                     10, 11, 12, 13, 14, 15, 16};
// 中间无法循线，看造化
node_t ProcedureNodeSubprogressBack[] = {16, 15, 14, 13, 12, 11, 10, 9,
                                         8,  7,  19, 17, 2,  3,  4};

#endif // BARERUN_OVERALL

#ifdef BARERUN

node_t ProcedureNodeInitial[] = {0, 1, 2, 17, 19, 7, 8};
// 中间无法循线，看造化
// 进入取球区后轨迹不确定
node_t ProcedureNodeInitialBack[] = {8, 7, 19 ,17, 2, 3, 4};

node_t ProcedureNodeSubprogress[] = {4, 3, 2, 17, 19, 7, 8};
// 中间无法循线，看造化
// 进入取球区后轨迹不确定
node_t ProcedureNodeSubprogressBack[] = {8, 7, 19, 17, 2, 3, 4};

#endif // BARERUN


// 恢复 enumerated type mixed with another type 188-D Warning。
#pragma diag_warning 188


#define arrsize(arr) ((sizeof(arr)) / sizeof(arr[0]))
#define ProcedureNodeInitialSize arrsize(ProcedureNodeInitial)
#define ProcedureNodeSubprogressSize arrsize(ProcedureNodeSubprogress)
#define ProcedureNodeInitialBackSize arrsize(ProcedureNodeInitialBack)
#define ProcedureNodeSubprogressBackSize arrsize(ProcedureNodeSubprogressBack)
#undef arrsize

node_t Procedure_CurrentNode = Node_InValid;

/*             CrossOver Overall Implementation                               */
#ifdef CROSSOVER_OVERALL



/**
 * @brief 启动。走到第一个结点处
 * 
 */
void Procedure_Default(void) {
    CurrentProcedure = eProcedure_Default; // 手动接管启动阶段的结点控制
    CurrentNode = Node_0;
    Motion_MoveForward(MOTION_HIGH_SPEED);
    while (1) {
        Motion_CorrectWhenMovingAtY();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[3]) > 5) {
            // 压到黑线了
            break;
        }
    }
    Motion_MoveForward(MOTION_HIGH_SPEED);
    Motion_CorrectWhenMovingAtY();
    // 不停，直接进入下一阶段
    HAL_Delay(100);

    /*TODO*/
}

void Procedure_HeadForPickingArea(void) {
    CurrentProcedure = eProcedure_HeadForPickingArea;
    // 到达结点2
    Motion_MoveForwardStable(2);
    // 到达结点4
    Motion_MoveLeftStable(2);
    // 越障
    Motion_MoveForwardStable(1);
    Motion_MoveForwardCrossing(1);
    Motion_MoveForwardStable(1);
    // 到达取球区门口
    // 转到EnterPickingArea
    /*TODO*/
}

#define MASK(n) ((1 << (n)))
#ifdef READ
#undef READ
#define READ(a, mask) ((a) & (mask))
#endif
#define SET(a, mask) ((a) = ((a) | (mask)))
#define RESET(a, mask) ((a) = ((a) & ~(mask)))

void Procedure_EnterPickingArea(void) {
    CurrentProcedure = eProcedure_EnterPickingArea;
    // 进入取球区
    static uint8_t CurrentBallCnt = 0;

    /**
     * @brief 
     * 
     * 取球区编号方法：
     * 当前结点编号 * 2， 在结点上方
     * 当前结点编号 * 2 + 1，在节点下方
     * 
     */
    for (uint8_t i = 0; i < 40; ++i) {
        BallStatus[i] = NotDetected;
    }
    // 到达结点8
    Motion_MoveToLeft(MOTION_LOW_SPEED);
    HAL_Delay(100);
    while (1) {
        Motion_CorrectWhenMovingAtX();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[2]) >= 5) {
            printf("Enter Node 8 \r\n");
            break;
        }
    }
    // Motion_MoveToLeft(0);
    // Motion_CorrectInPickingArea();
    Motion_CurrentNodeUpdate();
    CurrentNode = Node_8;
    HAL_Delay(100);

    // To node 9
    Motion_MoveLeftStableInPickingArea(1);
    CurrentNode = Node_9;

    Com_SendWorkingCommand();
    Com_DataTypeDef info;
    Com_Receive(&info);

    for (int i = 0; i < 6; ++i) {
        /*
                    4 2 0
                    + + +
                    +-+-+
                    + + +
                    5 3 1
        */
        if (READ(info.info, MASK(i))) {
            BallStatus[(CurrentNode - 1) * 2 + i] = isBasketball;
        } else {
            BallStatus[(CurrentNode - 1) * 2 + i] = isNOTBasketball;
        }
    }

    // To node 12
    Motion_MoveLeftStableInPickingArea(3);

    Com_SendWorkingCommand();
    Com_Receive(&info);

    for (int i = 0; i < 6; ++i) {
        /*
                    4 2 0
                    + + +
                    +-+-+
                    + + +
                    5 3 1
        */
        if (READ(info.info, MASK(i))) {
            BallStatus[(CurrentNode - 1) * 2 + i] = isBasketball;
        } else {
            BallStatus[(CurrentNode - 1) * 2 + i] = isNOTBasketball;
        }
    }

    // To node 15
    Motion_MoveLeftStableInPickingArea(3);

    printf("present at %d\r\n", CurrentNode);
    CurrentNode = Node_15;

    Com_SendWorkingCommand();
    Com_Receive(&info);

    for (int i = 0; i < 6; ++i) {
        /*
                    4 2 0
                    + + +
                    +-+-+
                    + + +
                    5 3 1
        */
        if (READ(info.info, MASK(i))) {
            BallStatus[(CurrentNode - 1) * 2 + i] = isBasketball;
        } else {
            // BallStatus[(CurrentNode - 1) * 2 + i] = isBasketball;
            BallStatus[(CurrentNode - 1) * 2 + i] = isNOTBasketball;
        }
    }
    ///***********************************************************************//
    // return;
    ///***********************************************************************//

    // 捡球
    if (BallStatus[16 * 2] == isBasketball || BallStatus[16 * 2 + 1] == isBasketball) {
        Motion_MoveLeftStableInPickingArea(1);
        Motion_CorrectInPickingArea();
        if (BallStatus[16 * 2] == isBasketball) {
            BallStatus[16 * 2] = wasBasketball;
            CurrentBallCnt += Motion_PickUpBallForward();
        }
        if (BallStatus[16 * 2 + 1] == isBasketball) {
            BallStatus[16 * 2 + 1] = wasBasketball;
            CurrentBallCnt += Motion_PickUpBallBackward();
        }
        Motion_MoveRightStableInPickingArea(1);
        Motion_CorrectInPickingArea();
    } 
    // 够了？
    if (CurrentBallCnt == 2)
        return;
    while (1) {
        uint8_t beg = CurrentNode;
        while (BallStatus[beg * 2] != isBasketball &&
            BallStatus[beg * 2 + 1] != isBasketball && beg > 7) {
            --beg;
        }
        if (beg == 7)
            return;
        if (beg != CurrentNode) {
            Motion_MoveRightStableInPickingArea(CurrentNode - beg);
            Motion_CorrectInPickingArea();
        }
        if (BallStatus[CurrentNode * 2] == isBasketball) {
            BallStatus[CurrentNode * 2] = wasBasketball;
            CurrentBallCnt += Motion_PickUpBallForward();
            Motion_CorrectInPickingArea();
        }
        if (CurrentBallCnt == 2)
            return;
        if (BallStatus[CurrentNode * 2 + 1] == isBasketball) {
            BallStatus[CurrentNode * 2 + 1] = wasBasketball;
            CurrentBallCnt += Motion_PickUpBallBackward();
            Motion_CorrectInPickingArea();
        }
        if (CurrentBallCnt == 2)
            return;
    }
    /*TODO*/
}

void Procedure_ExitPickingArea(void) {
    CurrentProcedure = eProcedure_ExitPickingArea;
    // 返回
    // while (CurrentNode != Node_8) {
    //     Motion_MoveRightStableInPickingArea(1);
    // }
    Motor_Decode(0, 0, 0);
    HAL_Delay(100);
    if (CurrentNode != Node_8) {
        Motion_MoveRightStableInPickingArea(CurrentNode - Node_8);
    }
    CurrentNode = Node_8;
    // 特判返回Node_7
    Motion_MoveToRight(MOTION_LOW_SPEED);
    HAL_Delay(100);
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    while (1) {
        Motion_CorrectWhenMovingAtX();
        ptr = Sensor_GetCurrentInfo();

        if (count_bits(ptr[1]) > 5) {
            Motion_MoveToRight(MOTION_LOW_SPEED - 10);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (IsActive(ptr[0], 6) || IsActive(ptr[3], 6)) {
                    break;
                }
            }
            Motion_MoveToRight(-50);
            break;
        }
    }
    CurrentNode = Node_7;


    /*TODO*/
}

void Procedure_HeadForThrowingArea(void) {
    CurrentProcedure = eProcedure_HeadForThrowingArea;
    Motion_MoveBackwardStable(1);
    Motion_MoveBackwardCrossing(1);
    Motion_MoveBackwardStable(1);
    HAL_Delay(110);
    Motor_Decode(0, 0, 0);
}

void Procedure_StayInThrowingArea(void) {
    CurrentProcedure = eProcedure_StayInThrowingArea;

    CurrentNode = Node_4;
    Motor_Decode(0, 0, 0);
    Motion_MoveLeftInThrowingArea();
    Motor_Decode(0, 0, 0);

    Cannon_SetTargetSpeed(5000);
    HAL_Delay(3000);
    // Motion_CorrectWhenThrowing();

    // 发射第一次
    /*TODO*/
    // 老规矩，修正
    // Motion_CorrectWhenThrowing();
    
    // 发射第二次
    /*TODO*/
    Pushrod_MoveBackward(65000);
    HAL_Delay(4000);
    // Cannon_SetTargetSpeed(5000);
    // HAL_Delay(3000);
    Pushrod_MoveBackward(65000);
    HAL_Delay(3000);
    // 老规矩，修正
    // Motion_CorrectWhenThrowing();
    // 步进电机归位

    Pushrod_MoveForward(65000 * 2);
    CurrentBallCnt = 0;
    Cannon_SetTargetSpeed(0);

    Motion_MoveRightInThrowingArea();
    Motor_Decode(0, 0, 0);
    /*TODO*/
}

void Procedure_HeadForPickingAreaSecondly(void) {
    CurrentProcedure = eProcedure_HeadForPickingAreaSecondly;
    // // 特判返回Node 4
    // Motion_MoveToRight(MOTION_LOW_SPEED);
    // HAL_Delay(500);
    // Motion_MoveToRight(0);
    // Motion_CorrectAtCross();

    TraceInfo_t *ptr = Sensor_GetCurrentInfo();

    // To Node 4
    // Motion_MoveToRight(MOTION_LOW_SPEED - 10);
    // while (1) {
    //     Motion_CorrectWhenMovingAtX();
    //     ptr = Sensor_GetCurrentInfo();
    //     if (IsActive(ptr[0], 6) || IsActive(ptr[3], 6)) {
    //         break;
    //     }
    // }
    // Motion_MoveToRight(0);


    // To Node 7
    Motion_MoveForwardStable(1);
    Motion_MoveForwardCrossing(1);
    Motion_MoveForwardStable(1);
    CurrentNode = Node_7;
    HAL_Delay(80);
    Motion_MoveForward(0);
}

void Procedure_EnterPickingAreaSecondly(void) {

    // 到达结点8
    Motion_MoveToLeft(MOTION_LOW_SPEED - 5);
    while (1) {
        Motion_CorrectWhenMovingAtX();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[2]) >= 5) {
            CurrentNode = Node_8;
            Motion_MoveToLeft(0);
            break;
        }
    }
    // Motion_MoveToLeft(0);
    // Motion_CorrectInPickingArea();
    // Motion_CurrentNodeUpdate();
    HAL_Delay(100);
    printf("Enter area at 8\r\n");



    node_t beg = Node_8;
    // 找最右边的一个篮球
    while (BallStatus[beg * 2] != isBasketball && BallStatus[beg * 2 + 1] != isBasketball) {
        ++beg;
    }
    if (beg != CurrentNode) {
        Motion_MoveLeftStableInPickingArea(beg - CurrentNode);
    }

    while (CurrentBallCnt != 2) {
        if (BallStatus[CurrentNode * 2] == isBasketball) {
            CurrentBallCnt += Motion_PickUpBallForward();
            BallStatus[CurrentNode * 2] = wasBasketball;
        }
        if (CurrentBallCnt == 2)
            return;
        if (BallStatus[CurrentNode * 2 + 1] == isBasketball) {
            CurrentBallCnt += Motion_PickUpBallBackward();
            BallStatus[CurrentNode * 2 + 1] = wasBasketball;
        }
        if (CurrentBallCnt == 2)
            return;

        //   向左找
        // beg = (uint8_t)CurrentNode + 1;
        beg = CurrentNode;
        while (BallStatus[beg * 2] != isBasketball &&
               BallStatus[beg * 2 + 1] != isBasketball && beg < 17) {
            ++beg;
        }
        if (beg == 17)
            return;
        printf("Head for beg:%d\r\n", beg);
        if (beg != CurrentNode)
            Motion_MoveLeftStableInPickingArea(beg - CurrentNode);
    }




    /*TODO*/
}

#endif // CROSSOVER_OVERALL


/*             CrossOver Implementation                                       */
#ifdef CROSSOVER

/**
 * @brief 启动。走到第一个结点处
 * 
 */
void Procedure_Default(void) {
    CurrentProcedure = eProcedure_Default;
    // 手动接管启动阶段的结点控制
    CurrentNode = Node_0;
    Motion_MoveForward(MOTION_HIGH_SPEED);
    while (1) {
        Motion_CorrectWhenMovingAtY();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (ptr[1] || ptr[2]) {
            // 压到黑线了
            break;
        }
    }
    Motion_MoveForward(MOTION_LOW_SPEED);
    Motion_CorrectWhenMovingAtY();
    // 不停，直接进入下一阶段

    /*TODO*/
}

void Procedure_HeadForPickingArea(void) {
    CurrentProcedure = eProcedure_HeadForPickingArea;
    // 到达结点2
    Motion_MoveForwardStable(2);
    // 到达结点4
    Motion_MoveLeftStable(2);
    // 越障
    Motion_MoveForwardCrossing(3);
    // 到达取球区门口
    // 转到EnterPickingArea
    /*TODO*/
}

void Procedure_EnterPickingArea(void) {
    CurrentProcedure = eProcedure_EnterPickingArea;
    // 进入取球区
    static BallTypeDef BallStatus[40];
    static uint8_t CurrentBallCnt = 0;
    /**
     * @brief 
     * 
     * 取球区编号方法：
     * 当前结点编号 * 2， 在结点上方
     * 当前结点编号 * 2 + 1，在节点下方
     * 
     */
    for (uint8_t i = 0; i < 40; ++i)
    {
        BallStatus[i] = NotDetected;
    }
    // 到达结点8
    Motion_MoveToLeft(MOTION_HIGH_SPEED);
    HAL_Delay(500);
    Motion_MoveToLeft(0);
    Motion_CorrectInPickingArea();
    Motion_CurrentNodeUpdate();

    // 循环到发现两个球为止
    while (1) {
        Com_SendWorkingCommand();
        Com_DataTypeDef info;
        Com_Receive(&info);

        if (info.info & 0xF0) {
            BallStatus[CurrentNode * 2] = isBasketball;
            CurrentBallCnt += Motion_PickUpBallForward();
            //  取球之后，标记上不再是篮球了!!!
            BallStatus[CurrentNode * 2] = wasBasketball;
            if (CurrentBallCnt == 2) {
                // 跳出循环返回
                break;
            }
        } else {
            BallStatus[CurrentNode * 2] = isNOTBasketball;
        }

        if (info.info & 0x0F) {
            BallStatus[CurrentNode * 2 + 1] = isBasketball;
            CurrentBallCnt += Motion_PickUpBallBackward();
            //  取球之后，标记上不再是篮球了!!!
            BallStatus[CurrentNode * 2 + 1] = wasBasketball;
            if (CurrentBallCnt == 2) {
                // 跳出循环返回
                break;
            }
        } else {
            BallStatus[CurrentNode * 2 + 1] = isNOTBasketball;
        }
        Motion_MoveLeftStableInPickingArea(1);
    }

    /*TODO*/
}

void Procedure_ExitPickingArea(void) {
    CurrentProcedure = eProcedure_ExitPickingArea;
    // 返回
    while (CurrentNode != Node_8) {
        Motion_MoveRightStableInPickingArea(1);
    }

    // 特判返回Node_7
    Motion_MoveToRight(MOTION_HIGH_SPEED);
    HAL_Delay(500);
    Motion_MoveToLeft(0);
    Motion_CorrectAtCross();


    /*TODO*/
}

void Procedure_HeadForThrowingArea(void) {
    CurrentProcedure = eProcedure_HeadForThrowingArea;
    Motion_MoveBackwardCrossing(3);

    // 想办法停在一个不错的地方
    Motion_MoveToLeft(MOTION_LOW_SPEED);
    uint16_t distance;
    while ((distance = Sensor_GetLeftDistance()) >= 114514) {
        Motion_CorrectWhenMovingAtX();
    }
    Motion_MoveToLeft(0);
    // 老规矩，修正
    Motion_CorrectWhenThrowing();

    /*TODO*/
    /*TODO*/
}

void Procedure_StayInThrowingArea(void) {
    CurrentProcedure = eProcedure_StayInThrowingArea;
    /*TODO*/
    // 发射第一次
    /*TODO*/

    HAL_Delay(1000);
    // 老规矩，修正
    Motion_CorrectWhenThrowing();
    // 发射第二次
    /*TODO*/
    
    HAL_Delay(1000);
    // 老规矩，修正
    Motion_CorrectWhenThrowing();
    // 步进电机归位
    /*TODO*/

}

void Procedure_HeadForPickingAreaSecondly(void) {
    CurrentProcedure = eProcedure_HeadForPickingAreaSecondly;
    /*TODO*/
}



#endif // CROSSOVER


/*               BareRun Overall Implementation                               */
#ifdef BARERUN_OVERALL

void Procedure_Default(void) {
    /*TODO*/
}

void Procedure_HeadForPickingArea(void) {
    /*TODO*/
}

void Procedure_EnterPickingArea(void) {
    /*TODO*/
}

void Procedure_ExitPickingArea(void) {
    /*TODO*/
}

void Procedure_HeadForThrowingArea(void) {
    /*TODO*/
}

void Procedure_StayInThrowingArea(void) {
    /*TODO*/
}

void Procedure_HeadForPickingAreaSecondly(void) {
    /*TODO*/
}

#endif // BARERUN_OVERALL 

/*             BareRun Implementation                                         */
#ifdef BARERUN 

void Procedure_Default(void) {
    /*TODO*/
}

void Procedure_HeadForPickingArea(void) {
    /*TODO*/
}

void Procedure_EnterPickingArea(void) {
    /*TODO*/
}

void Procedure_ExitPickingArea(void) {
    /*TODO*/
}

void Procedure_HeadForThrowingArea(void) {
    /*TODO*/
}

void Procedure_StayInThrowingArea(void) {
    /*TODO*/
}

void Procedure_HeadForPickingAreaSecondly(void) {
    /*TODO*/
}

#endif // BARERUN 
