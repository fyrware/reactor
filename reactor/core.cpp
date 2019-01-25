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
            fusion::emitter<reactor::event*>* core_emitter;
            fusion::executor* core_executor;
            std::vector<std::function<void(core&)>> core_instructions;
            bool core_running;
            int core_status;

        public:
            explicit core (const std::vector<std::function<void(core&)>>& instructions = { }, int thread_count = 0) {
                core_emitter = new fusion::emitter<reactor::event*>();
                core_executor = new fusion::executor(thread_count);
                core_instructions = instructions;
                core_running = false;
                core_status = 0;
            }

            core (const core& copy) {
                core_emitter = copy.core_emitter;
                core_executor = copy.core_executor;
                core_instructions = copy.core_instructions;
                core_running = copy.core_running;
                core_status = copy.core_status;
            }

            fusion::observable<event*>& observe (const std::string& event_type = "") {
                return core_emitter->observe(event_type).use_executor(*core_executor);
            }

            void emit (const std::string& event_type, event* event_ptr) {
                return core_emitter->emit(event_type, event_ptr);
            }

            bool running () {
                return core_running;
            }

            int abort (int code = 0) {
                core_running = false;
                core_status = code;

                core_executor->flush();

                emit("abort", new event("abort"));

                return core_status;
            }

            void connect () {
                core_running = true;

                for (const std::function<void(core&)>& instruction : core_instructions) {
                    instruction(*this);
                }

                emit("connect", new event("connect"));
            }

            int status () {
                return core_status;
            }
    };
}