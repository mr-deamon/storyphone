#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include "RotaryDialer.h"


#define PIN_READY D3 //GPIO 0
#define PIN_PULSE D4 //GPIO 2
RotaryDialer dialer = RotaryDialer(PIN_READY, PIN_PULSE);

#define PIN_BUSY D5
SoftwareSerial mp3Serial(D1, D2);

String number;
int playat, started, runs, playing;



void setup () {
  Serial.begin (115200);

  pinMode(PIN_BUSY, INPUT);
  pinMode(PIN_READY, INPUT);
  pinMode(PIN_PULSE, INPUT_PULLUP);

  Serial.println("Setting up mp3 player");
  mp3Serial.begin(9600);
  mp3_set_serial(mp3Serial);

  dialer.setup();

  delay(1000);
  mp3_set_volume (10);
  started = millis();
  mp3_stop();
}

void loop () {

    Serial.println(digitalRead(PIN_BUSY));
    if (number == "") {
      if (millis() > started + 15000) {
        Serial.println("nothing received,sleeping");
        ESP.deepSleep(0);
      }
      Serial.println("waiting for number");
      if (dialer.update()) {
        Serial.println("something happens");
        number = String(dialer.getNextNumber());
        playat = millis() + 5000;
      }
    }
    else {
      if (playat > millis()) {
        Serial.println("still waiting");
        if (dialer.update()) {
          Serial.println("something happens");
          number += (char) dialer.getNextNumber();
          playat = millis() + 5000;
        }
      }
      else {
        Serial.println("play music" + number);
        mp3_stop();
        delay(200);
        mp3_single_play(number.toInt());
        delay(200);
        ESP.deepSleep(0);
  }

  }
  delay(100);

}
