#include <avr/interrupt.h>

const int button_pin = 0;
unsigned long previous_time = 0;
volatile bool buttonPressed = false;

int mode = 1;
const int modes_amount = 5;

int mode2_step = 1;
int mode3_step = 1;
int mode4_step = 1;
int mode5_step = 1;

// Transistor layout:
// Pin1 - Q1, Top Left LEDs
// Pin2 - Q2, Bottom left
// Pin3 - Q3, Top right
// Pin4 - Q4, Bottom right

// Mode ideas:
// Mode 1 - default, just all lights constantly on so all 4 transistors toggled. 
// Mode 2 - beating heart, all come on for half a second then go off for half a second
// Mode 3 - faster beating heart, all come on for 1/4 a second then go off for 1/4 a second
// Mode 4 - Cycling LEDs: Q1 toggles for 1/8 a second, then Q2, then Q4, then Q3, in a circular cycle.
// Mode 5 - Opposite sides cycle: Q1 + Q4 for 1/4 a second, then Q2 + Q3 for 1/4 a second

void setup() {
  pinMode(button_pin, INPUT_PULLUP);      // PB0 normally HIGH, button pulls it to GND

  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  GIMSK |= (1 << PCIE);          // enable pin-change interrupts
  PCMSK |= (1 << PCINT0);        // enable interrupt specifically on PB0
  GIFR  = (1 << PCIF);           // clear any old pending interrupt flag
  sei();                         // enable interrupts globally
}

ISR(PCINT0_vect) {
  if (digitalRead(button_pin) == LOW) {
    buttonPressed = true;
  }
}

void mode1() {
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
}


void mode2() {
    if ((millis() - previous_time >= 500) || (previous_time == 0)) {
      previous_time = millis();
      
      if (mode2_step == 1) {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        mode2_step += 1;
      }
      else {
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        mode2_step = 1;
      }    

    }  
}


void mode3() {
    if ((millis() - previous_time >= 250) || (previous_time == 0)) {
      previous_time = millis();
      
      if (mode3_step == 1) {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        mode3_step += 1;
      }
      else {
        digitalWrite(1, LOW);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        mode3_step = 1;
      }    

    }  
}


void mode4() {
  if ((millis() - previous_time >= 125) || (previous_time == 0)) {
    previous_time = millis();
    
    // turn everything off
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);

    if (mode4_step == 1) {
      digitalWrite(1, HIGH);
      mode4_step += 1;
    }
    else if (mode4_step == 2)
    {
      digitalWrite(2, HIGH);
      mode4_step += 1;
    }
    else if (mode4_step == 3)
    {
      digitalWrite(4, HIGH);
      mode4_step += 1;
    }
    else
    {
      digitalWrite(3, HIGH);
      mode4_step = 1;
    }

  }
}


void mode5() {
  if ((millis() - previous_time >= 250) || (previous_time == 0)) {
    previous_time = millis();

    // turn everything off
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);

    if (mode5_step == 1) {
      digitalWrite(1, HIGH);
      digitalWrite(4, HIGH);
      mode5_step += 1;
    }
    else {
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      mode5_step = 1;
    }
  }
}
unsigned long last_button_press = 0;
const unsigned long debounce_time = 25;

void loop() {
  if (buttonPressed == true) {
    buttonPressed = false;

    if (millis() - last_button_press >= debounce_time) {
      last_button_press = millis();

      if (mode < modes_amount) {
        mode += 1;
      }
      else {
        mode = 1;
      }

      previous_time = 0;

      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);

      mode2_step = 1;
      mode3_step = 1;
      mode4_step = 1;
      mode5_step = 1;
    }
  }

  if (mode == 1) {
    mode1();
  }
  
  if (mode == 2) {
    mode2();
  }

  if (mode == 3) {
    mode3();
  }

  if (mode == 4) {
    mode4();
  }

  if (mode == 5) {
    mode5();
  }
}