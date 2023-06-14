#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

#define trigger_pin 5
#define Echo_pin 18

const char* ssid = "freeismat";
const char* password = "";

String phoneNumber = "";
String apiKey = "";

/* two variables to store duraion and distance value */
long duration;
int distance;

void getDistance(){
  digitalWrite(trigger_pin, LOW); //set trigger signal low for 2us
delayMicroseconds(2);

/*send 10 microsecond pulse to trigger pin of HC-SR04 */
digitalWrite(trigger_pin, HIGH);  // make trigger pin active high
delayMicroseconds(10);            // wait for 10 microseconds
digitalWrite(trigger_pin, LOW);   // make trigger pin active low

/*Measure the Echo output signal duration or pulss width */
duration = pulseIn(Echo_pin, HIGH); // save time duration value in "duration variable
distance= duration*0.034/2; //Convert pulse duration into distance

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
delay(1000);
}

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

/* configure D9 and D11 as digital input and output respectively */
void setup() {
pinMode(trigger_pin, OUTPUT); // configure the trigger_pin(D9) as an Output
pinMode(Echo_pin, INPUT); // configure the Echo_pin(D11) as an Input
Serial.begin(9600); // Enable the serial with 9600 baud rate
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  getDistance();
  if(distance<5 && distance!=0)
    sendMessage("Alerta Vermelho");

}