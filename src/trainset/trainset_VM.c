#include <stdio.h>
#include "trainset_VM.h"
#include "trainset_defs.h"
#include "calls.h"
#include "route_table.h"
#include <stdbool.h>
#include <string.h>
#include "k_terminal.h"


//void userIO(void) {
//
//    bind(USER_BOX);
//
//    uint8_t currentPosition;
//    uint8_t spot1, spot2;
//    int valid = 0;
//    char buff[MSG_MAX_SIZE];
//
//    while(1){
//
//
//        send_user(USER_BOX, "What point is the train closest to? \n\r");
//        //Get input from the user
//        recv_user(USER_BOX, buff, MSG_MAX_SIZE);
//
//        currentPosition = atoi(buff);
//
//        send_user(USER_BOX, "Which two points should the train end up between? eg 4 5 \n\r");
//
//        recv_user(USER_BOX, buff, MSG_MAX_SIZE);
//
//        valid = sscanf(buff, "%d %d", &spot1, &spot2);
//
//        if (valid != 2){
//            send_user(USER_BOX, "give me correct format");
//        }
//        else {
//
//        }
//
//        send_user(USER_BOX, CLEAR_SCREEN);
//
//        int i = 0;
//        for (i=0; i <30; i++){
//            send_user(USER_BOX, )
//        }
//
//
//    }
//}
