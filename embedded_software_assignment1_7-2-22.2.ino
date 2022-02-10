#define switch_1 23
#define switch_2 23
#define ledA 15
#define ledB 21


float a = 400*1000;
float b = 100*1000;
float c = 15;  // counter
float d = 2500*1000;
float sigBlength = 50*1000;

int ledAstate;
int ledBstate;
int counter;
long interval; // variable interval

unsigned long prevTime;
unsigned long currentTime;
int sigswitch;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins to the leds as outputs.
  
  pinMode(switch_1, INPUT); 
  pinMode(switch_2, INPUT); 
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  
  
  unsigned long currentTime;

  
  interval = 50*1000;
  
  
}

// the loop function runs over and over again forever
void loop() {
  currentTime = micros();
  if (sigswitch == 1){
    signalA();
  }
  else{
    signalB();
  }
}

void signalA(){
  

  
  if (currentTime - prevTime >= interval) {
    prevTime = currentTime;
    
    if (counter == c){      // c = 15 
      ledAstate = 0;
      interval = b + d;
      counter = 0;
      digitalWrite(ledA, LOW);
      sigswitch = 0;
      }
      
    else{
      
      if (ledAstate == 0){
        interval = a + (counter * 50 * 1000);
        counter++;
        ledAstate = 1;
        digitalWrite(ledA, HIGH);
      }
      
      else{
        ledAstate = 0;
        interval = b;
        digitalWrite(ledA, LOW);
      }  
    }
  }
  
}  


void signalB(){
  
  if (currentTime - prevTime >= interval){
    prevTime = currentTime;
    if (ledBstate == 1) {
      ledBstate = 0;
      interval = 0;
      sigswitch = 1;
      digitalWrite(ledB, LOW);
    }
    else {
      ledBstate = 1;
      interval = 50*1000;
      digitalWrite(ledB, HIGH);
    

    }
  
  }
}
