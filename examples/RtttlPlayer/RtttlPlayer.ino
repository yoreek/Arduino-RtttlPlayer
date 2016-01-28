#include <Arduino.h>
#include <avr/pgmspace.h>
#include <RtttlPlayer.h>

#define BUZZER_PIN 7

const char song[] PROGMEM = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";

RtttlPlayer player(BUZZER_PIN);

void setup() {
    Serial.begin(9600);

    player.play_P(song);
}

void loop() {
    player.loop();
}
