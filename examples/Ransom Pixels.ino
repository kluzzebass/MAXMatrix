

/*

MIT License

Copyright (c) 2018 Jan Fredrik Leversund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/


#include <MAXMatrix.h>

// Number of displays in our chain
#define PANELS 5

// Which pin is the LOAD (CS) pin connected to
#define LOAD_PIN D4

// Instantiate a MAXMatrix object. Since MOSI and SCK is predefined
// there's no need to supply these.
MAXMatrix matrix(LOAD_PIN, PANELS);

// Allocate one byte per segment. Each 8x8 display has 8 segments.
uint8_t buffer[PANELS * 8];

// How long do we wait between each paint
unsigned int paintDelay = 20;

// When did we last paint the matrix?
unsigned long lastPaint = 0;

// Generate random garbage in our buffer
void garbage() {
	for (int i = 0; i < PANELS * 8; i++) {
		buffer[i] = (uint8_t)random(256);
	}
}

// Srsly, you know what this does.
void setup() {

	// Seed the random generator
	randomSeed(analogRead(A0));

	// Generate initial garbage
	garbage();

	// Reduce the transfer speed slightly if you experience
	// glitches. The MAX7219/7221 is theoretically capable of
	// handling up to 10MHz, but YMMV. 
	matrix.spiClock(8000000);

	// Enable the display
	matrix.shutdown(false);
}

// Srsly, you also know what this does.
void loop() {

	// Grab the current time
	unsigned long now = millis();

	// Is it time to paint again?
	if (now < (lastPaint + paintDelay)) {
		return; // No.
	}

	// Paint the buffer
	matrix.paint((uint8_t *)buffer);

	// Generate more garbage
	garbage();

	// Keep track of time
	lastPaint = now;
}
