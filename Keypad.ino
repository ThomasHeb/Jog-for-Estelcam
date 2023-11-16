
// ===========================================================================================
// Keyboard für die Fräse mit Estelcam
//
// Hardware Board: Teensy LC    https://www.prjc.com/teensy
// Update Arduino IDE 2.0.x     https://www.pjrc.com/teensy/td_download.html
// 
// Select Tools > USB Type > Keyboard in Arduino IDE to enable Keyboard HID
// Select Tools > Keyboard Layout < Germany 
//
// Use Verify in the Arduino IDE to compile
// The Teensy Loader opens and requests a short Push of the Reset button on the teensy
//
// ===========================================================================================
// How to get startet:
// Wire the hardware step by step and test eachstep with the related debug function.
// Enabling the debugfunktion will show debugging information on the serial monitor.
//
// wireing: See section Pin-Out
// 
// Joystick Debugging
//#define _DebugJoystick     
//   Pressing the joystick switches on the on board LED
//   Releasing the joystick switches off the on board LED
//   Trouble shooting: 
//   - if LED is not working, check wireing of Joystick (GND, MS)
//   - load file / example / 01.basic / blink, the LED should blink 
//
//   Moving the joystick will trigger a debug message in the serial monitor
//   - left should show 0x20 as POS
//   - right should show 0x10 as POS
//   - up should show 0x01 as POS
//   - down should show 0x02 as POS
//   - center should show 0x00 as POS
//   Trouble shooting: 
//   - if you get no messages, check the wireing
//   - if you don't get these values at the positions, change the following defines
#define Center          0x00
#define Left            0x20
#define Right           0x10     
#define Up              0x01 
#define Down            0x02     
//
//
// Slider Debugging
//#define _DebugSlider
//   Moving the slider will trigger a debug message in the serial monitor
//   - min should show 0x0 as POS
//   - 25% should show 0x1 as POS
//   - 50% should show 0x2 as POS
//   - 75% should show 0x3 as POS
//   - max should show 0x4 as POS
//   Trouble shooting: 
//   - if you get no messages, check the wireing
//   - if you don't get these values at the positions, change the following defines
#define Position1       0x0
#define Position2       0x1
#define Position3       0x2
#define Position4       0x3
#define Position5       0x4
//
//
// Buttons Debugging
//#define _DebugButtons
//   Turning the rotary knob for Feed and Speed will trigger a debug message in the serial monitor
//   Trouble shooting: 
//   - if you get no messages, check the wireing
//   - if spinle and feed is mixed up, change wireing or position of the rotary encoders
//   - if the direction is not correct, enable/disable de defines below
//#define FeedDirection
#define SpindleDirection






// ===========================================================================================
// Pin-Out:
// Rotary knob KY-040 for Feed Speed
#define PIN_FEED_A            9
#define PIN_FEED_B            10
//
// Rotary knob KY-040 for Spindle Speed
#define PIN_SPINDLE_A         11     
#define PIN_SPINDLE_B         12    
//
// Joystick for X/Y & push to enable JOG
#define PIN_X_ANALOG          A0
#define PIN_Y_ANALOG          A1
#define PIN_JOG_ENABLE        16
//
// Knobs for Z up/down
#define PIN_Z_UP              7     
#define PIN_Z_DOWN            8    
//
// Slider / poti for jog speed
#define PIN_JOG_SPEED         A3
//
//
//
//
// ===========================================================================================

// ===========================================================================================
// Button Funktionen:

typedef struct {
  uint32_t  state;            // pressed buttons
  uint32_t  risingEdge;       // rising edge of buttons
  uint32_t  fallingEdge;      // falling edge of buttons
  uint32_t  prevState;        // previous state of buttons
  uint32_t  count;            // counter to stabilize the buttons
  uint32_t  image;            // image of the current buttons to be stabilized
  uint8_t   encoderFeed;      // state of the feed speed encoder
  uint8_t   encoderSpindle;   // state of the spindle speed encoder
 } buttons_t;
buttons_t buttons;

// buttons:
#define BTN_JOG_ENABLE                0x00000001
#define BTN_FEED_A                    0x00000002
#define BTN_FEED_B                    0x00000004
#define BTN_FEED                      0x00000008
#define BTN_SPINDLE_A                 0x00000010
#define BTN_SPINDLE_B                 0x00000020
#define BTN_SPINDLE                   0x00000040
#define BTN_Z_UP                      0x00000080
#define BTN_Z_DOWN                    0x00000100


// Enable weak pullups
#define DIR_NONE        0x00
#define DIR_CW          0x10
#define DIR_CCW         0x20
#define R_START         0x00

#define R_CW_FINAL      0x01
#define R_CW_BEGIN      0x02
#define R_CW_NEXT       0x03
#define R_CCW_BEGIN     0x04
#define R_CCW_FINAL     0x05
#define R_CCW_NEXT      0x06

const unsigned char encoderTable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

bool button(uint32_t id) {
  if ((buttons.state & id) != 0) {
    return true;
  }
  return false;
}

bool risingEdge(uint32_t id) {
  if ((buttons.risingEdge & id) != 0) {
    return true;
  }
  return false;
}

bool fallingEdge(uint32_t id) {
  if ((buttons.fallingEdge & id) != 0) {
    return true;
  }
  return false;
}

void buttonsInit() {
  // call one during setup
  buttons.state             = 0;
  buttons.risingEdge        = 0;    
  buttons.fallingEdge       = 0;    
  buttons.prevState         = 0;
  buttons.count             = 0;
  buttons.image             = 0;
  buttons.encoderFeed       = R_START;
  buttons.encoderSpindle    = R_START;

  pinMode(PIN_FEED_A,       INPUT_PULLUP);
  pinMode(PIN_FEED_B,       INPUT_PULLUP);
  pinMode(PIN_SPINDLE_A,    INPUT_PULLUP);
  pinMode(PIN_SPINDLE_B,    INPUT_PULLUP);
  pinMode(PIN_JOG_ENABLE,   INPUT_PULLUP);
  pinMode(PIN_Z_UP,         INPUT_PULLUP);
  pinMode(PIN_Z_DOWN,       INPUT_PULLUP);

}


void buttonsRead() {
  // call once at the beginning of the main loop

  uint32_t image = 0;

  // delete the edge indicator
  buttons.risingEdge            = 0;
  buttons.fallingEdge           = 0;
 
  // read the pins
  if (digitalRead(PIN_JOG_ENABLE) == LOW)
    image                       |=  BTN_JOG_ENABLE;
  if (digitalRead(PIN_FEED_A) == HIGH)
    image                       |=  BTN_FEED_A;
  if (digitalRead(PIN_FEED_B) == HIGH)
    image                       |=  BTN_FEED_B;
  if (digitalRead(PIN_SPINDLE_A) == HIGH)
    image                       |=  BTN_SPINDLE_A;
  if (digitalRead(PIN_SPINDLE_B) == HIGH)
    image                       |=  BTN_SPINDLE_B;
  if (digitalRead(PIN_Z_UP) == LOW)
    image                       |=  BTN_Z_UP;
  if (digitalRead(PIN_Z_DOWN) == LOW)
    image                       |=  BTN_Z_DOWN;

     
  // wait until all pins are stable
  if (image == buttons.image) {
    if (buttons.count >= 20) {
      buttons.count             = 0;
      // set previous buttons state
      buttons.prevState         = buttons.state;
      // set new button state
      buttons.state             = image;
      // check for rising edges
      buttons.risingEdge        = (buttons.state ^ buttons.prevState) & buttons.state;
      // check for falling edges
      buttons.fallingEdge       = (buttons.state ^ buttons.prevState) & buttons.prevState; 
      
    } else {
      buttons.count             +=  1;
    }
  } else {
    buttons.image               =   image;
    buttons.count               =   0;
  }


  uint8_t encoderState;

  // process feed speed encoder
  encoderState          =  0;
  if (button(BTN_FEED_B))
    encoderState        |= 2;
  if (button(BTN_FEED_A))
    encoderState        |= 1;
  //encoderState          = (digitalRead(PIN_FEED_B) << 1) | digitalRead(PIN_FEED_A);
  buttons.encoderFeed   =  encoderTable[buttons.encoderFeed & 0xf][encoderState];
  encoderState          =  buttons.encoderFeed & 0x30;
  
#ifdef FeedDirection
  if (encoderState == DIR_CW) 
    buttons.fallingEdge  |=  BTN_FEED;
  if (encoderState == DIR_CCW) 
    buttons.risingEdge |=  BTN_FEED;
#else
  if (encoderState == DIR_CW) 
    buttons.risingEdge  |=  BTN_FEED;
  if (encoderState == DIR_CCW) 
    buttons.fallingEdge |=  BTN_FEED;
#endif

// process spindle speed encoder
  encoderState          =  0;
  if (button(BTN_SPINDLE_B))
    encoderState        |= 2;
  if (button(BTN_SPINDLE_A))
    encoderState        |= 1;
  //encoderState          = (digitalRead(PIN_FEED_B) << 1) | digitalRead(PIN_FEED_A);
  buttons.encoderSpindle =  encoderTable[buttons.encoderSpindle & 0xf][encoderState];
  encoderState           =  buttons.encoderSpindle & 0x30;

#ifdef SpindleDirection 
  if (encoderState == DIR_CW) 
    buttons.fallingEdge  |=  BTN_SPINDLE;
  if (encoderState == DIR_CCW) 
    buttons.risingEdge |=  BTN_SPINDLE;  
#else
  if (encoderState == DIR_CW) 
    buttons.risingEdge  |=  BTN_SPINDLE;
  if (encoderState == DIR_CCW) 
    buttons.fallingEdge |=  BTN_SPINDLE;
#endif

}


// ===========================================================================================
// Joystick Funktionen:

typedef struct {
  uint32_t  x;            
  uint32_t  y;   
  uint8_t   position; 
  uint8_t   prevPosition; 
  bool      newPosition; 
} joystick_t;
joystick_t joystick;

uint8_t joystickPosition() {
  return joystick.position;
}

bool joystickHasNewPosition() {
  return joystick.newPosition;
}

void joystickInit() {
  pinMode(PIN_X_ANALOG,     INPUT);
  pinMode(PIN_Y_ANALOG,     INPUT);

  joystick.x              = 512;
  joystick.y              = 512;
  joystick.position       = Center;
  joystick.prevPosition   = Center;
  joystick.newPosition    = false;
}

void joystickRead() {
  // debounce
  joystick.x              +=  analogRead(PIN_X_ANALOG);
  joystick.x              /=  2;
  joystick.y              +=  analogRead(PIN_Y_ANALOG);
  joystick.y              /=  2;
  
  // reset trigger for new position
  joystick.newPosition    =  false;

  uint8_t           position;

  // read postion in x and y from analog values
  position = joystick.position;
  if (joystick.x > (1023 - 150))                                    position  |= 0x01;
  if (joystick.x < (0    + 150))                                    position  |= 0x02;
  if ((joystick.x > (512  - 150)) &&  (joystick.x < (512  + 150)))  position  &= 0xF0;

  if (joystick.y > (1023 - 150))                                    position  |= 0x10;
  if (joystick.y < (0    + 150))                                    position  |= 0x20;
  if ((joystick.y > (512  - 150)) &&  (joystick.y < (512  + 150)))  position  &= 0x0F;

  // check if new position is entered
  if (joystick.position != position) {
    joystick.newPosition    = true;
    joystick.prevPosition   = joystick.position;
    joystick.position       = position;
  }
}


// ===========================================================================================
// Slider Funktionen:

typedef struct {
  uint32_t  value;
  uint8_t   position; 
  uint8_t   prevPosition; 
  bool      newPosition; 
} slider_t;
slider_t slider;

uint8_t sliderPosition() {
  return slider.position;
}

bool sliderHasNewPosition() {
  return slider.newPosition;
}

void sliderInit() {
  pinMode(PIN_JOG_SPEED,     INPUT);
 
  slider.value            = 0;
  slider.position         = 0x00;
  slider.prevPosition     = 0x00;
  slider.newPosition      = false;
}

void sliderRead() {
  // debounce
  slider.value            +=  analogRead(PIN_JOG_SPEED);
  slider.value            /=  2;
    
  // reset trigger for new position
  slider.newPosition       =  false;

  uint8_t           position;

  // read postion from analog values
  position = slider.position;
  if (slider.value > (1023 - 150))                                      position  = 0x04;
  if (slider.value < (0    + 150))                                      position  = 0x00;
  if ((slider.value > 225) &&  (slider.value < (225  + 150)))           position  = 0x01;
  if ((slider.value > 450) &&  (slider.value < (450  + 150)))           position  = 0x02;
  if ((slider.value > 675) &&  (slider.value < (675  + 150)))           position  = 0x03;

    // check if new position is entered
  if (slider.position != position) {
    slider.newPosition    = true;
    slider.prevPosition   = slider.position;
    slider.position       = position;
  }
}


// ===========================================================================================
// main programm:

uint8_t         jogSpeed;
uint8_t         jogDirection;
typedef struct {
  unsigned long   spindleUp;
  unsigned long   spindleDown;
  unsigned long   feedUp;
  unsigned long   feedDown;
} mytimer_t;
mytimer_t timer;

void timerInit() {
  timer.spindleUp      = 0;
  timer.spindleDown    = 0;
  timer.feedUp         = 0;
  timer.feedDown       = 0;
}

void timerRead() {
  if (timer.spindleUp != 0) {
    if (millis() > timer.spindleUp) {
      Keyboard.release(KEY_T);
      timer.spindleUp = 0;
    }
  }
  if (timer.spindleDown != 0) {
    if (millis() > timer.spindleDown) {
      Keyboard.release(KEY_U);
      timer.spindleDown = 0;
    }
  }
  if (timer.feedUp != 0) {
    if (millis() > timer.feedUp) {
      Keyboard.release(KEY_E);
      timer.feedUp = 0;
    }
  }
  if (timer.feedDown != 0) {
    if (millis() > timer.feedDown) {
      Keyboard.release(KEY_R);
      timer.feedDown = 0;
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);                   // set the internal led as output
  
  buttonsInit();                                  // init the buttons
  joystickInit();                                 // init the joystick
  sliderInit();                                   // init the slider
  timerInit();                                    // init the timer

  jogSpeed        = Position1;                    // asume that jog speed is minimum
  jogDirection    = Center;                       // asume that jog direction is center (no movement)

  Serial.begin(15200);    // todo
}

void pressJogSpeed(uint8_t position) {
  switch (position) {
      case Position2: 
        Keyboard.press(KEY_A);
        break;
      case Position3: 
        Keyboard.press(KEY_B);
        break;
      case Position4: 
        Keyboard.press(KEY_C);
        break;
      case Position5: 
        Keyboard.press(KEY_D);
        break;
      default: 
        break;
    }  
}
void releaseJogSpeed(uint8_t position) {
  switch (position) {
      case Position2: 
        Keyboard.release(KEY_A);
        break;
      case Position3: 
        Keyboard.release(KEY_B);
        break;
      case Position4: 
        Keyboard.release(KEY_C);
        break;
      case Position5: 
        Keyboard.release(KEY_D);
        break;
      default: 
        break;
    }  
}

void pressJogDirection(uint8_t position) {
  if (position == (Right))                 Keyboard.press(KEYPAD_6);
  if (position == (Right | Up))            Keyboard.press(KEYPAD_9);
  if (position == (Up))                    Keyboard.press(KEYPAD_8);
  if (position == (Left | Up))             Keyboard.press(KEYPAD_7);
  if (position == (Left))                  Keyboard.press(KEYPAD_4);
  if (position == (Left | Down))           Keyboard.press(KEYPAD_1);
  if (position == (Down))                  Keyboard.press(KEYPAD_2);
  if (position == (Right | Down))          Keyboard.press(KEYPAD_3);
}

void releaseJogDirection(uint8_t position) {
  if (position == (Right))                 Keyboard.release(KEYPAD_6);
  if (position == (Right | Up))            Keyboard.release(KEYPAD_9);
  if (position == (Up))                    Keyboard.release(KEYPAD_8);
  if (position == (Left | Up))             Keyboard.release(KEYPAD_7);
  if (position == (Left))                  Keyboard.release(KEYPAD_4);
  if (position == (Left | Down))           Keyboard.release(KEYPAD_1);
  if (position == (Down))                  Keyboard.release(KEYPAD_2);
  if (position == (Right | Down))          Keyboard.release(KEYPAD_3);
}


void loop() {
  buttonsRead();                                  // read the buttons
  joystickRead();                                 // read the joystick
  sliderRead();                                   // read the slider
  timerRead();                                    // read the timer 

  // Send K on rising edge to enable/disable jogging
  if (risingEdge(BTN_JOG_ENABLE)) {
    Keyboard.press(KEY_K);
  } 
  if (fallingEdge(BTN_JOG_ENABLE)) {
    Keyboard.release(KEY_K);
  } 

  // Send E or R and T or U for increase/decrease Feed and Spindle
  if (risingEdge(BTN_FEED)) {
    if (timer.feedUp == 0) {            // check if it is the first turn of the knop
      Keyboard.press(KEY_E);            // first turn.... press the button, start the timer
    }
    timer.feedUp               = millis() + 200;
  } 
  if (fallingEdge(BTN_FEED)) {
    if (timer.feedDown == 0) {          // check if it is the first turn of the knop
      Keyboard.press(KEY_R);            // first turn.... press the button, start the timer
    }
    timer.feedDown             = millis() + 200;
  } 
  if (risingEdge(BTN_SPINDLE)) {
    if (timer.spindleUp == 0) {         // check if it is the first turn of the knop
      Keyboard.press(KEY_T);            // first turn.... press the button, start the timer
    }
    timer.spindleUp            = millis() + 200;
  } 
  if (fallingEdge(BTN_SPINDLE)) {
    if (timer.spindleDown == 0) {       // check if it is the first turn of the knop
      Keyboard.press(KEY_U);            // first turn.... press the button, start the timer
    }
    timer.spindleDown          = millis() + 200;
  } 

  // read position of slider and simulate press & hold of key for Jog Speed
  if (sliderHasNewPosition()) {
    releaseJogSpeed(jogSpeed);      // release last key
    jogSpeed = sliderPosition();
    pressJogSpeed(jogSpeed);        // press last key 
  }  

  // read position of joystick and simulate press & hold of key for Jog Direction
  if (joystickHasNewPosition()) {
    releaseJogDirection(jogDirection);      // release last key
    jogDirection = joystickPosition();
    pressJogDirection(jogDirection);        // press last key 
  }  

  // Send q or w a for z axis up/down
  if (risingEdge(BTN_Z_UP)) {
     Keyboard.press(KEY_Q);
  } 
  if (fallingEdge(BTN_Z_UP)) {
    Keyboard.release(KEY_Q);
  }
  if (risingEdge(BTN_Z_DOWN)) {
     Keyboard.press(KEY_W);
  } 
  if (fallingEdge(BTN_Z_DOWN)) {
    Keyboard.release(KEY_W);
  }


// ============================================================
// Debuging 
#ifdef _DebugJoystick
  if (risingEdge(BTN_JOG_ENABLE)) {
    digitalWrite(LED_BUILTIN, HIGH);
  } 
  if (fallingEdge(BTN_JOG_ENABLE))  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (joystickHasNewPosition()) {
    Serial.print("Joystick value: ");
    Serial.print(joystick.position);
    Serial.print("   POS: 0x");
    Serial.println(joystickPosition(), HEX);
  }
#endif
#ifdef _DebugSlider
 if (sliderHasNewPosition()) {
    Serial.print("Slider value: ");
    Serial.print(slider.value);
    Serial.print("   POS: 0x");
    Serial.println(sliderPosition(), HEX);
  }
#endif
#ifdef _DebugButtons
  if (risingEdge(BTN_FEED)) {
    Serial.println("Rotary Feed: Clockwise");
  } 
  if (fallingEdge(BTN_FEED)) {
    Serial.println("Rotary Feed: Counter Clockwise");
  } 
  if (risingEdge(BTN_SPINDLE)) {
    Serial.println("Rotary Spindle: Clockwise");
  } 
  if (fallingEdge(BTN_SPINDLE)) {
    Serial.println("Rotary Spindle: Counter Clockwise");
  } 
  if (risingEdge(BTN_Z_UP)) {
    Serial.println("Z-Axis Up: Pressed");
  } 
  if (fallingEdge(BTN_Z_UP)) {
    Serial.println("Z-Axis Up: Released");
  } 
  if (risingEdge(BTN_Z_DOWN)) {
    Serial.println("Z-Axis Down: Pressed");
  } 
  if (fallingEdge(BTN_Z_DOWN)) {
    Serial.println("Z-Axis Down: Released");
  } 

#endif
}
