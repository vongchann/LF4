// Very basic line follower

// Testing Git sync is working properly

// No turning to the correct angle - just sensor calibration on one eye.

// After calibration the bot will shift backwards momentarily to indicate it's ready

// Then it begins. 

// Ultra-simple algorithm: ww - straight. bw - forward left. wb - forward right. bb - stop.

// By Leo 2016

// Eye variables

const int leftEye = A0;
const int rightEye = A1;

int sensorValue = 0;
int bestHigh = 0;
int bestLow = 1023;

int riseAbove = 0;
int dropBelow = 0;

// Motor variables

const int enable_left = 9;
const int enable_right = 5;

const int dir1_left = 4;
const int dir2_left = 6;
const int dir1_right = 7;
const int dir2_right = 8;

int last_left = 0; // motor settings
int last_right = 0;

int left = 0;
int right = 0;

int full_speed = 120;
int half_speed = 5;

// Extras

const int indicatorLED = 3;

void setup(){
  
  // Prepare to talk to the computer
  
  Serial.begin(9600);
  
  pinMode(indicatorLED, OUTPUT);
  
  pinMode(9, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  // Enable pins start at 0 speed
  analogWrite(enable_left, 0);
  analogWrite(enable_right, 0);
  
  // Direction pins start in forwards state (0 speed)
  digitalWrite(dir1_left, HIGH);
  digitalWrite(dir2_left, LOW);
  digitalWrite(dir1_right, HIGH);
  digitalWrite(dir2_right, LOW);
  
  // Scan the right eye sensor for levels
  scan(rightEye);
  
  // Jog forwards very briefly to indicate it's ready to go.
  analogWrite(enable_left, 255);
  analogWrite(enable_right, 255);
  delay(100);
  
  analogWrite(enable_left, 0);
  analogWrite(enable_right, 0);
  delay(2000);
  
  Serial.println("SETUP DONE!");
  
}

// In this code the motors will never be in reverse - so we'll ignore the direction pins.

void scan(int sensor){
  
  // Take readings from the reflectance sensor over a period of 5 seconds.
  // Choose the highest and the lowest reflected value, and report them over Serial.
  // Suggest a cut-off point (with hysterisis) for the line-follower to use.
  
  Serial.println("Beginnning 5 second scan from right eye...\n");
  
  digitalWrite(indicatorLED, HIGH);
  delay(500);
  
  for (int i = 0; i < 250; i++) { // do this 50 times
    
    sensorValue = analogRead(sensor);
    
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
  
  dropBelow = bestLow + round(0.44 * difference);
  riseAbove = bestHigh - round(0.44 * difference);
  
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
  
  // And that's it.
  
  Serial.println("Scan complete\n");
  
  digitalWrite(indicatorLED, LOW);
  
}

void motors(int l, int r){
  analogWrite(enable_left, l);
  analogWrite(enable_right, r);
  Serial.print("Left : \t");
  Serial.print(l);
  Serial.print("\tRight : \t");
  Serial.println(r);
}  

void loop(){
  
  // Now use the values defined in setup() and scan() to perform a line following. 
  
  // Terminate on the condition that you see black on both sensors simultaneously.
  
  boolean finished = false;
  
  while (!finished){
    
    int left_val = analogRead(leftEye);
    int right_val = analogRead(rightEye);
    
    // Now change the motors' state depending on current sensor reading and previous motor values.
    
    if (last_left < full_speed){
      last_left = left;
      if (left_val >= riseAbove){
        left = full_speed;
      }
    }
    else {
      last_left = left;
      if (left_val <= dropBelow){
        left = half_speed;
      }
    }
    
    if (last_right < full_speed){
      last_right = right;
      if (right_val >= riseAbove){
        right = full_speed;
      }
    }
    else {
      last_right = right;
      if (right_val <= dropBelow){
        right = half_speed;
      }
    }
    
//    if ((left == half_speed) && (right == half_speed)){
//      
//      // We have reached a black line. Stop the bot.
//      
//      left = 0;
//      right = 0;
//      
//      finished = true;
//      
//    }
    
    // Set the speed
    motors(left,right);
    delay(10); // Re-evaluate 10 times a second.
  }
}
