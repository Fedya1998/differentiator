#define DEF_CMD(name, num) CMD_##name = num,
    enum Operations {
        #include "supercmd.h"
        CMD_EMPTY
    };
#undef DEF_CMD

#define FUNC(name, num) FUNC_##name = num,
    enum functions {
        #include "super_functions.h"
        FUNC_EMPTY
    };
#undef FUNC