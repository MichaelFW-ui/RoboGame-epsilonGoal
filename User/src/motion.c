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
#include "sensor.h"
#include "procedure.h"
#include "motor.h"
#include "pid.h"


node_t CurrentNode = Node_InValid;

void Motion_CurrentNodeUpdate(void) {
    /*TODO*/
    // 根据porcedure中的枚举判断Node如何变化
    if (CurrentProcedure == eProcedure_Default) {
        /*TODO*/
        // Test only
        return;
    }
    if (CurrentProcedure == eProcedure_HeadForPickingArea ||
        CurrentProcedure == eProcedure_EnterPickingArea) {
        uint8_t i = 0;
        while (ProcedureNodeInitial[i] != CurrentNode)
            ++i;
        CurrentNode = ProcedureNodeInitial[++i];
        return;
    }

    if (CurrentProcedure == eProcedure_ExitPickingArea ||
        CurrentProcedure == eProcedure_HeadForThrowingArea) {
        uint8_t j = 0;
        while (ProcedureNodeInitialBack[j] != CurrentNode) {
            ++j;
        }
        CurrentNode = ProcedureNodeInitialBack[++j];
        return;
    }

    if (CurrentProcedure == eProcedure_HeadForPickingAreaSecondly) {
        uint8_t k = 0;
        while (ProcedureNodeSubprogress[k] != CurrentNode) {
            ++k;
        }
        CurrentNode = ProcedureNodeSubprogress[++k];
        return;
    }
}


void Motion_CurrentNodeDecreaseUpdate(void) {
    /*TODO*/
    // 根据porcedure中的枚举判断Node如何变化
    if (CurrentProcedure == eProcedure_Default) {
        /*TODO*/
        // Test only
        return;
    }
    if (CurrentProcedure == eProcedure_HeadForPickingArea ||
        CurrentProcedure == eProcedure_EnterPickingArea) {
        uint8_t i = 0;
        while (ProcedureNodeInitial[i] != CurrentNode)
            ++i;
        CurrentNode = ProcedureNodeInitial[--i];
        return;
    }

    if (CurrentProcedure == eProcedure_ExitPickingArea ||
        CurrentProcedure == eProcedure_HeadForThrowingArea) {
        uint8_t j = 0;
        while (ProcedureNodeInitialBack[j] != CurrentNode) {
            ++j;
        }
        CurrentNode = ProcedureNodeInitialBack[--j];
        return;
    }

    if (CurrentProcedure == eProcedure_HeadForPickingAreaSecondly) {
        uint8_t k = 0;
        while (ProcedureNodeSubprogress[k] != CurrentNode) {
            ++k;
        }
        CurrentNode = ProcedureNodeSubprogress[--k];
        return;
    }

}

void Motion_MoveFromNodeToNode(node_t From, node_t To) {
    if (1) {

    }
}

void Motion_MoveFromBeginningArea(node_t To) {

}

void Motion_MoveLeftStable(uint8_t num) {
    /*TODO*/
}

void Motion_MoveRightStable(uint8_t num) {
    /*TODO*/
}

void Motion_MoveForwardStable(uint8_t num) {
    uint8_t isBetweenNode = 0;
    Motion_MoveForward(MOTION_HIGH_SPEED);
    while (1) {
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        if (!ptr[1] && !ptr[2] && !isBetweenNode) {
            // 如果这时两边首次没有示数，即首次进入无节点的区域
            isBetweenNode = 1;
            Motion_CurrentNodeUpdate();
            --num;
            // 剩余结点数减一
        } else {
            isBetweenNode = 0;
        }
        Motion_CorrectWhenMovingAtY();
        if (!num) {
            // 即将到达目的地
            Motion_MoveForward(MOTION_LOW_SPEED);
            HAL_Delay(500);
            Motion_MoveForward(0);
            Motion_CorrectAtCross();
        }
    }
    /*TODO
    Check if it works*/
}

void Motion_MoveBackwardStable(uint8_t num) {
    /*TODO*/
}

void Motion_MoveLeftStableInPickingArea(uint8_t num) {
    /*TODO*/
}

void Motion_MoveRightStableInPickingArea(uint8_t num) {
    /*TODO*/
}

void Motion_MoveForwardCrossing(uint8_t num) {
    /*TODO*/
}

void Motion_MoveBackwardCrossing(uint8_t num) {
    /*TODO*/
}

void Motion_CorrectWhenMovingAtY(void) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    uint8_t FrontBegin, FrontEnd, BackBegin, BackEnd;
    // 得到前后两边上的点位
    Position_GetOneActive(ptr[0], 9, &FrontBegin, &FrontEnd);
    Position_GetOneActive(ptr[3], 9, &BackBegin, &BackEnd);
    if (FrontBegin == 0 && BackEnd == 9 - 1) {
        // 这一刻，它重叠了
        return;
    }
    uint8_t Front = (FrontBegin + FrontEnd) >> 1;
    uint8_t Back = (BackBegin + BackEnd) >> 1;
    // 反馈调节
    // static PID_InformationTypeDef pid;
    // PID_InformationInit(&pid);
    // pid.Kp = -60;
    // pid.Ki = -18;
    // PID_Calculate_Locational_CounterOverflow(&pid, (int16_t)((int8_t)Front - (int8_t)Back), -400, 400);
    Motor_SetW((int16_t)((int8_t)Front - (int8_t)Back) * (-50));
    // Motor_SetW((int16_t)pid.Output);
    Motor_SetX((int16_t)((int8_t)Front + (int8_t)Back - 8) * (3));
    // HAL_Delay(100);
    // Motor_SetW(0);
     

    /*TODO*/
}

void Motion_CorrectWhenMovingAtX(void) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    uint8_t LeftBegin, LeftEnd, RightBegin, RightEnd;
    // 得到左右的点位
    Position_GetOneActive(ptr[1], 11, &LeftBegin, &LeftEnd);
    Position_GetOneActive(ptr[2], 11, &RightBegin, &RightEnd);

    if (LeftBegin == 0 && LeftEnd == 11 - 1) {
        // 这一刻，它重叠了
        return;
    }
    uint8_t Left = (LeftEnd + LeftBegin) >> 1;
    uint8_t Right = (RightEnd + RightBegin) >> 1;
    // 反馈调节
    int16_t Front = (((int16_t)((int8_t)Left + (int8_t)Right) - 10) * (-2));
    Motor_SetY(Front);
    static int cht = 0;
    ++cht;
    if (cht == 10) {
        // printf("sssssssss%d, %d\r\n", Left, Right);
        cht = 0;
    }
    // if (Front > 0) {
    // }
    // printf("TTT%d\r\n", Front);
    Motor_SetW(((int16_t)((int8_t)Left - (int8_t)Right)) * (7));
}

void Motion_CorrectAtCross(void) {
    TraceInfo_t *ptr;

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
            (Begins[0] + Ends[0]) >> 1,
            (Begins[1] + Ends[1]) >> 1,
            (Begins[2] + Ends[2]) >> 1,
            (Begins[3] + Ends[3]) >> 1
        };

        // 得到中心点
        int16_t CenterX = (Middle[0] + Middle[3]) >> 1;
        int16_t CenterY = (Middle[1] + Middle[2]) >> 1;

        // 修正之
        Motor_SetW((int16_t)((int16_t)Middle[0] - (int16_t)Middle[3]) * (20));
        Motor_SetX((int16_t)((CenterX - 4) * (-20)));
        Motor_SetY((int16_t)(CenterY - 5) * (-20));
        HAL_Delay(50);
    }
    /*TODO*/
}

void Motion_CorrectInPickingArea(void) {
    TraceInfo_t *ptr;
    uint8_t Begins[4], Ends[4];
    /*TODO*/
    for (int i = 0; i < 5; ++i) {
        Motion_CorrectWhenMovingAtX();
        HAL_Delay(50);
    }
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

void Motion_CorrectWhenThrowing(void) {
    /*TODO*/
}

uint8_t Motion_PickUpBallForward(void) {
    /*TODO*/
}

uint8_t Motion_PickUpBallBackward(void) {
    /*TODO*/
}

