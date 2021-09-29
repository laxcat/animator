#include <stdio.h>
#include <chrono>
#include <thread>
#include "Animator.h"


int main() {
    // state vars
    bool quit = false;
    double now;

    // quit run loop after n seconds
    Animator::doAfter(7, [&]{
        quit = true;
        printf("QUITING\n");
    });

    // run an animation
    Animator::create({
        .tick = [] (Animator::Animation const & a) {
            printf("Animation %zu progress: %f\n", a.getId(), a.getProgress());
        },
        .complete = [] (Animator::Animation const & a) {
            printf("COMPLETE ANIMATION (%zu)-------------\n", a.getId());
        },
        .duration = 3,
    });

    // run another animation during the first animation
    Animator::create({
        .start = [] (auto a) {
            printf("STARTING ANIMATION (%zu)-------------\n", a.getId());
        },
        .tick = [] (auto a) {
            printf("...Animation %zu progress: %f\n", a.getId(), a.getProgress());
        },
        .complete = [] (auto a) {
            printf("COMPLETE ANIMATION (%zu)-------------\n", a.getId());
        },
        .duration = 1,
        .delay = 1,
    });

    // run two more animations using the same config object
    Animator::AnimationConfig config = {
        .tick = [] (auto a) {
            printf("Animation %zu progress: %f\n", a.getId(), a.getProgress());
        },
        .complete = [] (auto a) {
            printf("COMPLETE ANIMATION (%zu)-------------\n", a.getId());
        },
        .duration = 4,
        .delay = 4,
    };
    size_t animA = Animator::create(config);
    size_t animB = Animator::create(config);

    // cancel one animation and completeNow the other
    Animator::doAfter(5.5, []{ 
        printf("CANCELING %zu...\n", animA);
        Animator::cancel(animA);
    });
    Animator::doAfter(6.0, []{ 
        Animator::completeNow(animB);
    });

    // setup timer
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<double>;
    TimePoint start = Clock::now();

    // run loop
    while(!quit) {
        now = Duration(Clock::now() - start).count();
        Animator::tick(now);

        // approximate 60fps
        std::this_thread::sleep_for(std::chrono::microseconds(16667));
    }
    
    return 0;
}
