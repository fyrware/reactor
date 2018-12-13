# pragma once

# include <string>

namespace reactor {

    class event {

        private:
            std::string event_type;

        public:
            explicit event (const std::string& type) {
                event_type = type;
            }

            std::string type () {
                return event_type;
            }
    };
}