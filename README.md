<p align="center">
 <img width="300px" src="https://z3.ax1x.com/2021/07/20/Wt9wW9.jpg" align="center" alt="GitHub Readme Stats" />
 <!-- <h2 align="center">这里写你的标题</h2> -->
 <p align="center">红专并进，理实交融</p>
<p/>

# Team epsilonGoal 电控设计

## 前言

本机器人控制程序按照2021年中国科学技术大学Robo Game机器人给定任务设计。本程序的开发和使用旨在学习机器人控制系统、电机控制理论和常用传感器的使用，仅供学习和娱乐用途使用。

本程序中可能使用部分公开API或他人自行设计的接口代码，一般情况下有所标注。如有侵权，联系本人即可。

由于个人精力有限，代码的注释和实际逻辑可能出现不匹配的情况。事实上，在二审之后编辑的代码均不配备详细的注释。主要原因是二审后开学，并不具备充足的时间将注释一一补全。同时，本电控设计技术文档中的示例代码也可能和实际使用代码出现不匹配的情况。如果你遇到了上述情形，推荐自行理解。读懂他人的代码是一件考验个人能力的事。

## 介绍

### 比赛要求

本场比赛要求，制作一个具备平面位移功能的机器人，设计合理的机械结构，使之能够在给定的场地上完成任务，包括根据黑线网格进行位移、通过三种方法之一识别球的类别、拾取篮球、发射篮球到篮框中。

### 主控平台

根据预先粗略评估，本次比赛中我组使用STM32F103ZET6为主控平台。后期发现使用F407可以提高工作效率，降低消耗成本。但是已经无法修改，作罢。

### 辅助控制板

由于前期对控制板的外设数量做出了错误估计，中期我组采取了一块STM32F103VET6辅助控制板。

### 综合传感器

本次比赛中，主要传感器不多，包括底盘的红外对管阵列、四个VL53L0X激光测距模块和RFID读取模块。事实上最终采用的模块也只有红外对管阵列，其他模块因为时间问题未能成功调试并投入使用。

### 电机、舵机控制系统

电机由直流无刷电机和直流有刷电机组成。直流无刷电机存在两种驱动方案，分别是利用简单驱动板进行PWM调速，和利用复杂隔离驱动板手动进行换向调速。直流有刷电机相对简单，使用一款质量足够的驱动模块即可。由于后期隔离驱动板未能成功隔离造成自己损坏，我组成员一夜之间将辅助控制板的代码重新改写，线路重新设计，完成了这一切换的过程。

舵机控制则选择了大众化的PCA9685模块，操作简单，效果优良。具体资料可以上网查找。

### 视觉识别

视觉识别不是本人操作，该程序中只需要发送操作指令即可。

## 代码实现流程和全部代码展示

### 启动流程

程序启动于`main.c`，其中调用了位于`main_.c`中的`Main_()`函数，作为程序的主体。在调试的时候，也有时调用`debug.c`中的`Debug_Main()`函数进行调试。在卸载`FreeRTOS`后，`Debug_Main()`函数被弃置，其中的函数内容也变成了中断回调函数恢复其原有功能。

### 代码框架

在`\User`目录下，我们可以看到如下文件

```text
inc
src
VL53L0X
createfile.py
img.png
```

我们只讨论`inc, src, VL53L0X`中的文件。

事实上，程序中只有VL53L0X的API是得到独立目录待遇的。其他API大部分被我改写后直接丢进了`inc, src`之中。

`src`中有如下文件

```text
arm_ctrl.c
callback.c
cannon.c
can_.c
com.c
debug.c
delay.c
detect.c
error.c
flag.c
foc.c
iso15693.c
main_.c
motion.c
motor.c
motor_ctrl.c
motor_feedback.c
nrf.c
pid.c
pn5180.c
position.c
procedure.c
pushrod.c
pwm_generate.c
ranging.c
rfid.c
sensor.c
steer_ctrl.c
trace.c
```

`inc`目录下有相应的头文件，不再赘述。

### 空文件

空文件是由于前期规划和后期实际开发进度不一致造成的。主要包括`can_.c, detect.c, error.c, flag.c, foc.c, nrf.c, ranging.c, trace.c`文件。

### main_.c

该文件负责主要运动流程的调用。

文件中定义了`FrequentlyCalledUpdate()`函数，用于定时调用任务的调用。该函数调用周期为1ms。

文件中定义了`Main_()`函数，主要运动流程就在这里展现出来。

### procedure.c

该文件负责全局运动过程的记录和封装。

文件中定义了

```c
typedef enum {
    eProcedure_Default = 0u,
    eProcedure_HeadForPickingArea = 1u,
    eProcedure_EnterPickingArea = 2u,
    eProcedure_ExitPickingArea = 3u,
    eProcedure_HeadForThrowingArea = 4u,
    eProcedure_StayInThrowingArea = 5u,
    eProcedure_HeadForPickingAreaSecondly = 6u,
    eProcedure_EnterPickingAreaSecondly = 7u
} Procedure_t;

Procedure_t CurrentProcedure;
```

用于全局过程的调控。

```c
typedef enum {
    isBasketball = 0u,
    isNOTBasketball = 1u,
    wasBasketball = 2u,
    NotDetected = 3u
} BallTypeDef;
```

用于记录识别到的篮球的状态。

```c
node_t ProcedureNodeInitial[];
node_t ProcedureNodeInitialBack[];
node_t ProcedureNodeSubprogress[];
node_t ProcedureNodeSubprogressBack[];
```

用于结点记录。

其中也定义了

```c
void Procedure_Default(void);
void Procedure_HeadForPickingArea(void);
void Procedure_EnterPickingArea(void);
void Procedure_ExitPickingArea(void);
void Procedure_HeadForThrowingArea(void);
void Procedure_StayInThrowingArea(void);
void Procedure_HeadForPickingAreaSecondly(void);
void Procedure_EnterPickingAreaSecondly(void);
```

用于全局流程操作。这些流程的具体实现使用了宏定义单独实现，可以用于不同的运动流程，比如是否越障之类。

在函数的具体实现中，主要调用了`motion.c`中的函数。

### motion.c

该文件实现了主要的运动流程。

文件中定义了

```c
typedef enum {
    Node_0 = 0u,
    Node_1 = 1u,
    Node_2 = 2u,
    Node_3 = 3u,
    Node_4 = 4u,
    Node_5 = 5u,
    Node_6 = 6u,
    Node_7 = 7u,
    Node_8 = 8u,
    Node_9 = 9u,
    Node_10 = 10u,
    Node_11 = 11u,
    Node_12 = 12u,
    Node_13 = 13u,
    Node_14 = 14u,
    Node_15 = 15u,
    Node_16 = 16u,
    Node_17 = 17u,
    Node_18 = 18u,
    Node_19 = 19u,
    Node_InValid = 20u
} node_t;

extern node_t CurrentNode;
```

负责当前结点记录。

有函数

```c
void Motion_CurrentNodeUpdate(void);
void Motion_CurrentNodeDecreaseUpdate(void);
```

用于当前节点的更新和下降更新。

该文件中还定义了函数

```c
void __STATIC_INLINE Motion_MoveForward(MotorInput_t speed);
void __STATIC_INLINE Motion_MoveBackward(MotorInput_t speed);
void __STATIC_INLINE Motion_MoveToLeft(MotorInput_t speed);
void __STATIC_INLINE Motion_MoveToRight(MotorInput_t speed);
void __STATIC_INLINE Motion_RotateClockwise(MotorInput_t speed);
void __STATIC_INLINE Motion_RotateCounterClockwise(MotorInput_t speed);
```

用于覆盖式设置当前运动情况。其中调用了`Motor_Decode`函数将三轴速度解析成麦克纳姆轮的目标速度。

其中还定义了函数

```c
void Motion_CorrectWhenMovingAtY(void);
void Motion_CorrectWhenMovingAtX(void);
```

用于修正机器人在不同运动状态下因为种种原因产生的轨迹偏移。其中主要调用了底盘的红外对管阵列的数据。

```c
void Motion_MoveLeftStable(uint8_t num);
void Motion_MoveRightStable(uint8_t num);
void Motion_MoveForwardStable(uint8_t num);
void Motion_MoveBackwardStable(uint8_t num);
void Motion_MoveForwardCrossing(uint8_t num);
void Motion_MoveBackwardCrossing(uint8_t num);
void Motion_MoveLeftStableInPickingArea(uint8_t num);
void Motion_MoveRightStableInPickingArea(uint8_t num);
```

这些函数用于稳定移动，其中相比简单的设置速度，增加了反馈调节的方案，可以小幅度修正，较为稳定。

```c
uint8_t Motion_PickUpBallForward(void);
uint8_t Motion_PickUpBallBackward(void);
```

这些函数用于两个捡球的动作，返回值是捡到的球的个数，即0或者1。理论上，由于传感器缺失，返回值总是1。这属于设计失误。

### motor.c

该文件提供了底盘电机的运动解析，主要是参照麦克纳姆轮的特性构造的。文件在明显的位置上规定了小车的种种正方向。

文件中规定了

```c
typedef int32_t MotorInput_t;

typedef struct {
    MotorInput_t x;
    MotorInput_t Kx;
    MotorInput_t y;
    MotorInput_t Ky;
    MotorInput_t w;
    MotorInput_t Kw;
} MotorInputTpyeDef;

extern MotorInputTpyeDef Motor_InputInstance;
```

用于电机速度控制，和电机的三轴运动解析。 `MotorInputTypeDef`中包括三轴的目标速度和运动解析时的系数。事实上为了方便起见，系数都是1。

```c
__STATIC_FORCEINLINE void Motor_SetX(MotorInput_t x);
__STATIC_FORCEINLINE void Motor_SetY(MotorInput_t y);
__STATIC_FORCEINLINE void Motor_SetW(MotorInput_t w);
__STATIC_FORCEINLINE void Motor_AddX(MotorInput_t x);
__STATIC_FORCEINLINE void Motor_AddY(MotorInput_t y);
__STATIC_FORCEINLINE void Motor_AddW(MotorInput_t w);
```

这里是电机三轴运动的API，可以直接设定速度，或者设定速度的增加量。

```c
__STATIC_INLINE void Motor_Init(void);
void Motor_Decode(MotorInput_t x, MotorInput_t y, MotorInput_t w);
```

是电机解算的初始化函数和电机解算的API。在`Motor_Decode()`中写入期望的三轴速度即可。

### motor_ctrl.c

该文件是电机控制的实现区域。其中包括如下内容

```c
typedef enum {
  Motor_A = 0,
  Motor_B = 1,
  Motor_C = 2,
  Motor_D = 3
} MotorOrdinal_t;

typedef enum {
  Motor_CW = 0, Motor_CCW = 1
} MotorDirection_t;

typedef enum {
  Motor_PID_P = 0,
  Motor_PID_I = 1,
  Motor_PID_D = 2
} MotorPIDTypeDef;

typedef int16_t MotorSpeed_t;
```

定义了三个枚举量，用于控制具体的方向和电机编号。`MotorPIDTypeDef`在别的地方有所用到。`MotorSpeed_t`是电机速度的类型，有符号说明电机是可以进行不同方向的运作的。

```c
#define Motor_Encode(Motor, Direction) (uint8_t)((Motor << 1) | Direction)
```

该宏定义用于编码不同电机和方向，相当于Python语言中的元组。

```c
#define Motor_OutputFix ...
#define Motor_FeedbackFix ...
```

宏定义了两个`Fix`函数，主要作用是完成(编码器信号 -> PID 计算 -> 输出电压信号)的转换过程。

```c
__STATIC_INLINE void MotorCtrl_Init(void);
```

初始化函数，调用即可。

```c
__STATIC_FORCEINLINE void MotorCtrl_SetDutyCycle(MotorOrdinal_t Motor, uint16_t DutyCycle);
void MotorCtrl_SetDirection(MotorOrdinal_t Motor, MotorDirection_t direction);
void __STATIC_INLINE MotorCtrl_SetPIDArguments(MotorOrdinal_t motor, MotorPIDTypeDef pid, double val);
double __STATIC_INLINE MotorCtrl_GetPIDArguments(MotorOrdinal_t motor, MotorPIDTypeDef pid);
void __STATIC_INLINE MotorCtrl_PrintArguments(void) {
void __STATIC_INLINE MotorCtrl_SetTarget(MotorSpeed_t speed, uint8_t ord) {
```

十分面向对象思想的Set和Get函数，字面意思理解即可。

```c
void MotorCtrl_UpdateControlFlow(void);
MotorSpeed_t *MotorCtrl_UpdateFeedback(MotorFeedback_InformationTypeDef *info);
```

Update函数，顾名思义，周期调用即可。前者负责控制流的周期更新，后者负责反馈信息的周期更新。后者调用后会返回一个数组表示当前电机的实际速度。函数的具体实现需要理解`motor_feedback.c`文件，接下来会有所介绍。

```c
void __STATIC_INLINE MotorCtrl_CalculateNextOutput(void);
void MotorCtrl_CalculateNextOutputByTargets(PID_InformationTypeDef *PIDs, MotorSpeed_t *target);
```

API，计算下一次的PID或者根据新的target计算下一次的PID。一般地，我们是周期调用前者即可。Target取值取决于一个全局变量。

### motor_feedback.c

`motor_feedback.c / .h`文件，实现了对底盘电机的编码器解密，获得当前的实际速度。

由于前期设计错误，这样的代码会出现突发爆转。具体原因是本程序中采取的计数方式是通过TIM输入捕获间隔时间来计算速度，而传统的方式是根据一定时间内的输入捕获次数来计算速度。这就导致遗漏输入捕获时造成速度的异常突变。具体逻辑如下。如果想要更好的逻辑，请参考其他队伍的手册。

```c
typedef enum {
  MotorFeedback_CW  = 0,
  MotorFeedback_CCW = 1
} MotorFeedback_Direction_t;
typedef struct {
  MotorFeedback_TimeTick_t TimeTicks[4];
  MotorFeedback_ReloadTime_t ReloadTimes[4];
  MotorFeedback_Direction_t Directions[4];
} MotorFeedback_InformationTypeDef;

extern MotorFeedback_InformationTypeDef Motor_InformationInstance;
```

这里是一些枚举和结构体的定义，主要作用在于提高程序的抽象程度，赋予程序良好的可读性。`MotorFeedback_Direction_t`顾名思义，是反馈信号读取方向的类型。`MotorFeedback_InformationTypeDef`是当前读取信号的一个结构体，在`Motor_InformationInstace`中实例化。

```c
__STATIC_INLINE void MotorFeedback_TIM_PeriodElapsedCallback(void) {
  Motor_InformationInstance.ReloadTimes[0] += 1;
  Motor_InformationInstance.ReloadTimes[1] += 1;
  Motor_InformationInstance.ReloadTimes[2] += 1;
  Motor_InformationInstance.ReloadTimes[3] += 1;
}
```

顾名思义，是TIM的溢出中断的转写。我们在`HAL_TIM_PeriodElapsedCallback()`函数中调用了该函数。溢出中断后要将数据结构中的`ReloadTimes`增加。一般情况下根本达不到这个时刻。

```c
__STATIC_INLINE void MotorFeedback_Init(void) {
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_4);
}
```

开启各种输入捕获通道。

```c
#define Temp_GetTicks(arr)                                \
    do {                                                  \
        int32_t cnt = htim->Instance->CNT;                \
        Motor_InformationInstance.TimeTicks[arr] =        \
            Motor_InformationInstance.ReloadTimes[arr] *  \
                MOTOR_TIM_COUNTER_PERIOD +                \
            ((cnt - MotorFeedback_LastTick[arr] > 0)      \
                 ? (cnt - MotorFeedback_LastTick[arr])    \
                 : ((cnt - MotorFeedback_LastTick[arr]) + \
                    MOTOR_TIM_COUNTER_PERIOD));           \
        Motor_InformationInstance.ReloadTimes[arr] = 0;   \
        MotorFeedback_LastTick[arr]                = cnt; \
    } while (0u)

```

用于根据`ReloadTimes`和`LastTick`计算本次经过的`TimeTicks`。将在之后的程序中转换为速度。

```c
void MotorFeedback_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  static uint16_t MotorFeedback_LastTick[4];
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_1) {
    Motor_InformationInstance.Directions[0] =
      ((Motor_InA_GPIO_Port->IDR & Motor_InA_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    Temp_GetTicks(0);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_2) {
    Motor_InformationInstance.Directions[1] =
      ((Motor_InB_GPIO_Port->IDR & Motor_InB_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    Temp_GetTicks(1);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_3) {
    Motor_InformationInstance.Directions[2] =
      ((Motor_InC_GPIO_Port->IDR & Motor_InC_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    Temp_GetTicks(2);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_4) {
    Motor_InformationInstance.Directions[3] =
      ((Motor_InD_GPIO_Port->IDR & Motor_InD_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    Temp_GetTicks(3);
  }
}
```

输入捕获中断函数，用于计算实际的速度和方向。其中`Temp_GetTicks()`函数在之前有所介绍。

### cannon.c

`cannon.c`中介绍了摩擦轮的使用方法。具体而言，我们将摩擦轮的驱动放在了辅助控制板，原本计划使用如下的数据结构。

```c  
typedef struct {
  uint16_t header;
  int16_t speed;
} Cannon_CommunicationTypeDef;
```

由于种种原因，我们未能使用这样的数据结构。最终，我们仅仅使用了`uint16_t`作为传输的内容。

在传输的内容中，我们做出了如下规定：

- 如果`uint16_t sent > 0`，则将`sent`值赋给摩擦轮速度。
- 如果`uint16_t sent == 0`，则返回底盘的红外对管信息。

具体的控制代码部分如下。

```c  
__STATIC_INLINE void Cannon_SetTargetSpeed(int16_t speed) {
  Cannon_CommunicationTypeDef sent;
  sent.speed = speed;
  sent.header = (uint16_t)speed >> 2;
  // DMA 和阻塞不能混用
  HAL_StatusTypeDef ret = HAL_UART_Transmit(&CANNON_HANDLE, (uint8_t *)&sent.speed, sizeof(sent.speed), 0x00FF);
  if (ret != HAL_OK) {
    printf("Failed to set\r\n");
  } else {
    printf("Set target%d\r\n", speed);
  }
}
```

可以注意到其中的数据结构`Cannon_CommunicationTypeDef`并没有被很好地使用。这属于历史遗留问题。如果你在代码中看到了这样的更改，即代码被改写避免了上述冗余，你可以自行查看commit日志，尝试找出我们在哪一天修复了这个问题。

### com.c

`com.c`实现了和上位机树莓派的通信。按照原计划，流程应当由树莓派调度。由于沟通问题和考虑了实际情况，流程由主控制板提供。因此，上位机树莓派仅仅作为视觉识别的工具使用。我们使用如下的数据结构。

```c  
typedef struct {
    uint8_t header;
    uint8_t info;
} Com_DataTypeDef;
```

为了提高识别效率，简化取球区代码的实现，我们做出如下规定：

- header == info,用于简易的通信校验。
- info 中从低到高存储着六位篮球识别情况。是篮球则返回1，不是则返回0。位置如下

  ```text
    4 2 0
    + + +
    -----
    + + +
    5 3 1
  ```

我们还使用了如下的函数便于调用。

```c  
#define READ(x, n) (!!((x & (1 << n))))

__STATIC_INLINE HAL_StatusTypeDef Com_SendWorkingCommand(void) {
    uint8_t cmd = 0xf0;
    return HAL_UART_Transmit(&COM_HEADER, &cmd, 1, 0x00ff);
}

__STATIC_INLINE HAL_StatusTypeDef Com_Receive(Com_DataTypeDef *cmd) {
    HAL_StatusTypeDef ret;
    cmd->info = 0;
    ret = HAL_UART_Receive(&COM_HEADER, (uint8_t*)cmd, sizeof(Com_DataTypeDef), 0x0FFF);
    printf("Tried to receive, and\r\n");
    if (ret != HAL_OK) {
        return HAL_ERROR;
    }

    if (cmd->info != cmd->header)
        return HAL_ERROR;

    printf("Received Information %d\r\n", cmd->info);
    printf("%d, %d, %d\r\n", READ(cmd->info, 4), READ(cmd->info, 2), READ(cmd->info, 0));
    printf("%d, %d, %d\r\n", READ(cmd->info, 5), READ(cmd->info, 3), READ(cmd->info, 1));
    HAL_Delay(100);

    return ret;
}

__STATIC_INLINE Com_DataTypeDef *Com_WorkAndReceive(void) {
    static Com_DataTypeDef info;
    info.info = 0;
    Com_SendWorkingCommand();
    printf("Command sent\r\n");
    if (HAL_OK == Com_Receive(&info)) {
        printf("Verified ok\r\n");
    } else {
        printf("Failed to receive new information\r\n");
        info.info = 0;
        info.header = 0;
    }
    return &info;
}
```

使用的时候，既可以采取`Com_SendWorkingCommand()` + `Com_Receive(&info)`的组合方式，也可以直接使用`Com_WorkAndReceive()`函数获得。

### debug.c

`debug.c`中实现了我们的大部分debug函数和操作。debug的需要，是各种参数的调定、动作的执行和调试信息的输出。参数调定的可视化和条理化需要较高的编程技巧，我们没有实现。而动作的执行和调试信息的输出，我们则做到了良好的、友好的实现方式。

在进行具体讨论之前，我们想先提及如下函数。

```c  
void Debug_BugCatcher(HAL_StatusTypeDef status) {
  /*
   *  TODO: 根据返回值判断是否成功，并记录
   */
  return;
}
```

这是一个我们没有实现的函数。通常意义下，这个函数应当在各种函数返回值中使用，用于判断函数是否有被正确地执行。一般地，由于种种特异化请求，该函数仅仅用于debug模块中，还是比较合适的。但是，由于debug模式一旦出错，可能造成无法输出调试信息。所以便没有什么用处，除非添加其他的错误提示，如信号灯和蜂鸣器。本次实验中，这个函数没有起到任何作用，也没有进行函数实现。

debug的串口需要一定的初始化。

```c  
void Debug_Init(void) {
  uint8_t welcome[] = "Program running well.\r\nLoading debug function...\r\n";

  HAL_UART_Transmit(&Debug_Handle, welcome, sizeof(welcome), 0xFFFF);

  // 参考https://www.cnblogs.com/pacexdong/p/12118193.html
  // https://blog.csdn.net/as480133937/article/details/105013368
  // 可以实现任意长接受信号
  __HAL_UART_ENABLE_IT(&Debug_Handle, UART_IT_IDLE);
  Debug_BugCatcher(HAL_UART_Receive_DMA(
    &Debug_Handle, Debug_USART_CommandBuffer, DEBUG_USART_BUFFER_SIZE));
}
```

其中使用了串口的IDLE中断实现了变长信号的接收，便于调试信号的解析。

```c  
void Debug_CommandHandler(uint8_t *str) {
  printf("Received\r\n");
  int n = 0;
  int ret = 0;
  Com_DataTypeDef info;
  switch (str[0]) {
    case 'P':
      /*    TODO      */
      Debug_PrintHandler(str);
      break;
    case 'S':
      Debug_SetArgumentHandler(str);
      break;
    case 'M':
      Debug_MotionHandler(str);
      break;
    case 'L':
      Motor_Decode(0, 0, 0);
      Motor_X = Motor_Y = Motor_W = 0;
      break;
    case 'Z':
      sscanf((char *)str, "Z%d\r\n", &n);
      Cannon_SetTargetSpeed(n);
      break;
    case 'D':
      Debug_MotionControlHandler(str);
      break;
    case 'W':
      Com_SendWorkingCommand();
      printf("Command sent\r\n");
      if (HAL_OK == Com_Receive(&info)) {
        printf("Verified ok\r\n");
      } else {
        printf("Failed to receive new information\r\n");
      }
      break;
    case 'K':
      sscanf((char *)str, "K%d,%d\r\n", &n, &ret);
      Steer_SetAngleByDegree(n, ret);
      break;
    case 'X':
      printf("X: List all the commands\r\n");
      printf("L: Stop the engine, not preventing correcting\r\n");
      printf("W: Send working command to Pi\r\n");
      printf("K: Set steer angles\r\n");
      printf("D: Set motions\r\n");
      printf("Z: Set cannon speed\r\n");
      printf("M: Set some motors, but not useful always\r\n");
      printf("P, S: Not used anylonger\r\n");
      break;

    default:
      /*TODO 错误处理*/
      printf("Not supported command\r\n");
      break;
  }
}
```

我们实现的调试指令如上图所示。具体含义可以阅读代码，可以说是通俗易懂。其中的辅助函数如下。

```c  
/**
 * @brief 打印指令的控制
 *    TODO
 * 
 *      遇到浮点数可以使用std库的sscanf方法哦
 *      TODO
 *      P{控制单元}{可选编号}{参数，如P/I/D}
 * 
 * @param str 命令的字符串
 * @return None 
 */
__STATIC_INLINE void Debug_PrintHandler(uint8_t *str) {
  /*TODO:COMPLETION*/
  uint8_t cmd, unit, ord, arg;
  double val;
  switch(str[1]) {
    case 'M':
      sscanf((char *)str, "%c%c%c%c", &cmd, &unit, &ord, &arg);
      // 设置成为枚举值
      ord -= 'A';
      arg = (arg == 'P') ? (Motor_PID_P)
                         : ((arg == 'I') ? (Motor_PID_I) : (Motor_PID_D));
      // 设置
      val = MotorCtrl_GetPIDArguments((MotorOrdinal_t)ord, (MotorPIDTypeDef)arg);
      printf("%lf\r\n", val);
      break;
    default:
      /*TODO*/
      break;
  }
}

/**
 * @brief 设置函数的控制，主要用于PID调试
 *      遇到浮点数可以使用std库的sscanf方法哦
 *      TODO
 *      S{控制单元}{可选编号}{参数，如P/I/D}{值}
 *      SMAP0.5   设置Motor A 的 P 为 0.5
 * 
 * @param str 
 * @return None 
 */
__STATIC_INLINE void Debug_SetArgumentHandler(uint8_t *str) {
  /*TODO:COMPLETION*/
  uint8_t cmd, unit, ord, arg;
  double val;
  switch(str[1]) {
    case 'M':
      sscanf((char *)str, "%c%c%c%c%lf", &cmd, &unit, &ord, &arg, &val);
      // 设置成为枚举值
      ord -= 'A';
      arg = (arg == 'P') ? (Motor_PID_P)
                         : ((arg == 'I') ? (Motor_PID_I) : (Motor_PID_D));
      // 设置
      MotorCtrl_SetPIDArguments((MotorOrdinal_t)ord, (MotorPIDTypeDef)arg, val);
      break;
    default:
      /*TODO*/
      break;
  }
}


/**
 * @brief 运动控制的信息处理
 * 
 *      格式：M{控制单元}{可选编号}{控制要求}
 *  
 *       控制单元：P推杆，M电机，G摩擦轮，S舵机。
 *       MPU     推杆前进
 *       MPB     推杆后退
 *       MMAU    电机 A 提速
 *       MMBB    电机 B 降速
 *      
 *      警告：使用时请关闭Motor的PID更新！
 *  TODO：Else?
 * @param str 
 * @return None 
 */
__STATIC_INLINE void Debug_MotionHandler(uint8_t *str) {
  static uint16_t TemporaryMotorCompare[] = {500, 500, 500, 500, 300};
  switch (str[1]) {
    case 'P':
      if (str[2] == 'U') {
        Pushrod_MoveForward(65000);
      } else if (str[2] == 'B') {
        Pushrod_MoveBackward(65000 * 2);
      }
      break;
    case 'M':
      switch (str[2]) {
        case 'A':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                                  TemporaryMotorCompare[0] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                                  TemporaryMotorCompare[0] -= 50);
          }
          printf("A%d\r\n", TemporaryMotorCompare[0]);
          break;
        case 'B':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,
                                  TemporaryMotorCompare[1] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,
                                  TemporaryMotorCompare[1] -= 50);
          }
          printf("B%d\r\n", TemporaryMotorCompare[1]);
          break;
        case 'C':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,
                                  TemporaryMotorCompare[2] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,
                                  TemporaryMotorCompare[2] -= 50);
          }
          printf("C%d\r\n", TemporaryMotorCompare[2]);
          break;
        case 'D':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,
                                  TemporaryMotorCompare[3] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,
                                  TemporaryMotorCompare[3] -= 50);
          }
          printf("D%d\r\n", TemporaryMotorCompare[3]);
          break;
        case 'M':
          if (str[3] == 'U') {
            TemporaryMotorCompare[4] += 50;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, TemporaryMotorCompare[4]);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, TemporaryMotorCompare[4]);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, TemporaryMotorCompare[4]);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, TemporaryMotorCompare[4]);
          } else {
            TemporaryMotorCompare[4] -= 50;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, TemporaryMotorCompare[4]);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, TemporaryMotorCompare[4]);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, TemporaryMotorCompare[4]);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, TemporaryMotorCompare[4]);
          }
          printf("M%d\r\n", TemporaryMotorCompare[4]);
        default:
          /*TODO*/
          break;
      }
      break;
    case 'G':
      /*TODO*/
      break;
    case 'S':
      /*TODO*/
      break;
    default:
      /*TODO错误处理*/
      break;
  }
}

void Debug_MotionControlHandler(uint8_t * str) {
  sscanf((char *)str, "D%hd,%hd,%hd\r\n", &Motor_X, &Motor_Y, &Motor_W);
  Motor_Decode(Motor_X, Motor_Y, Motor_W);
  printf("D Set To: %d,%d,%d\r\n", Motor_X, Motor_Y, Motor_W);
}
```

这里的实现也不是复杂的事情，根据注释可以简单理解。值得注意的是，`MM`类的指令只有在关闭PID更新后可以使用。这里也只是在开始时调试使用了，后期完全没有任何作用。有相似作用的便是`P`和`S`指令。我们完全没有使用它们，仅仅是实现了而已。

### position.c

这组代码中规定了底盘的`TraceInfo_t`和具体编码形式。值得注意的是，由于上面板安装方向问题，我们在操作时实际方向是偏转了180度的。不过这一点对于代码的读者没有什么影响。

文件中还有值得注意的代码是`Position_GetOneActive()`函数，作用是当一条边上只有一个黑点时，我们通过两边查找的方式将它的两端点找出来。其中提供了`mask`的功能，因为实施情况中边缘位置的红外对管往往失去了一定的灵敏度，变得不可信。

``` c
#define IsActive(info, cur) ((info & (1 << cur)) ? 1 : 0)

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
```

### pushrod.c

```c  
```

```c  
```

```c  
```

```c  
```

```c  
```

```c  
```

```c  
```

```c  
```

### steer_ctrl.c

### delay.c

### pid.c

### pwm_generate.c

### callback.c

### iso15693.c

### pn5180.c

### sensor.c

### arm_ctrl.c

### VL53L0X系列

## 重点函数实现介绍

### 主控逻辑和流程

### 电机使用

### 摩擦轮调用

### 多方通讯

### 舵机使用

### 底盘循迹、运动和计数

### 步进电机使用

## 出现的一些问题

### 电路板地干扰

### 接线震动接触不良

### 程序设计内存泄漏

### 通信中断，外设异常

ORE，接线，波特率

### 未能正常使用模块

### 麦克纳姆轮方向错误

### 未能做到短路保护

电路板反面，和接线裸漏部分

### 接线造成异常重启和无法烧录

### PID逻辑误差较大

### 电机力矩不足

### 底盘计数错误，误差较大

### 备份零件不足，时常停滞

### 机械结构设计缺陷

### 电路构建和测试的顺序不当

## 感想
