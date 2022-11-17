#ifndef _INCLUDED_SWERVE_CAN_
#define _INCLUDED_SWERVE_CAN_

#include "mbed.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#define STEER_RESOLUTION 1080

class SwerveCAN
{
public:
    enum struct MOTOR_CODE {
        motor_A,
        motor_B,
        motor_C,
        motor_D,
    };

    SwerveCAN(CAN &can);
    int write(MOTOR_CODE code, float angle, float velocity);
    void setID(unsigned int id_a, unsigned int id_b, unsigned int id_c, unsigned int id_d);
    void getCharArray(char* array);
    void encode(char* send_array, float angle, float velocity);
    
private:
    unsigned int conversion_angle(float angle);
    
    CAN *can;
    unsigned int id[4] = {1337,1338,1339,1340};
    char send_data[4];
};

#endif