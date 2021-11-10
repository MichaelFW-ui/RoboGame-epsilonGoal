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

`motor_feedback.c/.h`文件，实现了对底盘电机的编码器解密，获得当前的实际速度。

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
  // 以下省略 ...
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
  // ...
}

__STATIC_INLINE HAL_StatusTypeDef Com_Receive(Com_DataTypeDef *cmd) {
  // ...
}

__STATIC_INLINE Com_DataTypeDef * Com_WorkAndReceive(void) {
  // ...
}
```

使用的时候，既可以采取`Com_SendWorkingCommand()` + `Com_Receive(&info)`的组合方式，也可以直接使用`Com_WorkAndReceive()`函数获得。具体源代码简易参考代码仓库。

### debug.c

`debug.c`中实现了我们的大部分debug函数和操作。debug的需要，是各种参数的调定、动作的执行和调试信息的输出。参数调定的可视化和条理化需要较高的编程技巧，我们没有实现。而动作的执行和调试信息的输出，我们则做到了良好的、友好的实现方式。

在进行具体讨论之前，我们想先提及如下函数。

```c  
void Debug_BugCatcher(HAL_StatusTypeDef status);
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
void Debug_CommandHandler(uint8_t *str);
```

我们实现了一个指令解析函数。指令是一个字符串，包括大写字母、数字和英语逗号。根据不同的指令类型，可以得到不同的相应动作。查看源代码后便可以发现其中的实现也不是复杂的事情，根据注释可以简单理解。值得注意的是，`MM`类的指令只有在关闭PID更新后可以使用。这里也只是在开始时调试使用了，后期完全没有任何作用。有相似作用的便是`P`和`S`指令。我们完全没有使用它们，仅仅是实现了而已。

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

这个函数实现起来并不复杂，代码也写得比较美观，适合理解与学习。

### pushrod.c

`pushrod.c/.h`文件实现了简单的脉冲控制步进电机的功能。具体实现为创建全局变量`Pushrod_DistanceInstace`和`Pushrod_DirectionInstance`，分别用于控制剩余移动步数和移动方向。

而且需要提供如下的update函数。Update函数以一个自定义的周期定义。这个周期应当使得步进电机可以旋转到位又不至于过慢。具体参数自己调节。

```c
void Pushrod_TIM_UpdateHandler(void) {
  static int cnt = 0;
  cnt += 1;
  if (Pushrod_DistanceInstance) {
    HAL_GPIO_WritePin(Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin,
                      ((Pushrod_DistanceInstance & 0x0001) == 1)
                        ? GPIO_PIN_SET
                        : GPIO_PIN_RESET);
    --Pushrod_DistanceInstance;
  }
}
```

这个函数没有体现出来方向的切换，事实上，方向由如下函数决定。

```c
__STATIC_INLINE void Pushrod_SetDirection(Pushrod_DirectionTypeDef dir) {
  HAL_GPIO_WritePin(Pushrod_Direction_GPIO_Port, Pushrod_Direction_Pin,
                    dir ? GPIO_PIN_SET : GPIO_PIN_RESET);
  Delay_us(5);
}
```

那么，控制步进电机的方法就很容易了。

```c
__STATIC_INLINE void Pushrod_MoveForward(Pushrod_Distance_t distance) {
  Pushrod_DistanceInstance = distance;
  Pushrod_SetDirection(Pushrod_CCW);
}
__STATIC_INLINE void Pushrod_MoveBackward(Pushrod_Distance_t distance) {
  Pushrod_DistanceInstance = distance;
  Pushrod_SetDirection(Pushrod_CW);
}
__STATIC_INLINE void Pushrod_SuddenStop(void){
  HAL_GPIO_WritePin(Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin, GPIO_PIN_RESET);
  Pushrod_DistanceInstance = 0;
}
```

### steer_ctrl.c

这里定义了舵机的运动方式。舵机是PWM驱动的，显然本MCU的PWM输出不足以满足这些舵机的需要，所以我们使用了PCA9685芯片/模块满足这种需求。

这个文件中，只有舵机的操作API，如下。

```c
typedef uint8_t Steer_COMNumber_t;
void __STATIC_FORCEINLINE Steer_SetAngleByDegree(Steer_COMNumber_t SteerNumber,
                                                 int16_t angle) {
  PWM_SetPWM_ByDutyCycle(SteerNumber, (uint16_t)((angle * 2.0 / 180 + 0.5) / 20 * 4096));
}
void __STATIC_FORCEINLINE Steer_Init(void) {
  /* 代码 */
}
```

顺带一提，我们精调了`Steer_Init()`函数的默认值，让机器人的待机动作更加美观。想看的观众可以去看比赛回放欣赏。

### pwm_generate.c

这里是PCA9685的驱动代码。其中的代码是我根据Datasheet和Arduino的驱动源代码重写的。

具体包括如下API。

```c
void PWM_SetFrequencyAndStartUp(double frequency);
void PWM_SetPWM_ByDutyCycle(uint8_t Ordinal, uint16_t DutyCycle);
```

其他的函数大多是辅助函数，用于PCA9685的协议解析。PCA9685有着不错的PWM生成能力，也建议读者自行实验。此外，PWM产生芯片现货其实不多，SG3525也能起到这个作用，但是只有两路，事实上意义不大。所以，如果想板载一个PWM生成器，又买不到PCA9685，可以考虑板载一个小型MCU，也能达到相同的目的。

这里使用模块主要原因还是我太菜了。

### delay.c

`delay.c`中实现了延迟函数。事实上HAL库实现了毫秒级别的延迟可以使用。但是呢，在处理电机方向切换的时候，需要微秒级别的死区，如果改用毫秒代替，那电机的转动就十分笨拙了，还会耽误中断函数的处理。

由于先前搭载了`FreeFTOS`，这里的系统时钟由`TIM7`这个基本定时器提供。这一点可以在`main.c`中看出，由`HAL_IncTick()`函数调用。

这也就意味着，`SysTick`这个不靠谱的定时器可以被我们使用。`SysTick`由HAL库自动初始化。

```c
void Delay_Init(void) {
  Delay_Factor_us = HAL_RCC_GetHCLKFreq() / 1000 / 1000;
  Delay_Factor_ms = Delay_Factor_us * 1000;
}
void Delay_us(uint16_t us) {
  uint32_t tmp;
  SysTick->LOAD = us * Delay_Factor_us;
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do {
    tmp = SysTick->CTRL;
  } while (tmp & 0x01 && !(tmp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
}
```

简单快捷地实现了微秒级延迟。

### pid.c

PID是老生常谈地话题。通常我们学习的PID包括位置PID和增量PID。这里的`pid.c/.h`中定义了PID的基本格式，和一些相关的操作方法。

*这里的API是本人初始构建使用，后面移植时发现由许多细小问题。建议读者学习原理后自主实现。不对程序漏洞造成的损害负责。*

```c
typedef struct {
  double Ki;              // Ki
  double Kp;              // Kp
  double Kd;              // Kd
  double Integral;        // 积分累计
  double Derivative;      // 微分项
  double Target;          // 目标值
  double Current;         // 当前值
  double Previous;        // 上一值
  double Output;          // 输出值
  double prePrevious;     // 上上一值
} PID_InformationTypeDef;
```

```c
__STATIC_INLINE void PID_SetTarget(PID_InformationTypeDef *handle,
                                   double target) {
  handle->Target = target;
}
void PID_InformationInit(PID_InformationTypeDef *handle);

void PID_Calculate_Locational(PID_InformationTypeDef *handle, double current);

void PID_Calculate_Locational_CounterOverflow(PID_InformationTypeDef *handle,
```

我也听到位置是`positional`的说法。我的英语很菜，凑合着阅读吧。

### callback.c

众所周知，HAL库引入了先进的回调函数体系，让一些任务的处理更加简易。

这个`callback.c`里面包括了原生的`callback`，和转发出来的自定义`callback`。当然实际使用的时候，还是原生`callback`更符合项目的要求。

```c
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    MotorFeedback_IC_CaptureCallback(htim);
  }
}
```

### iso15693.c 和 pn5180.c

这里是源自作者`lakun@qq.com`的驱动。我在其中加入了`RFID_Ordinal_t`用于不同方向的RFID切换。

但是，由于视觉的效果超出预期，我们放弃了RFID方案。这套代码没有经过测试，但是因为原作者已经测试通过，应该也没有问题。

这里就不作代码介绍了，贴一个枚举`RFID_Ordinal_t`。

```c
typedef enum {
    RFID_A = 0,
    RFID_B = 1
} RFID_Ordinal_t;
```

### sensor.c

`Sensor`模块是我思维震荡的主要部分。这里实现了底盘信息读取和解析的基本盘。原本打算是底盘建模配合角速度计联合调控，后来发现实现方法十分复杂就放弃了。

后来的巡线设计可能在重点函数部分讲解。这里只剩下了简单的获取底盘信息的函数。底盘信息由另一块系统板提供。另一块系统板代码十分简单，所以不提供开源了。

```c
__STATIC_INLINE void Sensor_RefreshUART(UART_HandleTypeDef *hd) {
    UNUSED(hd);
    MX_USART1_UART_Init();
}
__STATIC_INLINE TraceInfo_t* Sensor_GetCurrentInfo(void) {
    uint16_t cmd = 0x8000;
    HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, (uint8_t *)&cmd, 2, 0x004f);
    if (ret != HAL_OK) {
        printf("Failed to transmit 'Get Trace' command\r\n");
        Sensor_RefreshUART(&huart1);
    }
    ret = HAL_UART_Receive(&huart1, (uint8_t*)CurrentTrace, 4 * (sizeof(TraceInfo_t)), 0x004F);
    if (ret != HAL_OK) {
        printf("Failed to receive trace information\r\n");
        Sensor_RefreshUART(&huart1);
    } else {
        // printf("Received\r\n");
    }
    Sensor_RefreshUART(&huart1);
    CurrentTrace[0] &= (1 << 9) - 1;
    CurrentTrace[1] &= (1 << 11) - 1;
    CurrentTrace[2] &= (1 << 11) - 1;
    CurrentTrace[3] &= (1 << 9) - 1;
    return CurrentTrace;
}
```

这里的错误处理十分有意思。实际使用的时候，因为连接问题，经常出现地盘掉线的错误。一般地，地盘掉线的原理，是上控制板Uart发生错误，或者下控制板Uart发生错误。下控制板发生错误，可以手动复位。但是上控制板运行着流程控制，不能复位。所以就使用了软件刷新的方法。函数`Sensor_RefreshUART()`就实现了这一功能。

还有值得一提的是，这里也实现了一个奇怪的函数。它的作用是输出一个二进制数中1的个数。这个函数来自于知乎，具体作者我也不清楚。但是这个函数相当高效，十分感谢。

```c
__STATIC_INLINE int count_bits(int x) {
    // From zhihu
    int xx = x;
    xx = xx - ((xx >> 1) & 0x55555555);
    xx = (xx & 0x33333333) + ((xx >> 2) & 0x33333333);
    xx = (xx + (xx >> 4)) & 0x0f0f0f0f;
    xx = xx + (xx >> 8);
    return (xx + (xx >> 16)) & 0xff;
}
```

### arm_ctrl.c

这个文件定义了各种舵机动作。水花队的舵机调控十分流畅，但是我们的电控人员水平不足，没有使用什么高深技巧。代码主要实现了如下函数。

```c
__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_TalonClose(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_TalonOpen(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_Raise(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_PutDown(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_TakeBall(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_TalonClose(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_TalonOpen(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_Raise(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_PutDown(void);
__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_TakeBall(void);
```

### VL53L0X系列

VL53L0X是ST公司开发的激光测距芯片/模块，使用的API全套提供，并且具有HAL库一样的高封装性。只需要提供I2C接口，就可以轻松使用。我很喜欢，就抑移植了过来。但是比赛中并没有使用，因为机械结构不允许。所以我也不做代码介绍了，有兴趣的读者可以自行观看。

## 重点部分实现的思路和方法

由于个人时间有限，很难把重点代码一一解释清楚。这里只是大致编写部分流程的思路。如果有疑问的话，推荐根据程序的执行流程阅读代码，这样便能充分理解代码的作用。

由于MCU的特性，阅读代码的时候应当十分注意中断函数和回调函数的使用。通常，这两种函数会将程序的执行流程打散，降低了程序的易读性。但是话说回来，中断的形式是十分符合人类直觉的。所以，推荐读者充分理解中断函数的作用，知道它们调整了什么变量，产生了什么作用。这样便能事半功倍。

### 主控逻辑和流程

主控逻辑在`main_.c`和`procedure.c`中充分体现。

`main_.c`文件中，主要由函数`Main_()`控制全部流程。流程的具体实现，由`procedure.c`提供细节。在每个`procedure`中，机器人可能完成了移动、识别、抓取、投篮等动作。这些动作通常被良好封装了起来，比如`Motion_MoveForwardStable()`, `Motion_PickUpBallBackward`。

这些函数的调用之间，往往需要部分延时或者调控。比如有的运动函数之后需要`Motor_Decode(0, 0, 0);`以防止机器人的继续运动。同时也起到了减速刹车的作用。

机器人在正常运行的过程中也产生了丰富的`log`消息。在调试阶段，我们通过`NRF24L01`模组将消息打印在电脑上，借此检查机器人是否正常运行。这样的消息打印无论何时都是十分必要的。

`procedure`的具体实现是一个很奇妙的过程。每个`procedure`中，一般地，可以认为是一个或者多个(移动-动作)的任务组。每个任务组也是由更小的函数实现的。至于巡线、姿态之类的问题，在后文将得到解答。

### 电机使用

前文已经提到，这里的电机编码器使用了较为奇怪的读取方式。我们在每个输入捕获的回调函数中完成了速度的解析。这样做的好处是速度更新及时，且具有较高精度。但是可以预见的是，如果出现遗漏的捕获，便会产生一个极低的速度值。这种情况并不多见，但是一旦出现便会十分致命。

在其他RoboMaster例程中，经常使用的方法是采取低通滤波器进行一阶滤波，消除这样的极高误差。但是由于个人能力有限，我们使用的仅仅是差分检查。

```c
    if (info->Directions[i] == MotorFeedback_CW) {
      MotorSpeed_t cur = Motor_FeedbackFix(info->TimeTicks[i]);
      if (cur - PreSpeed[i] > -MAX_INC && cur - PreSpeed[i] < MAX_INC)
        motorSpeed_data[i] = cur;
      if (motorSpeed_data[i] < MIN_FLOW)
        motorSpeed_data[i] = 0;
      PreSpeed[i] = cur;
      if (info->ReloadTimes[i] > 1) {
        motorSpeed_data[i] = 0;
      }
    } else {
      MotorSpeed_t cur = Motor_FeedbackFix(info->TimeTicks[i]);
      if (cur - PreSpeed[i] > -MAX_INC && cur - PreSpeed[i] < MAX_INC)
        motorSpeed_data[i] = -cur;
      if (motorSpeed_data[i] > -MIN_FLOW)
        motorSpeed_data[i] = 0;
      PreSpeed[i] = cur;
      if (info->ReloadTimes[i] > 1) {
        motorSpeed_data[i] = 0;
      }
    }
```

可以看到，`cur - PreSpeed[i]`就是我们使用的差分检查方法。如果差分值小于`MAX_INC`，即`Max_Increment`，则认为这次是合法的输入捕获结果，将其记录在速度上。否则，认为是极高误差，忽略即可。

注意，即使是异常误差，也需要在`PreSpeed[i]`中记录。否则可能出现速度锁死的情况，影响程序的继续运行。因为部分时候正常的加速也会超过`MAX_INC`，我们不能将标准卡的太死。

### 摩擦轮调用

摩擦轮电机采取了直流无刷电机。直流无刷电机具有转速高、力矩可控、寿命长的特点。但是直流无刷电机的控制方法十分复杂，不是简简单单的PWM即可控制。一般地，初学者可能学习的，应当是根据霍尔编码器进行周期换向控制的方法。这种方法实现简单，但是控制精度不高，而且速度受电压变化波动较大。由于本人实力有限，这里采用了现成的控制模块，使得无刷电机的控制和有刷电机一样简单。

由于简单控制方法受到电压限制，我们采取了额外的稳压模块进行调整。极端的时候甚至达到了八投八中，可以说是比较成功的。

### 步进电机使用

步进电机是一个十分容易控制的东西。当然，前提是你不需要自己实现驱动器。在驱动器的帮助下，步进电机的前进后退只是脉冲的事。脉冲怎么实现呢？

一般地，我们可以借用TIM的PWM功能伪装成脉冲，只需要占空比不要太离谱就行；但是PWM功能的缺点是无法精确控制步数，这也就将步进电机的精确控制优点抛弃了。

所以我们使用TIM的溢出中断实现了手工的脉冲。

```c
void Pushrod_TIM_UpdateHandler(void) {
  static int cnt = 0;
  cnt += 1;
  if (Pushrod_DistanceInstance) {
    HAL_GPIO_WritePin(Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin,
                      ((Pushrod_DistanceInstance & 0x0001) == 1)
                        ? GPIO_PIN_SET
                        : GPIO_PIN_RESET);
    --Pushrod_DistanceInstance;
  }
}

可以看到，这里只是简单的根据奇偶判断高低电平的程序。

```

### 舵机使用

舵机的控制只需要PWM。但是由于需要的数量较多，我们采取了成熟的PWM模块实现这一功能。PWM产生芯片不多，市面上常见的好像只有这一种。按需使用即可。

使用方法上，我个人推荐的流程是，先去搜索引擎中搜索相关的例程和博客，大致了解它的使用方法。然后参阅datasheet，了解其中每个寄存器的作用。最后移植例程，并根据datasheet微调你需要的内容。

这样，博客中你不理解的事物，和datasheet看不懂的专有名词，得到了相互介绍，一举两得。

### 底盘循迹、运动和计数

底盘巡线是一个头疼的事物。一开始我们打算采取全局建模的方式，即根据底盘信息实时计算出机器人在平面上的三维坐标。这显然是不现实的，因为根据去年`HelloWorld`队的计划书看来，这样的作法会造成各种各样误差，每一个误差都需要特判修正。

但是，我们也看到其他队伍的程序设计，"灯亮了，反着走"，这样的思路在具体实现中会频繁出现跑飞情况。即使是八强比赛中也看到了这样的不堪。

所以我们结合了二者，采取了多红外对管阵列 + 弹簧式反馈调节方法。这里放一个示例。
```c
void Motion_CorrectWhenMovingAtX(void) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    uint8_t LeftBegin, LeftEnd, RightBegin, RightEnd;
    // 得到左右的点位
    Position_GetOneActive(ptr[1], 11, &LeftBegin, &LeftEnd);
    Position_GetOneActive(ptr[2], 11, &RightBegin, &RightEnd);

    uint8_t Left = (LeftEnd + LeftBegin);
    uint8_t Right = (RightEnd + RightBegin);

    // 反馈调节
    int16_t Front = (((int16_t)((int8_t)Left + (int8_t)Right) - 20) * (-2));
    Motor_SetY(Front);
    Motor_SetW(((int16_t)((int8_t)Left - (int8_t)Right)) * (50));
}
```

可以看到，我们解析底盘点位之后，使用了弹簧式的一阶反馈模型进行调节。一方面，避免了大幅度调整的抖动。另一方面，避免了循迹面窄小造成的跑飞现象。实际测试中，一般一天测试下来，至多会有一次跑飞。这个成功率是相当有水平的。

### 多方通讯

主要的通信方向，是主控板和辅控板，主控板和树莓派，主控板和NRF24L01。这里全部采用了UART方式，其中NRF24L01使用了模块转接。

具体使用时，由于机器人震动剧烈，容易造成接触不良通讯中断，从而卡死USART外设。所以使用时的波特率全部设置为了9600，牺牲了通讯速率，提高了稳定性。

## 出现的一些问题

这里记录制作过程中的各种问题，读者可以有所参考。

### 电路板地干扰

众所周知，数字电路、模拟电路、功率电路的运行中会产生不同的噪声。这些噪声会在地平面里回荡，如果不能好好处理便会影响电路工作。这里我们将功率地和数字地分开，使用零欧电阻单点接地，一定程度上隔离了噪声。具体的工作原理上网查询便可得知。

### 接线震动接触不良

正常运动的机器人的震动是非常大的。仅仅使用杜邦线连接的端口在一两次运作下便会断开。在实际调试中，我们一方面对控制电路进行了热熔胶加固，保证控制电路的连接性；另一方面对通讯电路进行了焊接处理，保证通讯电路的稳定性。

没曾想到的是，决赛的时候，现成的USB转TTL模块发生了接触不良的故障。这属于寿命到头了，我们没有办法解决。最后惨遭淘汰。

### 程序设计访问异常内存

在设计节点计数的程序时，经常出现跑飞的情况。检查返回值，发现机器人跑到了几十万的节点处。排除重复计数的影响，只可能是`for`语句没有设置更加合理的终止条件，使程序跑到了内存之外。

当然，也有的时候，程序直接进入了`HardFault`状态。这个也是访问了异常内存的原因。这次不是跑飞了，是直接挂掉了。因为部分内存空间是不允许访问的。

### 通信中断，外设异常

通信中出现了线路中断现象，最常见的情况是Uart错乱。原因是上一数据帧和下一数据帧的数据交错，造成数据检验不通过。而且这时候Uart并不认为自己没有正常工作。这时候就需要软件重启Uart了。

还有的时候，发生了ORE错误，这是因为发送频率太高，前一个数据帧还没有被处理，就又有下一个数据送来。合理调整发送频率即可。

对于工作环境不良的通信线路，推荐使用较低的波特率。在调低波特率后，通信稳定性有所提升。

### 未能正常使用模块

对于任何使用的模块，一定要根据芯片的Datasheet，或者参照官方例程使用。对于网络上的教程，推荐大致浏览后对照Datasheet再进行解读。点名批评CSDN，坑害我许久。

### 麦克纳姆轮方向错误

麦克纳姆轮的X型和O型是从下往上看的，但是教程中给反了，所以测试的时候总是无法正确行驶。调试时间长达一个月，将近退赛。再次批评CSDN。

### 未能做到短路保护

初次构建电路系统的时候，多次出现短路情况。短路地方包括电路板反面焊点，以及接线裸露的地方。部分接线的地方使用了剥线钳直接连接，这就增大了裸漏的铜丝相互接触短路的可能性。

### 接线造成异常重启和无法烧录

SWD调试和烧写的引脚需要格外注意。出现过引线接错到SWD上，造成无法烧录的情况。表现通常为`Internal error`或者其他内核错误。也有可能是`Not found`，具体问题具体分析。

也出现过接线接到了`NRST`接口上的情况。造成的影响是自动重启。

### PID逻辑误差较大

PID代码写错了 + 参数没调好。这个问题是最普遍的。

### 电机力矩不足

理论值和实际值是不同的。就算算出来需要的力矩很小，也需要一个较大的力矩保证运行的稳定。而且力矩较小，速度控制将成为一个难题。具体表现为输出电压和速度控制在接近0的部分不是线性的。电机在低电压（低占空比）的情况下更难控制。

### 底盘计数错误，误差较大

问题出在计数逻辑上。需要在每次计数后延时一段时间，防止重复计数。经实验测试出来的值为100ms。

### 备份零件不足，时常停滞

这属于工程经验不足，未能备足足够的零件。备份应当根据个人能力和调试方法适时调整。比如说，作为一个工程新手，单片机和电池的损坏概率很大，需要多备份。但在多次参加工程之后，对各种保护措施了解比较透彻之后，理论上进行1:1的零件备份便是足够的。

### 电路构建和测试的顺序不当

电路的构建和测试应当遵循先下后上，先底层后上层的顺序。特别地，debug调试通信应当位于首位。在实际构建的时候，犯了上面的错误，造成了建设中期需要频繁拆除零件进行调试的局面。

### 机械结构设计缺陷

请参考《epsilonGoal队技术总结》。
