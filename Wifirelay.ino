/* =================================================
*   Controlling NodeMCU through an Android APP
*
*  Devices
*   ==> Relay1 represented by led 1
*   ==> Relay2 represented by led 2
*   
*  Groups 
*   ==> All Devices ON/OFF
*
*  Program by Luc Volders
*  http://lucstechblog.blogspot.nl/
*=================================================*/
#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "XXXXXXXXXX";
const char* password = "YYYYYYYYYY";
String  command =""; // Command received from Android device

// Set led/relay Pins
int switch1 = 13;
int switch2 = 15;

void setup()
{
  Serial.begin(115200);

  pinMode(switch1, OUTPUT); 
  pinMode(switch2, OUTPUT);   

  digitalWrite(switch1,LOW);
  digitalWrite(switch2,LOW);
      
  connectWiFi();
  server.begin();
}

void loop()
{
    client = server.available();
    if (!client) return; 
    command = checkClient ();

         if (command == "sw1on") digitalWrite(switch1,HIGH);
    else if (command == "sw1off") digitalWrite(switch1,LOW);
    else if (command == "sw2on") digitalWrite(switch2,HIGH);     
    else if (command == "sw2off") digitalWrite(switch2,LOW);
    else if (command == "allon") 
    {
      digitalWrite(switch1,HIGH);
      digitalWrite(switch2,HIGH);
    }
    else if (command == "alloff") 
    {
      digitalWrite(switch1,LOW);
      digitalWrite(switch2,LOW);
    }
    sendBackEcho(command); // send command echo back to android device
    command = "";
}

/* connecting WiFi */
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connection established");
  Serial.println("IP number is : ");
  Serial.print((WiFi.localIP()));
}

/* check command received from Android Device */
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

/* send command echo back to android device */
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}
