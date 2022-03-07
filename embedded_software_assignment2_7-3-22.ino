/*
 * task 1 total time = 12800 microseconds 0.0128s
 * on for 50 off for 12750
 */

// double check pin numbers
#define monitrswitch 12 // far end button
#define potentiometer 34 
#define ledwatchdog 25 // green
#define ledvis_error 13 // red


// parameters
///////////////////////////////////////////////////////////
// add individual task periods in ms
int watchdog_off = 12750;
int watchdog_on = 50;
unsigned long monitrlength = 200000; // 5hz
unsigned long measurelength = 1000000; // 1hz
unsigned long analoglength = 1000000/24; // 24hz
unsigned long filtered_analoglength = 1000000/24; // 24hz
unsigned long assemblelength = 100000; // 10hz
unsigned long checklength = 1000000/3; // 3hz
unsigned long vis_errorlength = 1000000/3; // 3hz
unsigned long loglength = 5000000; // 0.2hz
///////////////////////////////////////////////////////////

unsigned long analog1 = 0;
unsigned long analog2 = 0;
unsigned long analog3 = 0;
unsigned long analog4 = 0;
unsigned long analog_average;

int ledwatchdogstate;
int monitrswitchstate = 0;
int analoginput = 0;
int ledvis_errorstate;

int counter = 0; 
long interval; // variable interval

unsigned long prevTimewatchdog; 
unsigned long prevTimemonitr;
unsigned long prevTimemeasure;
unsigned long prevTimeanalog;
unsigned long prevTimefiltered_analog;
unsigned long prevTimeassemble;
unsigned long prevTime
unsigned long prevTimecheck;
unsigned long prevTimevis_error;
unsigned long currentTime;

void setup() {
  // put your setup code here, to run once:

  pinMode(monitrswitch, INPUT); 
  pinMode(potentiometer, INPUT); 
  pinMode(ledwatchdog, OUTPUT);
  pinMode(ledvis_error, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  watchdog()
  monitr()
  measure()
  analog()
  filtered_analog()
  assemble()
  check()
  vis_error()
  Log()


}
void watchdog(){
  if (ledwatchdogstate == 0);
    if (currentTime - prevTimewatchdog >= watchdog_off){ // interval would need to be 12750ms, the time its off
      prevTimewatchdog = currentTime;
      
      if (ledwatchdogstate == 0) { //set watchdog state to on if off
        ledwatchdogstate = 1;
        digitalWrite(ledwatchdog, HIGH);
      }
      
    else if (currentTime - prevTimewatchdog >= watchdog_on){ // interval would need to be 50ms, the time its on
      prevTimewatchdog = currentTime;

      if (ledwatchdogstate == 1) { //set watchdog state to off if on
        ledwatchdogstate = 0;
        digitalWrite(ledwatchdog, LOW);
      }
    }

void monitr(){
  if (currentTime - prevTimemonitr >= monitrlength){
    prevTimemonitr = currentTime;
    monitrbuttonstate = digitalRead(monitrswitch);
  }
}

void measure(){
  
}

void analog(){
  if (currentTime - prevTimeanalog >= analoglength){ 
    prevTimeanalog = currentTime;
    analog1 = digitalRead(potentiometer);
    }
    
  }
}

void filtered_analog(){
  if (currentTime - prevTimefiltered_analog >= filtered_analoglength){ // 24 times a second average the last 4 analog values
    prevTimefiltered_analog = currentTime;
    analog2 = analog1;
    analog3 = analog2;
    analog4 = analog3;
    analog_average = ((analog4 + analog3 + analog2 + analog1)/4)
  }
}
  
}

void assemble(){

}

void check(){
  
}

void vis_error(){
  if (currentTime - prevTimevis_error >= vis_errorlength){
    prevTimevis_error = currentTime;
    if (ledvis_errorstate == 0){
      ledvis_errorstate = 1;
      digitalWrite(ledvis_error, HIGH);

    else (ledvis_errorstate == 1){
      ledvis_errorstate = 0;
      digitalWrite(ledvis_error, LOW);
      }
    }
}

void Log(){
  
}
