
// Define the buttons and LED's to pins on the ESP32
#define switch_1 23
#define switch_2 23
#define ledA 15
#define ledB 21

// Initialise parameters from surname: Wakefield
/* the timings for a, b, d and the length of signal B were mutiplied 
 *  by 1000 during development to make squence more visible
 */
int a = 400; //*1000;
int b = 100; //*1000;
int c = 15;  // counter max
int d = 2500; //*1000;

// variable for statuses of signals A and B

int ledAstate;
int ledBstate;

int counter; // counter keeps track of how many intervals have occured
long interval; // variable interval

int waveform_on; // variable for button 1 status
int waveform_extend; // variable for button 2 status

unsigned long prevTime; 
unsigned long currentTime;
int sigswitch; // variable for switching between A and B signals

// the setup function runs once when you press reset or power the board

void setup() {
  // initialize digital pins to the leds as outputs and buttons as inputs
  
  pinMode(switch_1, INPUT); 
  pinMode(switch_2, INPUT); 
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  
  
  unsigned long currentTime;
  
  waveform_on = HIGH; // kill button when HIGH
  waveform_extend = LOW; // extend the waveform by 3 (c + 3)
  
  interval = 50; //*1000; // initial interval
}

// the loop function runs over and over again forever
void loop() {
  
  currentTime = micros(); // microsecond function to keep track of time
  
  int button1State = digitalRead(switch_1); // read status of button 1
  int button2State = digitalRead(switch_2); // read status of button 2

  // When button 1 pushed waveform is toggled off if it is previously on
  if ((button1State == HIGH) && (waveform_on == HIGH)){
    waveform_on = LOW;
    delay(1000);
  }
  // When button 1 pushed waveform is toggled on if it is off
  else if ((button1State == HIGH) && (waveform_on == LOW)){
    waveform_on = HIGH;
    delay(1000);
  }

  // When button 1 allows signal
  if (waveform_on == HIGH){

    // mode 3: when button 2 pushed waveform extender alters c by + 3
    if ((button2State == HIGH) && (waveform_extend == HIGH)){
      waveform_extend = LOW;
      c = 15;
      delay(1000);
    }
    // when button 2 pushed waveform extender is toggled on if previously off
    else if ((button2State == HIGH) && (waveform_extend == LOW)){
      c = 18;
      delay(1000);
    }

    // activate signals A or B
    if (sigswitch == 1){
      signalA();
    }
    else{
      signalB();
    }
  }
}

void signalA(){

  // the difference of previous time to current time used to give interval between A pulses
  if (currentTime - prevTime >= interval) {
    prevTime = currentTime; // update previous time

    // when counter reaches max turn off A, reset counter, increase interval by d, 
    //toggle signal switch to activate B
    if (counter == c){      // c = 15 normal, 18 extended
      ledAstate = 0;
      interval = b + d;
      counter = 0;
      digitalWrite(ledA, LOW);
      sigswitch = 0;
      }
      
    else{
      // if A is off
      if (ledAstate == 0){
        interval = a + (counter * 50 /** 1000*/); // increase interval for larger A pulse
        counter++; // increase counter
        ledAstate = 1; // turn A back on with new interval
        digitalWrite(ledA, HIGH);
      }
      
      else{ // turn A off, use b as interval
        ledAstate = 0;
        interval = b;
        digitalWrite(ledA, LOW);
      }  
    }
  }
}  

void signalB(){
  // the difference of previous time to current time used to give interval b + d
  if (currentTime - prevTime >= interval){
    prevTime = currentTime;
    
    if (ledBstate == 1) { //switch to signal A if B is on at end of interval
      ledBstate = 0;
      interval = 0; // no time between end of B and start of A
      sigswitch = 1;
      digitalWrite(ledB, LOW);
    }
    else {
      ledBstate = 1; // turn on B
      interval = 50; //*1000; //length of signal B
      digitalWrite(ledB, HIGH);
    }
  }
}
