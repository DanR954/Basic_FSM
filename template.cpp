#include <vector>
#include <string>
#include <iostream>

// Deifnitions
enum class LEDColour
{
    RED,
    GREEN,
    BLUE,
    PURPLE
};

enum class next_transition
{
    no_change,
    go_idle,
    go_listen,
    go_process,
    go_respond,
    go_playing,
    go_paused,
    go_error
};

namespace States
{

    class context_class
    {
    private:
        int volume = 1;
        std::string track = "Viva la vida";

    public:
        int get_volume()
        {
            return volume;
        }

        std::string get_track()
        {
            return track;
        }

        void set_volume(int new_vol)
        {
            volume = new_vol;
        }

        void set_track(std::string new_track)
        {
            track = new_track;
        }
    };

    class transfer_states
    {

    };

    class state
    {
        LEDColour LED;

    public:
        virtual void enter(context_class &ctx)
        {
        }
        virtual void update(context_class &ctx)
        {
        }
        virtual void exit(context_class &ctx)
        {
        }
        virtual void name()
        {
        }
    };

    class idle : public state
    {
    public:
        void enter(context_class &ctx) override
        {
            std::cout << "hello from idle \n";
        }
        void update(context_class &ctx) override
        {
            std::cout << "update output from idle" << ctx.get_volume() << "\n";
        }
        void exit(context_class &ctx) override
        {
            std::cout << "bye from idle \n";
        }
        void name() override
        {
            std::cout << "The current state is idle \n";
        }
    };

    class listen : public state
    {
    public:
        void enter(context_class &ctx) override
        {
            std::cout << "hello from listen \n";
        }
        void update(context_class &ctx) override
        {
            std::cout << "update output from listen " << ctx.get_volume() << "\n";
        }
        void exit(context_class &ctx) override
        {
            std::cout << "bye from listen \n";
        }
        void name() override
        {
            std::cout << "The current state is listen \n";
        }
    };

    class process : public state
    {
    public:
        void enter(context_class &ctx) override
        {
            std::cout << "hello from process \n";
        }
        void update(context_class &ctx) override
        {
            std::cout << "update output from process \n";
        }
        void exit(context_class &ctx) override
        {
            std::cout << "bye from process \n";
        }
        void name() override
        {
            std::cout << "The current state is process \n";
        }

        void process_input()
        {
            //  process logic
        }
    };

    class respond : public state
    {
    public:
        void enter(context_class &ctx) override
        {
            std::cout << "hello from respond \n";
        }
        void update(context_class &ctx) override
        {
            std::cout << "update output from respond \n";
        }
        void exit(context_class &ctx) override
        {
            std::cout << "bye from respond \n";
        }
        void name() override
        {
            std::cout << "The current state is respond \n";
        }
        void generate_response()
        {
            // Response generation logic
        }
    };

    class error : public state
    {
    public:
        std::string error_message;
        void enter(context_class &ctx) override
        {
            std::cout << error_message;
        }
        void update(context_class &ctx) override
        {
            std::cout << error_message;
        }
        void exit(context_class &ctx) override
        {
        }
        void name() override
        {
            std::cout << "Currently in an error state \n";
        }

        void set(std::string error_text)
        {
            error_message = error_text;
        }
    };
    class media_state : public state
    {
    public:
        virtual void play_media() = 0;

        virtual void stop_media()
        {
            // Stop media playback logic
        }

        virtual void pause_media() = 0;

        virtual void currently_playing(context_class &ctx)
        {
            // outputs the current song name
        }

        virtual void volume_control(context_class &ctx)
        {
            // Volume control
        }

        virtual void rewind()
        {
            // Go back to the start or previous song
        }

        virtual void skip() = 0;
    };

    class media_paused_state : public media_state
    {

    public:
        void stop_media() override
        {
        }
        void play_media() override
        {
            std::cout << "print";
        }

        void pause_media() override
        {
        }
        void currently_playing(context_class &ctx) override
        {
        }
        void volume_control(context_class &ctx) override
        {
            // Volume control
        }

        virtual void rewind() override
        {
            // Go back to the start or previous song
        }

        virtual void skip() override
        {
            std::cout << "print";
            // go to the next song and RESUME by transitioning to the playing state
        }
    };

    class media_playing_state : public media_state
    {
    public:
        void play_media() override
        {
        }
        void stop_media() override
        {
        }
        void pause_media() override
        {
            std::cout << "print";
        }

        void currently_playing(context_class &ctx) override
        {
        }

        void volume_control(context_class &ctx) override
        {
            // Volume control
        }

        virtual void rewind() override
        {
            // Go back to the start or previous song
        }

        virtual void skip() override
        {

            // go to the next song
        }
    };

    class state_manager
    {
        state *currentState; // this is for any state within the state address.
        media_state *currentMediaState;

    private: // MUST BE HERE IF I'M TO ACCESS IT WHILE STILL BEING ENCAPSULATED
        idle *idleState;
        listen *listenState;
        process *processState;
        media_paused_state *pausedState;
        media_playing_state *playingState;
        error *errorstate;

        context_class ctx;

        void change_state(state *newstate) // Type state* + newState
        {
            if (currentState)
            {
                currentState->exit(ctx);
            }

            currentState = newstate;
            currentState->enter(ctx);
        };

    public:
        state_manager() // no void as it is a constructor. use ~ if wanting to make a deconstructor
        {
            // Initlise all classes at the start
            idleState = new idle();
            listenState = new listen();
            processState = new process();
            playingState = new media_playing_state();
            pausedState = new media_paused_state();
            errorstate = new error();

            currentState = idleState;
            currentState->enter(ctx);
        }

        // Use these to switch state.
        void go_to_idle() { change_state(idleState); };
        void go_to_listen() { change_state(listenState); }
        void go_to_process() { change_state(processState); };
        void go_to_play() { change_state(playingState); };
        void go_to_paused() { change_state(pausedState); };
        void go_to_error() { change_state(errorstate); };

        void update_state()
        {
            if (!currentState)
            {
                errorstate->set("error in switching");
                currentState = errorstate;
            }
            else
            {
                currentState->update(ctx);
            }
        };

        void get_state()
        {
            currentState->name();
        }

        ~state_manager()
        {
            delete idleState;
            delete listenState;
            delete processState;
            delete pausedState;
            delete playingState;
            delete errorstate;
        }
    };
};

int main()
{
    States::state_manager manager;

    manager.go_to_idle(); // Remember to do it from the manager
    manager.go_to_listen();
    manager.go_to_idle(); // Remember to do it from the manager
    manager.go_to_listen();
    manager.update_state(); // Want it to update only the state selected
    manager.get_state();
}

// State Transitions

// Use a base state class for use inherentence or polymorphism if needed in the future

// Remember to pass information between instances as needed

// Error handling in the logic to deal with edge cases and exceptions

// return conditions

// Concurrency?

// State class / manager to handle the transitions

// Model View Controller (MVC) pattern for better separation of concerns?

// Documentation and comments for clarity

// Unit tests for each class and method

// Git version control for tracking changes