#include "ServoSweep.h"

#include <Servo.h>

#include "pin.h"

static Servo g_servo;

void ServoSweep::begin() {
    g_servo.attach(PIN_SERVO, MIN_US, MAX_US);
    g_servo.write(90);
}

void ServoSweep::update() {
    g_servo.write(_angle);
    _angle += _step;

    if (_angle >= 180 || _angle <= 0) {
        _step = -_step;
    }
}
