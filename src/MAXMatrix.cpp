
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

// The MOSI and SCK pins are already defined in Arduino.h
MAXMatrix::MAXMatrix(uint8_t loadPin, uint8_t devs = 1) {
	mLoadPin = loadPin;
	if (devs >= MIN_DEVICES && devs <= MAX_DEVICES) {
		mNumDevices = devs;
	}

	mPackets = (packet_t *)malloc(mNumDevices * sizeof(packet_t));

	pinMode(MOSI, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(mLoadPin, OUTPUT);
	digitalWrite(mLoadPin, HIGH);

	SPI.begin();

	shotgun(REG_DECODEMODE, 0);
	shotgun(REG_DISPLAYTEST, 0);
	intensity(1);
	scanLimit(MAX_SCANLIMIT);
	clear();
	shutdown(true);
}

void MAXMatrix::transfer() {
	 // For storing incoming garbage
	uint8_t dummy[mNumDevices * sizeof(packet_t)];

	SPI.beginTransaction(SPISettings(mSpiClock, MSBFIRST, SPI_MODE0));
	digitalWrite(mLoadPin, LOW);

	SPI.transferBytes((uint8_t *)mPackets, dummy, mNumDevices * sizeof(packet_t));

	digitalWrite(mLoadPin, HIGH);
	SPI.endTransaction();
}

void MAXMatrix::spiClock(uint32_t clock) {
	if (clock > MAX_SPI_CLOCK) {
		return;
	}

	mSpiClock = clock;
}

uint32_t MAXMatrix::spiClock() {
	return mSpiClock;
}


void MAXMatrix::shutdown(bool shut) {
	mShutdown = shut;
	shotgun(REG_SHUTDOWN, !mShutdown);
}

bool MAXMatrix::shutdown() {
	return mShutdown;
}

void MAXMatrix::scanLimit(uint8_t limit) {
	if (limit > MAX_SCANLIMIT) {
		return;
	}

	mScanLimit = limit;
	shotgun(REG_SCANLIMIT, mScanLimit);
}

uint8_t MAXMatrix::scanLimit() {
	return mScanLimit;
}

void MAXMatrix::intensity(uint8_t intensity) {
	if (intensity > MAX_INTENSITY) {
		return;
	}

	mIntensity = intensity;
	shotgun(REG_INTENSITY, mIntensity);
}

uint8_t MAXMatrix::intensity() {
	return mIntensity;
}

void MAXMatrix::shotgun(uint8_t reg, uint8_t data) {
	for (int i = 0; i < mNumDevices; i++) {
		mPackets[i].reg = reg;
		mPackets[i].data = data;
	}
	transfer();
}

void MAXMatrix::clear() {
	for (int j = 0; j <= mScanLimit; j++) {
		shotgun(REG_DIGIT0 + j, 0);
	}
}

void MAXMatrix::paint(uint8_t *data) {
	for (int j = 0; j <= mScanLimit; j++) {
		for (int i = 0; i < mNumDevices; i++) {
			mPackets[mNumDevices - i - 1].reg = REG_DIGIT0 + j;
			mPackets[mNumDevices - i - 1].data = data[j * mNumDevices + i];
		}
		transfer();
	}
}
