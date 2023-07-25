
//Include libraries here:
#include <WiFi.h>
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <firebase_settings.h>

//Declare constants here:
#define WIFI_SSID "EiG-LAB"
#define WIFI_PASSWORD "12345678"

//Put function declarations here:
void WiFi_init();
void firebase_init();
void firebase_write(String path, String data);
String firebase_read(String path);

// Intialize Variables Here:
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

//Firebase Objects:
FirebaseData fbda;
FirebaseAuth auth;
FirebaseConfig conf;



void setup() {

  Serial.begin(115200);
  WiFi_init();
  firebase_init();
}

void loop() {

  firebase_write("test/data", "Apple");
  vTaskDelay(1000);
  Serial.print(firebase_read("test/data"));

}

void WiFi_init(){
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void firebase_init(){
  /* Assign the api key (required) */
  conf.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  conf.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&conf, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", conf.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  conf.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&conf, &auth);
  Firebase.reconnectWiFi(true);
}

void firebase_write(String path, String data){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
      sendDataPrevMillis = millis();
      // Write an Int number on the database path test/int
    if (Firebase.RTDB.setString(&fbda, path, data)){
      Serial.println("PASSED");
    }
    else {
      Serial.println("FAILED");
    }
  }
}

String firebase_read(String path){
  if (Firebase.RTDB.getString(&fbda, path)){
        return fbda.stringData();
      }

    else {
        return fbda.errorReason();
      }
}