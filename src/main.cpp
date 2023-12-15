#include <Arduino.h>
#include "DisplayManager.h"
#include "WifiManager.h"

#define CLK 4
#define DT 19
#define SW 5
#define BB 15

DisplayManager displayManager;
WifiManager wifiManager;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
unsigned long lastBButtonPress = 0;

void setup() {
  Serial.begin(9600);
  displayManager.begin();
	wifiManager.begin(displayManager);

	pinMode(SW, INPUT_PULLUP);
	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	pinMode(BB, INPUT_PULLUP);

  lastStateCLK = digitalRead(CLK);
}

void loop() {
  displayManager.loop();
	wifiManager.loop();

  // Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
			currentDir ="CCW";
		} else {
			// Encoder is rotating CW so increment
			counter ++;
			currentDir ="CW";
		}

		Serial.print("Direction: ");
		Serial.print(currentDir);
		Serial.print(" | Counter: ");
		Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Read the button state
	int btnState = digitalRead(SW);
	int bBtnState = digitalRead(BB);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			Serial.println("Button pressed!");
		}

		// Remember last button press event
		lastButtonPress = millis();
	}

	if (bBtnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastBButtonPress > 50) {
			Serial.println("BButton pressed!");
		}

		// Remember last button press event
		lastBButtonPress = millis();
	}

	// Put in a slight delay to help debounce the reading
	delay(1);
}