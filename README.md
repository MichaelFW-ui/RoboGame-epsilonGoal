<p align="center">
 <img width="300px" src="https://z3.ax1x.com/2021/07/20/Wt9wW9.jpg" align="center" alt="GitHub Readme Stats" />
 <!-- <h2 align="center">这里写你的标题</h2> -->
 <p align="center">红专并进，理实交融</p>
<p/>

# Team epsilonGoal 电控设计

## 前言

本机器人控制程序按照2021年中国科学技术大学Robo Game机器人给定任务设计。本程序的开发和使用旨在学习机器人控制系统、电机控制理论和常用传感器的使用，仅供学习和娱乐用途使用。

本程序中可能使用部分公开API或他人自行设计的接口代码，一般情况下有所标注。如有侵权，联系本人即可。

由于个人精力有限，其中可能出现注释逻辑与实际代码逻辑不匹配的情况，自行理解即可。

## 介绍

### 比赛要求

本场比赛要求，制作一个具备平面位移功能的机器人，设计合理的机械结构，使之能够在给定的场地上完成任务，包括根据黑线网格进行位移、通过三种方法之一识别球的类别、拾取篮球、发射篮球到篮框中。

### 主控平台

根据预先粗略评估，本次比赛中我组使用STM32F103ZET6为主控平台。后期发现使用F407可以提高工作效率，降低消耗成本。但是已经无法修改，作罢。

### 辅助控制板

由于前期对控制板的外设数量做出了错误估计，中期我组采取了一块STM32F103VET6辅助控制板。

### 综合传感器

本次比赛中，主要传感器不多，包括底盘的红外对管阵列、四个VL53L0X激光测距模块和RFID读取模块。

### 电机、舵机控制系统

电机由直流无刷电机和直流有刷电机组成。直流无刷电机存在两种驱动方案，分别是利用简单驱动板进行PWM调速，和利用复杂隔离驱动板手动进行换向调速。直流有刷电机相对简单，使用一款质量足够的驱动模块即可。

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

TODO

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

#### motion.c

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
```


## 重点函数实现介绍
