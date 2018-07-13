#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
uint8_t servonum = 0;

const char* ssid = "ESP_Servos";  // SSID of esp8266
//const char* password = "123";   //
bool toggle=0;                  //Variable to switch on and off the solenoid
ESP8266WebServer server(80);    //Specify port for TCP connection

void handleGenericArgs()
{ //Handler

  String message = "Number of args received:";
  String argn="";
  String argVal="";
  message += server.args();            //Get number of parameters
  message += "\n";                            //Add a new line

  for (int i = 0; i < server.args(); i++) {

    message += "Arg n" + (String)i + "–> ";   //Include the current iteration value
    message += server.argName(i) + ": ";     //Get the name of the parameter
    argn=server.argName(i);
    message += server.arg(i) + "\n";              //Get the value of the parameter
    argVal=server.arg(i);

  } 
  if (argn=="1f"){
     pwm.setPWM(0, 0, argVal.toInt());///first number represents the servo number
  }
  else if(argn=="2f"){
    pwm.setPWM(4, 0, argVal.toInt());
  }

  server.send(200, "text/plain", message);       //Response to the HTTP request

}
void handleRoot() {
 // toggle=!toggle;               //Toggling Led Variable
   // digitalWrite(2,toggle);     //Toggle Led
        String s = "\r\n\r\n  <!DOCTYPE HTML>\r\n<html><h1>Esp8266 Communication</h1> ";
        s += "<p>Success!!!</html>\r\n\r\n";  
        
        server.send(200,"text/html",s);      //Reply to the client
        //s=s.toInt();
        if(s.indexOf("/1")!=-1){
          pwm.setPWM(0, 0, s.toInt());///first number represents the servo number
        }
         if(s.indexOf("/2")!=-1){
          pwm.setPWM(4, 0, s.toInt());
        }
        delay(10);

  //        for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
             
  
}

void setup() {
  delay(200);                           //Stable Wifi
  Serial.begin(115200);                 //Set Baud Rate
  pinMode(2, OUTPUT);                   //Led/Solenoid at pin 2
  WiFi.softAP(ssid);//, password);      //In Access Point Mode

  IPAddress myIP = WiFi.softAPIP();     //Check the IP assigned. Put this Ip in the client host.
  Serial.print("AP IP address: ");
  Serial.println(myIP);                 //Print the esp8266-01 IP(Client must also be on the save IP series)
  server.on("/Pres", handleGenericArgs);           //Checking client connection
  server.begin();                       // Start the server
  Serial.println("Server started");
  pwm.begin();
 
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay(10);
}

void loop() {
  // Check if a client has connected. On first connection switch on the Solenoid on next switch off.
  server.handleClient();
}
