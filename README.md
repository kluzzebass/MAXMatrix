# MAXMatrix - MAX7219/7221 LED Display Driver Library

None of the dozen or so MAX7219/7221 LED Display Driver libraries that I've been able to find for Arduino did
what I needed in an efficient way (they were all slow, flickering, not using SPI, etc.), so I wrote
my own library. At this point, it's only been tested on ESP8266 boards.

## A word of warning ##

This library does not mollycoddle you. If you're looking for a library that has fonts, scroll
texts, generic graphics functions, etc., this is not it. It does one thing; it transfers an
array of bytes to set daisy chained displays efficiently. How you manipulate that byte array
to display what you need is entirely up to you.

## Working principles ##

The MAX7219/7221 LED Display Driver is controlled by shifting in 16 bit packets, in which 4 bits are throwaway
bits, 4 bits are used as a register address, and 8 bits are used as a data value. As more packets are
shifted in, the previous ones are shifted out the other end, so that if you have multiple drivers, the
packets get shifted down the line until you're ready to latch everything into place.

The display driver can handle up to eight 8-bit segments (dots on a matrix or digits on a number display), but
you can only set one segment at a time per display driver. This doesn't mean that you need to update
the same segment on all display drivers, though; you could conceivably update segment 1 on the first display driver,
segment 2 on the second display driver, and so on. In reality, though, you'll be updating the same segment on all the
display drivers down the line.

So, to do a complete repaint of a 4 display driver matrix (8x8x4 pixels), you'll do something like this:

1. Allocate a byte array large enough to contain 4 16-bit packets (that's 8 bytes), one packet for
   each display driver.
2. Fill the array with packets for segment 1, for all 4 display drivers.
3. Pull the LOAD line high, shift everything out to the display drivers, and pull the LOAD line low again
   to latch everything in.
4. Repeat step 2 and 3 for the remaining 7 segments.

Whatever was shifted out the last time around is left in the buffer, so you really need to shift out
a complete set of packets to clear out old stuff. Because of this, changing a single pixel or segment
becomes a relatively inefficient operation; so much so that you might as well just repaint the whole
thing. Completely repainting an 8 driver display (that's 8x64 pixels) takes about 1ms on an ESP8266
using this library, so it's not like you're crunched for time.






