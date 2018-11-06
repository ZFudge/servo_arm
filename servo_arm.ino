#include <Servo.h>

Servo base_servo; 
Servo knee_servo;
Servo elbo_servo;
Servo neck_servo;
Servo grab_servo;

int base_pos = 90;
int knee_pos = 90;
int elbo_pos = 90;
int neck_pos = 90;
int grab_pos = 0;

int grab_read = 1;
int grab_it = 1;

const int Grab_pin = 2; // lower digital switch output

const int Lo_X_pin = 0; // lower analog X axis output
const int Lo_Y_pin = 1;

const int Up_X_pin = 2; // upper servos
const int Up_Y_pin = 3;

int print_limiter = 1; // variables to control print frequency

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
  base_pos = map(analogRead(Lo_X_pin), 0, 1023, 58, 125);  
  int knee_read = analogRead(Lo_Y_pin);
  int elbo_read = analogRead(Up_X_pin);
  int neck_read = analogRead(Up_Y_pin);
  grab_read = digitalRead(Grab_pin);

  int knee_move = (knee_read > 511) ? map(knee_read, 511, 1023, 0, 180 - knee_pos) : map(knee_read, 0, 510, -1 * knee_pos, 0);
  int elbo_move = (elbo_read > 511) ? map(elbo_read, 511, 1023, 0, 180 - elbo_pos) : map(elbo_read, 0, 510, -1 * elbo_pos, 0);
  int neck_move = (neck_read > 511) ? map(neck_read, 511, 1023, 0, 180 - neck_pos) : map(neck_read, 0, 510, -1 * neck_pos, 0);

  if ((knee_move < 0 and knee_pos > 0) or (knee_move > 0 and knee_pos < 180)) knee_pos = constrain(knee_pos + knee_move, 0, 180);
  if ((elbo_move < 0 and elbo_pos > 0) or (elbo_move > 0 and elbo_pos < 180)) elbo_pos = constrain(elbo_pos + elbo_move, 0, 180);
  if ((neck_move < 0 and neck_pos > 0) or (neck_move > 0 and neck_pos < 180)) neck_pos = constrain(neck_pos + neck_move, 0, 180);

  if (grab_read == 0 and grab_it == 1) {
    grab_it = 0;
    grab_pos = (grab_pos != 180) ? 180 : 0;
  } else if (grab_read == 1) {
    grab_it = 1;
  }

  base_servo.write(base_pos);
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
