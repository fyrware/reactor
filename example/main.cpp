# include <iostream>
# include <thread>

# include "fusion/executor.cpp"
# include "reactor/core.cpp"
# include "reactor/event.cpp"

namespace example {

    void instruction (reactor::core& application) {
        application.observe("start").for_each([ & ] (reactor::event* event) {
            std::cout << "[ " << std::this_thread::get_id() << " ] " << "application has started" << std::endl;
            application.emit("foo", new reactor::event("foo"));
        });

        application.observe("shutdown").for_each([ & ] (reactor::event* event) {
            std::cout << "[ " << std::this_thread::get_id() << " ] " << "application has shut down 1" << std::endl;
        });

        application.observe("shutdown").for_each([ & ] (reactor::event* event) {
            std::cout << "[ " << std::this_thread::get_id() << " ] " << "application has shut down 2" << std::endl;
        });

        application.observe("shutdown").for_each([ & ] (reactor::event* event) {
            std::cout << "[ " << std::this_thread::get_id() << " ] " << "application has shut down 3" << std::endl;
        });

        application.observe("foo").for_each([ & ] (reactor::event* event) {
            std::cout << "[ " << std::this_thread::get_id() << " ] " << "foo" << std::endl;
            application.shutdown(0);
        });
    }

    int main () {
        fusion::executor thread_pool(3);
        reactor::core application({ instruction });

        application.start(thread_pool);

        while (application.running());

        thread_pool.terminate();
        thread_pool.flush();

        return application.status();
    }
}

int main () {
    return example::main();
}
