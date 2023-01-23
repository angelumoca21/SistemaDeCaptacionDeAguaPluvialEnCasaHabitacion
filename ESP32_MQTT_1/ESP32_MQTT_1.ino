/*
   Proyecto Capstone
   por: Angel Morales
   Fecha: 04 de enero de 2023
*/

//Bibliotecas
#include <WiFi.h>  // Biblioteca para el control de WiFi
#include <PubSubClient.h> //Biblioteca para conexion MQTT

#define SLLUVIA 32
#define SNIVEL1 22
#define SNIVEL2 23
#define S1CONTENEDOR1SLLUVIAS 34
#define S2CONTENEDOR1SLLUVIAS 35
#define BOMBA 21
#define VALVULA 19
#define ECHO 18
#define TRIGGER 5 

//Datos de WiFi
const char* ssid = "QWERTYUIOP";  // Aquí debes poner el nombre de tu red
const char* password = "AAAM21211105qwerty@@c";  // Aquí debes poner la contraseña de tu red

//Datos del broker MQTT
const char* mqtt_server = "192.168.0.162"; // Si estas en una red local, coloca la IP asignada, en caso contrario, coloca la IP publica
IPAddress server(192, 168, 0, 162);

// Objetos
WiFiClient espClient; // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient); // Este objeto maneja los datos de conexion al broker

// Variables
int edoSLluvia = 0;
int statusLedPin = 33;
long timeNow, timeLast; // Variables de control de tiempo no bloqueante
int wait = 1000;  // Indica la espera cada 5 segundos para envío de mensajes MQTT
int edoSensorNivel1 = 0;
int edoSensorNivel2 = 0;
int edoS1Contenedor1sLluvias = 0;
int edoS2Contenedor1sLluvias = 0;
int edoBomba = 0;
int edoValvula = 0;
float d = 0; 
float t = 0;
// Inicialización del programa
void setup()
{
  // Iniciar comunicación serial
  Serial.begin (115200);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);
  pinMode (SNIVEL1, INPUT);
  pinMode (SNIVEL2, INPUT);
  pinMode (BOMBA, OUTPUT);
  pinMode (VALVULA, OUTPUT);
  pinMode (statusLedPin, OUTPUT);
  digitalWrite (statusLedPin, HIGH);
  Serial.println();
  Serial.print("Conectar a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Esta es la función que realiz la conexión a WiFi

  while (WiFi.status() != WL_CONNECTED)
  {
    // Este bucle espera a que se realice la conexión
    digitalWrite (statusLedPin, HIGH);
    delay(500); //dado que es de suma importancia esperar a la conexión, debe usarse espera bloqueante
    digitalWrite (statusLedPin, LOW);
    Serial.print(".");  // Indicador de progreso
    delay(5);
  }
  // Cuando se haya logrado la conexión, el programa avanzará, por lo tanto, puede informarse lo siguiente
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  // Si se logro la conexión, encender led
  if (WiFi.status () > 0)
  {
    digitalWrite (statusLedPin, LOW);
  }
  delay (1000); // Esta espera es solo una formalidad antes de iniciar la comunicación con el broker
  // Conexión con el broker MQTT
  client.setServer(server, 1883); // Conectarse a la IP del broker en el puerto indicado
  client.setCallback(callback); // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);  // Esta espera es preventiva, espera a la conexión para no perder información
  timeLast = millis (); // Inicia el control de tiempo
}// fin del void setup ()

// Cuerpo del programa, bucle principal
void loop()
{
  //Verificar siempre que haya conexión al broker
  if (!client.connected())
  {
    reconnect();  // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }// fin del if (!client.connected())
  client.loop(); // Esta función es muy importante, ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker
  timeNow = millis(); // Control de tiempo para esperas no bloqueantes
  if (timeNow - timeLast > wait)
  { // Manda un mensaje por MQTT cada cinco segundos
    timeLast = timeNow; // Actualización de seguimiento de tiempo
    
    edoSensorNivel1 = digitalRead(SNIVEL1);
    edoSensorNivel2 = digitalRead(SNIVEL2);
    if (edoSensorNivel1 == 0)
      client.publish("casaMaqueta/cisterna/s1", "OFF");
    else
      client.publish("casaMaqueta/cisterna/s1", "ON");
    if (edoSensorNivel2 == 0)
      client.publish("casaMaqueta/cisterna/s2", "OFF");
    else
      client.publish("casaMaqueta/cisterna/s2", "ON");
      
    edoS1Contenedor1sLluvias = analogRead(S1CONTENEDOR1SLLUVIAS);
    //Serial.println(edoS1Contenedor1sLluvias);
    if (edoS1Contenedor1sLluvias != 0)
      client.publish("casaMaqueta/contenedor/sensor1", "ON");
    else
      client.publish("casaMaqueta/contenedor/sensor1", "OFF");

    edoS2Contenedor1sLluvias = analogRead(S2CONTENEDOR1SLLUVIAS);
    //Serial.println(edoS2Contenedor1sLluvias);
    if (edoS2Contenedor1sLluvias != 0)
      client.publish("casaMaqueta/contenedor/sensor2", "ON");
    else
      client.publish("casaMaqueta/contenedor/sensor2", "OFF");

    edoSLluvia = analogRead(SLLUVIA);
    Serial.println(edoSLluvia);
    if (edoSLluvia <= 2000)
      client.publish("casaMaqueta/techo/sLluvia", "ON");
    else
      client.publish("casaMaqueta/techo/sLluvia", "OFF");

    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);   
    digitalWrite(TRIGGER, LOW);
    t = pulseIn(ECHO, HIGH);
    d = t/59; 
    //Serial.println(d);
    char dataString[10];// Define una arreglo de caracteres para enviarlos por MQTT, especifica la longitud del mensaje en 8 caracteres
    dtostrf(d, 3, 2, dataString);
    client.publish("casaMaqueta/techo/sUltrasonico", dataString);
  }// fin del if (timeNow - timeLast > wait)
}// fin del void loop ()

// Funciones de usuario

// Esta función permite tomar acciones en caso de que se reciba un mensaje correspondiente a un tema al cual se hará una suscripción
void callback(char* topic, byte* message, unsigned int length)
{
  // Indicar por serial que llegó un mensaje
  Serial.print("Llegó un mensaje en el tema: ");
  Serial.print(topic);
  // Concatenar los mensajes recibidos para conformarlos como una varialbe String
  String messageTemp; // Se declara la variable en la cual se generará el mensaje completo
  for (int i = 0; i < length; i++)
  { // Se imprime y concatena el mensaje
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);
  // En esta parte puedes agregar las funciones que requieras para actuar segun lo necesites al recibir un mensaje MQTT
  // Ejemplo, en caso de recibir el mensaje true - false, se cambiará el estado del led soldado en la placa.
  // El ESP323CAM está suscrito al tema esp/output
  if (String(topic) == "casaMaqueta/cisterna/bomba")
  { // En caso de recibirse mensaje en el tema esp32/output
    if (messageTemp == "ON")
    {
      Serial.println("Bomba encendida");
      digitalWrite(BOMBA,1);
    }// fin del if (String(topic) == "esp32/output")
    else if (messageTemp == "OFF")
    {
      Serial.println("bomba apagada");
      digitalWrite(BOMBA,0);
    }// fin del else if(messageTemp == "false")
  }// fin del if (String(topic) == "esp32/output")

  if (String(topic) == "casaMaqueta/contenedor/valvula")
  { // En caso de recibirse mensaje en el tema esp32/output
    if (messageTemp == "ON")
    {
      Serial.println("Valvula encendida");
      digitalWrite(VALVULA,1);
    }// fin del if(messageTemp == "ON")
    else if (messageTemp == "OFF")
    {
      Serial.println("Valvula apagada");
      digitalWrite(VALVULA,0);
    }// fin del else if(messageTemp == "OFF")
  }// fin del if String(topic) == "casaMaqueta/contenedor/valvula"
}// fin del void callback

// Función para reconectarse
void reconnect()
{
  // Bucle hasta lograr conexión
  while (!client.connected())
  { // Pregunta si hay conexión
    Serial.print("Tratando de contectarse...");
    // Intentar reconexión
    if (client.connect("ESP32CAMClient"))
    { //Pregunta por el resultado del intento de conexión
      Serial.println("Conectado");
      client.subscribe("casaMaqueta/#"); // Esta función realiza la suscripción al tema
    }// fin del  if (client.connect("ESP32CAMClient"))
    else
    { //en caso de que la conexión no se logre
      Serial.print("Conexion fallida, Error rc=");
      Serial.print(client.state()); // Muestra el codigo de error
      Serial.println(" Volviendo a intentar en 5 segundos");
      // Espera de 5 segundos bloqueante
      delay(5000);
      Serial.println (client.connected ()); // Muestra estatus de conexión
    }// fin del else
  }// fin del bucle while (!client.connected())
}// fin de void reconnect(
