// Define the pins using port names
#define MOTOR_PIN PC6 // Pin to which the servo motor is connected
#define BLUETOOTH_STATE_PIN PC7
#define LED_PIN LED_BUILTIN

#define BLUETOOTH_RX_PIN PD6  // Onboard RX pin
#define BLUETOOTH_TX_PIN PD7  // Onboard TX pin

bool doorLocked = true; // Flag to track the door lock state
bool faultCondition = false; // Flag to simulate a fault condition
unsigned long lastRebootTime = 0; // Time of the last reboot
const unsigned long rebootInterval = 60000; // Reboot every 60 seconds 

// Counter for controlling reboots
unsigned long loopCounter = 0;
const unsigned long rebootThreshold = 6000; // Reboot after 6000 loops 

const String authorizedCode = "OK"; // Example authorization code
bool authorized = false; // Flag to indicate if the user is authorized
unsigned long lastCommandTime = 0; // Time of the last received command
const unsigned long commandInterval = 5000; // Minimum interval between commands in milliseconds

// Function prototypes
void checkBluetooth();
void openDoor(int openDelay = 1000);
void closeDoor();
void lockDoor();
void setServoAngle(int angle);
void rebootSystem();
bool verifyAuthorization(String command);
bool debounceProtection();

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

  // Initialize the last reboot time
  lastRebootTime = millis();

  lockDoor(); // Lock the door initially
}

void loop() {
  // Increment loop counter
  loopCounter++;

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

  // Check if it's time to reboot based on loop counter
  if (loopCounter >= rebootThreshold) {
    Serial.println("Rebooting system...");
    rebootSystem();
  }
}

void checkBluetooth() {
  if (Serial.available()) {
    String command = Serial.readString();
    command.trim(); // Remove any leading/trailing whitespace

    if (!authorized) {
      if (verifyAuthorization(command)) {
        authorized = true;
        Serial.println("Authorization successful.");
      } else {
        Serial.println("Unauthorized command attempt.");
      }
    } else {
      if ((command == "O" || command == "C" || (command >= "A" && command <= "Z")) && debounceProtection()) {
        if (command == "O") {  // Command to open the door
          openDoor();
        } else if (command == "C") {  // Command to close the door
          closeDoor();
        } else {  // Command to simulate a fault
          faultCondition = true;
          Serial.println("Fault simulation command received");
        }
        authorized = false; // Reset authorization after a command is executed
      }
    }
  }
}

void openDoor(int openDelay) {
  Serial.println("Access Granted");
  digitalWrite(LED_PIN, HIGH);
  // Set PWM signal to open the door
  setServoAngle(90); // Assuming 90 degrees to open the door
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
  // Perform actions to save critical state before rebooting
  doorLocked = true;
  faultCondition = false;
  loopCounter = 0;
  lastRebootTime = millis(); // Update last reboot time
  // Code to reset the microcontroller or perform software reset
}

// Function to verify authorization
bool verifyAuthorization(String command) {
  return command == authorizedCode;
}

// Function for debounce protection
bool debounceProtection() {
  unsigned long currentTime = millis();
  if (currentTime - lastCommandTime > commandInterval) {
    lastCommandTime = currentTime;
    return true;
  } else {
    Serial.println("Command ignored due to debounce protection.");
    return false;
  }
}
