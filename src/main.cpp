#include "pesto.h"

int main(int argc, char* argv[])
{
    pesto::init(argc, argv);
    pesto::run();
    pesto::shutdown();

    return 0;
}
