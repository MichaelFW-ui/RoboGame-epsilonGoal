/**
 * @file procedure.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 全局过程，进行当前过程
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include "motion.h"



typedef enum {
    eProcedure_Default = 0u,
    eProcedure_HeadForPickingArea = 1u,
    eProcedure_EnterPickingArea = 2u,
    eProcedure_ExitPickingArea = 3u,
    eProcedure_HeadForThrowingArea = 4u,
    eProcedure_StayInThrowingArea = 5u,
    eProcedure_HeadForPickingAreaSecondly = 6u
} Procedure_t;


typedef enum {
    isBasketball = 0u,
    isNOTBasketball = 1u,
    wasBasketball = 2u,
    NotDetected = 3u
} BallTypeDef;
extern Procedure_t CurrentProcedure;

extern node_t ProcedureNodeInitial[];
extern node_t ProcedureNodeInitialBack[];

extern node_t ProcedureNodeSubprogress[];
extern node_t ProcedureNodeSubprogressBack[];

void Procedure_NewNodeUpdate(void);



/*            Procedures during the competition.                              */

void Procedure_Default(void);

void Procedure_HeadForPickingArea(void);

void Precedure_EnterPickingArea(void);

void Precedure_ExitPickingArea(void);

void Precedure_HeadForThrowingArea(void);

void Precedure_StayInThrowingArea(void);

void Precedure_HeadForPickingAreaSecondly(void);


/*            Procedures during the competition end here.                     */

#endif //  __PROCEDURE_H
