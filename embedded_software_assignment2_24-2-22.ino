/*
 * task 1 total time = 12800 microseconds 0.0128s
 * on for 50 off for 12750
 */

// double check pin numbers
#define monitrswitch 23
#define potentiometer 23 
#define ledwatchdog 15
#define ledvis_error 21


// parameters
// add individual task periods in ms
int watchdog_off = 12750;
int watchdog_on = 50;
int monitrlength = 200000; // 5hz
int measurelength = 1000000; // 1hz
float analoglength = 1000000/24; // 24hz
float filtered_analoglength = 1000000/24; // 24hz
int assemblelength = 100000; // 10hz
float checklength = 1000000/3; // 3hz
float vis_errorlength = 1000000/3; // 3hz
int loglength = 5000000; // 0.2hz

int ledwatchdogstate;
int monitrswitchstate = 0;
int analoginput = 0;

int counter; 
long interval; // variable interval

unsigned long prevTime; 
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
    if (currentTime - prevTime >= watchdog_off){ // interval would need to be 12750ms, the time its off
      prevTimewatchdog = currentTime;
      
      if (ledwatchdogstate == 0) { //set watchdog state to on if off
        ledwatchdogstate = 1;
        digitalWrite(ledwatchdog, HIGH);
      }
      
    else if (currentTime - prevTime >= watchdog_on){ // interval would need to be 50ms, the time its on
      prevTimewatchdog = currentTime;

      if (ledwatchdogstate == 1) { //set watchdog state to off if on
        ledwatchdogstate = 0;
        digitalWrite(ledwatchdog, LOW);
      }
    }

void monitr(){
  if (currentTime - prevTime >= monitrlength){
    monitrbuttonstate = digitalRead(monitrswitch);
  }
}

void measure(){
  
}

void analog(){
  if (currentTime - prevTime >= analoglength){
    analoginput = digitalRead(potentiometer);
  }
}

void filtered_analog(){
  
}

void assemble(){

}

void check(){
  
}

void vis_error(){
  
}

void Log(){
  
}
