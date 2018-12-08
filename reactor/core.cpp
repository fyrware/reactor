# pragma once

# include <functional>
# include <vector>

# include "fusion/emitter.cpp"
# include "reactor/event.cpp"

namespace reactor {

    class core : fusion::emitter<event> {

        private:
            bool core_running = false;
            int core_status = 1;
            std::vector<std::function<void(core&)>> core_instructions;

        public:
            core (const std::vector<std::function<void(core&)>>& instructions) {
                core_instructions = instructions;
            }

            int shutdown (const int code = 0) {
                core_running = false;
                core_status = code;

                return core_status;
            }

            void start () {
                core_running = true;

                for (const std::function<void(core&)>& instruction : core_instructions) {
                    instruction(*this);
                }
            }

            bool running () {
                return core_running;
            }

            int status () {
                return core_status;
            }
    };
}