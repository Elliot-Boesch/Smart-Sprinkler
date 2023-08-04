# Adaptive Irrigation System

![plot](./images/smart-sprinkler-logo.png)

### Background
- Water scarcity is becoming a problem in our society as global warming worsens & water usage increase. To tackle this problem, we wanted to emphasize improving how homeowners water their lawns because 30% of household water usage is outdoor water and 50% of this water used is wasted.

### Goals
- Affordability to Consumer
- Measure Moisture
- Water Plants
- Wireless Capability
- Leak Detection
- Digital User Control

### Approach
- Capacitive Moisture Sensor
  - Adafruit moisture sensor has I2C interface to the pi and gives a reading from 200-2000
- Raspberry Pi
  - Collects the soil moisture and flow meter data and communicates to the website 
- Ultrasonic Transducers
  - The cold-water ultrasonic transducer collects data that can be used to calculate water flow rate
- TI Ultrasonic Sensing Evaluation Module
  - Calculates flow rate from ultrasonic transducer data and communicates this data to the Raspberry Pi
- Solenoid Water Valves
  - Dual solenoid water valves allow separate control of two watering zones with a signal from the Raspberry Pi 

### Tech Stack
![plot](./images/logos/react-logo.svg)
![plot](./images/logos/js-logo.svg)
![plot](./images/logos/python-logo.svg)


### Summary
- The Adaptive Irrigation System is an affordable, user-friendly addition to a manual sprinkler system that helps prevent water waste and ensure lawn health. 
  - Allows the user to use water more efficiently by letting the system turn off and on sprinklers depending on moisture in the soil.
  - Alert the user of any leaks in their sprinklers so it can be shut down through our website, preventing water waste.
  - Calculate and show water usage by the system to evaluate water savings and trends.
