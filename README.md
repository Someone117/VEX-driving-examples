# VEX-driving-examples
3 levels of VEX drive code, from beginner to advanced.


Why use voltage, not percent for motors:

TLDR: It is smoother and a quick change

In VEX motors, there is an encoder, which keeps track of how fast the motors are spinning (in RPM). These encoders are not perfect and have inherent error. This leads the motors to constantly overshoot and undershoot the target RPM, leading to inconsistent acceleration. By controlling the voltages, we can directly control the acceleration, which leads to smoother driving, and more control over torque. It's also not that hard to implement, so the change is worth it.

How to use:
1) Remember to set up your motor groups etc. (including stopping mode and torque).
2) Remember to include cmath for the intermediate and advanced drives
3) Copy the function and the helper function into your project (remember the logDrive function for the advanced drive)
4) Drive!


C++ documentation:
https://en.cppreference.com/w/

VEX API documentation:
https://www.robotmesh.com/studio/content/docs/vexv5-cpp//html/namespacevex.html (really complex, but it has everything)
