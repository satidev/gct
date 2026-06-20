#define CATCH_CONFIG_RUNNER

#include "../../thirdparty/catch.hpp"
#include <filesystem>
#include "../test_data.h"

std::string TEST_FILE_DIR;

int main(int argc, char *const argv[])
{
    if (argc >= 2) {
        TEST_FILE_DIR = std::string{argv[1]};
    }
    if (std::filesystem::exists(std::filesystem::path{TEST_FILE_DIR})) {
        Catch::Session session;
        return session.run();
    }
    else {
        std::cout << "Please provide the name of the test data directory as the first command-line argument."
                  << std::endl;
    }
}
