#include <stdint.h>

extern int osek_sim_run(uint32_t duration_ms);

int main(void)
{
    return osek_sim_run(2000);
}
