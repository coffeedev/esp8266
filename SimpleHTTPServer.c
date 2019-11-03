#include <ESP8266WiFi.h>

const short int BUILTIN_LED1 = 2; //GPIO2
void upLed(int led);
void downLed(int led) ;
void blinkLed(int ms, int times, int led) ;
void doSOS() ;
String prepareHTMLPage() ;

#define DIT 400
#define WPM 1
#define turnOn 0
#define turnOff 1

WiFiServer server(80); //Initialize the server on Port 80
IPAddress HTTPS_ServerIP ;

void setup()
{

  pinMode(BUILTIN_LED1, OUTPUT);

  WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
  WiFi.softAP("BeltIOT", "12345678"); // Provide the (SSID, password);


  server.begin(); // Start the HTTP Server

  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
  HTTPS_ServerIP = WiFi.softAPIP(); // Obtain the IP of the Server
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(HTTPS_ServerIP);
  Serial.println("/");

  downLed(BUILTIN_LED1) ;

}

void upLed(int led = BUILTIN_LED1)
{
  digitalWrite(led, turnOn) ;
}

void downLed(int led = BUILTIN_LED1)
{
  digitalWrite(led, turnOff) ;
}

void blinkLed(int ms = 500, int times = 1, int led = BUILTIN_LED1)
{
  int ctr = 0 ;
  while (ctr < times)
  {
    upLed(led) ;
    delay(ms) ;
    downLed(led) ;
    delay(ms) ;
    ctr = ctr + 1 ;
  }
}

void doSOS()
{
  blinkLed((DIT / WPM), 3) ;
  delay(DIT) ;

  blinkLed(((DIT * 3) / WPM), 3) ;
  delay(DIT) ;

  blinkLed((DIT / WPM), 3) ;
  delay(DIT) ;

}

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request) ;
  client.flush() ;



  if (request.indexOf("/on") > 0)  {
    upLed() ;

  }
  if (request.indexOf("/off") > 0)  {
    downLed() ;
  }

  if (request.indexOf("/blink") > 0)  {
    blinkLed() ;
  }

  if (request.indexOf("/sos") > 0)  {
    doSOS() ;
  }

  client.println(prepareHTMLPage()) ;
  //client.connect(HTTPS_ServerIP, 80) ;
  
 
}

String prepareHTMLPage()
{
  String header = String("HTTP/1.1 200 OK\r\n")
                  + "Content-Type: text/html"
                  + " "
                  + "Connection: close\r\n"
                  + "\r\n" ;

  String body = String(" ")
                + "<!DOCTYPE HTML>"
                + "<html>"
                + "<head>"
                + "<meta name='apple-mobile-web-app-capable' content='yes' />"
                + "<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />"
                + "</head>"
                + "<body bgcolor = \"#f7e6ec\">"
                + "<hr>"
                + "<h1><center> Belthur Automation </center></h1>"
                + "<hr>"
                + "<br><br>"
                + "<center>"
                + "<a href=\"/on\"\"><button style=\"font-size: 48px\">Turn On</button></a><br />"
                + "<br/>"
                + "<a href=\"/off\"\"><button style=\"font-size: 48px\">Turn Off</button></a><br />"
                + "<br/>"
                + "<a href=\"/blink\"\"><button style=\"font-size: 48px\">Blink</button></a><br />"
                + "<br/>"
                + "<a href=\"/sos\"\"><button style=\"font-size: 48px\">S O S</button></a><br />"
                + "</center>"
                + "<br><br>"
                + "</body></html>"
                + "\r\n";

  return header + body ;

}
