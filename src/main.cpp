#include <HX711.h> //You must have this library in your arduino library folder

#define CLK 22
#define DOUT 23

HX711 scale;

float Calibration_Factor_Of_Load_cell = -8.1;
float U;
float O;

void setup()
{
    Serial.begin(115200);
    Serial.println("HX711 calibration sketch");
    Serial.println("Remove all weight from scale");
    Serial.println("After readings begin, place known weight on scale");
    Serial.println("Press + or a to increase calibration factor");
    Serial.println("Press - or z to decrease calibration factor");

    scale.begin(DOUT, CLK);
    scale.set_scale();
    scale.tare();                            // Reset scale to 0
    long zero_factor = scale.read_average(); // Get basic reading
    Serial.print("Zero factor: ");           // This can be used to eliminate the need to tare the scale. Useful in constant scale projects.
    Serial.println(zero_factor);
}

void loop()
{

    scale.set_scale(Calibration_Factor_Of_Load_cell); // Adjust this calibration factor
    Serial.print("Reading: ");
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
}