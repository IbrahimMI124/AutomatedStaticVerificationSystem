extern float servo;
extern float angle;
extern bool e_stop;
extern float motor_power;
void viol_joint_limits(void);
void viol_servo_delta(void);
void viol_estop_motor_power(void);
void step(void);
