

#include "route_table.h"
#include <stdio.h>

// Row = Where you at
// Column = Where you need to go

//static const route_t table[2][4] =
//{
///*            |      column 0        |      column 1        |      column 2        |      column 3        | */
//{/* row 0  */  {   .code = DONE    }, {.code = ROUTE_1_CCW}, {.code = ROUTE_1_CCW}, {.code = ROUTE_1_CCW},
///*            |      column 4        |      column 5        |      column 6        |      column 7        | */
//               {.code = ROUTE_1_CCW}, {.code = ROUTE_1_CCW}, {.code = ROUTE_1_CCW}, {.code = ROUTE_1_CCW},
///*            |      column 8        |      column 9        |      column 10       |      column 11       | */
//               {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW },
///*            |      column 12       |      column 13       |      column 14       |      column 15       | */
//               {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW },
///*            |      column 16       |      column 17       |      column 18       |      column 19       | */
//               { .code = ROUTE_1_17}, { .code = ROUTE_1_17}, {  .code = ROUTE_CW }, {  .code = ROUTE_CW },
///*            |      column 20       |      column 21       |      column 22       |      column 23       | */
//               {.code = ROUTE_1_CCW}, {.code = ROUTE_1_CCW}, {  .code = ROUTE_CW }, {  .code = ROUTE_CW }
//},
///*            |      column 0        |      column 1        |      column 2        |      column 3        | */
//{/* row 1  */  { .code = ROUTE_CW  }, {   .code = DONE    }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW },
///*            |      column 4        |      column 5        |      column 6        |      column 7        | */
//               { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW },
///*            |      column 8        |      column 9        |      column 10       |      column 11       | */
//               { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW },
///*            |      column 12       |      column 13       |      column 14       |      column 15       | */
//               {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW },
///*            |      column 16       |      column 17       |      column 18       |      column 19       | */
//               {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, { .code = ROUTE_CCW }, {  .code = ROUTE_CW },
///*            |      column 20       |      column 21       |      column 22       |      column 23       | */
//               { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW }
//},
///*            |      column 0        |      column 1        |      column 2        |      column 3        | */
//{/* row 2  */  { .code = ROUTE_CW  }, { .code = ROUTE_CW  }, {   .code = DONE    }, { .code = ROUTE_CCW },
///*            |      column 4        |      column 5        |      column 6        |      column 7        | */
//               { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW },
///*            |      column 8        |      column 9        |      column 10       |      column 11       | */
//               { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW },
///*            |      column 12       |      column 13       |      column 14       |      column 15       | */
//               {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, {  .code = ROUTE_CW },
///*            |      column 16       |      column 17       |      column 18       |      column 19       | */
//               {  .code = ROUTE_CW }, {  .code = ROUTE_CW }, { .code = ROUTE_CCW }, {  .code = ROUTE_CW },
///*            |      column 20       |      column 21       |      column 22       |      column 23       | */
//               { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW }, { .code = ROUTE_CCW }
//},
///* row 3  */
///* row 4  */
///* row 5  */
///* row 6  */
///* row 7  */
///* row 8  */
///* row 9  */
///* row 11 */
///* row 12 */
///* row 13 */
///* row 14 */
///* row 15 */
///* row 16 */
///* row 17 */
///* row 18 */
///* row 19 */
///* row 20 */
///* row 21 */
///* row 22 */
///* ROW 23 */
//};


//route_t* table_access(uint8_t row, uint8_t col)
//{
//    if (row < 2 && col < 4) {
//        return &table[row][col];
//    }
//    else {
//        return NULL;
//    }
//}
