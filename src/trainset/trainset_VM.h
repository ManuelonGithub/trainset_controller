

#ifndef TRAINSET_VM_H
#define TRAINSET_VM_H

struct CUPch
{
char esc;
char sqrbrkt;
char line[2];   /* 01 through 24 */
char semicolon;
char col[2];    /* 01 through 80 */
char cmdchar;
char nul;
char ch;
};
void userIO(void);

#endif //
