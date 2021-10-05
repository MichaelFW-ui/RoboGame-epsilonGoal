/**
 * @file motion.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 运动模式封装，和自主修正偏差
 * 
 * 编程中遇到的主要问题是，如何才能合理地控制机器人在缺乏传感器的情况下完成姿态的自我保持。
 * 如果使用精度较高的两层PID，即对机器人在三轴上的运动，进行速度环和位置环的控制，会造成
 * 获取信息的缺乏。直截了当地讲，我们无法仅仅通过底盘的红外阵列，得到足够的反馈信息。
 * 
 * 根据组委会的思考提示，我们可以使用阻塞调整的办法，粗糙地、动态地调整机器人的姿态。因为
 * 比赛中并没有要求机器人多线程任务并行进行，同时本代码中FreeRTOS发生了故障，我们仅仅采
 * 用阻塞的修正办法即可。
 * 
 * 在motor.c中，我们实现了一个函数，Motor_Decode，它将目标的机器人三轴运动解码为电机
 * 的运动速度。其中参数有待调定。
 * 
 * 这里我们将实现循线的方式。很简单，利用Motor_Decode设定速度，然后阻塞使用Correct函
 * 数，不断调整w轴上的分量和保持机器人在线上即可。同时，我们需要不断检测是否即将到达目的地，
 * 如果即将到达，则进行刹车动作，并在停止后使用适用于静止状态的correct函数即可。
 * 
 * Correct函数的实现也分为两种。普通区域中，线的密度很小，直接处理即可。取球区域中，黑线
 * 密度较高，需要对不同的黑线进行标号，或者只取接近两边中心的黑线作为姿态调整的反馈即可。
 * 
 * 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "motion.h"

#include "arm_ctrl.h"
#include "motor.h"
#include "pid.h"
#include "procedure.h"
#include "sensor.h"
#include "stdio.h"
#include "steer_ctrl.h"

node_t CurrentNode = Node_InValid;

void Motion_CurrentNodeUpdate(void) {
    HAL_Delay(50);
    /*TODO*/
    // 根据porcedure中的枚举判断Node如何变化
    if (CurrentProcedure == eProcedure_Default) {
        /*TODO*/
        // Test only
        printf("[WARN] Now you are still in default procedure\r\n");
        return;
    }
    if (CurrentProcedure == eProcedure_HeadForPickingArea ||
        CurrentProcedure == eProcedure_EnterPickingArea ||
        CurrentProcedure == eProcedure_HeadForPickingAreaSecondly) {
        uint8_t i = 0;
        while (ProcedureNodeInitial[i] != CurrentNode)
            ++i;
        CurrentNode = ProcedureNodeInitial[++i];
    }

    if (CurrentProcedure == eProcedure_ExitPickingArea ||
        CurrentProcedure == eProcedure_HeadForThrowingArea) {
        uint8_t j = 0;
        while (ProcedureNodeInitialBack[j] != CurrentNode) {
            ++j;
        }
        CurrentNode = ProcedureNodeInitialBack[++j];
    }

    // if (CurrentProcedure == eProcedure_HeadForPickingAreaSecondly) {
    //     uint8_t k = 0;
    //     while (ProcedureNodeSubprogress[k] != CurrentNode) {
    //         ++k;
    //     }
    //     CurrentNode = ProcedureNodeSubprogress[++k];
    // }
    printf("[N]Updated %d\r\n", (uint8_t)CurrentNode);
}


void Motion_CurrentNodeDecreaseUpdate(void) {
    HAL_Delay(50);
    // Motion_CurrentNodeUpdate();
    // return;
    /*TODO*/
    // 根据porcedure中的枚举判断Node如何变化
    if (CurrentProcedure == eProcedure_Default) {
        /*TODO*/
        // Test only
        printf("[WARN] Now you are still in default procedure\r\n");
        return;
    }
    if (CurrentProcedure == eProcedure_HeadForPickingArea ||
        CurrentProcedure == eProcedure_EnterPickingArea ||
        CurrentProcedure == eProcedure_HeadForPickingAreaSecondly) {
        uint8_t i = 0;
        while (ProcedureNodeInitial[i] != CurrentNode)
            ++i;
        CurrentNode = ProcedureNodeInitial[--i];
    }

    if (CurrentProcedure == eProcedure_ExitPickingArea ||
        CurrentProcedure == eProcedure_HeadForThrowingArea) {
        uint8_t j = 0;
        while (ProcedureNodeInitialBack[j] != CurrentNode) {
            ++j;
        }
        CurrentNode = ProcedureNodeInitialBack[++j];
    }

    // if (CurrentProcedure == eProcedure_HeadForPickingAreaSecondly) {
    //     uint8_t k = 0;
    //     while (ProcedureNodeSubprogress[k] != CurrentNode) {
    //         ++k;
    //     }
    //     CurrentNode = ProcedureNodeSubprogress[++k];
    // }

    printf("[N]Deupdated %d\r\n", (uint8_t)CurrentNode);
}

// void Motion_MoveFromNodeToNode(node_t From, node_t To) {
//     if (1) {

//     }
// }

void Motion_MoveFromBeginningArea(node_t To) {
    // Should not be implemented.
}

void Motion_MoveLeftStable(uint8_t num) {
    uint8_t isBetweenNode = 0;
    Motion_MoveToLeft(MOTION_HIGH_SPEED - 10);
    while (1) {
        Motion_CorrectWhenMovingAtX();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[2]) >= 5) {
            if (isBetweenNode) {
                isBetweenNode = 0;
                Motion_CurrentNodeUpdate();
                --num;
                printf("Decreased number %d to %d\r\n", num + 1, num);
                // 剩余结点数减一
            }
        } else {
            isBetweenNode = 1;
        }
        // if (count_bits(ptr[2]) >= 6 && isBetweenNode) {
        //     isBetweenNode = 0;
        //     Motion_CurrentNodeUpdate();
        //     --num;
        //     // 剩余结点数减一
        //     printf("Decreased number %d to %d\r\n", num + 1, num);
        // } else {
        //     isBetweenNode = 1;
        // }
        if (!num) {
            // 即将到达目的地
            Motion_MoveToLeft(MOTION_LOW_SPEED - 23);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (ptr[0] & (1 << 4) || ptr[3] & (1 << 4)) {
                    break;
                }
            }
            Motion_MoveToLeft(-50);
            // Motion_CorrectAtCross();
            return;
        }
    }
    /*TODO
    Check if it works*/
}

void Motion_MoveRightStable(uint8_t num) {
    uint8_t isBetweenNode = 0;
    Motion_MoveToRight(MOTION_HIGH_SPEED);
    while (1) {
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[1]) >= 5) {
            if (isBetweenNode) {
                isBetweenNode = 0;
                Motion_CurrentNodeDecreaseUpdate();
                --num;
                printf("Decreased number %d to %d\r\n", num + 1, num);
                // 剩余结点数减一
            }
        } else {
            isBetweenNode = 1;
        }
        // if (count_bits(ptr[1]) >= 5 && isBetweenNode) {
        //     // 如果这时两边首次没有示数，即首次进入无节点的区域
        //     isBetweenNode = 0;
        //     Motion_CurrentNodeDecreaseUpdate();
        //     --num;
        //     // 剩余结点数减一
        // } else {
        //     isBetweenNode = 1;
        // }
        Motion_CorrectWhenMovingAtX();
        if (!num) {
            // 即将到达目的地
            Motion_MoveToRight(MOTION_LOW_SPEED);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (ptr[0] & (1 << 5) || ptr[3] & (1 << 5)) {
                    break;
                }
            }
            Motion_MoveToRight(0);
            // Motion_CorrectAtCross();
            return;
        }
    }
    /*TODO
    Check if it works*/
}

void Motion_MoveForwardStable(uint8_t num) {
    uint8_t isBetweenNode = 0;
    if (CurrentNode == Node_4) {
        Motion_MoveForward(MOTION_HIGH_SPEED - 10);
    } else {
        Motion_MoveForward(MOTION_HIGH_SPEED);
    }
    while (1) {
        Motion_CorrectWhenMovingAtY();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[3]) >= 5) {
            if (isBetweenNode) {
                isBetweenNode = 0;
                Motion_CurrentNodeUpdate();
                --num;
                printf("Decreased number %d to %d\r\n", num + 1, num);
                // 剩余结点数减一
            }
        } else {
            isBetweenNode = 1;
        }
        // if (count_bits(ptr[3]) >= 5 && isBetweenNode) {
        //     isBetweenNode = 0;
        //     Motion_CurrentNodeUpdate();
        //     --num;
        //     printf("Decreased number %d to %d\r\n", num + 1, num);
        //     // 剩余结点数减一
        // } else {
        //     isBetweenNode = 1;
        // }
        if (!num) {
            // 即将到达目的地
            Motion_MoveForward(MOTION_LOW_SPEED - 25);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (ptr[1] & (1 << 4) || ptr[2] & (1 << 4)) {
                    break;
                }
            }
            Motion_MoveForward(-60);
            HAL_Delay(100);
            // Motion_CorrectAtCross();
            return;
        }
    }
    /*TODO
    Check if it works*/
}

void Motion_MoveBackwardStable(uint8_t num) {
    uint8_t isBetweenNode = 0;
    if (CurrentNode == Node_7) {
        Motion_MoveBackward(MOTION_HIGH_SPEED - 10);
    } else {
        Motion_MoveBackward(MOTION_HIGH_SPEED);
    }
    while (1) {
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[0]) >= 5) {
            if (isBetweenNode) {
                isBetweenNode = 0;
                Motion_CurrentNodeDecreaseUpdate();
                --num;
                printf("Decreased number %d to %d\r\n", num + 1, num);
                // 剩余结点数减一
            }
        } else {
            isBetweenNode = 1;
        }
        // if (count_bits(ptr[0]) >= 5 && isBetweenNode) {
        //     // 如果这时两边首次没有示数，即首次进入无节点的区域
        //     isBetweenNode = 0;
        //     Motion_CurrentNodeDecreaseUpdate();
        //     --num;
        //     // 剩余结点数减一
        //     printf("Decreased number %d to %d\r\n", num + 1, num);
        // } else {
        //     isBetweenNode = 1;
        // }
        Motion_CorrectWhenMovingAtY();
        if (!num) {
            // 即将到达目的地
            Motion_MoveBackward(MOTION_LOW_SPEED - 25);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (ptr[1] & (1 << 6) || ptr[2] & (1 << 6)) {
                    break;
                }
            }
            Motion_MoveBackward(-60);
            HAL_Delay(100);
            // Motion_CorrectAtCross();
            return;
        }
    }
    /*TODO: 
    Check if it works*/
}

void Motion_MoveLeftStableInPickingArea(uint8_t num) {
    uint8_t HasLine = 1;
    Motion_MoveToLeft(MOTION_LOW_SPEED);
    HAL_Delay(100);
    while (1) {
        Motion_CorrectWhenMovingAtX();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        // if (IsActive(ptr[0], 5) || IsActive(ptr[3], 5)) {
        if (count_bits(ptr[2]) >= 5) {
            if (HasLine) {
                HasLine = 0;
                --num;
                Motion_CurrentNodeUpdate();
                HAL_Delay(50);
            }
        } else {
            HasLine = 1;
        }

        // if (num && ) {
        //     Motion_CurrentNodeUpdate();
        //     --num;
        // }
        // if (num == 1) {
        if (!num) {
            Motion_MoveToLeft(10);
            while (1) {
                Motion_CorrectWhenMovingAtX();
                ptr = Sensor_GetCurrentInfo();
                if (IsActive(ptr[0], 3) || IsActive(ptr[3], 3)) {
                    break;
                }
            }
            Motion_MoveToLeft(0);
            // Motion_CurrentNodeUpdate();
            return;
        }
    }
    /*TODO*/
}

void Motion_MoveRightStableInPickingArea(uint8_t num) {
    uint8_t HasLine = 1;
    Motion_MoveToRight(MOTION_LOW_SPEED);
    HAL_Delay(100);
    while (1) {
        Motion_CorrectWhenMovingAtX();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        // if (IsActive(ptr[0], 5) || IsActive(ptr[3], 5)) {
        if (count_bits(ptr[1]) >= 5) {
            if (HasLine) {
                HasLine = 0;
                Motion_CurrentNodeDecreaseUpdate();
                --num;
            }
        } else {
            HasLine = 1;
        }
        // if (num == 1) {
        if (!num) {
            Motion_MoveToRight(10);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (IsActive(ptr[0], 6) || IsActive(ptr[3], 6)) {
                    break;
                }
                Motion_CorrectWhenMovingAtX();
            }
            Motion_MoveToRight(0);
            // Motion_CurrentNodeDecreaseUpdate();
            return;
        }
    }
    /*TODO*/
}

void Motion_MoveForwardCrossing(uint8_t num) {
    UNUSED(num);

    Motion_MoveForward(MOTION_LOW_SPEED);
    while (1) {
        Motion_CorrectWhenMovingAtY();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[3]) >= 5) {
            // 即将到达目的地
            Motion_MoveForward(MOTION_LOW_SPEED - 10);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (ptr[1] & (1 << 5) || ptr[2] & (1 << 5)) {
                    break;
                }
            }
            Motion_MoveForward(-40);
            CurrentNode = Node_6;
            // ???? Sure is it? Why not Node_6?
            return;
        }
    }
    /*TODO
    Check if it works*/
}

void Motion_MoveBackwardCrossing(uint8_t num) {
    UNUSED(num);

    Motion_MoveBackward(MOTION_LOW_SPEED);
    while (1) {
        Motion_CorrectWhenMovingAtY();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (count_bits(ptr[0]) >= 5) {
            // 即将到达目的地
            Motion_MoveBackward(MOTION_LOW_SPEED - 10);
            while (1) {
                ptr = Sensor_GetCurrentInfo();
                if (ptr[1] & (1 << 5) || ptr[2] & (1 << 5)) {
                    break;
                }
            }
            Motion_MoveBackward(-50);
            CurrentNode = Node_4;
            // ???? Sure is it? Why not Node_5?
            // Motion_CorrectAtCross();
            return;
        }
    }
    /*TODO*/
}

void Motion_CorrectWhenMovingAtY(void) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    uint8_t FrontBegin, FrontEnd, BackBegin, BackEnd;
    // 得到前后两边上的点位
    Position_GetOneActive(ptr[0], 9, &FrontBegin, &FrontEnd);
    Position_GetOneActive(ptr[3], 9, &BackBegin, &BackEnd);

    uint8_t Front = (FrontBegin + FrontEnd);
    uint8_t Back = (BackBegin + BackEnd);
    // 反馈调节
    Motor_SetW((int16_t)((int8_t)Front - (int8_t)Back) * (-49));
    Motor_SetX((int16_t)((int8_t)Front + (int8_t)Back - 16) * (-4));
}

void Motion_CorrectWhenMovingAtX(void) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    uint8_t LeftBegin, LeftEnd, RightBegin, RightEnd;
    // 得到左右的点位
    Position_GetOneActive(ptr[1], 11, &LeftBegin, &LeftEnd);
    Position_GetOneActive(ptr[2], 11, &RightBegin, &RightEnd);

    uint8_t Left = (LeftEnd + LeftBegin);
    uint8_t Right = (RightEnd + RightBegin);

    // 反馈调节
    int16_t Front = (((int16_t)((int8_t)Left + (int8_t)Right) - 20) * (-2));
    Motor_SetY(Front);
    Motor_SetW(((int16_t)((int8_t)Left - (int8_t)Right)) * (50));
}

/**
 * @brief 路口处修正函数，不一定安全。
 * 
 */
void Motion_CorrectAtCross(void) {
    TraceInfo_t *ptr;

    // uint16_t mask9 = 0x7f << (((9 - 1) >> 1) - 3);
    // uint16_t mask11 = 0x7f << (((11 - 1) >> 1) - 3);
    uint8_t Begins[4], Ends[4];

    for (int i = 0; i < 5; ++i) {
        ptr = Sensor_GetCurrentInfo();
        // 拿激活点
        Position_GetOneActive(ptr[0], 9, &Begins[0], &Ends[0]);
        Position_GetOneActive(ptr[1], 11, &Begins[1], &Ends[1]);
        Position_GetOneActive(ptr[2], 11, &Begins[2], &Ends[2]);
        Position_GetOneActive(ptr[3], 9, &Begins[3], &Ends[3]);

        // 得到中点
        uint8_t Middle[] = {
            (Begins[0] + Ends[0]),
            (Begins[1] + Ends[1]),
            (Begins[2] + Ends[2]),
            (Begins[3] + Ends[3])
        };

        // 得到中心点
        int16_t CenterX = (Middle[0] + Middle[3]);
        int16_t CenterY = (Middle[1] + Middle[2]);

        // 修正之
        Motor_SetW((int16_t)((int16_t)Middle[0] - (int16_t)Middle[3]) * (70));
        Motor_SetX((int16_t)((CenterX - 4 * 4) * (-3)));
        Motor_SetY((int16_t)(CenterY - 5 * 4) * (-3));
        // HAL_Delay(50);
    }
    /*TODO*/
}

/**
 * @brief 捡球区的路口Correct函数，
 * 有待测试
 * 
 */
void Motion_CorrectInPickingArea(void) {
    return;
    TraceInfo_t *ptr;
    uint8_t Begins[4], Ends[4];
    /*TODO*/
    // for (int i = 0; i < 5; ++i) {
    //     Motion_CorrectWhenMovingAtX();
    //     HAL_Delay(50);
    // }
    Motion_MoveToLeft(0);
    for (int i = 0; i < 5; ++i) {
        ptr = Sensor_GetCurrentInfo();
        Begins[0] = Ends[0] = 4;
        Begins[3] = Ends[3] = 4;
        while (!IsActive(ptr[0], Begins[0]))
            --Begins[0];
        while (!IsActive(ptr[0], Ends[0]))
            ++Ends[0];
        if (Ends[0] - 4 >= 4 - Begins[0]) {
            Motor_SetX(10);
            // 直到差一点到达正确位置
            while (!IsActive(Sensor_GetCurrentInfo()[0], 3))
                Motion_CorrectWhenMovingAtX();
            Motor_SetX(0);
        } else {
            Motor_SetX(-10);
            // 直到差一点到达正确位置
            while (!IsActive(Sensor_GetCurrentInfo()[0], 5))
                Motion_CorrectWhenMovingAtX();
            Motor_SetX(0);
        }

    }
}


void Motion_CorrectWhenMovingAtXInThrowingArea(void) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    /*
    辅助底盘逻辑
              ii      ^
              ii      |
             AiiB     |
              ii      |
             CiiD     |
              ii      |
              ii      |
        <-------------+

    */

    uint8_t A = IsActive(ptr[0], 9), B = IsActive(ptr[1], 11),
            C = IsActive(ptr[2], 11), D = IsActive(ptr[3], 9);

    Motor_SetW(0);
    Motor_SetY(0);

    if (A) {
        Motor_AddY(-10);
    }
    if (C) {
        Motor_AddY(10);
    }
    if (B) {
        Motor_AddW(-40);
    }
    if (D) {
        Motor_AddW(40);
    }
}

void Motion_MoveLeftInThrowingArea(void) {
    Motion_MoveToLeft(MOTION_LOW_SPEED - 10);
    // while (1) {
    //     // Motion_CorrectWhenMovingAtXInThrowingArea();
    //     Motion_CorrectWhenMovingAtX();
    //     TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    //     if (count_bits(ptr[1]) >= 6) {
    //         Motion_MoveToLeft(0);
    //         break;
    //     }
    // }
    for (size_t i = 0; i < 160; i++)
    {
        // Motion_CorrectWhenMovingAtXInThrowingArea();
        Motion_CorrectWhenMovingAtX();
        HAL_Delay(20);
        /* code */
    }
    Motion_MoveToLeft(0);
    

    /*TODO*/
}

void Motion_MoveRightInThrowingArea(void) {
    /*TODO*/
    Motion_MoveToRight(MOTION_LOW_SPEED - 5);

    while (1) {
        Motion_CorrectWhenMovingAtX();
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        // if (count_bits(ptr[1]) > 5 || IsActive(ptr[0], 8) || IsActive(ptr[3], 8)) {
        if (count_bits(ptr[1]) > 5) {
            Motion_MoveToRight(10);
            while (1) {
                Motion_CorrectWhenMovingAtX();
                ptr = Sensor_GetCurrentInfo();
                if (IsActive(ptr[0], 4) || IsActive(ptr[3], 4)) {
                    Motion_MoveToRight(0);
                    return;
                }
            }
        }

    }
}

uint8_t Motion_PickUpBallForward(void) {
    printf("RUNNING for balls AT %d Forward\r\n", CurrentNode);
    ARM_Forward_TalonOpen();
    Motion_MoveForward(MOTION_LOW_SPEED - 14);
    HAL_Delay(400);
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    // if (CurrentNode == Node_8 || CurrentNode == Node_9 || CurrentNode == Node_10) {
    //     HAL_Delay(15 * 70);

    // } else
    while (1) {
        Motion_CorrectWhenMovingAtY();
        ptr = Sensor_GetCurrentInfo();

        if (count_bits(ptr[0] > 6)) {
            for (int i = 0; i < 33; ++i) {
                Motion_CorrectWhenMovingAtY();
                HAL_Delay(50);
            }
            // HAL_Delay(600);
            break;
        }
    }

    Motion_MoveForward(0);

    ARM_Forward_TalonOpen();
    ARM_Forward_Raise();
    HAL_Delay(200);
    ARM_Forward_TakeBall();
    HAL_Delay(400);
    ARM_Forward_TalonClose();
    HAL_Delay(500);
    Motion_MoveBackward(15);
    HAL_Delay(800);
    Motion_MoveBackward(0);
    ARM_Forward_Raise();
    HAL_Delay(300);
    ARM_Forward_PutDown();
    HAL_Delay(500);
    ARM_Forward_TalonOpen();
    HAL_Delay(1000);
    Steer_Init();


    Motion_MoveBackward(MOTION_LOW_SPEED - 10);

    while (1) {
        Motion_CorrectWhenMovingAtY();
        ptr = Sensor_GetCurrentInfo();

        if (IsActive(ptr[1], 6) || IsActive(ptr[2], 6)) {
            break;
        }
    }
    Motion_MoveBackward(0);
    HAL_Delay(100);
    return 1;
    /*TODO*/
}

uint8_t Motion_PickUpBallBackward(void) {
    /*TODO*/
    printf("RUNNING for balls AT %d Backward\r\n", CurrentNode);
    ARM_Backward_TalonOpen();
    Motion_MoveBackward(MOTION_LOW_SPEED - 10);
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    HAL_Delay(400);
    while (1) {
        Motion_CorrectWhenMovingAtY();
        ptr = Sensor_GetCurrentInfo();


        if (count_bits(ptr[3] > 5)) {
            for (int i = 0; i < 27; ++i) {
                Motion_CorrectWhenMovingAtY();
                HAL_Delay(45);
            }
            // HAL_Delay(600);
            break;
        }
    }

    Motion_MoveBackward(0);

    ARM_Backward_TalonOpen();
    ARM_Backward_Raise();
    HAL_Delay(200);
    ARM_Backward_TakeBall();
    HAL_Delay(400);
    ARM_Backward_TalonClose();
    HAL_Delay(500);
    Steer_SetAngleByDegree(0, 153);
    Motion_MoveForward(15);
    HAL_Delay(800);
    Motion_MoveForward(0);
    ARM_Backward_Raise();
    HAL_Delay(300);
    ARM_Backward_PutDown();
    HAL_Delay(500);
    ARM_Backward_TalonOpen();
    HAL_Delay(1000);
    Steer_Init();


    Motion_MoveForward(MOTION_LOW_SPEED - 10);

    while (1) {
        Motion_CorrectWhenMovingAtY();
        ptr = Sensor_GetCurrentInfo();

        if (IsActive(ptr[1], 3) || IsActive(ptr[2], 3)) {
            break;
        }
    }
    Motion_MoveForward(0);
    HAL_Delay(100);

    return 1;
}

