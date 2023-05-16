volatile int count;
int ENA = 10; // Enable Pin
int TEST_LED = 11; // Test LED Pin
int IN1 = 9; // IN1 Pin
int IN2 = 8; // IN2 Pin
int INTA = 6; // Interrupt A Pin
int INTB = 7; // Interrupt B Pin
int SENSE_1 = 22; // Ball sensor read pin
unsigned long lastSerialTime = 0;
const unsigned long serialInterval = 100;
bool motorOn = false;
unsigned long motorStartTime = 0;
int beamEnd = 985;
int beamStart = 82;
const desiredPosition = 0;

void setup() {
  Serial.begin(9600);
  // Initialising Pins  
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT); 
  pinMode(ENA, OUTPUT); 
  pinMode(SENSE_1, INPUT);

  // Creating interrupts
  attachInterrupt(digitalPinToInterrupt(INTA), ISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTB), ISR_B, CHANGE);
}




void loop() {

  // If statement using millis to store the old position and tune it.

  if(degrees > 30 || degrees < -30){
    vOut = 0;
  }

  float position = analogRead(SENSE_1);

  float mappedPos = map(position, 64, 981, -825, 825);

  float degrees = map(count, 0, 540, 0, 6.28);

  float vp = (desiredPosition - pos)*-100;
  float vd =  vdVar * kD;
  float vdd = degrees * kDD;

  float vOut = vp + vd + vdd;
  



  // Control the beam angle using if statements

}





// Interrupt Service Routine A
void ISR_A() {
if(digitalRead(INTA) == HIGH){
    if (digitalRead(INTB) == LOW){
        count++;
    } else {
      count--;
    }
  }
  if(digitalRead(INTA) == LOW) {
    if(digitalRead(INTB) == HIGH) {
      count++;
    } else {
      count--;
    }
  }
}

// Interrupt Service Routine B
void ISR_B() {
if(digitalRead(INTB) == HIGH){
    if (digitalRead(INTA) == LOW){
        count--;
    } else {
      count++;
    }
  }
  if(digitalRead(INTB) == LOW) {
    if(digitalRead(INTA) == HIGH) {
      count--;
    } else {
      count++;
    }
  }

}

// Function for turning off the motor
void motorOff() {
    analogWrite(10, 0);
}

// Function to turn the motor clockwise
void motorForwards(float voltage) {
  analogWrite(ENA, voltage);
  digitalWrite(8, HIGH);
  analogWrite(9, LOW);
}

// Function to turn the motor anti-clockwise
void motorBackwards(float voltage) {
  analogWrite(ENA, voltage);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
}
