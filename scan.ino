// Example of a function, scan(), that calibrates a reflectance sensor, ready to be used for line-following

// By Leo Jofeh 2016

const int reflectPin = A1;
const int indicatorLED = 3;

int sensorValue = 0;
int bestHigh = 0;
int bestLow = 1023;

void setup(){
  
  // Prepare to talk to the computer
  
  Serial.begin(9600);
  pinMode(indicatorLED, OUTPUT);
  
}

void loop(){
  
  // Listen for a command from the computer - the letter 'r'
  
  if(Serial.available()){
    char temp = Serial.read();
    if (temp == 'r'){
      Serial.println("Starting scan...");
      scan();
    }
  }
}

void scan(){
  
  // Take readings from the reflectance sensor over a period of 5 seconds.
  // Choose the highest and the lowest reflected value, and report them over Serial.
  // Suggest a cut-off point (with hysterisis) for the line-follower to use.
  
  for (int i = 0; i < 250; i++) { // do this 50 times
    
    sensorValue = analogRead(reflectPin);
    
    if (sensorValue > bestHigh) {
      bestHigh = sensorValue;
    }
    
    if (sensorValue < bestLow) {
      bestLow = sensorValue;
    }
    
    delay(20);
  }
  
  // Now we report the best values, and make a suggestion for cut-off.
  
  Serial.print("Lowest recorded value:\t");
  Serial.println(bestLow);
  Serial.print("Highest recorded value:\t");
  Serial.println(bestHigh);
  Serial.println("\n"); // give some space
  
  int difference = bestHigh - bestLow;
  
  int dropBelow = bestLow + round(0.3 * difference);
  int riseAbove = bestHigh - round(0.3 * difference);
  
  Serial.println("Suggested cut offs - ");
  Serial.print("Rising :\t");
  Serial.println(riseAbove);
  Serial.print("Falling :\t");
  Serial.println(dropBelow);
  
  Serial.println("\n"); // give some space
  
  
  // Finally reset these numbers
  
  sensorValue = 0;
  bestHigh = 0;
  bestLow = 1023;
  
  // And that's it. A real line-follower would then use this information to take a drive!
  
  // We will use it to make the LED on pin 13 display white or black
  
  use(riseAbove, dropBelow);
  
}

void use(int ra, int db){
  
  // This function uses the suggested cut-off values and lights up the LED accordingly, white is light, black is dark
  digitalWrite(indicatorLED, LOW);
  boolean on = false;
  
  for (int i = 0; i < 500; i++) { // do this 50 times
  
    sensorValue = analogRead(reflectPin);
    
    if ( !on && (sensorValue >= ra)) {
      digitalWrite(indicatorLED, HIGH);
      on = true;
    }
    
    if (on && (sensorValue <= db)) {
      digitalWrite(indicatorLED, LOW);
      on = false;
    }
    
    delay(20);
  }
  
  digitalWrite(indicatorLED, LOW);
}
  
  


