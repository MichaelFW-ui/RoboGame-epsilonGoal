# 可能的电控规划

| category | 文件             | 功能                     | 工程前缀                       |
| -------- | ---------------- | ------------------------ | ------------------------------ |
| 照搬的   | \*.*             | 从各个地方直接copy过来的 | 无                             |
| 底层     | motor_ctrl.*     | 底盘电机基本驱动         | MotorCtrl & MOTOR_CTRL         |
| 底层     | motor_feedback.* | 底盘电机反馈编码器       | MotorFeedback & MOTOR_FEEDBACK |
| 底层     | trace.*          | 底盘红外对管信号         | Trace & TRACE                  |
| 底层     | pushrod.*        | 推杆驱动信号             | Pushrod & PUSHROD              |
| 底层     | steer_ctrl.*     | 所有舵机驱动信号         | SteerCtrl & STEER_CTRL         |
| 底层     | can_.*           | CAN 通信逻辑             | CAN & CAN                      |
| 底层     | foc.*            | FOC 调用方法             | FOC & FOC                      |
| 底层     | pid.*            | 通用的PID使用库，手写    | PID & PID                      |
| 底层     | callback.*       | 适配HAL库的Callback函数  | 无                             |
| 底层     | cannon.*         | 摩擦轮电机驱动信号       | Cannon & CANNON                |
| 底层     | pwm_generate.*   | PWM，PCA9650模块使用     | PWM & PWM                      |
| 底层     | delay.*          | 延时函数，慎用           | delay                          |
| 底层     | iso15693.*       | 来源GitHub的协议代码     | ISO15693                       |
| 底层     | pn5180.*         | 来源GitHub的协议代码     | PN5180                         |
| 通用层   | flag.*           | 系统flag,可能弃用        | Flag & FLAG                    |
| 通用层   | debug.*          | 调试信息输出             | Debug & DEBUG                  |
| 通用层   | com.*            | 上位机通信               | Communication & COM            |
| 通用层   | detect.*         | 驱动电路上电检测         | Detect & DETECT                |
| 通用层   | rfid.*           | RFID 信号（选配）        | RFID & RFID                    |
| 通用层   | ranging.*        | 激光测距信号             | Ranging & RANGING              |
| 通用层   | position.*       | 当前位置的获取，包括姿态 | Position & POSITION            |
| 通用层   | nrf.*            | NRF 调试遥控             | NRF & NRF                      |
| 通用层   | sensor.*         | 传感器API汇总            | Sensor & SENSOR                |
| 应用层   | motor.*          | 底盘电机操作接口         | Motor & MOTOR                  |
| 应用层   | motion.*         | 移动控制方法             | Motion & MOTION                |
| 应用层   | error.*          | 错误模式处理             | Error & ERROR                  |
| 应用层   | procedure.*      | 全部流程                 | Procedure & PROCEDURE          |
| 应用层   | main_.*          | 启动全部流程             | Main_ & MAIN                   |

注：

- 程序启动流程为 main.c -> main_.c -> procedure.c
- 全局变量或函数中使用小写工程前缀，宏定义中使用大写工程前缀。宏函数使用小写工程前缀。
    示例：

    ```c
    #define Debug_CommandPush(ch) do {                              \
                Debug_CommandLine[Debug_CommandLen++] = ch;         \
            }while(0)
    // 宏函数使用小写前缀
    // 全局变量使用小写前缀

    #define Debug_CommandClean() do { Debug_CommandLen = 0; }while(0)

    #define DEBUG_COMMAND_MAX 30
    // 单指令最长为30字节
    // 宏定义使用大写前缀

    void Debug_CommandHandler() {
        // 函数使用小写前缀
        /*处理数据*/
    }
    ```

    当然，*具体情况*具体分析，写错了也没关系，只是建议。

- 考虑使用面向对象思维。
    例如：

    ```c
    /*
    * File name: example.h
    */
    typedef struct {
        uint8_t identifier;
        uint16_t Forward_GPIO_Pin;
        uint16_t Forward_GPIO_Port;
        uint16_t Backward_GPIO_Pin;
        uint16_t Backward_GPIO_Port;
    } Example_Name_HandleTypeDef;

    /*
    * File name: example.c
    */

    const static unit16_t Forward_GPIO_Pins[EXAMPLE_TOTAL] = {
        /* GPIO Pins*/
        Example_Pin
    };
    

    const static unit16_t Backward_GPIO_Pins[EXAMPLE_TOTAL] = {
        /* GPIO Pins*/
        Example_Pin
    };

    const static unit16_t Forward_GPIO_Ports[EXAMPLE_TOTAL] = {
        /* GPIO Ports*/
        Example_Port
    };

    const static unit16_t Backward_GPIO_Ports[EXAMPLE_TOTAL] = {
        /* GPIO Ports*/
        Example_Port
    };

    Example_Name_HandlerTypeDef NameHandle;

    void Example_Name_Init(*NameHandle, uint8_t identifier) {
        NameHandle->indentifier = identifier;
        NameHandle->Forward_GPIO_Pin = Forward_GPIO_Pins[identifer];
        /* And so on */
    }
    ```

    注意到上文中的全局变量是没有前缀的。失误是允许的，没有人会责备。本篇文章全部都是建议。
- 全部代码使用UTF-8编码。这个不是建议，是强制。Windows用户请注意编码。
- 充分利用寄存器，例如

    ```c
    void Debug_SendByte(uint8_t ch) {
        DebugHandle->Instance->TDR = ch;
        while (!DebugHandle->Instance->SR & USART_ISR_TC)
            ;
    }
    ```

- 使用Doxygen注释格式，例如

    ```c
    /* 
    * @brief Operate the accelerate of motor depending on the command.
    *   Operation time on a 10ms basis.
    * @param data Command that was sent.
    * @retval Void
    */
    void Motor_OnCommandLine(Remote_DataStructure *data) {
        /* 代码 */
        return;
    }
    ```

- 利用TODO 和 FIXME 注释，例如

  ```c
    void MOTOR_TIM_IRQHandler() {
        if (TIM_GetITStatus(MOTOR_TIM, TIM_IT_Update) != RESET) {
            /*
            **      TODO: Waiting for COMPLETION
            **      提醒你自己这里没有写完
            */
            TIM_ClearITPendingBit(MOTOR_TIM, TIM_IT_Update);
        }
    }
  ```

- 利用Debug调试信息输出，例如

  ```c
    #ifdef __DEBUG_PWM

    printf("Motor %d %d\r\n", MotorDirection, speed);

    #else

    Motor_PWM_SetDutyCycle((uint8_t)speed);

    #endif // __DEBUG_PWM
  ```

  其中printf可以将信息输出到USART蓝牙模块，进而输出到你的串口调试终端，例如手机。

- 注意每行代码长度不要超过80字符，可以适当折行，如

    ```c
    veryveryveryveryverylongTypeDef 
    veryveryveryveryverylongFunctionName(
        longTypeDef Longlonglonglongarguments1,
        longTypeDef Longlonglonglongarguments2,
        longTypeDef Longlonglonglongarguments3,
        longTypeDef Longlonglonglongarguments4,
        longTypeDef Longlonglonglongarguments5,
    ) {
        /* 代码 */
    }
    ```

- 使用clang-format调整代码格式，我将提供个人使用的format格式以供参考。

- 数组可以适当调整格式，使之对齐，增加视觉效果，例如

    ```c
    const uint16_t SineValue[] = {
        1943, 2123, 2302, 2478, 2648, 2812, 2967, 3112, 3246, 3368, 3475,
        3568, 3646, 3707, 3750, 3777, 3786, 3777, 3750, 3707, 3646, 3568,
        3475, 3368, 3246, 3112, 2967, 2812, 2648, 2478, 2302, 2123, 1943,
        1762, 1583, 1408, 1237, 1074, 919,  773,  639,  518,  410,  317,
        240,  179,  135,  108,  100,  108,  135,  179,  240,  317,  410,
        518,  639,  773,  919,  1074, 1237, 1408, 1583, 1762
    };
    ```

    这样的对齐可以使用clang-format自动进行。我将提供个人使用的format格式以供参考。

- 不要尝试使用汇编优化速度，你的优化比不上编译器的。

- debug方法：

  - 调试输出中间变量

  - keil debug模式

  - 学长
