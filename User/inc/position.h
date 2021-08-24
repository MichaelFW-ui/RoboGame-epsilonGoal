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
 *       +—————————+
 *       |         |
 *    1  |         |  2
 *       |         |
 *       |         |
 *       +—————————+
 *            3
 */



#define IsActive(info, cur) ((info & (1 << cur)) ? 1 : 0)
#define GetLestActiveMask(info) (info & (~(info - 1)))
// 示例
// info                 111011101011010111101111110000
// info - 1             111011101011010111101111101111
// ~(info-1)            000100010100101000010000010000
// info&(~(info-1))     000000000000000000000000010000

#define ClearLestActiveMask(info) (info & (info - 1))


uint8_t Position_GetOneActive(TraceInfo_t line, uint8_t len, uint8_t *lowerbound, uint8_t *upperbound);


// 以下为组委会例程。我不使用。
// u8 send_position(u8 (*data)[8]) {
//     //参数data为一个8x8的数组，为160个红外对管读取到的模拟值
//     u8 Position[4] = {0xff, 0xff, 0xff, 0xff};
//     int i          = 0, j, k;
//     int length;
//     int count0;        // count0用来对连续检测到黑线的红外对管进行计数
//     // count1用于对检测到的黑线个数进行计数
//     int count1 = 0, sum = 0, sum_weight;
//     //先查找到一个白色区域，再从白色区域开始计数。
//     //如果从黑线开始计数会把黑线“截断”导致错误
//     while (data[i / 8][i % 8] > THRESHOLDVALUE)
//         i++;        //从下标为i的红外对管开始计数
//     for (j = 0; j < 160; i++) {
//         //找到黑线，开始计数
//         if (data[(i % 160) / 8][(i % 160) % 8] < THRESHOLDVALUE) {
//             count0++;
//         }
//         //离开黑线，停止计数，开始计算位置
//         if (count0 > 0 &&
//             data[(i % 160) / 8][(i % 160) % 8] >= THRESHOLDVALUE) {
//             if (count0 < 2 || count0 > 9) {
//                 //小于2表示对管损坏，大于9表示一排红外对管全在黑线
//                 上 count0 = 0;
//             } else {
//                 //找到黑线阈值内的几个红外对管后，扩大范围，向前向后各取两个对管
//                 ////加权平均
//                 for (k = i - count0 - 2; k < i + 2; k++) {
//                     ////把256-data作为权重不一定合理，请自行调整
//                     sum +=
//                         k * (256 - data[((k + 160) % 160) / 8][(k % 160) % 8]);
//                     sum_weight +=
//                         (256 - data[((k + 160) % 160) / 8][(k % 160) % 8]);
//                 }
//                 sum /= sum_weight;
//                 sum = sum % 160;
//                 //正常十字路口只会找到4条以内黑线
//                 //本届比赛取球区黑线较多，请自行修改
//                 if (count1 < 4) Position[count1] = sum;
//                 count1++;
//                 sum        = 0;
//                 sum_weight = 0;
//                 count0     = 0;
//             }
//         }
//         j++;
//     }
//     uart2_send_mydata(Position, 4);
//     return 0;
// }

#endif // !__POSITION_H

