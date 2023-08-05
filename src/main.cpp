/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Sa'id Badareen                                            */
/*    Created:      Sat Aug 05 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// LeftDrive            motor_group   1, 2
// RightDrive           motor_group   3, 4
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath> // std::abs and std::pow

using namespace vex;

// utility, we are assuming arcade drive (one joystick for forward-reverse, one for left-right)
controller::axis driveAxis() { return Controller1.Axis3; }
controller::axis turnAxis() { return Controller1.Axis1; }

/**
Beginner drive program, gets the job done

Pros:
Simple
Easy to understand
Easy to implement

Cons:
Can't drive and turn at the same time
Many joysticks have drift

**/
void beginnerDrive() {
  while (true) { // forever
    int drive =
        driveAxis().position(); // variables that represent joystick positions
    int turn = turnAxis().position();

    if (turn > drive) { // if you are turning or driving
      LeftDrive.spin(vex::forward, turn,
                     percent); // turn, one motor is negative velocity
      RightDrive.spin(vex::forward, -turn, percent);
    } else {
      LeftDrive.spin(vex::forward, drive,
                     percent); // drive, both same velocity
      RightDrive.spin(vex::forward, drive, percent);
    }
  }
}

// helper function
void intermediateDriveHelper(double d, double t) {
  double r = d + t;
  double l = d - t;

  if (r > 12.8) // cap voltage at +/- 12.8
    r = 12.8;
  else if (r < -12.8)
    r = -12.8;

  if (l > 12.8)
    l = 12.8;
  else if (l < -12.8)
    l = -12.8;

  if (l == 0) { // stop motors at coast if voltage is 0
    LeftDrive.stop(coast);
  }
  if (r == 0) {
    RightDrive.stop(coast);
  }
  LeftDrive.spin(vex::forward, l,
                 volt); // voltage already reversed, so run them forward
  RightDrive.spin(vex::forward, r, volt);
}

/**
Intermediate drive program, gets the job done

Pros:
Simple
Deals with voltages (more above)
Can drive and turn
Has most of what you may want/need

Cons:
Lacks advanced features like logarithmic driving

**/
void intermediateDrive() {
  while (true) {         // forever
    while (true) {       // forever
      int deadZone = 15; // these joysticks are not accurate
      int drive = driveAxis().position();
      int turn;

      while (true) {
        // Drive/turn
        drive = driveAxis().position();
        turn = turnAxis().position();

        // deadzone
        if (std::abs(drive) < deadZone)
          drive = 0;
        if (std::abs(turn) < deadZone)
          turn = 0;

        intermediateDriveHelper(drive, turn); // actually drive
      }
    }
  }
}

double logDrive(double v, double pow) { // drive with a curve to change how the
                                        // joystick responds, drivers like this
  if (v > 0) {                          // keep sign
    return (std::pow(std::abs(v), pow) / std::pow(100, pow)) *
           12.8; // https://www.desmos.com/calculator/uofc21yvlq this is
                 // essentially the function, change "a" in the graph to change
                 // pow
  } else {
    return -1 * (std::pow(std::abs(v), pow) / std::pow(100, pow)) * 12.8;
  }
}

void advancedDriveHelper(double d, double t, bool flipDrive) {
  if (flipDrive) { // invert drive
    d *= -1;
  }
  d = logDrive(d, 2); // different turn and drive logs
  t = logDrive(-t, 3.5);
  double r = d + t;
  double l = d - t;

  if (r > 12.8) // cap voltage at +/- 12.8
    r = 12.8;
  else if (r < -12.8)
    r = -12.8;

  if (l > 12.8)
    l = 12.8;
  else if (l < -12.8)
    l = -12.8;

  if (l == 0) { // stop motors at coast if voltage is 0
    LeftDrive.stop(coast);
  }
  if (r == 0) {
    RightDrive.stop(coast);
  }
  LeftDrive.spin(vex::forward, l,
                 volt); // voltage already reversed, so run them forward
  RightDrive.spin(vex::forward, r, volt);
}

/**
Advanced drive program, works very well

Pros:
Deals with voltages (more above)
Has basically everything you need
Can reverse the drive direction easily

Cons:
Harder to understand and implement

**/
void advancedDrive() {
  bool flip = false;   // do we invert the drive
  while (true) {       // forever
    int deadZone = 15; // these joysticks are not accurate
    int drive = driveAxis().position();
    int turn;

    while (true) {
      // Drive/turn
      drive = driveAxis().position();
      turn = turnAxis().position();

      // deadzone
      if (std::abs(drive) < deadZone)
        drive = 0;
      if (std::abs(turn) < deadZone)
        turn = 0;

      advancedDriveHelper(drive, turn, flip); // actually drive

      // Drive inversion
      if (Controller1.ButtonB.pressing()) {
        flip = !flip;
        this_thread::sleep_for(
            100); // pause to not flip the drive constantly (debounce)
      }
      if (flip) {
        Controller1.rumble("-"); // Gives feedback to the driver
      }
      this_thread::sleep_for(
          50); // if using threads keep this, otherwise remove it
    }
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  LeftDrive.setStopping(coast);
  LeftDrive.setMaxTorque(100, percent);
  RightDrive.setStopping(coast);
  RightDrive.setMaxTorque(100, percent);

  // uncomment to use
  // beginnerDrive();
  // intermediateDrive();
  // advancedDrive();
}