# include <iostream>

# include "reactor/core.cpp"
# include "reactor/event.cpp"

namespace example {

    void instruction (reactor::core& application) {
        application.observe("start").for_each([ & ] (reactor::event* event) {
            std::cout << "application has started" << std::endl;

            application.emit("foo", new reactor::event("foo"));
            application.shutdown();
        });

        application.observe("shutdown").for_each([] (reactor::event* event) {
            std::cout << "application has shutdown" << std::endl;
        });

        application.observe("foo").for_each([] (reactor::event* event) {
            std::cout << "foo" << std::endl;
        });
    }

    int main () {
        reactor::core application({
            instruction
        });

        application.start();

        while (application.running());
        return application.status();
    }
}

int main () {
    return example::main();
}
