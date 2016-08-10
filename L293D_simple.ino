// Robot control using L293D plus IR remote from NEC TV.

#include <IRremote.h>

// motor pins
int enable_left = 9;
int enable_right = 5;

int dir1_left = 4;
int dir2_left = 6;
int dir1_right = 8;
int dir2_right = 7;

//IR rcv

int recvPin = 2;
IRrecv irrecv(recvPin); // IRrecv library implementation
decode_results results; // results structure for IR recv

boolean me = 1;

void setup(){

	Serial.begin(9600);
	pinMode(9, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
  pinMode(2, INPUT);


	// Enable pins start at 0 speed
	digitalWrite(enable_left, 0);
	digitalWrite(enable_right, 0);
	// Direction pins start in forwards state (0 speed)
	digitalWrite(dir1_left, HIGH);
	digitalWrite(dir2_left, LOW);
	digitalWrite(dir1_right, HIGH);
	digitalWrite(dir1_right, LOW);

	// IR rcv

	irrecv.enableIRIn(); // Start the receiver
  Serial.println("SETUP COMPLETE");

}

void loop(){
	//IR 
	if (irrecv.decode(&results)) {
	    byte c = 0; byte d = 0;
	    boolean ok = extractNEC(&d, &c, results.value);
	    Serial.println(results.value, HEX);
	    Serial.print("OK?:\t\t"); Serial.println(ok);
	    Serial.print("Device code:\t"); Serial.println(d);
	    Serial.print("Command code:\t"); Serial.println(c);

	    // Control code goes here

	    if ((d==0) && ok){
      		
      		switch(c){ // use numbers 1 to 9 to control direction. Use other buttons for speed
      			case 0: motors(0,0); break;
      			case 2: motors(1, 1); break;
      			case 4: motors(-1, 1); break;
      			case 5: motors(0,0); break; // centre
      			case 6: motors(1, -1); break;
      			case 8: motors(-1, -1); break;
            case 88: me = !me; motors(0,0); break; // topleft arrow activates/deactivates this bot. 86, 87 remain.
            case 20: if(me){special();} break; // Menu begins the special move.
      			default: break;
      		}
            
    	    }
            irrecv.resume(); // Receive the next value
        }

}

void special(){// Put something here to do a special move
  // You can use motors() and delay() as many times as you like
  // This should end with zeroing the motors again.
  motors(-1,-1);
  delay(500);
  motors(-1,1);
  delay(2000);
  motors(1,1);
  delay(500);
  motors(1,-1);
  delay(2000);
  motors(0,0);
            
}

void motors(int left, int right){


if (left>=0){digitalWrite(dir2_left, LOW);digitalWrite(dir1_left, HIGH);}
else {digitalWrite(dir1_left, LOW);digitalWrite(dir2_left, HIGH);}

if (right>=0){digitalWrite(dir2_right, LOW);digitalWrite(dir1_right, HIGH);}
else {digitalWrite(dir1_right, LOW);digitalWrite(dir2_right, HIGH);}

//DONT FORGET TO SET DIRECTIONS OFR EACH SIDE IN THIS FUNCTION!, PUT IN SOME usb DEBUGGING AND CHECK IT BEFORE ATTACHING MOTOS AND BATERY ETC.
	digitalWrite(enable_left, abs(left)); 
	digitalWrite(enable_right, abs(right)); 

}

// Code for IR reciever.
boolean extractNEC(byte *device, byte *command, unsigned long code){

  byte dev = code >> 24;
  byte ndev = (code << 8) >> 24;
  byte com = (code << 16) >> 24;
  byte ncom = (code << 24) >> 24;
  
  if (((com & ncom) == 0) && ((dev & ndev) == 0)){ // Everything is as it should be
    *device = reverse(dev); *command = reverse(com);
    return true;
  }
  else if (((dev & ndev) == 0) && ((com & ncom) != 0)){ // Mangled command code from recognised device
    *device = reverse(dev); *command = 0; 
    return false;
  }
  else if (((dev & ndev) != 0) && ((com & ncom) == 0)){ // Mangled device code for a recognised command
    *command = reverse(com); *device = 0; 
    return false;
  }
  else {
    *command = 0; *device = 0;
    return false;
  }
}

byte reverse(byte input){
	byte rev = 0;
  	for(int i=0;i<8;i++){
    	rev <<= 1;
    	rev += input & 1;
    	input >>= 1;
  }
  return rev;
}
