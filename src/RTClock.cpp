#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "Globals.h"

void setupClock(){
    Rtc.Begin();

    if (!Rtc.IsDateTimeValid()) {
        Serial.println("RTC lost confidence in the DateTime! Setting default.");
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        Rtc.SetDateTime(compiled);
    }

    RtcDateTime now = Rtc.GetDateTime();
    hours = now.Hour();
    minutes = now.Minute();
    seconds = now.Second();
}

void refreshTimeState(){
    if (!inSettingsMode) {
        RtcDateTime now = Rtc.GetDateTime();
        hours = now.Hour();
        minutes = now.Minute();
        seconds = now.Second();
    }
}