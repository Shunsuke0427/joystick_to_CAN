//f303k8想定
//MCP2561 TXD(1) - (PA_12, D2) Nucleo F303k8
//MCP2561 RXD(4) - (PA_11,D10) Nucleo F303k8
#include "mbed.h"
#include "SwerveCAN.h"

#define OMEGA_MAX 30 //30rad/s

Ticker ticker;
DigitalOut led1(LED1);
/* //F303
 AnalogIn x_axis(A0);
 AnalogIn y_axis(A1);
 CAN can1(PA_11, PA_12);
*/
 //F303
 AnalogIn x_axis(A1);
 AnalogIn y_axis(A2);
 CAN can1(D15, D14);

SwerveCAN can_master(can1);
int counter = 0;
float send_data = 0;
union {float f; int i;} dataUnion;

void getFromJoystick(float *angle, float *volume);
void send(float data);
void tickerEvent();
void getAngleVelocity(float *angle, float *omega);
float constrain_f(float value, float value_min, float value_max);


int main()
{
    printf("main()\n");
    //ticker.attach(&tickerEvent, 1s);
    float swerve_angle = 0; //rad -PI ~ PI
    float motor_omega = 0; //rad/s 0 ~ OMEGA_MAX
    float volume = 0; //0~1

    char data[4] = {11,22,33,44};

    while (1) {
        getAngleVelocity(&swerve_angle, &motor_omega);
        printf("angle: %f  omega: %f\n", swerve_angle, motor_omega);
        
        can_master.encode(data, swerve_angle, motor_omega);
        //printf("%3d, %3d, %3d, %3d  ", data[0], data[1], data[2], data[3]);
        if(can1.write(CANMessage(1337, (char*)data, 4))) {
            //printf("send success\n");
        } else {
            printf("send fail\n");
        }
        //tickerEvent();
        ThisThread::sleep_for(200ms);
    }
}

void getAngleVelocity(float *angle, float *omega) {
    float angle_joy = 0;
    float volume_joy = 0; //0~1
    float omega_calc = 0;
    getFromJoystick(&angle_joy, &volume_joy);
    omega_calc = OMEGA_MAX * volume_joy;
    omega_calc = constrain_f(omega_calc, 0, OMEGA_MAX);

    *angle = angle_joy;
    *omega = omega_calc;
}

void getFromJoystick(float *angle, float *volume) {
    float x = x_axis.read() - 0.5;
    float y = y_axis.read() - 0.5;
    float angle_joy;
    float volume_joy = 2 * sqrt(pow(x, 2.0) + pow(y, 2.0)); //0~1
    if(volume_joy < 0.05) volume_joy = 0;
    volume_joy = constrain_f(volume_joy, 0, 1);
    angle_joy = (volume_joy != 0) ? atan2(x,y)+M_PI : 0;

    *angle = angle_joy;
    *volume = volume_joy;
}

void send_rawVolume(float data)
{
    char send_array[4];
    dataUnion.f = data;
    //printf("union float: %f\n", dataUnion.f);
    int data_formatter = dataUnion.i;
    //printf("union int: %d\n", data_formatter);
    for(int j=0; j<4; j++) {
        send_array[j] = (data_formatter >> (8*j)) & 0xFF;
        //printf("send data[%d]: %d\n", j, send_array[j]);
    }
    
    if (can1.write(CANMessage(1337, send_array, 4))) {
        counter++;
        if(counter > 10) {
            printf("Message sent: %f\n", data);
            counter = 0;
        }
        
    }
    led1 = !led1;
}

void tickerEvent() {
    //send(volume.read());
    //printf("\n");
    //send_data += 0.125;
    if(send_data > 100) send_data = 0;
}

float constrain_f(float value, float value_min, float value_max) {
    float value_return = 0;
    if(value > value_max) {
        value_return = value_max;
    } else if (value < value_min) {
        value_return = value_min;
    } else {
        value_return = value;
    }
    return value_return;
}