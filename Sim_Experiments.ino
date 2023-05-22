volatile int count;
int ENA = 10;      // Enable Pin
int TEST_LED = 11; // Test LED Pin
int IN1 = 9;       // IN1 Pin
int IN2 = 8;       // IN2 Pin
int INTA = 6;      // Interrupt A Pin
int INTB = 7;      // Interrupt B Pin
int SENSE_1 = 22;  // Ball sensor read pin
float desiredPosition = 0;
float storedPosition = 0;
unsigned long positionTimer = 0;
unsigned long controlTimer = 0;
float kP = 0.0077;
float kD = -0.25;
float kDD = -0.32;
float vp = 0;
float vd = 0;
float vdd = 0;
float vOut = 0;
float mappedPos = 0;
float position = 0;
float degrees = 0;
float pyOutput = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(1);
  // Initialising Pins  and Interrupts
  pinSetup();
  // Getting intitial values
    position = analogRead(SENSE_1);
    mappedPos = map(position, 93, 976, -865, 865);
    degrees = map(count, 0, 540, 0, 360);
    storedPosition = mappedPos;
}

void loop(){
  if(millis() - controlTimer >= 5 ) {
    controlTimer = millis();
    doMeasurements();
    doControl();
    storedPosition = mappedPos;
    readImage();
  }

// Controlling the direction
  if(vOut < 0) {
    motorBackwards(map(vOut, 0, -15, 150, 255));
  } 

  if(vOut > 0) {
    motorForwards(map(vOut, 0, 15, 150, 255));
  }

  // Controlling between 30 degrees
  if(degrees >= 30 || degrees <= -30) {
    motorOff();
  }
}

void doControl() {
  vp = (desiredPosition - mappedPos)*kP;
  vd = (mappedPos - storedPosition)*kD;
  vdd = degrees * kDD;

  vOut = vp + vd + vdd;
}

void doMeasurements() {
  position = analogRead(SENSE_1);
  mappedPos = map(position, 93, 976, -865, 865);
  degrees = map(count, 0, 540, 0, 360);
   if(abs(mappedPos - storedPosition) >= 200) {
    mappedPos = storedPosition;
  }
}

void readImage() {
  if(Serial.available()){
    pyOutput = Serial.readString().toFloat(); 
    if(pyOutput >= 800) {
      pyOutput = 800;
    } 
    if(pyOutput <= -800) {
      pyOutput = -800;
    }
    desiredPosition = pyOutput;
  }
}

void doPrint() {
  Serial.print(position);
  Serial.print("     ");
  Serial.print(degrees);
  Serial.print("     ");
  Serial.print(vp);
  Serial.print("     ");
  Serial.print(vd);
  Serial.print("     ");
  Serial.print(vdd);
  Serial.print("     ");
  Serial.print(vOut);
  Serial.println("");
}

// Interrupt Service Routine A
void ISR_A()
{
  if (digitalRead(INTA) == HIGH)
  {
    if (digitalRead(INTB) == LOW)
    {
      count++;
    }
    else
    {
      count--;
    }
  }
  if (digitalRead(INTA) == LOW)
  {
    if (digitalRead(INTB) == HIGH)
    {
      count++;
    }
    else
    {
      count--;
    }
  }
}

// // Interrupt Service Routine B
void ISR_B()
{
  if (digitalRead(INTB) == HIGH)
  {
    if (digitalRead(INTA) == LOW)
    {
      count--;
    }
    else
    {
      count++;
    }
  }
  if (digitalRead(INTB) == LOW)
  {
    if (digitalRead(INTA) == HIGH)
    {
      count--;
    }
    else
    {
      count++;
    }
  }
}

// Function for turning off the motor
void motorOff() {
  analogWrite(10, 0);
}

// Function to turn the motor clockwise
void motorForwards(int voltage) { 
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  analogWrite(ENA, voltage);
  
}

// Function to turn the motor anti-clockwise
void motorBackwards(int voltage) {
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  analogWrite(ENA, voltage);
}

void pinSetup() {
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(SENSE_1, INPUT);

  // Creating interrupts
  attachInterrupt(digitalPinToInterrupt(INTA), ISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTB), ISR_B, CHANGE);
}
