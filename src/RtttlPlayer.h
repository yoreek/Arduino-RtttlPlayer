/*
 * RtttlPlayer.cpp - Arduino RTTTL player library
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
#pragma once
#ifndef _RTTTL_PLAYER_H_
#define _RTTTL_PLAYER_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <inttypes.h>
#include <stddef.h>
#include <StateMachine.h>

#define OCTAVE_OFFSET     0
#define DEF_OCTAVE        6
#define DEF_NOTE_DURATION 4
#define DEF_BPM           63
#define MIN_OCTAVE        3
#define MAX_OCTAVE        7

class RtttlParser {
    public:
        RtttlParser() :
            _eof(false),
            _empty(true)
        {
        };
        void parse(const char *song);
#ifdef F
        void parse_P(const char *song);
#endif
        inline bool eof() { return _eof; };
        inline bool empty() { return _empty; };
        inline uint8_t readByte() {
#ifdef F
            return _pgm ? pgm_read_byte(_curPos++) : *_curPos++;
#else
            return *_curPos++;
#endif
        };
        inline void reset() {
            _curPos = _startPos;
            _eof    = false;
        };
        bool readNote(uint16_t *frequency, uint32_t *duration);

    protected:
        bool        _eof;
        bool        _empty;
#ifdef F
        bool        _pgm;
#endif
        const char *_startPos;
        const char *_curPos;
        uint32_t    _defDuration;
        uint16_t    _defOctave;
        uint16_t    _bpm;
        uint32_t    _wholeNote;
        void        _init();
};

class RtttlPlayer : public StateMachine {
    public:
        RtttlPlayer(uint8_t buzzerPin) :
            StateMachine((PState) &RtttlPlayer::STOPPED),
            _buzzerPin(buzzerPin),
            _isPlaying(false) {
        };
        inline bool isPlaying() { return _isPlaying; };
        virtual void play(const char *song);
#ifdef F
        virtual void play_P(const char *song);
        virtual void play(const __FlashStringHelper *song) { play_P((const char *) song); };
#endif
        virtual void play();
        virtual void stop();
        virtual void pause();
        virtual void rewind();
        State STOPPED();
        State PLAYING();
        State PLAYING_NOTE();
        State PAUSE_BETWEEN_NOTES();
        State PAUSED();

    protected:
        uint8_t       _buzzerPin;
        bool          _isPlaying;

        RtttlParser   _parser;
        uint16_t      _noteFrequency;
        uint32_t      _noteDuration;
};

#endif
