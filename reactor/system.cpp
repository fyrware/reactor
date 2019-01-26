# pragma once

# include <cstdlib>
# include <iostream>
# include <map>
# include <string>
# include <tuple>
# include <utility>

# include "fusion/executor.cpp"

# include "reactor/core.cpp"

namespace reactor {

    class system {

        private:
            std::vector<reactor::core> system_cores;

        public:
            explicit system (std::vector<reactor::core> cores) {
                system_cores = std::move(cores);
            }

            ~ system () {
                for (reactor::core& core : system_cores) if (core.running()) {
                    core.abort();
                }
            }

            bool running () {
                for (reactor::core& core : system_cores) if (core.running()) {
                    return true;
                }

                return false;
            }

            int status () {
                for (reactor::core& core : system_cores) if (core.status() != 0) {
                    return core.status();
                }

                return 0;
            }

            std::string env (const std::string& variable) {
                return std::getenv(variable.c_str());
            }

            int exe (const std::string& command) {
                return std::system(command.c_str());
            }

            void log (const std::string& message) {
                std::cout << message << std::endl;
            }

            void run () {
                for (reactor::core& core : system_cores) {
                    core.connect();
                }
            }
    };
}