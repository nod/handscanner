/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

#include "HandScanner.h"

#define STEP_WAIT 0
#define STEP_SCAN 1
#define STEP_FAIL 2
#define STEP_PASS 3


//#define CLR_FAIL {255, 30, 30}
//#define CLR_PASS {30, 255, 30}

const int scans_wanted = 6;

const int clr_scan_off[] = {HIGH,HIGH,HIGH};
const int clr_scan_on[] = {HIGH,HIGH,LOW};

const int clr_pass_on[] = {LOW,HIGH,LOW};
const int clr_pass_off[] = {HIGH,HIGH,HIGH};

const int clr_fail_on[] = {HIGH,LOW,LOW};
const int clr_fail_off[] = {HIGH,HIGH,HIGH};

HandScanner::HandScanner(int red, int grn, int blu, int inp) {
    _red = red;
    _grn = grn;
    _blu = blu;
    _inp = inp;
    _ticks_remaining = 0;
    _num_scans = 0;
}

void HandScanner::setup() {
    _step = 0;
    _ticks_remaining = 0;
    pinMode(_red, OUTPUT);
    pinMode(_grn, OUTPUT);
    pinMode(_blu, OUTPUT);
    pinMode(_inp, INPUT);
}

void HandScanner::_step_wait() {
    digitalWrite(_red, LOW);
    digitalWrite(_grn, LOW);
    digitalWrite(_blu, LOW);
}

void HandScanner::_step_scan() {
    if (_num_scans % 2) {
        digitalWrite(_red, clr_scan_on[0]);
        digitalWrite(_grn, clr_scan_on[1]);
        digitalWrite(_blu, clr_scan_on[2]);
    } else {
        digitalWrite(_red, clr_scan_off[0]);
        digitalWrite(_grn, clr_scan_off[1]);
        digitalWrite(_blu, clr_scan_off[2]);
    }
}

void HandScanner::_step_fail() {
    if (_ticks_remaining % 2) {
        digitalWrite(_red, clr_fail_on[0]);
        digitalWrite(_grn, clr_fail_on[1]);
        digitalWrite(_blu, clr_fail_on[2]);
    } else {
        digitalWrite(_red, clr_fail_off[0]);
        digitalWrite(_grn, clr_fail_off[1]);
        digitalWrite(_blu, clr_fail_off[2]);
    }
    _ticks_remaining--;
}


void HandScanner::_step_pass() {
    if (_ticks_remaining % 2) {
        digitalWrite(_red, clr_pass_on[0]);
        digitalWrite(_grn, clr_pass_on[1]);
        digitalWrite(_blu, clr_pass_on[2]);
    } else {
        digitalWrite(_red, clr_pass_off[0]);
        digitalWrite(_grn, clr_pass_off[1]);
        digitalWrite(_blu, clr_pass_off[2]);
    }
    _ticks_remaining--;
}

void HandScanner::tick() {

    // must look for state change here!!!

    bool pushed = digitalRead(_inp) == HIGH;
    if (pushed && _step == STEP_WAIT) {
        // start scanning
        _step = STEP_SCAN;
        _num_scans = scans_wanted;
    } else if (pushed && _step == STEP_SCAN && _num_scans) {
        // keep scanning
        _num_scans--;
    } else if (pushed && _step == STEP_SCAN && _num_scans < 1) {
        // PASSED
        _step = STEP_PASS;
        _ticks_remaining = 5;
    } else if (!pushed && _step == STEP_SCAN && _num_scans) {
        // FAIL
        _step = STEP_FAIL;
        _ticks_remaining = 5;
    } else if (!pushed && _ticks_remaining < 1) {
        _step = STEP_WAIT;
        if (_num_scans != 0) _num_scans = 0;
        if (_ticks_remaining != 0) _ticks_remaining = 0;
    }

    switch (_step) {
        case STEP_WAIT:
            _step_wait();
            break;
        case STEP_SCAN:
            _step_scan();
            break;
        case STEP_FAIL:
            _step_fail();
            break;
        case STEP_PASS:
            _step_pass();
            break;
        default:
            _step = STEP_WAIT;
    }
}


