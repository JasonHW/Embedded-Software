/*
 * task 1 total time = 12800 microseconds 0.0128s
 * on for 50 off for 12750
 */

// esp pin numbers
#define monitrbutton 12 // far end button on the bread board
#define potentiometer 34 
#define ledwatchdog 25 // green led
#define ledvis_error 13 // red led task 8
#define wave 35 // square wave task 3


// Time parameters fo each task in microseconds
///////////////////////////////////////////////////////////
// add individual task periods in microseconds
/* watchdog timings come from assignment 1 with 50microseconds from sig B being added to 
 *  all pulses of sig A and the timings between plus the b + d time after A finies and 
 *  before b starts again coming to 12800 microsecond period, 50 microseconds of on pulse
 *  and 12750 off off pulse.
 *  This gives a frequency of 1*(1000000)seconds/12800 = 78.125Hz
 */
long watchdog_off = 12750; // task 1 time for when pulse is off 
int watchdog_on = 50; // task 1 time for when pulse is on

unsigned long monitrlength = 200000; // task 2 button monitor timing 5hz 
unsigned long measurelength = 1000000; // task 3 square wave measure timing 1hz
unsigned long analoglength = 1000000/24; // task 4 analog potentiometer measurement timing 24hz
unsigned long filtered_analoglength = 1000000/24; // task 5 average analog timing 24hz
unsigned long assemblelength = 100000; // task 6 assembly timing 10hz
unsigned long checklength = 1000000/3; //task 7 potentiometer check timing 3hz
unsigned long vis_errorlength = 1000000/3; // task 8 visual error cue timing for red led 3hz
unsigned long loglength = 5000000; // task 9 log to serial timing 0.2hz
float wavefreq; // generated square wave frequency 

///////////////////////////////////////////////////////////
// analogue potentiometer measurements
unsigned long analog1 = 0;
unsigned long analog2 = 0;
unsigned long analog3 = 0;
unsigned long analog4 = 0;
unsigned long average_analogue_in;

////////////////////////////////////////////////////////////
// toggle states 
int ledwatchdogstate; // task 1 watchdog led status 0 or 1
int monitrbuttonstate = 0; // task 2 monitor button status 0 or 1
int analoginput = 0; // task 4 initial potentiometer analog input
int error_code; // error status for potentiometer task 8

int currentwavestate; // task 3 current measure square wave status high or low
int desiredwavestate; // task 3 desired  square wave status high or low

int counter; // task 6 assembly counter up to 1000, 10 times a second 

unsigned long currentTime; // current measured time in microseconds using micros() 

// individual previous time variables for each task
////////////////////////////////////////////////////////////
unsigned long prevTimewatchdog; // task 1
unsigned long prevTimemonitr; //task 2
unsigned long prevTimemeasure; //task 3
unsigned long prevTimeanalog; //task 4
unsigned long prevTimefiltered_analog; //task 5
unsigned long prevTimeassemble; //task 6
unsigned long prevTime; //task 7
unsigned long prevTimecheck; //task 8
unsigned long prevTimevis_error; //task 8
unsigned long prevTimeLog; //task 9

////////////////////////////////////////////////////////////


void setup() {
  // put your setup code here, to run once:

  // pin mode setup
  pinMode(monitrbutton, INPUT); // task 2 button monitor
  pinMode(potentiometer, INPUT); 
  pinMode(ledwatchdog, OUTPUT); // green led output for watchdog
  pinMode(ledvis_error, OUTPUT); // red led output for analog error
  pinMode(wave, INPUT); // input of generated square wave

  Serial.begin(9600); // start serial
  Serial.print("-INIT-");

}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = micros(); // measure current time
  // run task functions within loop
  watchdog(); // task 1
  monitr(); // task 2
  measure(); // task 3
  analog(); // task 4
  filtered_analog(); // task 5
  assemble(); // task 6
  check(); // task 7
  vis_error(); // task 8
  Log(); // task 9


}
// green led watchdog function
void watchdog(){ // 78.125Hz
  
  if (ledwatchdogstate == 0){ // check if the led is off
    // if interval is 12750
    if (currentTime - prevTimewatchdog >= watchdog_off){ // interval would need to be 12750micros, the time its off
      prevTimewatchdog = currentTime; // save current time
      
      //set watchdog state to on if off
      ledwatchdogstate = 1;
      digitalWrite(ledwatchdog, HIGH);
      
    }
  }
  // if led is on check if the time passed is equal to pulse on length 50 microseconds
  else if (currentTime - prevTimewatchdog >= watchdog_on){ // interval would need to be 50ms, the time its on
    prevTimewatchdog = currentTime; // save current time
    // turn off led
    ledwatchdogstate = 0;
    digitalWrite(ledwatchdog, LOW);
      }
    
    
  }

// button monitor
void monitr(){
  
  if (currentTime - prevTimemonitr >= monitrlength){ // check if time 200000 microseconds has passed, 5Hz
    prevTimemonitr = currentTime; // save current time
    if (digitalRead(monitrbutton) == HIGH){ // if the button is pushed set button status to 1
      monitrbuttonstate = 1;
    }
    else{ // if button is off status is 0
      monitrbuttonstate = 0;
    }
    }
    
  }



void measure(){ // measure generated square wave between 500 and 1000Hz
  if (currentTime - prevTimemeasure >= measurelength){ // 1000000 microsecondsm, 1Hz
    prevTimemeasure = currentTime;
  
    currentwavestate = digitalRead(wave); // current wave status is the read vave output
    // to start measuring the wave period, the current wave state must be different to the desired start state, 
    // when the states are the same the time is measured
    if (currentwavestate == HIGH){ 
      desiredwavestate = LOW;
      
    }
      
    else{
      desiredwavestate = HIGH;
      
      }
    while (currentwavestate != desiredwavestate){ 
      currentwavestate = digitalRead(wave); // do nothing but measure the wave while current and desired are different
    }
    prevTimemeasure = micros(); // set the start time of the period measurement 
    while (currentwavestate == desiredwavestate){
      currentwavestate = digitalRead(wave); // do nothing but measure the wave while current and desired are the same for half the period
    }
    currentTime = micros(); // measure end time of half the period
    wavefreq = (1000000/((currentTime - prevTimemeasure)*2)); // only half of the wave period so times by 2 for full period, divide 1 sec by period for measured frequency
  }
}

void analog(){ // measure 4 analog values from the potentiometer
  
  if (currentTime - prevTimeanalog >= analoglength){ // 1000000/24 microseconds, 24Hz
    prevTimeanalog = currentTime;
    // update each prvious value of analog with the new reading
    analog4 = analog3;
    analog3 = analog2;
    analog2 = analog1;
    analog1 = analogRead(potentiometer);
    
  }
}

void filtered_analog(){ // 1000000/24 microseconds, 24Hz
  
  if (currentTime - prevTimefiltered_analog >= filtered_analoglength){ // 24 times a second average the last 4 analog values
    prevTimefiltered_analog = currentTime;
    
    average_analogue_in = ((analog4 + analog3 + analog2 + analog1)/4);
  }
}

void assemble(){ // assembly instruction that does essentially nothing
  
  if (currentTime - prevTimeassemble >= assemblelength){ // 10 times a second execute 1000 times, 10Hz
    prevTimeassemble = currentTime;
    counter = 0;
    if (counter <= 1000){ // counter increases to max 1000
      __asm__ __volatile__ ("nop");
      counter ++; 
    } 
  }
}

void check(){
  
  if (currentTime - prevTimecheck >= checklength){ // 1000000/3 microseconds, 3Hz
    prevTimecheck = currentTime;
    if (average_analogue_in >= 2048){ // average_analogue_in > half of maximum range for analogue input 4096/2
      error_code = 1; // set error to 1 if average analog exceeds half max analog range
    }
    
    else{
      error_code = 0;
    }
    
  }
    
}

void vis_error(){ // 1000000/3 microseconds, 3Hz
  
  if (currentTime - prevTimevis_error >= vis_errorlength){
    prevTimevis_error = currentTime;
    if (error_code == 1){
      digitalWrite(ledvis_error, HIGH); // turn on red led if the error status is high
    }

    else{ 
      digitalWrite(ledvis_error, LOW);
    }
    }
  }


void Log(){ // print to serial, button status, square wave freqency and average analog
  
  if (currentTime - prevTimeLog >= loglength){ // 5000000microseconds, 0.2Hz
    prevTimeLog = currentTime;
    Serial.print(monitrbuttonstate);
    Serial.print(";");
    Serial.print(wavefreq);
    Serial.print(";");
    Serial.print(average_analogue_in);
    Serial.println(";");
  }
}
