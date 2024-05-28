# NoRisk
## Introduction
<p align="justify">
In an age where technology seamlessly integrates with our daily lives, the concept of a <i>smart home</i> has evolved from a futuristic vision to a tangible reality. Central to this transformation are smart door systems, which epitomise the blend of convenience, security, and innovation. These systems, which were once a speculative idea in the realm of science fiction, now offer practical solutions to everyday challenges, enhancing the way we interact with our living spaces.

Smart door systems represent the convergence of advanced technologies such as the <i>Internet of Things (IoT)</i>, <i>artificial intelligence (AI)</i>, and <i>wireless communication</i>, designed to provide homeowners with a heightened sense of security, unparalleled convenience and sophisticated control over access to their properties. Traditional keys can be easily lost, stolen, or duplicated, posing a significant security risk. Smart door systems eliminate this vulnerability by reducing the need for physical key exchanges.

Smart door systems also facilitate seamless integration with broader smart home ecosystems. They can communicate with other devices such as lighting systems, thermostats, and home security systems to create a cohesive and automated living environment. 

## Overview
<p align="justify">
The Smart Door project showcased here introduces an innovative solution utilisingThe system integrates a servo motor, Bluetooth module, push-button, and onboard LED to control and monitor door access seamlessly. Its core functionality lies in its ability to open the door upon delivering a bluetooth instruction message. Here, a Servo Motor is placed on the door. When the bluetooth module receives an input, it triggers the VSD Squadron Mini to activate the Servo Motor, thereby initiating the door-opening mechanism. This technology-driven approach offers a hands-free and user-friendly solution for creating a secure and convenient door access solution. 
<ul>
<li><b>Servo Motor (Pin 6)</b>: Acts as the locking mechanism, controlled by the microcontroller.</li>
<li><b>Bluetooth Module (Pins 3, 4, 7, GND, 5V)</b>: Enables wireless communication between the door system and a user’s smartphone for remote control and Bluetooth module's State pin indicates connection status.</li>
<li><b>Onboard Push-Button (Pin 2)</b>: Provides manual control of the lock, allowing toggling between locked and unlocked states.</li>
<li><b>Onboard LED (LED_BUILTIN)</b>: Displays the system status, indicating locked/unlocked states.</li>
</ul>
Users can control the door lock via their smartphone using Bluetooth, or manually using the onboard button. The onboard LED provides visual feedback, making the system user-friendly and reliable. This project exemplifies the practical application of IoT technology in home security and automation.
</p>
