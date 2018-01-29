
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

#ifndef MAXMatrix_h
#define MAXMatrix_h

#include <Arduino.h>
#include <SPI.h>

#define MIN_DEVICES 1
#define MAX_DEVICES 255

#define MIN_SCANLIMIT 0x0
#define MAX_SCANLIMIT 0x7

#define MIN_INTENSITY 0x0
#define MAX_INTENSITY 0xf

// Strictly speaking, 10,000,000 is the max speed, but things might start getting glitchy at that speed.
#define MAX_SPI_CLOCK 10000000

// Register addresses
#define REG_NOOP        0x0
#define REG_DIGIT0      0x1
#define REG_DIGIT1      0x2
#define REG_DIGIT2      0x3
#define REG_DIGIT3      0x4
#define REG_DIGIT4      0x5
#define REG_DIGIT5      0x6
#define REG_DIGIT6      0x7
#define REG_DIGIT7      0x8
#define REG_DECODEMODE  0x9
#define REG_INTENSITY   0xa
#define REG_SCANLIMIT   0xb
#define REG_SHUTDOWN    0xc
#define REG_DISPLAYTEST 0xf


class MAXMatrix {
	public:

		// The MOSI and SCK pins are already defined in Arduino.h, so no need to specify here.
		MAXMatrix(uint8_t loadPin, uint8_t devs);

		void shutdown(bool shut);
		bool shutdown();
		void scanLimit(uint8_t limit);
		uint8_t scanLimit(); 
		void intensity(uint8_t intensity);
		uint8_t intensity();
		void spiClock(uint32_t clock);
		uint32_t spiClock();

		void clear();
		void paint(uint8_t *data);

	protected:
		void transfer();
		void shotgun(uint8_t reg, uint8_t data);

		uint8_t mLoadPin;
		uint8_t mNumDevices = 1;
		bool mShutdown = true;
		uint8_t mScanLimit = MAX_SCANLIMIT;
		uint8_t mIntensity = MAX_INTENSITY;
		uint32_t mSpiClock = MAX_SPI_CLOCK;

		// This matches the sequence the MAX72xx controller expects the bytes to come.
		struct packet_t {
			uint8_t reg : 4;
			uint8_t padding : 4;
			uint8_t data;
		};

		packet_t *mPackets = NULL;
};

#endif /* MAXMatrix_h */