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
#include "position.h"

Procedure_t CurrentProcedure = eProcedure_Default;

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
node_t ProcedureNodeInitialBack[] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};

node_t ProcedureNodeSubprogress[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
node_t ProcedureNodeSubprogressBack[] = {15, 14, 13, 12, 11, 10,
                                         9,  8,  7,  6,  5,  4};

#endif // CROSSOVER_OVERALL


#ifdef CROSSOVER

node_t ProcedureNodeInitial[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
// 进入取球区后轨迹不确定
node_t ProcedureNodeInitialBack[] = {8, 7, 6, 5, 4};

node_t ProcedureNodeSubprogress[] = {5, 6, 7, 8};
// 进入取球区后轨迹不确定
node_t ProcedureNodeSubprogressBack[] = {8, 7, 6, 5, 4};

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

void Procedure_Default(void) {
    /*TODO*/
}

void Procedure_HeadForPickingArea(void) {
    /*TODO*/
}

void Precedure_EnterPickingArea(void) {
    /*TODO*/
}

void Precedure_ExitPickingArea(void) {
    /*TODO*/
}

void Precedure_HeadForThrowingArea(void) {
    /*TODO*/
}

void Precedure_StayInThrowingArea(void) {
    /*TODO*/
}

void Precedure_HeadForPickingAreaSecondly(void) {
    /*TODO*/
}

#endif // CROSSOVER_OVERALL

/*             CrossOver Implementation                                       */
#ifdef CROSSOVER

void Procedure_Default(void) {
    /*TODO*/
}

void Procedure_HeadForPickingArea(void) {
    /*TODO*/
}

void Precedure_EnterPickingArea(void) {
    /*TODO*/
}

void Precedure_ExitPickingArea(void) {
    /*TODO*/
}

void Precedure_HeadForThrowingArea(void) {
    /*TODO*/
}

void Precedure_StayInThrowingArea(void) {
    /*TODO*/
}

void Precedure_HeadForPickingAreaSecondly(void) {
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

void Precedure_EnterPickingArea(void) {
    /*TODO*/
}

void Precedure_ExitPickingArea(void) {
    /*TODO*/
}

void Precedure_HeadForThrowingArea(void) {
    /*TODO*/
}

void Precedure_StayInThrowingArea(void) {
    /*TODO*/
}

void Precedure_HeadForPickingAreaSecondly(void) {
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

void Precedure_EnterPickingArea(void) {
    /*TODO*/
}

void Precedure_ExitPickingArea(void) {
    /*TODO*/
}

void Precedure_HeadForThrowingArea(void) {
    /*TODO*/
}

void Precedure_StayInThrowingArea(void) {
    /*TODO*/
}

void Precedure_HeadForPickingAreaSecondly(void) {
    /*TODO*/
}

#endif // BARERUN 
