#include <stdio.h>
#include <stdexcept>
#include <iostream>

#define LOG_FILE_NAME "OpenMap.log"
#define CONFIG_FILE_NAME "config"


int main() {

        freopen(LOG_FILE_NAME, "w", stdout);
        freopen(LOG_FILE_NAME, "w", stderr);

        try {
                // TODO
        } catch(const std::exception& e) {
                std::cout << e.what() << std::endl;
                return -1;
        }

        return 0;

}
