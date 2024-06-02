# NoRisk
## Introduction
<p align="justify">
In an age where technology seamlessly integrates with our daily lives, the concept of a <i>smart home</i> has evolved from a futuristic vision to a tangible reality. Central to this transformation are smart door systems, which epitomise the blend of convenience, security, and innovation. These systems, which were once a speculative idea in the realm of science fiction, now offer practical solutions to everyday challenges, enhancing the way we interact with our living spaces.

Smart door systems represent the convergence of advanced technologies such as the <i>Internet of Things (IoT)</i>, <i>artificial intelligence (AI)</i>, and <i>wireless communication</i>, designed to provide homeowners with a heightened sense of security, unparalleled convenience and sophisticated control over access to their properties. Traditional keys can be easily lost, stolen, or duplicated, posing a significant security risk. Smart door systems eliminate this vulnerability by reducing the need for physical key exchanges.

Smart door systems also facilitate seamless integration with broader smart home ecosystems. They can communicate with other devices such as lighting systems, thermostats, and home security systems to create a cohesive and automated living environment. </p>

## Overview
<p align="justify">
The Smart Door project showcased here introduces an innovative solution utilising a servo motor, Bluetooth module, push-button, and onboard LED to control and monitor door access. Its functionality lies in its ability to open the door upon delivering a bluetooth instruction message. When the bluetooth module receives an input, it triggers the VSD Squadron Mini to activate the Servo Motor, thereby initiating the door-opening mechanism. This technology-driven approach offers a hands-free and user-friendly solution for creating a secure and convenient door access solution. 
<ul>
<li><b>Servo Motor (Pin 6)</b>: Acts as the locking mechanism, controlled by the microcontroller.</li>
<li><b>Bluetooth Module (Pins 3, 4, 7, GND, 5V)</b>: Enables wireless communication between the door system and a user’s smartphone for remote control and Bluetooth module's State pin indicates connection status.</li>
<li><b>Onboard Push-Button (Pin 2)</b>: Provides manual control of the lock, allowing toggling between locked and unlocked states.</li>
<li><b>Onboard LED (LED_BUILTIN)</b>: Displays the system status, indicating locked/unlocked states.</li>
</ul>
Users can control the door lock via their smartphone using Bluetooth, or manually using the onboard button. The onboard LED provides visual feedback, making the system user-friendly and reliable. This project exemplifies the practical application of IoT technology in home security and automation.
</p>

## Components required with Bill of Materials
| Item                   | Quantity | Description                                                   | Links to Products                                      |
|------------------------|----------|---------------------------------------------------------------|---------------------------------------------------|
| VSD Squadron Mini      | 1        | Microcontroller board                                        | [VSD Squadron Mini ](https://pages.razorpay.com/vsdsqmnMAY24)           |
| Servo Motor            | 1        | Standard servo motor (e.g., SG90)                            | [SG90 Servo Motor](https://amzn.in/d/bPAGDrt)       |
| Bluetooth HC-05 Module | 1        | Bluetooth module for serial communication                    | [Bluetooth HC-05](https://amzn.in/d/8OtBQpe)        |
| Jumper Wires           | 1 set    | Male-to-male and female-to-male jumper wires                 | [Jumper Wires](https://amzn.in/d/abTh8bo)           |
| Micro USB Cable        | 1        | For programming and power supply                             | [Micro USB Cable](https://amzn.in/d/9b3ttSo)        |

## Table for Pin Connections
| Component            | VSD Squadron Mini Pin | Description                                             |
|----------------------|-----------------------|---------------------------------------------------------|
| Servo Signal         | PC6                   | Servo motor's control signal pin                        |
| Servo VCC            | 5V                    | Servo motor's power supply                              |
| Servo GND            | GND                   | Servo motor's ground                                    |
| Bluetooth RXD        | PD6                   | Bluetooth module's RXD pin (connect directly to 3.3V logic) |
| Bluetooth TXD        | PD7                   | Bluetooth module's TXD pin (connect directly to 5V logic) |
| Bluetooth GND        | GND                   | Bluetooth module's ground pin                           |
| Bluetooth VCC        | 3.3V                  | Bluetooth module's 3.3V pin (connect to microcontroller's 3.3V output pin) |
| Bluetooth State      | PC7                   | Bluetooth module's State pin (indicates connection status) |
| Onboard LED          | LED_BUILTIN           | Onboard LED for status indication                       |

## Pinout Diagram
![image](https://github.com/rmahathi/NoRisk/assets/167225765/b325a314-1d12-409b-9268-ac65b9c4823d)

## Working Code
```
// Define the pins using port names
#define MOTOR_PIN PC6 // Pin to which the servo motor is connected
#define BLUETOOTH_STATE_PIN PC7
#define LED_PIN LED_BUILTIN

#define BLUETOOTH_RX_PIN PD6  // Onboard RX pin
#define BLUETOOTH_TX_PIN PD7  // Onboard TX pin

bool doorLocked = true; // Flag to track the door lock state

// Function prototypes
void checkBluetooth();
void openDoor(int openDelay = 5000);
void closeDoor();
void lockDoor();
void setServoAngle(int angle);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Smart Door System Initialized");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BLUETOOTH_STATE_PIN, INPUT);

  pinMode(MOTOR_PIN, OUTPUT); // Set the motor pin as an output
  
  lockDoor(); // Lock the door initially
}

void loop() {
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
}

void checkBluetooth() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'O') {  // Command to open the door
      openDoor();
    } else if (command == 'C') {  // Command to close the door
      closeDoor();
    }
  }
}

void openDoor(int openDelay) {
  Serial.println("Access Granted");
  digitalWrite(LED_PIN, HIGH);
  
  // Set PWM signal to open the door
  setServoAngle(90); // Assuming 90 degrees to open the door
  delay(openDelay); // Keep door open for the specified delay
  closeDoor();
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
```
## Demo Video
https://github.com/rmahathi/NoRisk/assets/167225765/7a30f59d-298e-453f-b36c-02a275bf6081



