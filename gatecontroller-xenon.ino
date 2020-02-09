/*
 * Project GateController_v2 NO OLED
 * Description: Brains of controller down at the gate, reads FIRE input and fires relay accordingly
 * Author: Steven Bucher
 * Date: 1-2-19
 */


// Pins for controlling relay and reading FIRE
const int relayPin = 7;
const int fireInput = 6;

// Variables for scheduling
// Exposed to the cloud
int openTime = 1100;
int closeTime = 1700;
int tZone = -8;
String status = "open";
String prevStatus;
String tNow;
bool keepClosed = false;
bool keepOpen = false;

void setup() {

  // Setting up all necesarry pins
  pinMode(relayPin, OUTPUT);
  pinMode(fireInput, INPUT_PULLUP);

  Particle.syncTime();
  Time.zone(tZone);
  Particle.subscribe("gate_7878", recievingData);

  Particle.variable("openTime", openTime);
  Particle.variable("closeTime", closeTime);
  Particle.variable("status", status);
  Particle.variable("timeNow", tNow);
  Particle.variable("keepClosed", keepClosed);
  Particle.variable("keepOpen", keepOpen);
  

//   Particle.function("ChangeOpenTime", changeOpen);
//   Particle.function("ChangeCloseTime", changeClose);
}

void loop() {
 
  prevStatus = status;
 
  if(!digitalRead(fireInput)){
    digitalWrite(relayPin, HIGH);
    status = "open";
  }else{
    if(keepClosed){
      digitalWrite(relayPin, LOW);
      status = "closed";
   } else if(keepOpen){
      digitalWrite(relayPin, HIGH);
      status = "open";
    } else if(checkSchedule()){
      digitalWrite(relayPin, HIGH);
      status = "open";
    } else {
      digitalWrite(relayPin, LOW);
      status = "closed";
   }
  }
  delay(2000);
  if(status != prevStatus){
    Particle.publish("response_7878", status);
  }
/*
  if(status == "OPEN" && status != prevStatus){
    Particle.publish("response_7878", "open");
  } else if(status != prevStatus){
    Particle.publish("response_7878", "closed");
  }
*/

}



bool checkSchedule(void){
  if(Time.weekday() == 4){
      return false;
  }
  
  
  String hr_min = leadingZero(Time.hour()) + leadingZero(Time.minute());
  tNow = hr_min;
  int i_hr_min = atoi(hr_min);
  if(i_hr_min >= openTime && i_hr_min <= closeTime){
    return true;
  } else {
    return false;
  }
}

void recievingData(const char *event, String data){

  keepOpen = false;
  keepClosed = false;

  if(data.charAt(0) == '1'){
    pulseOnce();
  } else {
    if(data.charAt(2) == '1'){
      //alwasy keep closed 
      keepClosed = true;
    } else {
        if(data.charAt(1) == '1'){
          // always keep open
          keepOpen = true;
        }
    }
  }
}

void pulseOnce(void){
  digitalWrite(relayPin, HIGH);
  status = "open";
  Particle.publish("response_7878", status);
  delay(3000);
  digitalWrite(relayPin, LOW);
  status = "closed";
  Particle.publish("response_7878", status);
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
/*
int changeOpen(String command){
  if(command.length() != 4){
    return -1;
  }
  openTime = atoi(command);
  return 1;
}

int changeClose(String command){
  if(command.length() != 4){
    return -1;
  }
  closeTime = atoi(command);
  return 1;
}*/
