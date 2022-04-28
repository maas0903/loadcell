#include <HX711.h>
#include <Pushbutton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#define CLK 22
#define CLK 5
#define DOUT 23

#define BUTTON_PIN 19
Pushbutton button(BUTTON_PIN);

HX711 scale;
int reading;
int lastReading;

float Calibration_Factor_Of_Load_cell = 8.1;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
    Serial.begin(115200);
    scale.begin(DOUT, CLK);
    scale.set_scale(Calibration_Factor_Of_Load_cell);
    scale.tare();
    Serial.println("starting");

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    delay(2000);
    display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 17);
    display.println("Starting");
    display.display();
    delay(3000);
    display.clearDisplay();
    display.setCursor(10, 17);
    display.println("Ready");
    display.display();
}

void loop()
{
    if (button.getSingleDebouncedPress())
    {
        Serial.print("tare...");
        scale.tare();
    }

    delay(200);

    if (scale.wait_ready_timeout(200))
    {
        reading = round(scale.get_units());

        reading = reading * 0.035274; // ounce to gram

        if ((reading > lastReading + 1) or (reading < lastReading - 1))
        {
            Serial.print("Weight: ");
            Serial.println(reading);

            display.clearDisplay();
            display.setCursor(0, 17);
            display.print(String(reading) + "g");
            display.display();
            // delay(3000);
        }
        lastReading = reading;
    }
    else
    {
        Serial.println("HX711 not found.");
    }

    /*Serial.print("Reading: ");
    U = scale.get_units();
    if (U < 0)
    {
        U = 0.00;
    }
    O = U * 0.035274;
    Serial.print(O);
    Serial.print(" grams");
    Serial.print(" Calibration_Factor_Of_Load_cell: ");
    Serial.print(Calibration_Factor_Of_Load_cell);
    Serial.println();

    if (Serial.available())
    {
        char temp = Serial.read();
        delay(2);
        if (temp == '+' || temp == 'a')
            Calibration_Factor_Of_Load_cell += 0.5;
        else if (temp == '-' || temp == 'z')
            Calibration_Factor_Of_Load_cell -= 0.5;
    }
    */
}