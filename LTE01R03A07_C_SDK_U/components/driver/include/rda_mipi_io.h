#ifndef __RDA_MIPI_IO__H__
#define __RDA_MIPI_IO__H__
/*
 * Generic I/O
 */

#define readb(addr) \
    ({ unsigned char __v = (*(volatile unsigned char *)(addr)); __v; })
#define readw(addr) \
    ({ unsigned short __v = (*(volatile unsigned short *)(addr)); __v; })
#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *)(addr)); __v; })
#define writeb(b, addr) (void)((*(volatile unsigned char *)(addr)) = (b))
#define writew(b, addr) (void)((*(volatile unsigned short *)(addr)) = (b))
#define writel(b, addr) (void)((*(volatile unsigned int *)(addr)) = (b))
#endif /*__RDA_MIPI_IO__H__ */
