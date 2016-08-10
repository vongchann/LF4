// This program will control the 2 mini motors on our dancebots and 
// should get you used to how they work. Because motors tend to suck
// a lot of power, we will let the Nano tell them the direction and 
// speed, then let another chip called L293D actually provide the power.

// L293D can control two motors at once, and it can run them forwards or 
// backwards at variable speed. Each side of the chip controls 1 motor, 
// using 3 inputs from the Nano : Enable, Direction1 and Direction2.  

// We will combine speeds and directions to create different motion on our
// dancebots. Later, we will use IR control, but for now we will just 
// hardcode the movements we want in 'loop()' below.

// You can add your own moves into loop() using motors() and delay().

// The function 'motors()' takes two simple numbers and then uses them to
// tell the L293D what we want it to do. It looks a little complicated 
// because we don't want to accidently short-circuit the motors by sending
// them instructions to turn both ways at once.

// motor pins
int enable_left = 9;
int enable_right = 5;

int dir1_left = 4;
int dir2_left = 6;
int dir1_right = 7;
int dir2_right = 8;

// state variables for motor control
int LEFT = 0;
int RIGHT = 0;
int SPEED_MULT = 0;

void setup(){

	Serial.begin(9600);
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
	digitalWrite(dir1_right, LOW);
  Serial.println("DONE!");

}

void loop(){

  motors(250, 250);
  Serial.println("Forwards");
  delay(1500);
  motors(-250, 250);
  Serial.println("Anti-clockwise");
  delay(1500);
  motors(-250, -250);
  Serial.println("Backwards");
  delay(1500);
  motors(250, -250);
  Serial.println("Clockwise");
  delay(1500);
	
}

void motors(int left, int right){

	// left and right are +- 0->255 indicating desired speed of each motor

	// First check if there is a change of direction, if so, make the motor safe before switching.

	if ((left<0)&&(LEFT>=0)||(left>=0)&&(LEFT<0)){ // left has switched direction
		if (left>=0){digitalWrite(dir2_left, LOW);digitalWrite(dir1_left, HIGH);}
		else {digitalWrite(dir1_left, LOW);digitalWrite(dir2_left, HIGH);} // Set the direction safely
                Serial.println("Dir set left");
	}
	if ((right<0)&&(RIGHT>=0)||(right>=0)&&(RIGHT<0)){ // right has switched direction
		if (right>=0){digitalWrite(dir2_right, LOW);digitalWrite(dir1_right, HIGH);}
		else {digitalWrite(dir1_right, LOW);digitalWrite(dir2_right, HIGH);} // Set the direction safely
                Serial.println("Dir set right");
	}
	// If these are not true then the direction does not require any change.

	// Set the speed

	LEFT = left; // Update the state machine
	RIGHT = right;
	analogWrite(enable_left, abs(LEFT)); Serial.print("Left motor set:\t");Serial.println(LEFT);// Set the output speed
	analogWrite(enable_right, abs(RIGHT)); Serial.print("Right motor set:\t");Serial.println(RIGHT);

} // That's all for now folks!