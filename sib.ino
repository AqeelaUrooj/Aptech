 #include <TM1637Display.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h> 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int temp=0;
int ledPin   = 7;             // choose the pin for the LED
int inputPin = 12;             // choose the input pin (for PIR sensor)
int pirState = HIGH;          // we start, assuming no motion detected
int val      = LOW;             // variable for reading the pin status
int minSecAfterPIRLow = 5000; // If the sensor reports low 
int relay_pin=5;
int x=0;
int add= 7;
bool ex=1;
long unsigned int timeTurnedLow;                  
boolean takeLowTime;  //This variable is used to record the event where the sensor turns off

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;  
int NumStep = 0; 

const int CLK = 3; //Set the CLK pin connection to the display
const int DIO = 2  ; //Set the DIO pin connection to the display
 TM1637Display display(CLK, DIO);
void setup() {


  display.setBrightness(0x0a);
   pinMode(11,OUTPUT);
   pinMode(9,INPUT);
  pinMode(relay_pin,OUTPUT);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(relay_pin,HIGH);
  digitalWrite(11,HIGH);
 

   mlx.begin();
  takeLowTime = LOW;  
 
 Serial.begin(9600);
 display.showNumberDec(NumStep); 

}
 
void loop(){
  while(x==0)
  {
     
     
       pinMode(relay_pin,HIGH);
      Serial.println("Waiting for the sensor to warm up.");
      delay(calibrationTime * 1000); // Convert the time from seconds to milliseconds.
      x=1;
    }
    
    if(digitalRead(9)==HIGH)
    {
      NumStep=0;
      }
    
    Serial.println("SENSOR ACTIVE");
    
    Serial.print(mlx.readObjectTempF()+add);
    Serial.println(" °F");
    temp =mlx.readObjectTempF()+add;
  
    while(temp>100 && temp<109)
  {
    temp =mlx.readObjectTempF()+add;
   Serial.print(mlx.readObjectTempF()+add);
    Serial.println(" °F");
    display.showNumberDec(mlx.readObjectTempF()+add);
    digitalWrite(relay_pin,HIGH);
   

   
  }
    digitalWrite(relay_pin,LOW);
    
  if(x==1)
  {
  val = digitalRead(inputPin);
  }
  Serial.print(val); 
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
 
    if (pirState == LOW && ex==1) {
      // we have just turned on
      Serial.println("Motion detected!");
       NumStep++;
       display.showNumberDec(NumStep);
       Serial.print(NumStep);
       ex=0;
      // We only want to print on the output change, not state
      pirState = HIGH;
      
    }
  
  } else {
     
    if (pirState == HIGH){
      // we have just turned off
      Serial.println("Motion ended!");   
      // We only want to print on the output change, not state
      pirState = LOW;
      timeTurnedLow = millis(); //take the millis at the moment the sensor went LOW
    }
  }
   
  //This block checks to see if enough time has passed after the PRI went LOW.
  //If yes, and assuming that the PIR sensor did not go HIGH again, turn off the LED
  if(!pirState && (millis() - timeTurnedLow) > minSecAfterPIRLow){
     Serial.println("Extended time HIGH ended!");      // turn LED OFF
     digitalWrite(ledPin, LOW);   
     ex=1;
   
    }

  delay(1000);
  


  
}
