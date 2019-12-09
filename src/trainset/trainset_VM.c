#include <stdio.h>
#include "trainset_VM.h"
#include "trainset_defs.h"
#include "calls.h"
#include "route_table.h"
#include <stdbool.h>
#include <string.h>
#include "k_terminal.h"
void printSpot(int,int);
#define USER_BOX 12
#define ESC 27
void userIO(void) {


    uint8_t currentPosition;
    uint8_t spot1, spot2;
    int valid = 0;
    char buff[MSG_MAX_SIZE];
    drawTrack();

    while(1){

        drawTrack();

        printSpot(50,0);
        send_user(USER_BOX, "What point is the train closest to? \n\r");
        //Get input from the user
        recv_user(USER_BOX, buff, MSG_MAX_SIZE);

        currentPosition = atoi(buff);

        send_user(USER_BOX, "Which two points should the train end up between? eg 4 5 \n\r");

        recv_user(USER_BOX, buff, MSG_MAX_SIZE);

        valid = sscanf(buff, "%d %d", &spot1, &spot2);

        if (valid != 2){
            send_user(USER_BOX, "give me correct format");
        }
        else {

        }



        int i = 0;


    }
}

void drawTrack(void){

    //Starting at top left
    int i;
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
