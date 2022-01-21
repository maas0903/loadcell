#include <WiFi.h>
#include <WebServer.h>

#include <stdio.h>
#include <ArduinoJson.h>
#include <Arduino.h>

#include <credentials.h>
#include <HX711.h>

const int LOADCELL_DOUT_PIN = 23;
const int LOADCELL_SCK_PIN = 22;

#define DEC_POINT 2
#define STABLE 1

HX711 scale;

//#define DEBUG
IPAddress staticIP(192, 168, 63, 57);
String hostName = "skaal";
#define URI "/skaal"
IPAddress gateway(192, 168, 63, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 63, 21);
IPAddress dnsGoogle(8, 8, 8, 8);

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50

#define LED_0 0

WebServer http_rest_server(HTTP_REST_PORT);

void BlinkNTimes(int pin, int blinks, unsigned long millies)
{
    digitalWrite(pin, LOW);
    for (int i = 0; i < blinks; i++)
    {
        digitalWrite(pin, HIGH);
        delay(millies);
        digitalWrite(pin, LOW);
        delay(millies);
    }
}

void get_skaal()
{
}

void config_rest_server_routing()
{
    http_rest_server.on("/", HTTP_GET, []()
                        { http_rest_server.send(200, "text/html",
                                                "Welcome to the ESP8266 REST Web Server: " + hostName); });
    http_rest_server.on(URI, HTTP_GET, get_skaal);
}

void init_wifi()
{
    int retries = 0;

    Serial.println("Connecting to WiFi");

    WiFi.config(staticIP, gateway, subnet, dns, dnsGoogle);
    WiFi.mode(WIFI_STA);
    WiFi.hostname(hostName);
    WiFi.begin(ssid, password);

    while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY))
    {
        retries++;
        delay(WIFI_RETRY_DELAY);
        Serial.print("#");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("Connected to ");
        Serial.print(ssid);
        Serial.print("--- IP: ");
        Serial.println(WiFi.localIP());
        BlinkNTimes(LED_0, 3, 500);
    }
    else
    {
        Serial.print("Error connecting to: ");
        Serial.println(ssid);
    }
}

void setupSkaal()
{
    Serial.println("Initializing the scale");

    // Initialize library with data output pin, clock input pin and gain factor.
    // Channel selection is made by passing the appropriate gain:
    // - With a gain factor of 64 or 128, channel A is selected
    // - With a gain factor of 32, channel B is selected
    // By omitting the gain factor parameter, the library
    // default "128" (Channel A) is used here.
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    Serial.println("Before setting up the scale:");
    Serial.print("read: \t\t");
    Serial.println(scale.read()); // print a raw reading from the ADC

    Serial.print("read average: \t\t");
    Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

    Serial.print("get value: \t\t");
    Serial.println(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)

    Serial.print("get units: \t\t");
    Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight (not set) divided
                                           // by the SCALE parameter (not set yet)

    scale.set_scale(-1505.f); // this value is obtained by calibrating the scale with known weights; see the README for details
    scale.tare();             // reset the scale to 0

    Serial.println("After setting up the scale:");

    Serial.print("read: \t\t");
    Serial.println(scale.read()); // print a raw reading from the ADC

    Serial.print("read average: \t\t");
    Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

    Serial.print("get value: \t\t");
    Serial.println(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight, set with tare()

    Serial.print("get units: \t\t");
    Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight, divided
                                           // by the SCALE parameter set with set_scale

    Serial.println("Readings:");
}

void resetInit()
{
    init_wifi();
    delay(200);

    config_rest_server_routing();
    delay(200);
    http_rest_server.begin();

    Serial.println("HTTP REST Server Started");

    setupSkaal();
}

void setup(void)
{
    Serial.begin(115200);
    resetInit();
}

void loop(void)
{
    http_rest_server.handleClient();

    Serial.print("one reading:\t");
    Serial.print(scale.get_units(), 1);
    Serial.print("\t| average:\t");
    Serial.println(scale.get_units(10), 1);

    //scale.power_down(); // put the ADC in sleep mode
    delay(2000);
    //scale.power_up();
}
