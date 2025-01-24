// Define the pins using port names
#define MOTOR_PIN PC6 // Pin to which the servo motor is connected
#define BLUETOOTH_STATE_PIN PC7
#define LED_PIN LED_BUILTIN

#define BLUETOOTH_RX_PIN PD6  // Onboard RX pin
#define BLUETOOTH_TX_PIN PD7  // Onboard TX pin

bool doorLocked = true; // Flag to track the door lock state
bool faultCondition = false; // Flag to simulate a fault condition
unsigned long lastRebootCheck = 0; // Time of the last reboot check
const unsigned long rebootCheckInterval = 1000; // Check for reboot every 1 second

// Function prototypes
void checkBluetooth();
void openDoor(int openDelay = 1000);
void closeDoor();
void lockDoor();
void setServoAngle(int angle);
void rebootSystem();

void setup() {
  Serial.begin(9600);
  delay(100); // delay to ensure serial connection is established
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("System Initialized");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BLUETOOTH_STATE_PIN, INPUT);

  pinMode(MOTOR_PIN, OUTPUT); // Set the motor pin as an output

  randomSeed(analogRead(0)); // Initialize random seed
  
  lockDoor(); // Lock the door initially
}

void loop() {
  // Check for fault condition
  if (faultCondition) {
    Serial.println("Fault Detected! System is locking the door.");
    lockDoor(); // Lock the door immediately
    return; // Skip the rest of the loop
  }

  // Check Bluetooth connection status
  if (digitalRead(BLUETOOTH_STATE_PIN) == HIGH) {
    checkBluetooth();
  } else {
    Serial.println("Bluetooth not connected");
    // Handle Bluetooth disconnection here (e.g., close the door)
    if (!doorLocked) {
      closeDoor();
    }
  }

  // Check if it's time to randomly reboot
  if (millis() - lastRebootCheck > rebootCheckInterval) {
    lastRebootCheck = millis();
    if (random(0, 100) < 10) { // 10% chance to reboot
      Serial.println("Random reboot triggered!");
      rebootSystem();
    }
  }
}

void checkBluetooth() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'O') {  // Command to open the door
      openDoor();
    } else if (command == 'C') {  // Command to close the door
      closeDoor();
    } else if ((command >= 'A' && command <= 'Z') && command != 'O' && command != 'C') {  // Command to simulate a fault
      faultCondition = true;
      Serial.println("Fault simulation command received");
    }
  }
} 

void openDoor(int openDelay) {
  Serial.println("Access Granted");
  digitalWrite(LED_PIN, HIGH);

  // Set PWM signal to open the door
  setServoAngle(90); // Assuming 90 degrees to open the door
  delay(openDelay); // Keep door open for the specified delay
}

void closeDoor() {
  Serial.println("Door Closing");
  digitalWrite(LED_PIN, LOW);

  // Set PWM signal to close the door
  setServoAngle(0); // Assuming 0 degrees to close the door
  doorLocked = true; // Update door lock state
}

void lockDoor() {
  doorLocked = true; // Update door lock state
  setServoAngle(0); // Initially lock the door by setting servo to 0 degrees
}

// Function to set the servo angle
void setServoAngle(int angle) {
  // Constants for the servo control
  const int minPulseWidth = 544;  // Minimum pulse width in microseconds
  const int maxPulseWidth = 2400; // Maximum pulse width in microseconds
  const int refreshInterval = 20000; // Refresh interval in microseconds (20 ms)

  // Map the angle to the pulse width
  int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);

  // Generate the PWM signal on PC6
  digitalWrite(MOTOR_PIN, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(MOTOR_PIN, LOW);

  // Wait for the remainder of the refresh interval
  delayMicroseconds(refreshInterval - pulseWidth);
}

void rebootSystem() {
  // Simulate a system reboot by resetting variables and re-initializing
  Serial.println("Rebooting system...");
  doorLocked = true;
  faultCondition = false;
  lastRebootCheck = millis();

  setup(); // Call setup to re-initialize the system
}