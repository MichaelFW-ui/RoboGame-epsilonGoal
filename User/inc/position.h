/**
 * @file position.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 红外对管typedef
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __POSITION_H
#define __POSITION_H

#include "stm32f1xx_hal.h"


typedef uint16_t TraceInfo_t;

/**
 *            0
 *         +—————————+
 *         |         |
 *      1  |         |  2
 *         |         |
 *         |         |
 *         +—————————+
 *              3
 * 
 * 编号参考坐标轴！！！！
 */



#define IsActive(info, cur) ((info & (1 << cur)) ? 1 : 0)
#define GetLeastActiveMask(info) (info & (~(info - 1)))
// 示例
// info                 111011101011010111101111110000
// info - 1             111011101011010111101111101111
// ~(info-1)            000100010100101000010000010000
// info&(~(info-1))     000000000000000000000000010000

#define ClearLeastActiveMask(info) (info & (info - 1))

uint8_t Position_GetOneActive(TraceInfo_t line, uint8_t len,
                              uint8_t *lowerbound, uint8_t *upperbound);

#endif // !__POSITION_H

