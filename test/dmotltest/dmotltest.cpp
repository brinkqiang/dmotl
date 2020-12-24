
#include "dmotl.h"

int main( int argc, char* argv[] ) {

    Idmotl* module = dmotlGetModule();
    if (module)
    {
        module->Test();
        module->Release();
    }
    return 0;
}
