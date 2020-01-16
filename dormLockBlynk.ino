#define BLYNK_PRINT Serial

#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define LED BUILTIN_LED

char auth[] = "jwEgv4MqX-Y5nUEbMFFM8PJT5_dZoq6n";

char ssid[] = "Network";
char pass[] = "password";

String lastMsg = "";
int state = 0;

Servo servo;
const int UNLOCK = 40;
const int LOCK = 155;


void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  servo.attach(D1);

  Blynk.begin(auth, ssid, pass);
}

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

BLYNK_WRITE(V0) {
  lastMsg = param.asStr();
  Serial.println("Message: " + lastMsg);
  Blynk.virtualWrite(V0, "clr");
  Blynk.virtualWrite(V0, "Last Message:\n\n" + lastMsg);
}

BLYNK_WRITE(V1) {
    state = param.asInt();
    Serial.println("ON/OFF State: " + String(state));
    if(state == 0){
      digitalWrite(D4, LOW);
      digitalWrite(D2, HIGH);
      servo.write(LOCK);
    }else{
      digitalWrite(D4, HIGH);
      digitalWrite(D2, LOW);
      servo.write(UNLOCK);
    } 
}


void loop()
{
  Blynk.run();
}
