

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "trainset_VM.h"
#include "trainset_defs.h"
#include "calls.h"
#include "route_table.h"
#include "k_terminal.h"

void printSpot(int,int);
#define USER_BOX 12
#define ESC 27

void drawTrack(void);

void userIO(void) {

    bind(USER_BOX);

    uint8_t spot;
    train_t startTrain;

    int valid = 0;

    char buff[MSG_MAX_SIZE];
    drawTrack();


    while(1){



        //Get input from the user
        while (1){
        printSpot(50,0);
        send_user(USER_BOX, "What point is the train closest to? eg 1 \n\r");

        recv_user(USER_BOX, buff, MSG_MAX_SIZE);

        valid = sscanf(buff, "%d", &spot);

        if (valid !=1 || 1 > spot > 24){
            send_user(USER_BOX, "give me correct format \n \r");
        }
        else{
            startTrain.current = spot;
            spot = 0;
            valid = 0;
            break;
        }
        }

        while (1){
            printSpot(50,0);
            send_user(USER_BOX, "What is the train's destination sensor? \n\r");
            recv_user(USER_BOX, buff, MSG_MAX_SIZE);

            valid = sscanf(buff, "%d", &spot);

            if (valid != 2 || 1 > spot > 24){
                send_user(USER_BOX, "give me correct format");
            }
            else {
                startTrain.dst = spot;
                spot = 0;
                valid = 0;
                break;
            }


        }

        send(TRACK_BOX, USER_BOX, &startTrain, sizeof(train_t));
    }
}


void drawTrack(void)
{

    //Starting at top left
    printSpot(6,31);
    send_user(USER_BOX,  "--o------------o--");
    printSpot(7,30);
    send_user(USER_BOX, "/                  \\");
    printSpot(8,27);
    send_user(USER_BOX, "o-----o---o----o---o-----o");

    printSpot(9,26);
    send_user(USER_BOX, "/            /             \\");

    printSpot(10,25);
    send_user(USER_BOX, "o     -o---o--               o");

    printSpot(11,25);
    send_user(USER_BOX, "|                            |");

    printSpot(12,25);
    send_user(USER_BOX, "|                            |");

    printSpot(13,25);
    send_user(USER_BOX, "o               --o---o-     o");
    printSpot(14,26);
    send_user(USER_BOX, "\\             /            /");
    printSpot(15,27);
    send_user(USER_BOX,  "o-----o---o----o---o-----o");

    printSpot(16,30);
    send_user(USER_BOX,    "\\                  /");
    printSpot(17,31);
    send_user(USER_BOX,      "--o------------o--");

}

void printSpot(int row, int col){

/* Output a single character to specified screen position */
/* CUP (Cursor position) command plus character to display */
/* Assumes row and col are valid (1 through 24 and 1 through 80, respectively) */
    struct CUPch uart_data;
    /* Since globals aren�t permitted, this must be done each call */
    uart_data . esc = ESC;
    uart_data . sqrbrkt = '[';
    uart_data . line[0] = '0' + row / 10;
    uart_data . line[1] = '0' + row % 10;
    uart_data . semicolon = ';';
    uart_data . col[0] = '0' + col / 10;
    uart_data . col[1] = '0' + col % 10;
    uart_data . cmdchar = 'H';
    uart_data . nul = '\0';

    char *str = (char*) &uart_data;
    send_user(USER_BOX, str);
}
