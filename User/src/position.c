/**
 * @file position.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 获得当前位置的算法
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "position.h"

node_t CurrentNode = Node_0;
TraceInfo_t CurrentInfo[4];

void Position_SetProperVelocity() {
    Sensor_GetCurrentInfo(CurrentInfo);
    if (CurrentNode <= Node_7) {
        /**
         * @brief 可以认为这时候，四条边上分别最多一个点。
         * 否则，证明车身已经大幅度偏转。
         * 在大幅度偏转前，车身会经历上述阶段。
         * 那么，便会在之前得到矫正。
         * 
         */
        // 每边上端点记录
        uint8_t cur[8];
        // 四边边上点数合计
        uint8_t DotCnt = 0;
        for (uint8_t i = 0; i < 4; i++)
            DotCnt += Position_GetOneActive(CurrentInfo[i], &cur[i << 1], &cur[(i << 1) + 1]);
        if (DotCnt == 4) {
            // 十字式
            /*TODO*/
        } else if (DotCnt == 2) {
            // 一字式
            /*TODO*/
        } else {
            // 其他形式
            /*TODO*/
        }
        return;
    }
}

/**
 * @brief 得到一条边上的唯一一个黑点的两端坐标
 *      复杂度O(n)
 * 
 * @param line 边信息
 * @param lowerbound 低端坐标
 * @param upperbound 高端坐标
 * @return uint8_t 找到的点数，取值0或1
 */
uint8_t Position_GetOneActive(TraceInfo_t line, uint8_t *lowerbound, uint8_t *upperbound) {
    uint8_t i, j;
    for (i = 0, j = 9; i <= j && (!IsActive(line, i) || !IsActive(line, j));) {
        if (!IsActive(line, i)) ++i;
        if (!IsActive(line, j)) ++j;
    }
    *lowerbound = i;
    *upperbound = j;
    return i <= j;
}
