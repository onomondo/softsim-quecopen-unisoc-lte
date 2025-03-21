#include "platform_os.h"

void csi_irq_restore(unsigned int flag)
{
    return;
}

unsigned int csi_irq_save()
{
    return 0;
}

unsigned int platform_irq_save(void)
{
    unsigned int flag = csi_irq_save();
    return flag;
}

void platform_irq_restore(unsigned int flag)
{
    csi_irq_restore(flag);
}
