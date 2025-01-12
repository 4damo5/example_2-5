//=====[Libraries]=============================================================

#include "arm_book_lib.h"
#include "mbed.h"

//=====[Defines]===============================================================

#define NUMBER_OF_KEYS 4

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverPresent(D2);
DigitalIn passengerPresent(D3);
DigitalIn driverSeatbelt(D4);
DigitalIn passengerSeatbelt(D5);
DigitalIn ignitionButton(D6);

DigitalOut greenIndicator(LED1);
DigitalOut alarmLed(LED3); //change to D7 and hook up alarm later
DigitalOut blueIndicator(LED2);

//=====[Declaration and initialization of public global variables]=============

bool alarmState = OFF;

int ignitionCheck[NUMBER_OF_KEYS]   = { 0, 0, 0, 0 };
int ignitionPermission[NUMBER_OF_KEYS] = { 1, 1, 1, 1};

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

bool alarmActivationUpdate();
bool alarmDeactivationUpdate();
bool areEqual();

//=====[Main function, the program entry point after power on or reset]========

int main() {
  inputsInit();
  outputsInit();
  while (true) {
    if (driverPresent) {
      printf("Welcome to enhanced alarm system model 218-W24");
    }
    while (driverPresent) {
      if (alarmDeactivationUpdate()) {
        greenIndicator = ON;
      }

      if (ignitionButton) {
        if (alarmDeactivationUpdate()) {
          printf("Engine started.");
          greenIndicator = OFF;
          blueIndicator = ON;

          while (true) {
          } // keeps the code stuck here
        } else {
          printf("Ignition inhibited");
          printf("Reasons:"); // needs work to show reasons
              alarmActivationUpdate();

          while (true) {
          } // keeps the code stuck here
        }
      }
    }
  }
}

//=====[Implementations of public functions]===================================

void inputsInit() {
  driverPresent.mode(PullDown);
  passengerPresent.mode(PullDown);
  driverSeatbelt.mode(PullDown);
  passengerSeatbelt.mode(PullDown);
  ignitionButton.mode(PullDown);
}

void outputsInit() {
  alarmLed = OFF;
  blueIndicator = OFF;
  greenIndicator = OFF;
}

bool alarmActivationUpdate() {
    ignitionCheck[0] = driverPresent;
    ignitionCheck[1] = passengerPresent;
    ignitionCheck[2] = driverSeatbelt;
    ignitionCheck[3] = passengerSeatbelt;

  if (ignitionButton && !areEqual()) {
    alarmState = ON;
  }

  alarmLed = alarmState;
  return true;
}

bool alarmDeactivationUpdate() {
    ignitionCheck[0] = driverPresent;
    ignitionCheck[1] = passengerPresent;
    ignitionCheck[2] = driverSeatbelt;
    ignitionCheck[3] = passengerSeatbelt;

  if (ignitionButton && areEqual()) {
    alarmState = OFF;
  }

  alarmLed = alarmState;
  return false;
}

bool areEqual()
{
    int i;

    for (i = 0; i < NUMBER_OF_KEYS; i++) {
        if (ignitionCheck[i] != ignitionPermission[i]) {
            return false;
        }
    }

    return true;
}