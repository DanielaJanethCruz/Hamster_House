
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP32Servo.h>

//Servos
Servo servoPuerta1;
Servo servoPuerta2;
Servo servoComida;
//Flame con Buzzer
#define BuzzerFlame 25
#define SensorFlame 23
//DHT11 con Buzzer
#define BuzzerAlarma 18
#define SensorAlarma 14
DHT dht(SensorAlarma,DHT11);


//Variables
int ledPin = 2; //Connection state
int ledPin2 = 4; //Show received messages
long timeNow, timeLast; //Not blocking time
int wait = 5000; //Wait 5 seconds to send new message

//Conección a Internet 
const char* ssid = "Danis";
const char* password = "@Daniela";
//Conección a MQTT
const char* mqtt_server = "192.168.137.203";
IPAddress server(192,168,137,203);
const int mqtt_port = 1883;

//Communication objects
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  //Iniciando la Comunicación con el Serial---------------------------------------------------------------------------------------------
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  
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
  //Broker connect__________________________________________________
  client.setServer(server, 1883);
  client.setCallback(callback);
  delay(1500);
  //Control time initializing_______________________________________
  timeLast = millis();
  //-----------------------------------------------------------------------------------------------------------------------------------
  //Servos
  servoPuerta1.attach(2); // Configurar el servo 1 en el pin 2
  servoPuerta2.attach(4); // Configurar el servo 2 en el pin 4
  servoComida.attach(5); // Configurar el servo 3 en el pin 5
  //Flame y Buzzer
  pinMode(SensorFlame, INPUT);
  pinMode(BuzzerFlame,OUTPUT);
  //Buzzer DHT11
  pinMode(SensorAlarma,INPUT);
  pinMode(BuzzerAlarma,OUTPUT);
  dht.begin();
}

void loop() {
  Flame();
  Servos();
  AlarmaDHT();
  JSON();
  //Validación de Conección ---------------------------------------------------------------------------------------------
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  timeNow = millis();
  if(timeNow - timeLast>wait){
    timeLast = timeNow;
  }
}

void JSON(){
    float temperature1 = dht.readTemperature();
    float humidity=19.2;
    float flame=digitalRead(SensorFlame);
    float buzzer1=digitalRead(BuzzerFlame);
    float buzzer2=digitalRead(BuzzerAlarma);
    float servo1state=servoPuerta1.read();
    float servo2state=servoPuerta2.read();
    float servo3state=servoComida.read();

 
  String json = "{\"sensor_id\":\"1\", \"value\":\""+String(temperature1)+"\"}";
  String json3= "{\"sensor_id\":\"2\", \"value\":\""+String(flame)+"\"}";
  String json4= "{\"sensor_id\":\"3\", \"value\":\""+String(buzzer1)+"\"}";
  String json2= "{\"sensor_id\":\"4\", \"value\":\""+String(humidity)+"\"}";
  String json5= "{\"sensor_id\":\"5\", \"value\":\""+String(buzzer2)+"\"}";
  String json6= "{\"sensor_id\":\"6\", \"value\":\""+String(servo1state)+"\"}";
  String json7= "{\"sensor_id\":\"8\", \"value\":\""+String(servo2state)+"\"}";
  String json8= "{\"sensor_id\":\"9\", \"value\":\""+String(servo3state)+"\"}";

    //Sensor 1--Temperatura
    Serial.println(json);
    int str_len = json.length() + 1;//Calculte length of string
    char char_array[str_len];//Creating array with that length
    json.toCharArray(char_array, str_len);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array); // Function send by MQTT with topic and value
   //Sensor 2---Humedad
    Serial.println(json2);
    int str_len2 = json2.length() + 1;//Calculte length of string
    char char_array2[str_len2];//Creating array with that length
    json2.toCharArray(char_array2, str_len2);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array2); // Function send by MQTT with topic and value
  //Sensor 3--Sensor Flame
    Serial.println(json3);
    int str_len3 = json3.length() + 1;//Calculte length of string
    char char_array3[str_len3];//Creating array with that length
    json3.toCharArray(char_array3, str_len3);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array3); // Function send by MQTT with topic and value
  //Sensor 4--Buzzer 1
    Serial.println(json4);
    int str_len4 = json4.length() + 1;//Calculte length of string
    char char_array4[str_len4];//Creating array with that length
    json4.toCharArray(char_array4, str_len4);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array4); // Function send by MQTT with topic and value
  //Sensor 5--Buzzer 2
    Serial.println(json5);
    int str_len5 = json5.length() + 1;//Calculte length of string
    char char_array5[str_len5];//Creating array with that length
    json5.toCharArray(char_array5, str_len5);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array5); // Function send by MQTT with topic and value
  //Sensor 6--Servomotor
    Serial.println(json6);
    int str_len6 = json6.length() + 1;//Calculte length of string
    char char_array6[str_len6];//Creating array with that length
    json6.toCharArray(char_array6, str_len6);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array6); // Function send by MQTT with topic and value
  //Sensor 7--Servomotor 2
    Serial.println(json7);
    int str_len7 = json7.length() + 1;//Calculte length of string
    char char_array7[str_len7];//Creating array with that length
    json7.toCharArray(char_array7, str_len7);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array7); // Function send by MQTT with topic and value
  //Sensor 7--Servomotor 3
    Serial.println(json8);
    int str_len8 = json8.length() + 1;//Calculte length of string
    char char_array8[str_len8];//Creating array with that length
    json8.toCharArray(char_array8, str_len8);//Convert string to char array    
    client.publish("aiot/u3/jart", char_array8); // Function send by MQTT with topic and value
}

void AlarmaDHT(){
  float temp=dht.readTemperature();

  if(temp < 20){
    digitalWrite(BuzzerAlarma, HIGH);
    delay(500); // Enciende el zumbador durante medio segundo
    digitalWrite(BuzzerAlarma, LOW);
    delay(500); // Apaga el zumbador durante medio segundo
    digitalWrite(BuzzerAlarma, HIGH);
    delay(500); // Enciende el zumbador durante medio segundo
    digitalWrite(BuzzerAlarma, LOW);
    delay(500); // Apaga el zumbador durante medio segundo
    digitalWrite(BuzzerAlarma, HIGH);
    delay(500); // Enciende el zumbador durante medio segundo
    digitalWrite(BuzzerAlarma, LOW);
    delay(500); // Apaga el zumbador durante medio segundo
  }
  Serial.println(temp);
}
void Flame(){
//_______________________________________________________________________ Flame
//EL Buzzer se activa cuando el flame detecta flamas
  int flame = digitalRead(SensorFlame);
  
  if (digitalRead(SensorFlame)) {
    digitalWrite(BuzzerFlame, HIGH);
    delay(500); // Enciende el zumbador durante medio segundo
    digitalWrite(BuzzerFlame, LOW);
    delay(500); // Apaga el zumbador durante medio segundo
    digitalWrite(BuzzerFlame, HIGH);
    delay(500); // Enciende el zumbador durante medio segundo
    digitalWrite(BuzzerFlame, LOW);
    delay(500); // Apaga el zumbador durante medio segundo
    digitalWrite(BuzzerFlame, HIGH);
    delay(500); // Enciende el zumbador durante medio segundo
    digitalWrite(BuzzerFlame, LOW);
    delay(500); // Apaga el zumbador durante medio segundo
  }
}
void Servos(){
//El servo se abrira cada 25 segundos
servoPuerta1.write(180);
delay(500);
servoPuerta1.write(0);
//El servo se abrira cada 25 segundos
servoPuerta2.write(180);
delay(500);
servoPuerta2.write(0);
//El servo de la comida brindara entrada a la comida cada tres minutos
servoComida.write(45);
delay(500);
servoComida.write(0);
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
