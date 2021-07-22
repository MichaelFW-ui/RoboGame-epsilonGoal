#include ""
#define Speed_High 2
#define Speed_Low 1

void Procedure_Start(void){
  if(Motion_Control.Motion_Speed == 0){
    Motion_Control.Motion_Speed = Speed_High;
    Motion_GoNorth();
    while(Trace_Detect.North_Caculate < 3 && Trace_Detect.West_Caculate ==0);
  }
  Motion_Stay();
  if(Motion_Control.Motion_Speed == 0){
    Motion_GoWest();
    while(Trace_Detect.North_Caculate == 3 && Trace_Detect.West_Caculate <2);
  }
  Motion_Stay();
  if(Motion_Control.Motion_Speed == 0){
    Motion_GoNorth();
    while(Trace_Detect.North_Caculate < 4  && Trace_Detect.West_Caculate == 2);
    Motion_Control.Motion_Speed = Speed_Low;
    Motion_GoNorth();
    while(Trace_Detect.North_Caculate < 5  && Trace_Detect.West_Caculate == 2);
    Motion_Control.Motion_Speed = Speed_High;
    Motion_GoNorth();
    while(Trace_Detect.North_Caculate < 6  && Trace_Detect.West_Caculate == 2);
  }
  Motion_Stay();
}

void Procedure_GoToBalls(void){
  Clear_Caculate();
  if(Motion_Control.Motion_Speed == 0){
    Motion_Control.Motion_Speed = Speed_Low;
    Motion_GoEast();
    while(Trace_Detect.North_Caculate == 0 && Trace_Detect.East_Caculate < 1);
  }
  Motion_Stay();
  if(Motion_Control.Motion_Speed == 0){
    Motion_GoNorth();
    while(Trace_Detect.North_Caculate < 1  && Trace_Detect.East_Caculate == 1);
    Motion_Control.Motion_Speed = Speed_Low;
    Motion_GoNorth();
    while(Trace_Detect.North_Caculate < 2  && Trace_Detect.East_Caculate == 1);
    Motion_Control.Motion_Speed = Speed_High;
    Motion_GoNorth();
    while(Trace_Detect.North_Caculate < 3  && Trace_Detect.East_Caculate == 1);
  }
  Motion_Stay();
}

void Procedure_GoBack(void){
}

void Procedure_PickBalls(void){
}

