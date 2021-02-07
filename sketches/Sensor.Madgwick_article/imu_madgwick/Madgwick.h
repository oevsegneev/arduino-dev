extern volatile float beta;
extern volatile float q0, q1, q2, q3;

void MadgwickAHRSupdate(float tdelta, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MadgwickAHRSupdateIMU(float tdelta, float gx, float gy, float gz, float ax, float ay, float az);
float invSqrt(float x);
