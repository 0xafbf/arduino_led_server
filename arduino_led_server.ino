#include <ESP8266WiFi.h>

const char* ssid = "BoteroAguilar"; 
const char* pass = "ejecutivo2020";

//Para 1 LED
#define RED D0 
#define GREEN D1
#define BLUE D2


WiFiServer server(80); //Crea objeto server con puerto 80

void setup()
{
    pinMode(RED,OUTPUT);
    pinMode(GREEN,OUTPUT);
    pinMode(BLUE,OUTPUT);

	digitalWrite(RED,LOW);
    digitalWrite(GREEN,LOW);
    digitalWrite(BLUE,LOW);
    Serial.begin(9600);
    delay(10);

    //Conexion a la red WiFi
    Serial.println();
    Serial.print("Conectando a la red");
    Serial.println(ssid);

    WiFi.begin(ssid,pass); //Inicia la conexion 
    while(WiFi.status() != WL_CONNECTED) //sigue intentando conectarse
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWifi conectado");

    //Inicia el servidor
    server.begin();
    Serial.println("Inicia el servidor");
    Serial.println("Para  conectarse, utilizar la siguiente URL: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

int value_r = LOW;
int value_g = LOW;
int value_b = LOW;

void loop()
{
    WiFiClient client = server.available(); //Verifica si hay algun cleinte conectado
    if(!client) // Mientras no haya cleinte no contuinua
    {
        return;
    }

    //Cuando encuentra cliente conectado
    Serial.println("Nuevo cliente");
    while (!client.available()) {//Espera hasta recibir dato del cliente
        delay(1);
    }
    //Cuando recibe un dato, lee la primera liena del request del cliente
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    //Asocia el request a la accion correspondiente
    
    if(request.indexOf("/LEDR=ON")!=-1) value_r = HIGH;
    if(request.indexOf("/LEDR=OFF")!=-1) value_r = LOW;
    if(request.indexOf("/LEDG=ON")!=-1) value_g = HIGH;
    if(request.indexOf("/LEDG=OFF")!=-1) value_g = LOW;
	if(request.indexOf("/LEDB=ON")!=-1) value_b = HIGH;
    if(request.indexOf("/LEDB=OFF")!=-1) value_b = LOW;

	digitalWrite(RED, value_r);
	digitalWrite(GREEN, value_g);
	digitalWrite(BLUE, value_b);

    //Retorna una respuesta
    client.println("http/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");

    client.print("LED_RED: ");
	client.print(value_r == HIGH ? "ON" : "OFF");
	client.print("<br>");
    client.print("LED_GREEN: ");
	client.print(value_g == HIGH ? "ON" : "OFF");
	client.print("<br>");
    client.print("LED_BLUE: ");
	client.print(value_b == HIGH ? "ON" : "OFF");
	client.print("<br>");
    

    client.println("<br><br>");
    client.println("<a href=\"/LEDR=ON\"><button>Encender RED</button></a>");
    client.println("<a href=\"/LEDR=OFF\"><button>Apagar RED</button></a><br/>");
    
    client.println("<a href=\"/LEDG=ON\"><button>Encender GREEN</button></a>");
    client.println("<a href=\"/LEDG=OFF\"><button>Apagar GREEN</button></a><br/>");
    
    client.println("<a href=\"/LEDB=ON\"><button>Encender BLUE</button></a>");
    client.println("<a href=\"/LEDB=OFF\"><button>Apagar BLUE</button></a><br/>");

    client.println("</html>");
    delay(1);
    Serial.println("Cliente desconectado");
    
}



