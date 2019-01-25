# include <iostream>
# include <thread>

# include "reactor/core.cpp"
# include "reactor/event.cpp"
# include "reactor/system.cpp"

namespace example {

    void instruction (reactor::core& core) {
        std::cout << "[ " << std::this_thread::get_id() << " ] " << "instruction" << std::endl;

        core.observe("connect").for_each([ & ] (reactor::event* event) {
            std::cout << "[ " << std::this_thread::get_id() << " ] " << "connect" << std::endl;

            core.abort();
        });

        core.observe("abort").for_each([ & ] (reactor::event* event) {
            std::cout << "[ " << std::this_thread::get_id() << " ] " << "abort" << std::endl;
        });
    }

    int main () {
        reactor::core foo({ instruction }, 3);
        reactor::core bar({ instruction }, 3);
        reactor::core baz({ instruction }, 3);

        reactor::system app({ foo, bar, baz });

        app.run();

        while (app.running());
        return app.status();
    }
}

int main () {
    return example::main();
}
