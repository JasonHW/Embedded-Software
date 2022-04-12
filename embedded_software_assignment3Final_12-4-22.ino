// #include semaphore.h


/*
 * task 1 total time = 12800 microseconds 0.0128s
 * on for 50 off for 12750
 */
 
//////////////////////////////////////////   esp pin numbers    ///////////////////////////////////////////////


#define monitrbutton 12 // far end button on the bread board
#define potentiometer 34 
#define ledwatchdog 25 // green led
#define ledvis_error 13 // red led task 8
#define wave 35 // square wave task 3

#define GPIO  12



///////////////////////////    Time parameters for each task in microseconds    ////////////////////////////////

// add individual task periods in microseconds
/* watchdog timings come from assignment 1 with 50microseconds from sig B being added to 
 *  all pulses of sig A and the timings between plus the b + d time after A finies and 
 *  before b starts again coming to 12800 microsecond period, 50 microseconds of on pulse
 *  and 12750 off off pulse.
 *  This gives a frequency of 1*(1000000)seconds/12800 = 78.125Hz
 */
long watchdog_off = 12750/1000; // task 1 time for when pulse is off 
int watchdog_on = 50; // task 1 time for when pulse is on

unsigned long monitrlength = 200; // task 2 button monitor timing 5hz 
unsigned long measurelength = 1000; // task 3 square wave measure timing 1hz
unsigned long analoglength = 1000/24; // task 4 analog potentiometer measurement timing 24hz
unsigned long filtered_analoglength = 1000/24; // task 5 average analog timing 24hz
unsigned long assemblelength = 100; // task 6 assembly timing 10hz
unsigned long checklength = 1000/3; //task 7 potentiometer check timing 3hz
unsigned long vis_errorlength = 1000/3; // task 8 visual error cue timing for red led 3hz
unsigned long loglength = 5000; // task 9 log to serial timing 0.2hz
float wavefreq; // generated square wave frequency 


/////////////////////////////////////////////////     analogue potentiometer measurements    /////////////////////////////////////////////////

float analog1 = 0;
float analog2 = 0;
float analog3 = 0;
float analog4 = 0;
unsigned long average_analogue_in;

///////////////////////////////////////////////////     queue handler     //////////////////////////////////////////////////////////

xQueueHandle queue;


//////////////////////////////////////////////////////    toggle states     //////////////////////////////////////////////////////

int ledwatchdogstate; // task 1 watchdog led status 0 or 1
int monitrbuttonstate = 0; // task 2 monitor button status 0 or 1
int analoginput = 0; // task 4 initial potentiometer analog input
int error_code; // error status for potentiometer task 8

int currentwavestate; // task 3 current measure square wave status high or low
int desiredwavestate; // task 3 desired  square wave status high or low

int counter; // task 6 assembly counter up to 1000, 10 times a second 

unsigned long currentTime; // current measured time in microseconds using micros() 



//////////////////////////    individual previous time variables for each task    //////////////////////////////////

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

/////////////////////////////////////////////////    global structure    //////////////////////////////////////////////////////

struct serialstruct{
  int monitrbuttonstate; // task 2
  float wavefreq; // task 3
  float average_analogue_in; // task 5
};
  
serialstruct outputinfo; // declaring the output used for serial

///////////////////////////////////////////////////////    semaphore declare    //////////////////////////////////////////////////////////

static SemaphoreHandle_t sem;



///////////////////////////////////////--------------- task functions ---------------/////////////////////////////////////////////////

// semaphores applied to tasks 2, 3, and 5


///////////////////////////////////////    task 1 green led watchdog function    ///////////////////////////////////////

void watchdog(void *argp){ // 78.125Hz
  for (;;){
    
    digitalWrite(ledwatchdog, HIGH);
    delayMicroseconds(watchdog_on);
    digitalWrite(ledwatchdog, LOW);
    vTaskDelay(watchdog_off); // delay of specified length
  }
}



///////////////////////////////////////////////////     task 2 button monitor    //////////////////////////////////////////////////

void monitr(void *argp){
  for (;;){
    if (digitalRead(monitrbutton) == HIGH){ // if the button is pushed set button status to 1

      xSemaphoreTake(sem, 120); // time semaphore will wait for 120 milliseconds
      outputinfo.monitrbuttonstate = 1;
      xSemaphoreGive(sem);
      
      }
      
    else{ // if button is off status is 0
      
      xSemaphoreTake(sem, 120);
      outputinfo.monitrbuttonstate = 0;
      xSemaphoreGive(sem);
      }
    vTaskDelay(monitrlength);
    
  }
} 



//////////////////////////////////////////////////////    task 3 measure wave    ///////////////////////////////////////////////////

void measure(void *argp){ // measure generated square wave between 500 and 1000Hz
  for (;;){
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

    xSemaphoreTake(sem, 120);
    outputinfo.wavefreq = (1000000/((currentTime - prevTimemeasure)*2)); // only half of the wave period so times by 2 for full period, divide 1 sec by period for measured frequency
    xSemaphoreGive(sem);
   
    
    vTaskDelay(measurelength);
  }
}


/////////////////////////////////////////////////    task 4 analog values    ////////////////////////////////////////////////////

void analog(void *argp){ // measure 4 analog values from the potentiometer
  for (;;) {
    // update each prvious value of analog with the new reading
    analog4 = analog3;
    analog3 = analog2;
    analog2 = analog1;
    analog1 = analogRead(potentiometer);

    vTaskDelay(analoglength);
  }
 
  }

///////////////////////////////////////////////     task 5 average analog    //////////////////////////////////////////////////

void filtered_analog(void *argp){ // 1000000/24 microseconds, 24Hz
  for (;;) {
    xSemaphoreTake(sem, 120);    
    outputinfo.average_analogue_in = ((analog4 + analog3 + analog2 + analog1)/4);

    // add value to queue 
    
    xQueueSend(queue, &outputinfo.average_analogue_in, portMAX_DELAY);
    
    
    xSemaphoreGive(sem);
      
    
    vTaskDelay(filtered_analoglength);
  }
}


///////////////////////////////////////////////////    task 6 assembly    ///////////////////////////////////////////////////////
 
void assemble(void *argp){ // assembly instruction that does essentially nothing
  for (;;) {
    counter = 0;
    if (counter <= 1000){ // counter increases to max 1000
      __asm__ __volatile__ ("nop");
      counter ++; 
    } 
  vTaskDelay(assemblelength);
  }
}


///////////////////////////////////////////////////   task 7 error check    /////////////////////////////////////////////////////

void check(void *argp){
  for (;;) {
    // recieve filtered analog from queue
    xQueueReceive(queue, &average_analogue_in, portMAX_DELAY);
    
    if (outputinfo.average_analogue_in >= 2048){ // average_analogue_in > half of maximum range for analogue input 4096/2
      error_code = 1; // set error to 1 if average analog exceeds half max analog range
    }
    
    else{
      error_code = 0;
    }

    xQueueReset(queue); // reset queue
    
    vTaskDelay(checklength);
  }   
}
    

//////////////////////////////////////////////    task 8 visualise error    /////////////////////////////////////////////////////

void vis_error(void *argp){ // 1000000/3 microseconds, 3Hz
  for (;;){
    if (error_code == 1){
      digitalWrite(ledvis_error, HIGH); // turn on red led if the error status is high
    }

    else{ 
      digitalWrite(ledvis_error, LOW);
    }
    vTaskDelay(vis_errorlength);
  }
}



/////////////////////////////////////////////////////   task 9 log values    ///////////////////////////////////////////////////

void Log(void *argp){ // print to serial, button status, square wave freqency and average analog
  for (;;){
    if (outputinfo.monitrbuttonstate == 1){
    
      
      Serial.print(outputinfo.monitrbuttonstate); // task 2
      Serial.print(";");
      Serial.print(outputinfo.wavefreq); // task 3
      Serial.print(";");
      Serial.print(outputinfo.average_analogue_in); // task5
      Serial.println(";");
    }
    vTaskDelay(loglength);
  } 
  
  }


///////////////////////////////////////////////////    setup     /////////////////////////////////////////////////////////


void setup() {
  //int app_cpu = xPortGetCoreID(); // for pinned to core task

  
  sem = xSemaphoreCreateBinary();
  assert(sem);
  
  //////////////////////////////////////////////////   pin mode setup    ////////////////////////////////////////////////////
  
  pinMode(monitrbutton, INPUT); // task 2 button monitor
  pinMode(potentiometer, INPUT); 
  pinMode(ledwatchdog, OUTPUT); // green led output for watchdog
  pinMode(ledvis_error, OUTPUT); // red led output for analog error
  pinMode(wave, INPUT); // input of generated square wave

  // xTaskCreatePinnedToCore for created core

  ////////////////////////////////////////////-------- create tasks --------//////////////////////////////////////////////

  xTaskCreate(
    watchdog,
    "watchdog",
    2048,
    nullptr,
    1,
    nullptr
    //app_cpu
  );
  
  xTaskCreate(
    monitr,
    "monitr",
    2048,
    nullptr,
    1,
    nullptr
    //app_cpu
  );

  xTaskCreate(
    measure,
    "measure",
    2048,
    nullptr,
    2, // priority set higher in reading tasks 2 and 3 as they must be ran uninterupted to avoid inaccurate measurements
    nullptr
    //app_cpu
  );

  xTaskCreate(
    analog,
    "analog",
    2048,
    nullptr,
    2, // priority set higher in reading tasks 2 and 3 as they must be ran uninterupted to avoid inaccurate measurements
    nullptr
    //app_cpu
  );

  xTaskCreate(
    filtered_analog,
    "filtered_analog",
    2048,
    nullptr,
    1,
    nullptr
    //app_cpu
  );


  xTaskCreate(
    assemble,
    "assemble",
    2048,
    nullptr,
    1,
    nullptr
    //app_cpu
  );

  xTaskCreate(
    check,
    "check",
    2048,
    nullptr,
    1,
    nullptr
    //app_cpu
  );

  xTaskCreate(
    vis_error,
    "vis_error",
    2048,
    nullptr,
    1,
    nullptr
    //app_cpu
  );

  xTaskCreate(
    Log,
    "Log",
    2048,
    nullptr,
    1,
    nullptr
    //app_cpu
  );

  Serial.begin(9600); // start serial
  Serial.print("-INIT-");

  queue = xQueueCreate( 10, sizeof( int ) );
 
  if(queue == NULL){
    Serial.println("Error creating the queue");
  }

}

///////////////////////////////////////////////////////    loop    /////////////////////////////////////////////////////////

void loop() {
  currentTime = micros(); // measure current time
  vTaskDelete(xTaskGetCurrentTaskHandle());

}
