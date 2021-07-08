#include "Charliplexing.h"
#include "Font.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

void setup() {
    LedSign::Init();

}

static const char test[]="1";

void loop() {
  
    for (int8_t x=DISPLAY_COLS, i=0;; x--) {
  LedSign::Clear();
        for (int8_t x2=x, i2=i; x2<DISPLAY_COLS;) {
      int8_t w = Font::Draw(test[i2], x2, 0);
      x2 += w, i2 = (i2+1)%strlen(test);
      if (x2 <= 0)  // off the display completely?
    x = x2, i = i2;
  }
        delay(80);
    }
}
