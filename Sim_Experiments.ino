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
unsigned long motorStartTime = 0;
int beamEnd = 985;
int beamStart = 82;
float desiredPosition = 0;
int vOut = 255;
float storedPosition = 0;
unsigned long positionTimer = 0;


void setup()
{
  Serial.begin(9600);
  // Initialising Pins  and Interrupts
  pinSetup();
  // motorBackwards(100);
}

// Test comment

void loop()
{

// Block to periodically store the position for delay between updates.
  

  // If statement using millis to store the old position and tune it.

  float position = analogRead(SENSE_1);

  float mappedPos = map(position, 64, 981, -825, 825);

  // float degrees = map(count, 0, 540, 0, 360);


  // Serial.println(degrees);


  // if(degrees >= 5 || degrees <= -5) {
  //   motorOff();
  // }


  // Currently gives us a value between -8.18 and +8.18
  float vp = (desiredPosition - mappedPos)*0.01 ;
  
  Serial.println(vp);

  // Controlling the directions, the magnitudes come fron the map function
  if(vp < 0) {
    motorBackwards(map(vp, 0, -8.1, 0, 150));
  } 

  if(vp > 0) {
    motorForwards(map(vp, 0, 8.1, 0, 150));
  }
  
  // float vd = vdVar * kD;
  // float vdd = degrees * kDD;

  // float vOut = vp + vd + vdd;

  if(millis() - positionTimer >= 10) {
    positionTimer = millis();
    storedPosition = mappedPos;
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
  Serial.println("FORWARDS");
  
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  analogWrite(ENA, voltage);
  
}

// Function to turn the motor anti-clockwise
void motorBackwards(int voltage)
{
  Serial.println("BACKWARDS");
  
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
