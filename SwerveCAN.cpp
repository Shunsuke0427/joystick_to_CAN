#include "SwerveCAN.h"

SwerveCAN::SwerveCAN(CAN &can) {

}

void SwerveCAN::setID(unsigned int id_a, unsigned int id_b, unsigned int id_c, unsigned int id_d) {
    id[static_cast<int>(MOTOR_CODE::motor_A)] = id_a;
    id[static_cast<int>(MOTOR_CODE::motor_B)] = id_b;
    id[static_cast<int>(MOTOR_CODE::motor_C)] = id_c;
    id[static_cast<int>(MOTOR_CODE::motor_D)] = id_d;
}

unsigned int SwerveCAN::conversion_angle(float angle) {
    //float(0~2PI) -> unsigned int (0~1080)
    unsigned int angle_uint = 0;
    if(angle > 0) angle_uint = (unsigned int) (angle * STEER_RESOLUTION / (2*M_PI));
    return angle_uint;
}
void SwerveCAN::encode(char* send_array, float angle, float velocity) {
    unsigned int angle_uint = conversion_angle(angle);
    unsigned int velocity_uint = 0;
    if(velocity > 0) velocity_uint = (unsigned int) (velocity * 100);
    //float (0~50.00) -> unsigned int (0~5000)
    send_array[0] = 0;
    send_array[1] = 0xFF & (angle_uint >> 3);
    send_array[2] = (0xE0 & (angle_uint << 5)) | (0x1F & (velocity_uint >> 8));
    send_array[3] = 0xFF & velocity_uint;
}

void SwerveCAN::getCharArray(char* array) {
    for(int i=0; i<4; i++) {
        array[i] = send_data[i];
    }
}