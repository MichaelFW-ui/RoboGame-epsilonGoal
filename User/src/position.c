/**
 * @file position.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 红外对管typedef
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "position.h"
#include "sensor.h"


/**
 * @brief 得到一条边上的唯一一个黑点的两端坐标
 *      复杂度O(n)
 * 
 * @param line 边信息
 * @param len  这条边的长度
 * @param lowerbound 低端坐标
 * @param upperbound 高端坐标
 * @return uint8_t 找到的点数，取值0或1
 */
uint8_t Position_GetOneActive(TraceInfo_t line, uint8_t len, uint8_t *lowerbound, uint8_t *upperbound) {
    uint16_t mask = 0x7f << (((len - 1) >> 1) - 3);
    line &= mask;
    uint8_t i, j;

    for (i = 0, j = len - 1;
         i <= j && (!IsActive(line, i) || !IsActive(line, j));) {
        if (!IsActive(line, i)) ++i;
        if (!IsActive(line, j)) --j;
    }
    *lowerbound = i;
    *upperbound = j;

    return *lowerbound < *upperbound;
    
}
