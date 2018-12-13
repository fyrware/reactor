# pragma once

# include <functional>
# include <string>
# include <utility>
# include <vector>

# include "fusion/emitter.cpp"
# include "reactor/event.cpp"

namespace reactor {

    class core : public fusion::emitter<event*> {

        private:
            std::vector<std::function<void(core&)>> core_instructions;
            bool core_running = false;
            int core_status = 1;

        public:
            explicit core (const std::vector<std::function<void(core&)>>& instructions = { }) {
                core_instructions = instructions;
            }

            int shutdown (int code = 0) {
                core_running = false;
                core_status = code;

                emit("shutdown", new event("shutdown"));

                return core_status;
            }

            void start () {
                core_running = true;

                for (const std::function<void(core&)>& instruction : core_instructions) {
                    instruction(*this);
                }

                emit("start", new event("start"));
            }

            bool running () {
                return core_running;
            }

            int status () {
                return core_status;
            }
    };
}