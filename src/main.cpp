#include <HX711.h> //You must have this library in your arduino library folder

#define DOUT 23
#define CLK 22

HX711 scale;

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = -109525; //-106600 worked for my 40Kg max scale setup

//=============================================================================================
//                         SETUP
//=============================================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println("HX711 Calibration");
    Serial.println("Remove all weight from scale");
    Serial.println("After readings begin, place known weight on scale");
    Serial.println("Press q,s,d,f,g to increase calibration factor by 1,10,100,1000,10000 respectively");
    Serial.println("Press w,x,c,v,b to decrease calibration factor by 1,10,100,1000,10000 respectively");
    Serial.println("Press t for tare");
    scale.begin(DOUT, CLK);
    scale.set_scale();
    scale.tare(); //Reset the scale to 0

    long zero_factor = scale.read_average(); //Get a baseline reading
    Serial.print("Zero factor: ");           //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);
    calibration_factor = -1500.00;
}

//=============================================================================================
//                         LOOP
//=============================================================================================

void loop()
{

    scale.set_scale(calibration_factor); //Adjust to this calibration factor

    Serial.print("Reading: ");
    Serial.print(scale.get_units(), 3);
    Serial.print(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.println();

    if (Serial.available())
    {
        char temp = Serial.read();
        delay(2);
        Serial.print(temp);
        if (temp == '+' || temp == 'q')
            calibration_factor += 1;
        else if (temp == '-' || temp == 'w')
            calibration_factor -= 1;
        else if (temp == 's')
            calibration_factor += 10;
        else if (temp == 'x')
            calibration_factor -= 10;
        else if (temp == 'd')
            calibration_factor += 100;
        else if (temp == 'c')
            calibration_factor -= 100;
        else if (temp == 'f')
            calibration_factor += 1000;
        else if (temp == 'v')
            calibration_factor -= 1000;
        else if (temp == 'g')
            calibration_factor += 10000;
        else if (temp == 'b')
            calibration_factor -= 10000;
        else if (temp == 't')
            scale.tare(); //Reset the scale to zero
    }
}