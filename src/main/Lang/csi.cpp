#include "csi.h"

void InitChaiscript()
{
    chai.eval(R"(
        puts("Chaiscript initialized.\n");
        puts("\n");
    )");
}