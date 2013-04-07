#pragma config(Sensor, S2,     HTSMUX,         sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: lego-light-SMUX-test1.c 133 2013-03-10 15:15:38Z xander $
 */

/**
 * lego-light.h provides an API for the Lego Light Sensor.  This program
 * demonstrates how to use that API through a SMUX.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: More comments
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.59 AND HIGHER.

 * Xander Soldaat (xander_at_botbench.com)
 * 25 November 2009
 * version 0.2
 */

#include "drivers/hitechnic-sensormux.h"
#include "drivers/lego-light.h"

// The sensor is connected to the first port
// of the SMUX which is connected to the NXT port S1.
// To access that sensor, we must use msensor_S1_1.  If the sensor
// were connected to 3rd port of the SMUX connected to the NXT port S4,
// we would use msensor_S4_3

// Give the sensor a nice easy to use name
const tMUXSensor LEGOLS = msensor_S2_2;

task main() {
  int raw = 0;
  int nrm = 0;
  int tempRaw = 0;
  int tempNrm = 0;

  bool active = true;

  // Turn the light on
  LSsetActive(LEGOLS);

  nNxtButtonTask  = -2;

  nxtDisplayCenteredTextLine(0, "Lego");
  nxtDisplayCenteredBigTextLine(1, "LIGHT");
  nxtDisplayCenteredTextLine(3, "SMUX Test");
  nxtDisplayCenteredTextLine(5, "Connect SMUX to");
  nxtDisplayCenteredTextLine(6, "S2 and sensor to");
  nxtDisplayCenteredTextLine(7, "SMUX Port 2");
  wait1Msec(2000);

  nxtDisplayClearTextLine(7);
  nxtDisplayTextLine(5, "Press [enter]");
  nxtDisplayTextLine(6, "to toggle light");
  wait1Msec(2000);

  while (true) {
    // The enter button has been pressed, switch
    // to the other mode
    if (nNxtButtonPressed == kEnterButton) {
      active = !active;
      if (!active)
        LSsetInactive(LEGOLS);
      else
        LSsetActive(LEGOLS);

      // wait 500ms to debounce the switch
      wait1Msec(500);
    }

    raw = LSvalRaw(LEGOLS);
    nrm = LSvalNorm(LEGOLS);

    if (raw != tempRaw){
    	nxtDisplayClearTextLine(5);
    	nxtDisplayTextLine(5, "Raw:  %4d", raw);
    	tempRaw = raw;
    }

    if (nrm != tempNrm){
    	nxtDisplayClearTextLine(6);
   		nxtDisplayTextLine(6, "Norm: %4d", nrm);
   		tempNrm = nrm;
   	}

    wait1Msec(50);
  }
}

/*
 * $Id: lego-light-SMUX-test1.c 133 2013-03-10 15:15:38Z xander $
 */
