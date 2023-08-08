#include "mbed.h"
#include "LSM6DSLSensor.h"


#define PI 3.141592654

DigitalOut led1(LED1);  //level
DigitalOut led2(LED2);  //not level

static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5);// high address

float computeAngle(int x, int y, int z){

  float res = (atan((x)/(sqrt((y*y)+(z*z)))) * (180 / PI)); //equation 6 as given and converted to degrees.

  return res;
}

float computeroll(int x, int y, int z){

  float roll = (atan((y)/(sqrt((x*x)+(z*z)))) * (180 / PI));//equation 7 as given and converted to degrees.

  return roll;

}
/* Simple main function */
int main() {
    uint8_t id;
    int32_t axes[3];
    float res = 0;
    float roll = 0;

    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);
    printf ("The anlgles for pitch and roll are in degrees\r\n",res );
    printf("TKELLY_Lab_3\r\n");

    while (1) {// while loop

        acc_gyro.get_x_axes(axes); //get data from gyro


        res = computeAngle(axes[0], axes[1], axes[2]); //calling pitch angle function
        roll = computeroll(axes[0], axes[1], axes[2]); //calling roll angle function

        // Print the x,y,z values and the pitch angle and roll angle.

        printf("LSM6DSL: %6d, %6d, %6d, Pitch is %3.2f, Roll is %3.2f\r\n", axes[0], axes[1], axes[2], res, roll);
            if(axes[2] < 0){
                printf("your upside down \r\n"); //prints if upside down
            }
            if( res * res < 2 && roll * roll < 2 ) { //to make a positive number for level gauge
                printf("this is nice and level! \r\n");  //prints even if upside down
                led1 = 1; //toggles leds for level
                led2 = 0;
            }
            else  {
                printf("THIS IS NOT LEVEL  !!!!!!! \r\n");  //otherwise prints this
                led2 = 1;  //toggles leds for not level
                led1 = 0;

            }


        thread_sleep_for(2000);  //sleep 2 seconds and return to begining of loop
}