# pragma once

# include <functional>
# include <memory>
# include <mutex>
# include <shared_mutex>
# include <string>
# include <utility>
# include <vector>

# include "fusion/emitter.cpp"
# include "fusion/executor.cpp"
# include "reactor/event.cpp"

namespace reactor {

    class core {

        private:
            fusion::emitter<event*> core_emitter;
            fusion::executor* core_executor;
            std::vector<std::function<void(core&)>> core_instructions;
            bool core_running = false;
            int core_status = 1;

        public:
            explicit core (const std::vector<std::function<void(core&)>>& instructions = { }) : core_executor(&fusion::DEFAULT_OBSERVABLE_EXECUTOR) {
                core_instructions = instructions;
            }

            fusion::observable<event*>& observe (const std::string& event_type = "") {
                return core_emitter.observe(event_type).use_executor(*core_executor);
            }

            void emit (const std::string& event_type, event* event_ptr) {
                return core_emitter.emit(event_type, event_ptr);
            }

            bool running () {
                return core_running;
            }

            int shutdown (int code = 0) {
                core_running = false;
                core_status = code;

                emit("shutdown", new event("shutdown"));

                return core_status;
            }

            void start (fusion::executor& executor = fusion::DEFAULT_OBSERVABLE_EXECUTOR) {
                core_executor = &executor;
                core_running = true;

                for (const std::function<void(core&)>& instruction : core_instructions) {
                    instruction(*this);
                }

                emit("start", new event("start"));
            }

            int status () {
                return core_status;
            }
    };
}