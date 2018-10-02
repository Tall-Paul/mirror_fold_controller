//states
const int BUTTON_PUSHED = LOW;
const int BUTTON_UNPUSHED = HIGH;
const int IGNITION_RUNNING = LOW;
const int IGNITION_STOPPED = HIGH;
const int MIRROR_OPEN = 2; 
const int MIRROR_CLOSED = 1;

//pins
const int PIN_OUT_MIRROR_CLOSE = 15;
const int PIN_OUT_MIRROR_OPEN = 13;
const int PIN_IN_IGNITION = 4;
const int PIN_IN_BUTTON = 5;

//times
const int MOTOR_TIME = 4000;
const int DEBOUNCE_TIME = 100;

//variables
int mirror_status = MIRROR_CLOSED;
int motor_status = 0;
unsigned long motor_stop_time = 0;
unsigned long debouncing = 0;
int engine_on_triggered = 0;
int engine_off_triggered = 0;


void mirror_close(){
  motor_stop_time = millis() + MOTOR_TIME;
  digitalWrite(PIN_OUT_MIRROR_CLOSE,LOW);
  digitalWrite(PIN_OUT_MIRROR_OPEN,HIGH);
  mirror_status = MIRROR_CLOSED; 
  motor_status = 1;
}

void mirror_stop(){
  digitalWrite(PIN_OUT_MIRROR_CLOSE,HIGH);
  digitalWrite(PIN_OUT_MIRROR_OPEN,HIGH);
  motor_status = 0;
}

void mirror_open(){
  motor_stop_time = millis() + motor_run_time;
  digitalWrite(PIN_OUT_MIRROR_CLOSE,HIGH);
  digitalWrite(PIN_OUT_MIRROR_OPEN,LOW);
  mirror_status = MIRROR_OPEN; 
  motor_status = 1;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_OUT_MIRROR_OPEN, OUTPUT);
  pinMode(PIN_OUT_MIRROR_CLOSE, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(PIN_IN_IGNITION, INPUT_PULLUP);
  pinMode(PIN_IN_BUTTON, INPUT_PULLUP);
  Serial.begin(57600);
}

void loop() {
  int button_state = digitalRead(PIN_IN_BUTTON);
  int engine_state = digitalRead(PIN_IN_IGNITION);
  if (engine_state == RUNNING){
    engine_off_triggered = 0;
  } else {
    engine_on_triggered = 0;
  }
  
  
  if (motor_status == 0){
    if (button_state == PUSHED && debouncing < millis()){
      debouncing = millis() + debounce_time;
      if (mirror_status == MIRROR_OPEN){
        mirror_close();
      } else {
        mirror_open();
      }
    } else {
      if (engine_state == RUNNING && mirror_status == MIRROR_CLOSED && engine_on_triggered == 0){
        engine_on_triggered = 1;
        mirror_open();
      }
      if (engine_state == STOPPED && mirror_status == MIRROR_OPEN && engine_off_triggered == 0){
        engine_off_triggered = 1;
        mirror_close();
      }
    }
  } else {
    if (motor_stop_time < millis()){
      mirror_stop();
    }
  }
 }

