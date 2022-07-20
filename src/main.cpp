#include <boost/program_options.hpp>
#include <iostream>

#include <Centicom.hpp>

static int _handleCmdLine(int, char **, CenticomSettings &);

int main(int argc, char **argv) {
    std::cerr << "centicom" << std::endl;

    CenticomSettings cSettings;

    int ret = _handleCmdLine(argc, argv, cSettings);
    if(ret) {
        if (ret > 0) {
            /* Non-error exit case */
            return 0;
        } else {
            return 1;
        }
    }

    if(cSettings.cSettings.baud == 0) {
        std::cerr << "ERROR: No baud rate specified!" << std::endl;
        return -1;
    }
    if(cSettings.cSettings.portPath.size() == 0) {
        std::cerr << "ERROR: No port specified!" << std::endl;
        return -1;
    }

    Centicom centicom(cSettings);
    centicom.run();

    centicom.stop();
    return 0;
}

namespace po = boost::program_options;

static int _handleCmdLine(int argc, char **argv, CenticomSettings &_options) {
    po::options_description desc("Program options");

    /* TODO: Clean this up if more options are added. */
    desc.add_options()
        ("help,h",   "Print this help message")
        ("port,p",   po::value<std::string>(&_options.cSettings.portPath),           "Serial port")
        ("baud,b",   po::value<uint32_t>(&_options.cSettings.baud),                  "Override listening address");

    po::variables_map vmap;
    po::store(po::parse_command_line(argc, argv, desc), vmap);
    po::notify(vmap);

    if(vmap.count("help")) {
        std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    return 0;
}
