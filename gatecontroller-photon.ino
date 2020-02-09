/*
 * Project GateController_Sender
 * Description: Code for sending signal down to controller at gate for more physical control remotely
 * Author: Steven Bucher
 * Date: Jan 20 2020
 */

const int btn_7878 = 3;
const int openSwitch_7878 = 4;
const int closeSwitch_7878 = 2;
const int led_7878 = 0;

const int btn_7700 = 6;
const int openSwitch_7700 = 7;
const int closeSwitch_7700 = 5;
const int led_7700 = 1;

int data[3] = {0,0,0};
int databefore[3] = {0,0,0};
bool pub = false;
String data7878;

int data_1[3] = {0,0,0};
int databefore_1[3] = {0,0,0};
bool pub_1 = false;
String data7700;

bool b_led_7878 = false;


int openTime = 1100;
int closeTime = 1700;

void setup() {
  Particle.syncTime();
  Time.zone(-8);

  pinMode(btn_7878, INPUT_PULLUP);
  pinMode(openSwitch_7878, INPUT_PULLUP);
  pinMode(closeSwitch_7878, INPUT_PULLUP);
  pinMode(led_7878, OUTPUT);

  pinMode(btn_7700, INPUT_PULLUP);
  pinMode(openSwitch_7700, INPUT_PULLUP);
  pinMode(closeSwitch_7700, INPUT_PULLUP);
  pinMode(led_7700, OUTPUT);
  Particle.subscribe("response_7878", response_7878);
  Particle.subscribe("response_7700", response_7700);

  Particle.publish("gate_7878", "000");
  Particle.publish("gate_7700", "000");

  Particle.variable("LED", b_led_7878);
}

void loop() {
  databefore[0] = data[0];
  databefore[1] = data[1];
  databefore[2] = data[2];

  databefore_1[0] = data_1[0];
  databefore_1[1] = data_1[1];
  databefore_1[2] = data_1[2];

// Logic for 7878 Gate
  if(!digitalRead(btn_7878)){
    data[0] = 1;
    Particle.publish("gate_7878", String(data[0])+String(data[1])+String(data[2]));
    delay(5000);
    data[0] = 0;
    Particle.publish("gate_7878", String(data[0])+String(data[1])+String(data[2]));

  }
  if(!digitalRead(openSwitch_7878)){
    data[1] = 1;
  }else{
    data[1] = 0;
  }

  if(!digitalRead(closeSwitch_7878)){
    data[2] = 1;
  }else{
    data[2] = 0;
  }

// Logic for 7700 Gate
  if(!digitalRead(btn_7700)){
    data_1[0] = 1;
    data7700 = String(data_1[0])+String(data_1[1])+String(data_1[2]);
    Particle.publish("gate_7700", data7700);
    delay(1000);
    data_1[0] = 0;
    data7700 = String(data_1[0])+String(data_1[1])+String(data_1[2]);
    Particle.publish("gate_7700", data7700);
  }
  if(!digitalRead(openSwitch_7700)){
    data_1[1] = 1;
  }else{
    data_1[1] = 0;
  }

  if(!digitalRead(closeSwitch_7700)){
    data_1[2] = 1;
  }else{
    data_1[2] = 0;
  }

  pub = false;
  pub_1 = false;

  for(int i = 1; i <= 2; i++){
    if(data[i] != databefore[i]){
      pub = true;
    }

    if(data_1[i] != databefore_1[i]){
      pub_1 = true;
    }
  }

  delay(1000);
  if(pub){
    data7878 = String(data[0])+String(data[1])+String(data[2]);
    Particle.publish("gate_7878", data7878);
  }
  if(pub_1){
    data7700 = String(data_1[0]) + String(data_1[1]) + String(data_1[2]);
    Particle.publish("gate_7700", data7700);
  }

  /*
  if(data[2] == 1){
    digitalWrite(led_7878, LOW);
  }
  else if(data[1] == 1 || checkSchedule()){
    digitalWrite(led_7878, HIGH);
  }else {
    digitalWrite(led_7878, LOW);
  }



  if(b_led_7878){
    digitalWrite(led_7878, HIGH);
  } else {
    digitalWrite(led_7878, LOW);
  }*/

}


bool checkSchedule(void){
  String hr_min = leadingZero(Time.hour()) + leadingZero(Time.minute());

  int i_hr_min = atoi(hr_min);
  if(i_hr_min >= openTime && i_hr_min <= closeTime){
    return true;
  } else {
    return false;
  }
}
String leadingZero(int x){
  String str0;
  if(x >=0 && x <=9){
      str0 = "0" + String(x);
      return str0;
  }else{
      return String(x);
  }
}

void response_7878(const char *event, const char *data){
  if(strcmp(data, "open") == 0){
    digitalWrite(led_7878, HIGH);
  } else {
    digitalWrite(led_7878, LOW);
  }


}

void response_7700(const char *event, const char *data){
  if(strcmp(data, "open") == 0){
    digitalWrite(led_7700, HIGH);
  } else {
    digitalWrite(led_7700, LOW);

  }
}
