

#define switch_1 23
#define switch_2 23
#define ledA 15
#define ledB 21


float a = 400*1000;
float b = 100*1000;
float c = 15; /*counter*/
float d = 2500*1000;
float sigBlength = 50*1000;

int ledAstate;
int ledBstate;
int counter;
long interval;

unsigned long prevTime;
unsigned long currentTime;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins to the leds as outputs.
  pinMode(switch_1, INPUT); 
  pinMode(switch_2, INPUT); 
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);

  interval = b;
  
}

// the loop function runs over and over again forever
void loop() {
  signalB();
  signalA();
}

void signalA(){
  unsigned long currentTime = micros();
  
  if (currentTime - prevTime >= interval) {
    prevTime = currentTime;
    
    if (counter == c){      /* c = 15 */
      ledAstate = 0
      interval = interval + d;
      counter = 0;
      }
      
    else{
      if (ledAstate = 1){
        ledAstate = 0;
        counter ++;
        a = a + (c * 50);
        
    
      }
      
      else{
        ledAstate = 0;
      digitalWrite(ledA, ledAstate);
      
    digitalWrite(ledA, ledAstate);
      }
      
      }
  }
}   


  
void signalB(){
  unsigned long currentTime = micros();
  /*for (int counter=0; counter<c; counter++)*/ 
  ledBstate = 1;
  if (currentTime - prevTime >= sigBlength){
    if (ledBstate == 1) {
      ledBstate = 0:
      ledAstate = 1;
    }
    else {
    
    digitalWrite(ledB, ledBstate);
    digitalWrite(ledA, ledAstate);

    }
  }
}
