extern int32_t hr;
extern bool pump;
extern bool vitals_out_of_range;
extern bool alarm;
extern float rate;
void viol_hr_pump_stop(void);
void viol_infusion_rate_max(void);
void viol_vitals_alarm(void);
void step(void);
