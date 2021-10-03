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

## 代码实现流程

### 启动流程

程序启动于`main.c`，其中调用了位于`main_.c`中的`Main_()`函数，作为程序的主体。在调试的时候，也有时调用`debug.c`中的`Debug_Main()`函数进行调试。

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
    eProcedure_HeadForPickingAreaSecondly = 6u
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
void Precedure_EnterPickingArea(void);
void Precedure_ExitPickingArea(void);
void Precedure_HeadForThrowingArea(void);
void Precedure_StayInThrowingArea(void);
void Precedure_HeadForPickingAreaSecondly(void);
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
void Motion_CorrectAtCross(void);
void Motion_CorrectInPickingArea(void);
void Motion_CorrectWhenThrowing(void);
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

这些函数用于两个捡球的动作，返回值是捡到的球的个数，即0或者1。

该文件事实上留出了两个API，为

```c
void Motion_MoveFromNodeToNode(node_t from, node_t to);
void Motion_MoveFromBeginningArea(node_t to);
```

用于结点之间的移动。目前看来是没有实现的。可能删除。

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

初始化函数，调用即可，别管太多。

```c
__STATIC_FORCEINLINE void MotorCtrl_SetDutyCycle(MotorOrdinal_t Motor, uint16_t DutyCycle);
void MotorCtrl_SetDirection(MotorOrdinal_t Motor, MotorDirection_t direction);
void __STATIC_INLINE MotorCtrl_SetPIDArguments(MotorOrdinal_t motor, MotorPIDTypeDef pid, double val);
double __STATIC_INLINE MotorCtrl_GetPIDArguments(MotorOrdinal_t motor, MotorPIDTypeDef pid);
void __STATIC_INLINE MotorCtrl_PrintArguments(void) {
void __STATIC_INLINE MotorCtrl_SetTarget(MotorSpeed_t speed, uint8_t ord) {
```

十分OOP的Set,Get函数，字面意思理解即可。

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
```

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

### 接线造成异常重启和无法烧录

### PID逻辑误差较大

### 电机力矩不足

### 底盘计数错误，误差较大

### 备份零件不足，时常停滞

### 机械结构设计缺陷

### 电路构建和测试的顺序不当

## 感想

