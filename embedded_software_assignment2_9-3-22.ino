/*
 * task 1 total time = 12800 microseconds 0.0128s
 * on for 50 off for 12750
 */

// double check pin numbers
#define monitrswitch 12 // far end button
#define potentiometer 34 
#define ledwatchdog 25 // green
#define ledvis_error 13 // red
#define wave 35 // square wave


// parameters
///////////////////////////////////////////////////////////
// add individual task periods in ms
long watchdog_off = 12750;
int watchdog_on = 50;
unsigned long monitrlength = 200000; // 5hz
unsigned long measurelength = 1000000; // 1hz
unsigned long analoglength = 1000000/24; // 24hz
unsigned long filtered_analoglength = 1000000/24; // 24hz
unsigned long assemblelength = 100000; // 10hz
unsigned long checklength = 1000000/3; // 3hz
unsigned long vis_errorlength = 1000000/3; // 3hz
unsigned long loglength = 5000000; // 0.2hz
unsigned long wavefreq;
///////////////////////////////////////////////////////////
// analogue measurements
unsigned long analog1 = 0;
unsigned long analog2 = 0;
unsigned long analog3 = 0;
unsigned long analog4 = 0;
unsigned long average_analogue_in;
////////////////////////////////////////////////////////////
// states
int ledwatchdogstate;
int monitrbuttonstate = 0;
int analoginput = 0;
int error_code;
int ledvis_errorstate;
int currentwavestate;
int desiredwavestate;

int counter; 

// previous time variables
////////////////////////////////////////////////////////////
unsigned long prevTimewatchdog; 
unsigned long prevTimemonitr;
unsigned long prevTimemeasure;
unsigned long prevTimeanalog;
unsigned long prevTimefiltered_analog;
unsigned long prevTimeassemble;
unsigned long prevTime;
unsigned long prevTimecheck;
unsigned long prevTimevis_error;
unsigned long prevTimeLog;
////////////////////////////////////////////////////////////
unsigned long currentTime;

void setup() {
  // put your setup code here, to run once:

  pinMode(monitrswitch, INPUT); 
  pinMode(potentiometer, INPUT); 
  pinMode(ledwatchdog, OUTPUT);
  pinMode(ledvis_error, OUTPUT);
  pinMode(wave, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  watchdog();
  monitr();
  measure();
  analog();
  filtered_analog();
  assemble();
  check();
  vis_error();
  Log();


}
void watchdog(){
  currentTime = micros();
  if (ledwatchdogstate == 0){
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
    }
  }
}

void monitr(){
  currentTime = micros();
  if (currentTime - prevTimemonitr >= monitrlength){
    prevTimemonitr = currentTime;
    if (digitalRead(monitrswitch) == HIGH){
      monitrbuttonstate = 1;
    else{
      monitrbuttonstate = 0;
    }
    }
    
  }
}

// have read state as current state of wave, if read state differs from wavestate start time
void measure(){ // start timer when signal is on stop timer when signal is off, times by 2 for full period, divide 1 sec by period for measured frequency
  
  currentwavestate = digitalRead(wave); 
  if (currentwavestate == HIGH){
    desiredwavestate = LOW;
    currentwavestate = digitalRead(wave);
  }
    
  else{
    desiredwavestate = HIGH;
    currentwavestate = digitalRead(wave);
    }
 
  while (currentwavestate == desiredwavestate){
    currentTime = micros();
    prevTimemeasure = currentTime;
    currentwavestate = digitalRead(wave);
    if (currentwavestate != desiredwavestate){
      wavefreq = (1/((currentTime - prevTimemeasure)*2));
    }
  }
  if (currentTime - prevTimemeasure >= measurelength){ 
    prevTimemeasure = currentTime;
    
  }
}

void analog(){
  currentTime = micros();
  if (currentTime - prevTimeanalog >= analoglength){ 
    prevTimeanalog = currentTime;
    analog1 = digitalRead(potentiometer);
    
  }
}

void filtered_analog(){
  currentTime = micros();
  if (currentTime - prevTimefiltered_analog >= filtered_analoglength){ // 24 times a second average the last 4 analog values
    prevTimefiltered_analog = currentTime;
    analog4 = analog3;
    analog3 = analog2;
    analog2 = analog1;
    average_analogue_in = ((analog4 + analog3 + analog2 + analog1)/4);
  }
}

void assemble(){ // assembly instruction that does nothing
  currentTime = micros();
  if (currentTime - prevTimeassemble >= assemblelength){ // 10 times a second execute 1000 times
    prevTimeassemble = currentTime;
    counter = 0;
    if (counter <= 1000){
      __asm__ __volatile__ ("nop");
      counter ++;
    }
    
  }


}

void check(){
  currentTime = micros();
  if (currentTime - prevTimecheck >= checklength){
    prevTimecheck = currentTime;
    if (average_analogue_in > 512){ // average_analogue_in > half of maximum range for analogue input 1024/2
      error_code = 1;
    }
    
    else{
      error_code = 0;
    }
    
  }
    
}

void vis_error(){
  currentTime = micros();
  if (currentTime - prevTimevis_error >= vis_errorlength){
    prevTimevis_error = currentTime;
    if (ledvis_errorstate == 0){
      ledvis_errorstate = 1;
      digitalWrite(ledvis_error, HIGH);
    }

    else{ // (ledvis_errorstate == 1)
      
      ledvis_errorstate = 0;
      digitalWrite(ledvis_error, LOW);
    }
    }
  }


void Log(){
  currentTime = micros();
  if (currentTime - prevTimeLog >= loglength){
    prevTimeLog = currentTime;
    // Serial.print(";");
  }
}
