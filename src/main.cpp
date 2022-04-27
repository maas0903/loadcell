#include <HX711.h>
#include <Pushbutton.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#define CLK 22
#define DOUT 23
#define BUTTON_PIN 19
Pushbutton button(BUTTON_PIN);

HX711 scale;
int reading;
int lastReading;

float Calibration_Factor_Of_Load_cell = 8.1;

void setup()
{
    Serial.begin(115200);
    scale.begin(DOUT, CLK);
    scale.set_scale(Calibration_Factor_Of_Load_cell);
    scale.tare();
    Serial.println("starting");
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
        /*if (reading < 0)
        {
            reading = 0.00;
        }*/
        reading = reading * 0.035274;

        if ((reading > lastReading + 1) or (reading < lastReading - 1))
        {
            Serial.print("Weight: ");
            Serial.println(reading);
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