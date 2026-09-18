#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int board_early_init_hook(void) {
 /* Some function to run before boot */
    printk("Board Initialized!\n");
    return 0;
}

SYS_INIT(board_early_init_hook, APPLICATION, 0);