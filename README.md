# RoBin
**R**o**B**in project is automatic bin cover opener which uses Arduino Nano board, servo motor and HCSR04 ultrasonic distance measuring sensor. 

RoBin can be put into open state for replacing trash bags by pressing push button.

When an obstacle is too far RoBin will close sooner.

**See it in action:**
[![See it in action](/video_thumb.png?raw=true)](https://vimeo.com/436891435)

# Software
PlatformIO project can be build and uploaded to Arduino with PlatformIO extension for VSCode. 

**IMPORTANT** If you want to build your own RoBin, you should tweak parameters first and test whole system without cover attached to prevent any damage to your bin:
```cpp
constexpr int DISTANCE_THRESH = 35; // [cm] obstacle distance threshold
constexpr int SERVO_LIFT_UP = 59; // [deg] lift up (open cover) angle of servo
constexpr int SERVO_NORMAL = 180; // [deg] normal (closed cover) angle of servo
constexpr int SERVO_SPEED_UP = 40; // [%] opening speed of the cover
constexpr int SERVO_SPEED_DOWN = 15; // [%] closing speed of the cover
constexpr int OPEN_TIME = 3000; // [ms] how long should be cover open
constexpr int REACTIVE_TIME = 5000; // [ms] don't lift up for REACTIVE_TIME after closing trash by button
```

# Mechanical
RoBin uses [servo linear rack](https://www.thingiverse.com/thing:2882877) to pull up bin cover. All 3D model `.stl` files are available in `mechanical` folder. 
Linear rack should be printed with stronger materials eg. PETG. For front box (electronic housing) can be used PLA. 
For assembly you will need couple of screws to attach servo to its housing. Servo MG995 is used. 
Linear rack can be attached with screws and nuts to any type of trash bins by making holes into trash bin.
Front box (electronic housing) consists of two parts: base and cover which should be glued together.

**NOTE** Front box needs some sanding with a file tool or sandpaper.

![Assembled](/assembled.jpg?raw=true "Assembled")

# Hardware
- Arduino Nano
- RGB LED
- Push button
- Servo MG995
- HC-SR04 ultrasonic module
- Resistors for RGB LED (eg. 220Ω)

Electronic components can be simply placed on universal PCB.

# Wiring
![Wiring](/wiring.png?raw=true "Wiring")
