/*
 * RtttlPlayer.h - Arduino RTTTL player library
 * Copyright (C) 2016 Yuriy Ustushenko
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the version 3 GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "RtttlPlayer.h"
#include "Pitches.h"
#include <inttypes.h>

#define _IS_DIGIT(n)          ((n) >= '0' && (n) <= '9')
#define _NOTE_FREQUENCY(n, o) (pgm_read_word(&NOTE_FREQS[(n) - 1]) >> (MAX_OCTAVE - (o)))
//#define _NOTE_FREQUENCY(n, o) pgm_read_word(&NOTE_FREQS[((o) - 4) * 12 + (n) - 1])
//#define _NOTE_FREQUENCY(n, o) ((double) NOTE_C4 * pow(2.0, (double) (((o) - 4) * 12 + (n) - 1) / 12.0));

static const uint16_t NOTE_FREQS[] PROGMEM = {
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

State RtttlPlayer::STOPPED() {
    if (_isPlaying)
        setState((PState) &RtttlPlayer::PLAYING);
}

State RtttlPlayer::PLAYING() {
    if (!_isPlaying)
        setState((PState) &RtttlPlayer::STOPPED);

    if (_parser.readNote(&_noteFrequency, &_noteDuration)) {
        if (_noteFrequency) {
            tone(_buzzerPin, _noteFrequency);
        }
        else {
            noTone(_buzzerPin);
        }
        setState((PState) &RtttlPlayer::PLAYING_NOTE);
    }
    else {
        _isPlaying = false;
        noTone(_buzzerPin);
        setState((PState) &RtttlPlayer::STOPPED);
    }
}

State RtttlPlayer::PLAYING_NOTE() {
    if (isTimeout(_noteDuration)) {
        noTone(_buzzerPin);
        _noteDuration *= 0.2; // pause between notes
        setState((PState) &RtttlPlayer::PAUSE_BETWEEN_NOTES);
    }
}

State RtttlPlayer::PAUSE_BETWEEN_NOTES() {
    if (isTimeout(_noteDuration))
        setState((PState) &RtttlPlayer::PLAYING);
}

State RtttlPlayer::PAUSED() {
    if (_isPlaying)
        setState((PState) &RtttlPlayer::PLAYING);
}

void RtttlPlayer::play(const char *song) {
    stop();
    _parser.parse(song);
    _isPlaying = true;
}

#ifdef F
void RtttlPlayer::play_P(const char *song) {
    stop();
    _parser.parse_P(song);
    _isPlaying = true;
}
#endif

void RtttlPlayer::play() {
    if (!_parser.empty())
        _isPlaying = true;
}

void RtttlPlayer::stop() {
    noTone(_buzzerPin);
    _isPlaying = false;
}

void RtttlPlayer::pause() {
    if (_isPlaying) {
        noTone(_buzzerPin);
        _isPlaying = false;
        setState((PState) &RtttlPlayer::PAUSED);
    }
}

void RtttlPlayer::rewind() {
    if (!_parser.empty()) {
        _parser.reset();
        _isPlaying = true;
        setState((PState) &RtttlPlayer::PLAYING);
    }
}

void RtttlParser::parse(const char *song) {
#ifdef F
    _pgm = false;
#endif
    _curPos = song;
    _empty  = false;

    _init();
}

#ifdef F
void RtttlParser::parse_P(const char *song) {
    _pgm = true;

    _curPos = song;
    _empty  = false;

    _init();
}
#endif

// format: NAME:d=N,o=N,b=NNN:
void RtttlParser::_init() {
    uint16_t num;
    uint8_t  c;

    _defDuration = DEF_NOTE_DURATION;
    _defOctave   = DEF_OCTAVE;
    _bpm         = DEF_BPM;

    // skip name and ':'
    while (this->readByte() != ':')
        ;

    // get default duration
    c = readByte();
    if (c == 'd') {
        readByte();          // skip "="
        num = 0;
        while ((c = readByte()) && _IS_DIGIT(c)) {
            num = (num * 10) + (c - '0');
        }

        if (num > 0)
            _defDuration = num;

        c = readByte(); // skip comma
    }

    // get default octave
    if (c == 'o') {
        readByte(); // skip "="
        num = readByte() - '0';
        if (num >= MIN_OCTAVE && num <= MAX_OCTAVE)
            _defOctave = num;
        readByte(); // skip comma
        c = readByte();
    }

    // get BPM
    if (c == 'b') {
        readByte();              // skip "="
        num = 0;
        while ((c = readByte()) && _IS_DIGIT(c)) {
            num = (num * 10) + (c - '0');
        }
        _bpm = num;
        c = readByte(); // skip colon
    }


    // BPM usually expresses the number of quarter notes per minute
    _wholeNote = (60 * 1000L / _bpm) * 4;

    _startPos = --_curPos;
}

bool RtttlParser::readNote(uint16_t *frequency, uint32_t *duration) {
    uint16_t num;
    uint8_t  note;
    uint8_t  octave;
    uint8_t  c;

    if (_empty || _eof)
        return false;

    // first, get note duration, if available
    num = 0;
    while ((c = readByte()) && _IS_DIGIT(c)) {
        num = (num * 10) + (c - '0');
    }
    if (!num) {
        if (c == 0) {
            _eof = true;
            return false;
        }
        num = _defDuration;
    }
    *duration = _wholeNote / num;

    // now get the note
    switch (c) {
        case 'c': note =  1; break;
        case 'd': note =  3; break;
        case 'e': note =  5; break;
        case 'f': note =  6; break;
        case 'g': note =  8; break;
        case 'a': note = 10; break;
        case 'b': note = 12; break;
        default:  note = 0;
    }

    c = readByte();
    // now, get optional '#' sharp
    if (c == '#') {
        note++;
        c = readByte();
    }

    // now, get optional '.' dotted note
    if (c == '.') {
        *duration += *duration / 2;
        c = readByte();
    }

    // now, get scale
    if (_IS_DIGIT(c)) {
        octave = c - '0';
        c = readByte();
    }
    else {
        octave = _defOctave;
    }

    // skip comma

    if (note) {
        *frequency = _NOTE_FREQUENCY(octave + OCTAVE_OFFSET, note);
    }
    else {
        *frequency = 0;
    }

    return true;
}
