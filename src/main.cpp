#include <iostream>

#include <Centicom.hpp>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    std::cout << "centicom" << std::endl;

    CenticomSettings cSettings = {
        .cSettings = {
            .portPath = "/dev/ttyUSB0",
            .baud     = 9600,
        }
    };

    Centicom centicom(cSettings);
    centicom.run();

    centicom.stop();
    return 0;
}
