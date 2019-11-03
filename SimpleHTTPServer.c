#include <ESP8266WiFi.h>

const short int BUILTIN_LED1 = 2; //GPIO2
void upLed(int led);
void downLed(int led) ;
void blinkLed(int led, int ms) ;

#define turnOn 0 // It's reverse
#define turnOff 1

WiFiServer server(80); //Initialize the server on Port 80

void setup()
{

  pinMode(BUILTIN_LED1, OUTPUT);

  WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
  WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password);


  server.begin(); // Start the HTTP Server

  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
  IPAddress HTTPS_ServerIP = WiFi.softAPIP(); // Obtain the IP of the Server
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

void blinkLed(int led = BUILTIN_LED1, int ms = 500)
{
  upLed(led) ;
  delay(ms) ;
  downLed(led) ;

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



    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
  
    client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
    client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
    client.println("</head>");
    client.println("<body bgcolor = \"#f7e6ec\">");
    client.println("<hr>");
    client.println("<h1><center> Home Automation </center></h1>");
    client.println("<hr>");
    client.println("<br><br>");
  
    client.println("<center>");
    
    client.println("<a href=\"/on\"\"><button>Turn On </button></a><br />");
    client.println("<a href=\"/off\"\"><button>Turn Off </button></a><br />");
    client.println("<a href=\"/blink\"\"><button>Blink </button></a><br />");
    client.println("</center>");
    client.println("<br><br>");
    client.println("</body>") ;
    client.println("</html>");
  
}

