CODIGO CON TELEGRAM
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP32Servo.h>

#define relayPin 12
#define DHTPin 14
#define DHTPIN 18      // El pin de datos del sensor DHT11
#define DHTTYPE DHT11 // Indicamos que se trata del modelo DHT11
#define Sensor 23 
#define Buzzer 25
#define BUZZER_PIN 19  // El pin del buzzer
#define BUZZER_FREQ 2000 // La frecuencia del sonido del buzzer

DHT dht(DHTPin, DHT11);
DHT dht1(DHTPIN, DHTTYPE);
Servo servo1; // Servo conectado al pin 2
Servo servo2; // Servo conectado al pin 4
Servo servo3; // Servo conectado al pin 5

//Variables
int ledPin = 2; //Connection state
int ledPin2 = 4; //Show received messages
long timeNow, timeLast; //Not blocking time
int wait = 5000; //Wait 5 seconds to send new message

const int servo1Interval = 300000; // Intervalo de tiempo para mover el servo 1 (5 minutos)
const int servo2Interval = 180000; // Intervalo de tiempo para mover el servo 2 (3 minutos)
unsigned long servo1PreviousMillis = 0; // Tiempo anterior para el servo 1
unsigned long servo2PreviousMillis = 0; // Tiempo anterior para el servo 2
const int tempPin = A0; // Pin analógico para el sensor de temperatura
const float tempThreshold = 30.0; // Umbral de temperatura para el servo 3 (30 grados Celsius)

//Conección a Internet 
const char* ssid = "Danis";
const char* password = "@Daniela";
//Conección a MQTT
const char* mqtt_server = "192.168.43.134";
IPAddress server(192,168,43,134);
const int mqtt_port = 1883;

//Communication objects
WiFiClient espClient;
PubSubClient client(espClient);

//________________________________________________________________
//------------------------- TELEGRAM -----------------------------
#define BOTtoken "6160458310:AAGB5Bcy6GPLxyenMMyHuYcxprTWdtOg_tY"
#define CHAT_ID "6239852318"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
WiFiClientSecure clientRelay;
UniversalTelegramBot bot(BOTtoken, clientRelay);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
//const int ledPin = 2;
bool relayFState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "Usa los siguientes comanod para la casa de tu Hamster.\n\n";
      welcome += "/led_on enciende el Foco en la jaula\n";
      welcome += "/led_off apaga el Foco en la jaula \n";
      welcome += "/state muestra el estado actual del foco en la jaula \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "El Foco esta encendido", "");
      relayFState = HIGH;
      digitalWrite(relayPin, relayFState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "El foco esta apagado", "");
      relayFState = LOW;
      digitalWrite(relayPin, relayFState);
    }
    
    if (text == "/state") {
      if (digitalRead(relayPin)){
        bot.sendMessage(chat_id, "ENCENDIDO", "");
      }
      else{
        bot.sendMessage(chat_id, "APAGADO", "");
      }
    }
  }
}
//________________________________________________________________


void setup() {
  //________________________________________________________________
  //------------------------- TELEGRAM -----------------------------
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    clientRelay.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayFState);
  
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    clientRelay.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  //________________________________________________________________
  //Iniciando la Comunicación con el Serial---------------------------------------------------------------------------------------------
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  /*
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); //Connecting to wifi

  while(WiFi.status()!= WL_CONNECTED){
    Serial.print("."); // Progress indicator
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(5);
  }

  //Connection successfull__________________________________________
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  //Connection success led OFF______________________________________
  if(WiFi.status() > 0){
    digitalWrite(ledPin, LOW); 
  }
  delay(1000);
  */
  //Broker connect__________________________________________________
  client.setServer(server, 1883);
  client.setCallback(callback);
  delay(1500);
  //Control time initializing_______________________________________
  timeLast = millis();
  //-----------------------------------------------------------------------------------------------------------------------------------
  Serial.begin(115200);
  pinMode(Sensor, INPUT);
  pinMode(Buzzer, OUTPUT);
  servo1.attach(2); // Configurar el servo 1 en el pin 2
  servo2.attach(4); // Configurar el servo 2 en el pin 4
  servo3.attach(5); // Configurar el servo 3 en el pin 5
  dht.begin();
}

void loop() {
//________________________________________________________________
//------------------------- TELEGRAM -----------------------------

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
//________________________________________________________________
  //Validación de Conección ---------------------------------------------------------------------------------------------
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  timeNow = millis();
  if(timeNow - timeLast>wait){
    timeLast = timeNow;
  }
  //---------------------------------------------------------------------------------------------------------------------
  //Código de los Servomotores:__________________________________________________________________________________________
  unsigned long currentMillis = millis(); // Tiempo actual en milisegundos
  // Mover el servo 1 cada 5 minutos_______________________________________________________________________SERVO 1
  if (currentMillis - servo1PreviousMillis >= servo1Interval) {
    servo1.write(45); // Mover el servo 1 a 180 grados
    //servo1PreviousMillis = currentMillis; // Actualizar el tiempo anterior
    delay(5000);
    servo1.write(0);
  }else{
    servo1.write(0);
  }
  // Mover el servo 2 cada 3 minutos_______________________________________________________________________SERVO 2
  if (currentMillis - servo2PreviousMillis >= servo2Interval) {
    servo2.write(45); // Mover el servo 2 a 90 grados
    delay(5000);
    servo1.write(0);
   // servo2PreviousMillis = currentMillis; // Actualizar el tiempo anterior
  }else{
    servo1.write(90);
  }
  // Mover el servo 3 cuando la temperatura supera los 30 grados Celsius__________________________________SERVO 3
  int tempValue = analogRead(tempPin); // Leer el valor del sensor de temperatura
  float tempCelsius = (tempValue * 3.3 / 4095 - 0.5) * 100; // Convertir el valor en grados Celsius
  if (tempCelsius > tempThreshold) {
    servo3.write(45); // Mover el servo 3 a 180 grados
    delay(5000);
    servo1.write(0);
  }else{
    servo1.write(0);
  }
  //---------------------------------------------------------------------------------------------------------------------
  //Sensor Flame_________________________________________________________________________________________________________
  Serial.println(digitalRead(Sensor));

  if (digitalRead(Sensor)) {
    analogWrite(Buzzer, 200);
  } else {
    analogWrite(Buzzer,0);
  }
  //---------------------------------------------------------------------------------------------------------------------
  //Relay _______________________________________________________________________________________________________________
  float temperature= dht.readTemperature();
  if (temperature < 19 ){
  digitalWrite(relayPin, HIGH);
  Serial.println("Temperatura Baja");
  } else {
    digitalWrite(relayPin, LOW);
    Serial.println("Temperatura Normal");
  }
  delay(30000); // Esperamos 2 segundos para tomar una nueva lectura
  Serial.print("Temperatura: ");
  Serial.println(temperature);
  //ALarma de Temperatura ______________________________________________________________________________________________
  if (temperature > 30) { // Si la temperatura es mayor a 30 grados
    tone(BUZZER_PIN, BUZZER_FREQ); // Reproducimos un sonido de alarma
  }
  else if (temperature < 20) { // Si la temperatura es menor a 20 grados
    tone(BUZZER_PIN, BUZZER_FREQ / 2); // Reproducimos otro sonido diferente
  }
  else {
    noTone(BUZZER_PIN); // Si la temperatura está en el rango normal, apagamos el buzzer
  }
//_____________________________________________________________________________________________________________________________________
//--------------------------------------------------------------------------------------------------------ENVIO DE JSON A BASE DE DATOS

// ================================================================================================  Declaración de Variables para JSON
    float temperature1 = dht.readTemperature();
    float humidity=dht.readHumidity();
    float aTemp=analogRead(5);
    float buzzer1=1;
    float buzzer2=0;
    float servo1state=45;
    float servo2state=90;
    float relaystate;
// ================================================================================================  Creación de los objetos para JSON
      
  String json = "{\"sensor_id\":\"1\", \"value\":\""+String(temperature1)+"\"}";
  String json2= "{\"sensor_id\":\"4\", \"value\":\""+String(humidity)+"\"}";
  String json3= "{\"sensor_id\":\"2\", \"value\":\""+String(aTemp)+"\"}";
  String json4= "{\"sensor_id\":\"3\", \"value\":\""+String(buzzer1)+"\"}";
  String json5= "{\"sensor_id\":\"5\", \"value\":\""+String(buzzer2)+"\"}";
  String json6= "{\"sensor_id\":\"6\", \"value\":\""+String(servo1state)+"\"}";
  String json7= "{\"sensor_id\":\"7\", \"value\":\""+String(relaystate)+"\"}";

// ===========================================================================================  Converción y Envio de los Datos a JSON
//String json in serial
    //Sensor 1
    Serial.println(json);
    int str_len = json.length() + 1;//Calculte length of string
    char char_array[str_len];//Creating array with that length
    json.toCharArray(char_array, str_len);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array); // Function send by MQTT with topic and value
   //Sensor 2
    Serial.println(json2);
    int str_len2 = json2.length() + 1;//Calculte length of string
    char char_array2[str_len2];//Creating array with that length
    json2.toCharArray(char_array2, str_len2);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array2); // Function send by MQTT with topic and value
  //Sensor 3
    Serial.println(json3);
    int str_len3 = json3.length() + 1;//Calculte length of string
    char char_array3[str_len3];//Creating array with that length
    json3.toCharArray(char_array3, str_len3);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array3); // Function send by MQTT with topic and value
  //Sensor 4
    Serial.println(json4);
    int str_len4 = json4.length() + 1;//Calculte length of string
    char char_array4[str_len4];//Creating array with that length
    json4.toCharArray(char_array4, str_len4);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array4); // Function send by MQTT with topic and value
  //Sensor 5
    Serial.println(json5);
    int str_len5 = json5.length() + 1;//Calculte length of string
    char char_array5[str_len5];//Creating array with that length
    json5.toCharArray(char_array5, str_len5);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array5); // Function send by MQTT with topic and value
  //Sensor 6
    Serial.println(json6);
    int str_len6 = json6.length() + 1;//Calculte length of string
    char char_array6[str_len6];//Creating array with that length
    json6.toCharArray(char_array6, str_len6);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array6); // Function send by MQTT with topic and value
  //Sensor 7  
    Serial.println(json7);
    int str_len7 = json7.length() + 1;//Calculte length of string
    char char_array7[str_len7];//Creating array with that length
    json7.toCharArray(char_array7, str_len7);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array7); // Function send by MQTT with topic and value

//------------------------------------------------------------------------------------- FIN DEL CÓDIGO DE ENVIO DE JSON A BASE DE DATOS
//_____________________________________________________________________________________________________________________________________

}

void callback(char* topic, byte* message, 
                unsigned int length){
  Serial.print("Recieved message in topic");
  Serial.print(topic);

  String messageTemp;
  for(int i = 0; i<length; i++){
    Serial.print((char)message[i]);
    messageTemp+= (char)message[i];
  }
  Serial.println(messageTemp);

  if(String(topic) == "aiot/u3/jart"){
    if(messageTemp=="true"){
      Serial.println("LED ON");
      digitalWrite(ledPin2, HIGH);
    }else if(messageTemp=="false"){
      Serial.println("LED OFF");
      digitalWrite(ledPin2, LOW);
    }
  }
  
}
// Reconecting mqtt
void reconnect() {
  // Connection loop
  while (!client.connected()) {
    Serial.print("Tying connect...");
    // Intentar reconexión
    if (client.connect("client")) { 
      Serial.println("Connected");
      client.subscribe("aiot/u3/jart"); 
    }else {
      Serial.print("Failed connect, Error rc=");
      Serial.print(client.state());
      Serial.println(" Trying in 5 seconds");
      delay(5000);
      Serial.println (client.connected()); 
    }
  }
}
