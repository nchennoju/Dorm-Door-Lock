#include <ESP8266WiFi.h>
#include <Servo.h>

#define LED D0

unsigned char led_status=0;
const char* ssid = "NETGEAR";
const char* password = "password";

Servo servo;

const int ON = 60;
const int OFF = 150;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  servo.attach(D1);
  servo.write(OFF);

  Serial.print("CONNECTING TO");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print("-");
  }
  Serial.println("");
  Serial.println("WIFI CONNECTED! " + String(ssid));

  server.begin();
  Serial.println("MINI SERVER STARTS AT : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("CLIENT FOUND");
  while (!client.available())
  {
    delay (1);
  }
 
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
 
  if(req.indexOf("/off") != -1) {
    led_status=0;
    digitalWrite(LED, HIGH);
    for(int i = servo.read(); i <= OFF; i++){
      servo.write(i);
      delay(15);
    }
    //servo.write(OFF);
    Serial.println("OFF");
  }else if(req.indexOf("/on") != -1) {
    led_status=1;
    digitalWrite(LED,LOW);
    for(int i = servo.read(); i >= ON; i--){
      servo.write(i);
      delay(15);
    }
    //servo.write(ON);
    Serial.println("ON");
  }else if(req.indexOf("/disco") != -1){
    //while(true){
      for(int i = servo.read(); i <= OFF; i++){
        servo.write(i);
        delay(10);
      }
      delay(400);
      for(int i = servo.read(); i >= ON; i--){
        servo.write(i);
        delay(10);
      }
      delay(400);
      /*servo.write(ON);
      delay(400);
      servo.write(OFF);
      delay(400);*/
    //}
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Dorm Light Control</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  client.println("</head>");

  client.println("<style> .slidecontainer { width: 100%; }");
  client.println(".slider {    -webkit-appearance: none;    width: 100%;   height: 25px;  background: #d3d3d3;  outline: none;  opacity: 0.7;  -webkit-transition: .2s; transition: opacity .2s; }");
  client.println(".slider:hover {  opacity: 1; }");
  client.println(".slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 25px; height: 25px; background: #4CAF50; cursor: pointer; }");
  client.println("  .slider::-moz-range-thumb { width: 25px; height: 25px; background: #4CAF50; cursor: pointer; }");
  client.println("</style>");
  
  
  client.println("<body style=\"height:100%;margin:0;background:#f6f6f6\">");
  if(led_status == 1){
    client.println("<div style=\"height:6%;text-align:center;padding:3px;background:#78d665;color:white;\">");
    client.println("<h1 style=\"color:white;\">Light/Fan Control</h1>");
  }else if(led_status == 0){
    client.println("<div style=\"height:6%;text-align:center;padding:3px;background:#ff5c5c;color:white;\">");
    client.println("<h1 style=\"color:white;\">Light/Fan Control</h1>");
  }
  client.println("</div>");
  
  client.println("<br/>");
  client.println("<br/>");
  if(led_status == 1){
    client.println("<div style=\"color:#78d665;margin-left:20px;text-align:left;font-size:20px;font-weight:bold;\">ON</div>");
  }else if(led_status == 0){
    client.println("<div style=\"color:#ff5c5c;margin-left:20px;text-align:left;font-size:20px;font-weight:bold;\">OFF</div>");
  }else{
    client.println("<div style=\"color:#6850f2;margin-left:20px;text-align:left;font-size:20px;font-weight:bold;\">DISCO</div>");
  }
  client.println("<div style=\"color:#212121;margin-left:20px;text-align:left;\">status</div>");
  client.println("<br/>");
  client.println("<br/>");
  client.println("<div style=\"margin-left:20px;text-align:left;\">");
  client.println("<a href=\"/on\"\" style=\"color:white;background:#78d665;padding:10px;border-radius:20px;text-decoration: none;\">ON </a>");
  client.println("</div>");
  client.println("<br/>");
  client.println("<br/>");
  client.println("<div style=\"margin-left:20px;text-align:left;\">");
  client.println("<a href=\"/off\"\" style=\"color:white;background:#ff5c5c;padding:10px;border-radius:20px;text-decoration: none;\">OFF </a>");
  client.println("</div>");
  client.println("<br/>");
  client.println("<br/>");
  client.println("<div style=\"margin-left:20px;text-align:left;\">");
  client.println("<a href=\"/disco\"\" style=\"color:white;background:#6850f2;padding:10px;border-radius:20px;text-decoration: none;\">DISCO </a>");
  client.println("</div>");

  client.println("<br/>");
  client.println("<br/>");
  client.println("<br/>");

  client.println("<div class=\"slidecontainer\">");
  client.println("<input type=\"range\" min=\"60\" max=\"140\" value=\"100\" class=\"slider\" id=\"myRange\">");
  client.println("</div>");
  client.println("<br/>");
  client.println("<br/>");
  client.println("<br/>");
  
  client.println("<div style=\"text-align:center;\">");
  client.println("<a style=\"color:white;background:#000;padding:10px;border-radius:10px;\">FAN </a>");
  client.println("<p>Value: <span id=\"demo\"></span></p>");
  client.println("</div>");

  client.println("<script>");
  client.println("var slider = document.getElementById(\"myRange\");");
  client.println("var output = document.getElementById(\"demo\");");
  client.println("output.innerHTML = slider.value;");
  client.println("slider.oninput = function() { output.innerHTML = this.value; }");
  client.println("</script>");

  /*client.println("<p>Button State:<b> ");
  client.println(buttonState);
  client.println("</b></p>");*/

  
  client.println("</body>");
  client.println("</html>");
  

  delay(1);
  Serial.println("RESPONSE SENT");
  Serial.println("");

}
