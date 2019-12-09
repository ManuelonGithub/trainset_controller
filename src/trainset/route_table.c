

#include "route_table.h"
#include <stdio.h>

// Row = Where you at
// Column = Where you need to go

static const route_t table[HALL_SENSORS][HALL_SENSORS] =
{
/* row 0  */ {{.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW_S6}, {.code = ROUTE_CCW_S6}, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 1  */ {{.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 2  */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW_S5}, {.code = ROUTE_CCW_S5}, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 3  */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 4  */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 5  */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW_S4 }, {.code = ROUTE_CW_S4 }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 6  */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 7  */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 8  */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW_S3}, {.code = ROUTE_CCW_S3}, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 9  */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 10 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW_S2 }, {.code = ROUTE_CW_S2 }},
/* row 11 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 12 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 13 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW_S1 }, {.code = ROUTE_CW_S1 }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 14 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 15 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 16 */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 17 */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 18 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 19 */ {{.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }, {.code = ROUTE_CCW   }},
/* row 20 */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 21 */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }},
/* row 22 */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }, {.code = ROUTE_CCW   }},
/* row 23 */ {{.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = ROUTE_CW    }, {.code = DONE        }}
};

route_t* accessRoute(uint8_t row, uint8_t col)
{
    if (row < HALL_SENSORS && col < HALL_SENSORS) {
        return &table[row][col];
    }
    else {
        return NULL;
    }
}
