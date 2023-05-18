volatile int count;
int ENA = 10;      // Enable Pin
int TEST_LED = 11; // Test LED Pin
int IN1 = 9;       // IN1 Pin
int IN2 = 8;       // IN2 Pin
int INTA = 6;      // Interrupt A Pin
int INTB = 7;      // Interrupt B Pin
int SENSE_1 = 22;  // Ball sensor read pin
unsigned long lastSerialTime = 0;
const unsigned long serialInterval = 100;
bool motorOn = false;
float desiredPosition = 0;
float storedPosition = 0;
unsigned long positionTimer = 0;
float kP = 0.01;
float kD = -0.007;
float kDD = -0.09;
float vp = 0;
float vd = 0;
float vdd = 0;
float vOut = 0;

void setup()
{
  Serial.begin(9600);
  // Initialising Pins  and Interrupts
  pinSetup();
}

// Test comment

void loop()
{
  float position = analogRead(SENSE_1);

  float mappedPos = map(position, 90, 970, -825, 825);

  float degrees = map(count, 0, 540, 0, 360);

  if(millis() - positionTimer >= 5) {
    positionTimer = millis();
    storedPosition = mappedPos;
  }

  if(abs(storedPosition - mappedPos) > 700) {
    mappedPos = storedPosition;
  }


  // Controllers
  vp = (desiredPosition - mappedPos)*kP;
  vd = (desiredPosition - storedPosition)*kD;
  vdd = degrees * kDD;
  vOut = vp + vd + vdd;

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



// Controlling the direction
  if(vOut < 0) {
    motorBackwards(map(vOut, 0, -4, 100, 255));
  } 

  if(vOut > 0) {
    motorForwards(map(vOut, 0, 4, 100, 255));
  }



  // Jump tolerance if statement


    if(degrees >= 30 || degrees <= -30) {
    motorOff();
  }


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
void motorOff()
{
  analogWrite(10, 0);
}

// Function to turn the motor clockwise
void motorForwards(int voltage)
{ 
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  analogWrite(ENA, voltage);
  
}

// Function to turn the motor anti-clockwise
void motorBackwards(int voltage)
{
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  analogWrite(ENA, voltage);
  
}

void pinSetup()
{
  pinMode(IN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(SENSE_1, INPUT);

  // Creating interrupts
  attachInterrupt(digitalPinToInterrupt(INTA), ISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTB), ISR_B, CHANGE);
}
