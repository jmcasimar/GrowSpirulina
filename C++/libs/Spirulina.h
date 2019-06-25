/*
Library for Grow software package
Written by : José Manuel Casillas Martín
email: jmcasimar@sippys.com.mx
Date : 2019-*-*
Version : 0.1 (testing version)

This file is part of Grow Software Package.

Grow is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Grow is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Grow.  If not, see <https://www.gnu.org/licenses/>.
*/

// Spirulina.h

#ifndef Spirulina_h
#define Spirulina_h

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

// Class to control led´s in Kigen bioreactor
class SP_LED
 {  private:
        bool __State; // LED state
        byte __Pin, __On1, __Off1, __On2, __Off2;

    public:
         SP_LED(byte pin); // Constructor

         void begin(byte on1, byte off1, byte on2, byte off2); // Set hour parameters
         void turnOn(); // Turn On LED
         void turnOff(); // Turn Off LED
         bool getState(); // Return HIGH if On, else LOW|
         void run(byte hr); // Run the algorithm to control LED´s
  } ;

// Class to control WaveMakers in Kigen bioreactor
class waveMaker
{  private:
       bool __State; // WaveMaker state
       byte __Pin, __TimeOn, __On1, __On2, __On3, __On4;

   public:
        waveMaker(byte pin, byte timeOn); // Constructor

        void begin(byte on1, byte on2, byte on3, byte on4); // Set hour parameters
        void turnOn(); // Turn On WaveMakers
        void turnOff(); // Turn Off WaveMakers
        bool getState(); // Return HIGH if On, else LOW|
        void run(byte hr, byte mn); // Run the algorithm to control Wavemakers
 } ;


// Class to control airPump in Kigen bioreactor
class airPump
{  private:
      bool __State, __Enable; // airPump state & enable
      byte __Pin;
      unsigned int __pumpOnDay, __pumpOffDay, __pumpOnNight, __pumpOffNight;
      unsigned long __ActualTime;

  public:
       airPump(byte pin); // Constructor

       void begin(unsigned int onDay, unsigned int offDay, unsigned int onNight, unsigned int offNight); // Set cycle parameters
       void turnOn(); // Turn On airPump
       void turnOff(); // Turn Off airPump
       bool getState(); // Return HIGH if On, else LOW|
       void enable(bool en); // Enable or Disable airPump
       bool isEnable(); // Returns true if enable
       void resetTime(); // Reset time counter
       void control(unsigned long on, unsigned long off); // Control airPump
       void run(bool day); // Run the algorithm to control airPump
} ;

class heatResistor
{ private:
    bool __State;
    byte __Pin;
    int __Counter;
    float __tempMaxDay, __tempMinDay, __tempMaxNight, __tempMinNight;
    unsigned long __ActualTime;

  public:
      heatResistor(byte pin); // Constructor

      void begin(float tempMaxDay, float tempMinDay, float tempMaxNight, float tempMinNight);
      void turnOn();
      void turnOff();
      bool getState();
      void resetTime();
      void control(float tempMin, float tempMax, float t1, float t2, float t3);
      void run(bool day, float t1, float t2, float t3);

} ;

  #endif
