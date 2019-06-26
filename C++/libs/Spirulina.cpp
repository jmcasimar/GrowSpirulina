// Spirulina.cpp
//
// Copyright (C) 2019 Grow

#include "Spirulina.h"

/*** LED class definitions ***/
SP_LED::SP_LED(byte pin) // Constructor
  { __State = LOW;
    __Pin = pin;
    __On1 = 0;
    __Off1 = 0;
    __On2 = 0;
    __Off2 = 0;
  }

void SP_LED::begin(byte on1, byte off1, byte on2, byte off2)
  { pinMode(__Pin, OUTPUT);
    digitalWrite(__Pin, !LOW);
    __On1 = on1;
    __Off1 = off1;
    __On2 = on2;
    __Off2 = off2;
    Serial.println(F("LED: Setup correctly"));
  }

void SP_LED::turnOn()
  { if(__State==LOW){
      __State = HIGH;
      digitalWrite(__Pin, !__State);
      Serial.println(F("LED: Turn On"));
    }
  }

void SP_LED::turnOff()
  { if(__State==HIGH){
      __State = LOW;
      digitalWrite(__Pin, !__State);
      Serial.println(F("LED: Turn Off"));
    }
  }

bool SP_LED::getState()
  { return __State; }

void SP_LED::run(byte hr)
  { if( (hr>=__On1 && hr<__Off1) || (hr>=__On2 && hr<__Off2) ){
      turnOn();
    }
    else{
      turnOff();
    }
  }

/*** WaveMakers class definitions ***/
waveMaker::waveMaker(byte pin, byte timeOn) // Constructor
  { __State = LOW;
    __Pin = pin;
    __TimeOn = timeOn; // In minutes
    __On1 = 0;
    __On2 = 0;
    __On3 = 0;
    __On4 = 0;
  }

void waveMaker::begin(byte on1, byte on2, byte on3, byte on4)
  { pinMode(__Pin, OUTPUT);
    digitalWrite(__Pin, !LOW);
    __On1 = on1;
    __On2 = on2;
    __On3 = on3;
    __On4 = on4;
    Serial.println(F("WaveMakers: Setup correctly"));
  }

void waveMaker::turnOn()
  { if(__State==LOW){
      __State = HIGH;
      digitalWrite(__Pin, !__State);
      Serial.println(F("WaveMakers: Turn On"));
    }
  }

void waveMaker::turnOff()
  { if(__State==HIGH){
      __State = LOW;
      digitalWrite(__Pin, !__State);
      Serial.println(F("WaveMakers: Turn Off"));
    }
  }

bool waveMaker::getState()
  { return __State; }

void waveMaker::run(byte hr, byte mn)
  { if( ((hr==__On1 && mn>=21 && mn<21+__TimeOn) || (hr==__On2 && mn>=21 && mn<21+__TimeOn) || (hr==__On3 && mn>=21 && mn<21+__TimeOn) || (hr==__On4 && mn>=21 && mn<21+__TimeOn)) ){
      turnOn();
    }
    else{
      turnOff();
    }
  }

/*** airPump class definitions ***/
airPump::airPump(byte pin) // Constructor
  { __State = LOW;
    __Enable = LOW;
    __Pin = pin;
    __pumpOnDay = 0; // In minutes
    __pumpOffDay = 0; // In minutes
    __pumpOnNight = 0; // In minutes
    __pumpOffNight = 0; // In minutes
    __ActualTime = millis();
  }

void airPump::begin(unsigned int onDay, unsigned int offDay, unsigned int onNight, unsigned int offNight)
  { pinMode(__Pin, OUTPUT);
    digitalWrite(__Pin, !LOW);
    __pumpOnDay = onDay;
    __pumpOffDay = offDay;
    __pumpOnNight = onNight;
    __pumpOffNight = offNight;
    Serial.println(F("AirPumps: Setup correctly"));
  }

void airPump::turnOn()
  { if(__State==LOW){
      __State = HIGH;
      digitalWrite(__Pin, !__State);
      Serial.println(F("AirPumps: Turn On"));
    }
  }

void airPump::turnOff()
  { if(__State==HIGH){
      __State = LOW;
      digitalWrite(__Pin, !__State);
      Serial.println(F("AirPumps: Turn Off"));
    }
  }

bool airPump::getState()
  { return __State; }

void airPump::enable(bool en)
  { __Enable == en; }

bool airPump::isEnable()
  { return __Enable; }

void airPump::resetTime()
  { __ActualTime = millis(); }

void airPump::control(unsigned long on, unsigned long off)
  { if(millis()-__ActualTime>=0 && millis()-__ActualTime<on){
      turnOn();
    }
    else if(millis()-__ActualTime>=on && millis()-__ActualTime<off){
      turnOff();
    }
    else if(millis()-__ActualTime>=off){
      resetTime();
    }
  }

void airPump::run(bool day)
  { if(__Enable==HIGH){
      if( day ){
        unsigned long pumpOn = long(__pumpOnDay)*60ul*1000ul;
        unsigned long pumpOff = long(pumpOn) + long(__pumpOffDay)*60ul*1000ul;
        control(pumpOn, pumpOff);
      }
      else{
        unsigned long pumpOn = long(__pumpOnNight)*60ul*1000ul;
        unsigned long pumpOff = long(pumpOn) + long(__pumpOffNight)*60ul*1000ul;
        control(pumpOn, pumpOff);
      }
    }
  }

/*** heatResistor class definitions ***/
heatResistor::heatResistor(byte pin) // Constructor
  { __State = LOW;
    __Pin = pin;
    __Counter = 0;
    __tempMaxDay = 0;
    __tempMinDay = 0;
    __tempMaxNight = 0;
    __tempMinNight = 0;
    __Enable = HIGH;
    __ActualTime = millis();
  }

void heatResistor::begin(float tempMaxDay, float tempMinDay, float tempMaxNight, float tempMinNight)
  { pinMode(__Pin, OUTPUT);
    digitalWrite(__Pin, !LOW);
    __tempMaxDay = tempMaxDay;
    __tempMinDay = tempMinDay;
    __tempMaxNight = tempMaxNight;
    __tempMinNight = tempMinNight;
    Serial.println(F("heatResistor: Setup correctly"));
  }

void heatResistor::turnOn()
  { if(__State==LOW){
      __State = HIGH;
      digitalWrite(__Pin, !__State);
      Serial.println(F("heatResistor: Turn On"));
    }
  }

void heatResistor::turnOff()
  { if(__State==HIGH){
      __State = LOW;
      digitalWrite(__Pin, !__State);
      Serial.println(F("heatResistor: Turn Off"));
    }
  }

bool heatResistor::getState()
  { return __State; }

void heatResistor::resetTime()
  { __ActualTime = millis(); }

void heatResistor::enable(bool en)
  { __Enable = en; }

bool heatResistor::isEnabled()
  { return __Enable; }

void heatResistor::control(float tempMin, float tempMax, float t1, float t2, float t3)
  { float meanTemp = (t1+t2+t3)/3;
    if( (t1>=tempMax || t2>=tempMax || t3>=tempMax) && getState()==HIGH){
      __Counter++;
      if(__Counter>10){
        __Counter = 0;
        turnOff();
      }
    }
    else if( meanTemp<tempMin && getState()==LOW){
      __Counter--;
      if(__Counter<-10){
        __Counter = 0;
        turnOn();
      }
    }
    else{__Counter=0;}
  }

void heatResistor::run(bool day, float t1, float t2, float t3)
  { if(__Enable){
      if(millis()-__ActualTime>2000){
        resetTime();
        if( day ){
          control(__tempMinDay, __tempMaxDay, t1, t2, t3);
        }
        else{
          control(__tempMinNight, __tempMaxNight, t1, t2, t3);
        }
      }
    }
  }
