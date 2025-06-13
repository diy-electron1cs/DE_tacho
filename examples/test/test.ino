#include <DE_tacho.h>

DE_tacho enc(2);

void setup() {
  Serial.begin(9600);
  enc.begin(isr);
  enc.setRatio(8 * 63);
}

void isr() {
  enc.detect();
}

void loop() {
  enc.tick();
  Serial.print(enc.getRPM());
  Serial.print('\t');
  Serial.println(enc.getSpeed());
}
