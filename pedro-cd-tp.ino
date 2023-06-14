#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

#define trigger_pin 5
#define Echo_pin 18

//Ligacao a internet
const char* ssid = "freeismat";
const char* password = "";

//numero de telemovel e numero indentificador do mesmo na API
String phoneNumber = "";
String apiKey = "";

long duration;
int distance;

void getDistance(){
  digitalWrite(trigger_pin, LOW);
delayMicroseconds(2);

digitalWrite(trigger_pin, HIGH);  
delayMicroseconds(10);            
digitalWrite(trigger_pin, LOW);  


duration = pulseIn(Echo_pin, HIGH); 
distance= duration*0.034/2;

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
delay(1000);
}

void sendMessage(String message){

  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void setup() {
pinMode(trigger_pin, OUTPUT); 
pinMode(Echo_pin, INPUT); 
Serial.begin(9600); 
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
