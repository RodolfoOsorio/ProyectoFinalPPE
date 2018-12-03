
// Credit:
// Midi to Arduino Converter
//     - Andy Tran (extramaster), 2015
// https://www.extramaster.net/tools/midiToArduino/
//
// Process:
// Midi -> Midi tracks -> Note mappings -> Frequency
//
// CC0

// Set this to be the pin that your buzzer resides in. (Note that you can only have one buzzer actively using the PWM signal at a time).
int tonePin = 8;
void setup() {

}

void midi() {

    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 440, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 277, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 440, 106219287.125);
    delay(300);
    tone(tonePin, 391, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 369, 106219287.125);
    delay(300);
    tone(tonePin, 329, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);
    tone(tonePin, 277, 106219287.125);
    delay(300);
    tone(tonePin, 311, 106219287.125);
    delay(300);

}

void loop() {
    // Play midi
    midi();
}
