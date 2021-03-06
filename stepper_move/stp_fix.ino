//import time

//Declare pin functions on Redboard
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6

//Declare variables for functions
char user_input;
int select_bpm = 120;
int bpm;
int x;
int y;
int state;

void help()
{
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. Turn at default microstep mode.");
  Serial.println("2. Reverse direction at default microstep mode.");
  Serial.println("3. Turn at 1/8th microstep mode.");
  Serial.println("4. Step forward and reverse directions.");
  //80 rotate at 60 sec
  //1 rotation is 200 step
  //1 step is 1.8 angle
  Serial.println("5. 60sec 80rotate");
  Serial.println("6. BPM");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  help();  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
      //int t=0;
      user_input = Serial.read(); //Read user input and trigger appropriate function
      digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
      Serial.print("Choose:");
      Serial.println(user_input);
      if (user_input =='1')
      {
         StepForwardDefault();
      }
      else if(user_input =='2')
      {
        ReverseStepDefault();
      }
      else if(user_input =='3')
      {
        SmallStepMode();
      }
      else if(user_input =='4')
      {
        ForwardBackwardStep();
      }
      else if(user_input =='5')
      {
        StepMode6080();
      }
      else if(user_input =='6')
      {
        //select_bpm = Serial.read();
        //bpm = select_bpm;
        //int A = Serial.parseInt();
        //select_bpm = select_bpm - 0x30;
        //StepMode(select_bpm);
        Serial.print("BPM:");
        Serial.print(select_bpm);
        Serial.println();
        StepMode(select_bpm);
      }
      else if(user_input =='h')
      {
        help();
      }
      else{
        Serial.println("Invalid option entered.");
        help();
      }
      resetEDPins();
  }
}

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

//Default microstep mode function
void StepForwardDefault()
{
  int t=0;
  int d=1;
  Serial.println("Moving forward at default step mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  //loop is time, delay is angle?
  for(x= 1; x<200; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(d);//ms
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(d);
    t=t+2*d;
  }
  Serial.print("Time = ");
  Serial.print(t);
  Serial.println(" ms");
  Serial.println("Enter new option");
  Serial.println();
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  Serial.println("Moving in reverse at default step mode.");
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 1; x<1000; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

// 1/8th microstep foward mode function
void SmallStepMode()
{
  Serial.println("Stepping at 1/8th microstep mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
  digitalWrite(MS2, HIGH);
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

//Forward/reverse stepping function
void ForwardBackwardStep()
{
  Serial.println("Alternate between stepping forward and reverse.");
  for(x= 1; x<5; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    //Read direction pin state and change it
    state=digitalRead(dir);
    if(state == HIGH)
    {
      digitalWrite(dir, LOW);
    }
    else if(state ==LOW)
    {
      digitalWrite(dir,HIGH);
    }
    
    for(y=1; y<1000; y++)
    {
      digitalWrite(stp,HIGH); //Trigger one step
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }
  }
  Serial.println("Enter new option:");
  Serial.println();
}

//60sec 80rotate
void StepMode6080()
{
  double t=0;
  double d=1.875;
  int l = 16000;
  Serial.println("60sec, 80rotate");
  for(x= 1; x<l; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(d);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(d);
    t=t+2*d;
  }
  Serial.print("Time = ");
  Serial.print(t);
  Serial.println(" ms");
  Serial.print("Delay = ");
  Serial.print(d);
  Serial.println(" ms");
  Serial.print("Loop = ");
  Serial.print(l);
  Serial.println(" step");
  Serial.println("Enter new option:");
  Serial.println();
}

//select bpm
void StepMode(int select_bpm)
{
  double t=0;
  int l = select_bpm * 200;
  double d = 60000/(2*(double)l);
  Serial.println("BPM banging");
  for(x= 1; x<l; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(d);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(d);
    t=t+2*d;
  }
  Serial.print("Time = ");
  Serial.print(t);
  Serial.println(" ms");
  Serial.print("Delay = ");
  Serial.print(d);
  Serial.println(" ms");
  Serial.print("Loop = ");
  Serial.print(l);
  Serial.println(" step");
  Serial.println("Enter new option:");
  Serial.println();
}

