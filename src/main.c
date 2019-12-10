/**
 * @file    main.c
 * @brief   Entry point of the embedded kernel.
 * @details Main is technically kernel code, but the user will place here
 *          its process creation requests so they get registered with the kernel.
 * @author  Manuel Burnay
 * @date    2019.10.22  (Created)
 * @date    2019.11.21  (Last Modified)
 */

#include <string.h>
#include "k_handlers.h"
#include "frame_handler.h"
#include "packet_handler.h"
#include "track_manager.h"
#include "trainset_VM.h"
#include "calls.h"

/**
 * @brief   main.c
 * @details Initializes the kernel and all processes to run in the system.
 *          It also start's kernel's "run-mode".
 *
 */
int main(void)
{
    kernel_init();

    UART1_init();

    /* Place Process Create requests here */
    process_attr_t attr = {.id = 0, .priority = HIGH_PRIORITY, .name = "Packet Server", .arg = NULL};
    pcreate(&attr, &packet_server);

    attr.priority = USER_PRIORITY;
    strcpy(attr.name, "Track Server");
    pcreate(&attr, &trainset_controller);

    attr.priority = HIGH_PRIORITY;
    strcpy(attr.name, "IO proccess");
    pcreate(&attr, &userIO);
    /*                                    */

    kernel_start();

    return 0;
}
