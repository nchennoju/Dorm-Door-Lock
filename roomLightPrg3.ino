#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define LED BUILTIN_LED

char auth[] = "sfjhdjkhdjkhdjkhd";

char ssid[] = "wifi";
char pass[] = "password";

String lastMsg = "";
int state = 0;

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
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
      digitalWrite(LED, LOW);
    }else{
      digitalWrite(LED, HIGH);
    } 
}

void loop()
{
  Blynk.run();
}
