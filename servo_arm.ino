#include <Servo.h>

Servo base_servo; 
Servo knee_servo;
Servo elbo_servo;
Servo neck_servo;
Servo grab_servo;

float base_pos = 90;
float knee_pos = 90;
float elbo_pos = 90;
float neck_pos = 90;
int grab_pos = 0;

int grab_read = 1;
int grab_it = 1;

const int Grab_pin = 2; // lower digital switch output

const int Lo_X_pin = 0; // lower analog X output
const int Lo_Y_pin = 1;

const int Up_X_pin = 2; // upper
const int Up_Y_pin = 3;

int print_limiter = 1;

void setup() {
  Serial.begin(9600);
  
  pinMode(Grab_pin, INPUT);
  digitalWrite(Grab_pin, HIGH);
  
  base_servo.attach(8);
  knee_servo.attach(9);
  elbo_servo.attach(10);
  neck_servo.attach(11);
  grab_servo.attach(12);
}

void loop() {
  base_pos = round((analogRead(Lo_X_pin) / 14.628) + 55);
  int knee_move = map(analogRead(Lo_Y_pin), 0, 1023, -90, 90);
  int elbo_move = map(analogRead(Up_X_pin), 0, 1023, -90, 90);
  int neck_move = map(analogRead(Up_Y_pin), 0, 1023, -90, 90);
  grab_read = digitalRead(Grab_pin);
  
  base_servo.write(base_pos);

  if ((knee_move < 0 and knee_pos > 0) or (knee_move > 0 and knee_pos < 180)) knee_pos = constrain(knee_pos + knee_move, 0, 180);
  if ((elbo_move < 0 and elbo_pos > 0) or (elbo_move > 0 and elbo_pos < 180)) elbo_pos = constrain(elbo_pos + elbo_move, 0, 180);
  if ((neck_move < 0 and neck_pos > 0) or (neck_move > 0 and neck_pos < 180)) neck_pos = constrain(neck_pos + neck_move, 0, 180);

  if (grab_read == 0 and grab_it == 1) {
    grab_it = 0;
    grab_pos = (grab_pos != 180) ? 180 : 0;
  } else if (grab_read == 1) {
    grab_it = 1;
  }

  knee_servo.write(knee_pos);
  elbo_servo.write(elbo_pos);
  neck_servo.write(neck_pos);
  grab_servo.write(grab_pos);

  print_limiter += 1;
  if (print_limiter % 7 == 0) {
    print_limiter = 1;
    Serial.println("Base: " + String(base_pos));
    Serial.println("Knee: " + String(knee_pos) + " / move : " + String(knee_move));
    Serial.println("Elbo: " + String(elbo_pos) + " / move : " + String(elbo_move));
    Serial.println("Neck: " + String(neck_pos) + " / move : " + String(neck_move));
    Serial.println("Swch: " + String(digitalRead(Grab_pin)));
    Serial.println("Grab: " + String((grab_pos) ? "Engaged" : "Disenaged"));
    Serial.println("\n\n");
  }

  delay(150);
}
