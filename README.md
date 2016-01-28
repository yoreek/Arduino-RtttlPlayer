# Arduino [RTTTL](https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language) player library [![Build Status](https://travis-ci.org/yoreek/Arduino-RtttlPlayer.svg?branch=master)](https://travis-ci.org/yoreek/Arduino-RtttlPlayer)

* Version: 1.0.0
* Release Date: 2016-01-28

## How do I get set up? ##

 * [Download](https://github.com/yoreek/Arduino-RtttlPlayer/archive/master.zip) the Latest release from gitHub.
 * Unzip and modify the Folder name to "RtttlPlayer".
 * Paste the modified folder on your library folder (On your `Libraries` folder inside Sketchbooks or Arduino software).
 * Download and Install [StateMachine](https://github.com/yoreek/Arduino-StateMachine) library.
 * Restart the Arduino Software


## Usage ##

```
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
```

## Example ##

Included on example folder, available on Arduino IDE.


## Version History ##

 * 1.0.0 (2016-01-28): Initial version.


## Links

 * Wikipedia on [Ring Tone Text Transfer Language (RTTTL)](https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language)
 * [State Machine library](https://github.com/yoreek/Arduino-StateMachine)


## Who do I talk to? ##

 * [Yoreek](https://github.com/yoreek)
