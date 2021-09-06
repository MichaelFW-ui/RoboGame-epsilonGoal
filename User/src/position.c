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


// void Position_SetProperVelocity() {
//     Sensor_GetCurrentInfo(CurrentInfo);
//     if (CurrentNode <= Node_7) {
//         /**
//          * @brief 可以认为这时候，四条边上分别最多一个点。
//          * 否则，证明车身已经大幅度偏转。
//          * 在大幅度偏转前，车身会经历上述阶段。
//          * 那么，便会在之前得到矫正。
//          * 
//          */
//         // 每边上端点记录
//         uint8_t cur[8];
//         // 四边边上点数合计
//         uint8_t DotCnt = 0;
//         for (uint8_t i = 0; i < 4; i++)
//             DotCnt += Position_GetOneActive(CurrentInfo[i], &cur[i << 1], &cur[(i << 1) + 1]);
//         if (DotCnt == 4) {
//             // 十字式
//             /*TODO*/
//         } else if (DotCnt == 2) {
//             // 一字式
//             // 
//             /*TODO*/
//         } else {
//             // 其他形式
//             /*TODO*/
//         }
//         return;
//     }
// }

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
    // for (i = 0, j = len - 1;
    //      i <= j && (!IsActive(line, i) || !IsActive(line, j));) {
    //     if (!IsActive(line, i)) ++i;
    //     if (!IsActive(line, j)) --j;
    // }
    // uint8_t cnt = 0;


    // uint8_t i;
    // static uint8_t last = 0;
    // uint8_t lastLength = 0;
    // uint8_t beg, end;

    // line <<= 1;
    // for (i = 0; i < len; ++i) {
    //     if (IsActive(line, i + 1) && !IsActive(line, i)) {
    //         // ++cnt;
    //         beg = i;
    //     } else if (!IsActive(line, i + 1) && IsActive(line, i + 2)) {
    //         end = i;
    //         if ((((beg + end) >> 1) - last) <= lastLength ||
    //             (last - ((beg + end) >> 1)) <= lastLength) {
    //             *lowerbound = beg;
    //             *upperbound = end;
    //             lastLength =
    //                 (((beg + end) >> 1) > last ? ((beg + end) >> 1) - last
    //                                            : last - ((beg + end) >> 1));
    //             last = (beg + end) >> 1;
    //         }
    //     }
    // }
    // return *lowerbound < *upperbound;

    uint16_t mask = 0x1f << (((len - 1) >> 1) - 2);
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
