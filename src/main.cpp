// headers <system>

#include <ncurses.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstdlib>

// headers "project"
#include "version.h"

std::mutex mtx; // Mutex for protecting shared data
int progress = 0;
bool finished = false;

void exec(const char* command) {
    system(command);
}

void text(int row, int column, const char* str, ...) {
    mvprintw(row, column, str);
    refresh();
}


// Function to draw the progress bar in an ncurses window
void draw_progress_bar(WINDOW* win, int current_progress) {
    int max_x, max_y;
    getmaxyx(win, max_y, max_x);
    max_y = max_y + 1;
    max_y = max_y - 1; // just to satisfy compiler, ughh
    // Clear the line and draw the bar
    wclear(win);
    box(win, 0, 0); // Optional: adds a border

    int bar_width = max_x - 4;
    int pos = (bar_width * current_progress) / 100;

    mvwprintw(win, 1, 2, "Progress: [");
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) {
            mvwaddch(win, 1, i + 12, '='); // Use '=' for progress
        } else {
            mvwaddch(win, 1, i + 12, ' ');
        }
    }
    mvwprintw(win, 1, bar_width + 12, "]");
    mvwprintw(win, 1, bar_width + 14, "%d%%", current_progress);
    wrefresh(win);
}

void installer(){
    
    int total_steps = 100;
    for (int i = 0; i <= total_steps; ++i) {
        // Simulate a command or task
        exec("sudo apt update");
        exec("sudo apt install curl git python3 build-essential pkg-config");  
        exec("python3 -m pip install --upgrade pip");
        exec("python3 -m pip install pillow");      
        exec("curl \"https://git.planet-casio.com/Lephenixnoir/GiteaPC/raw/branch/master/install.sh\" -o /tmp/giteapc-install.sh && bash /tmp/giteapc-install.sh");
        exec("sudo apt install patch wget cmake python3-pil libusb-1.0-0-dev libsdl2-dev libudisks2-dev libglib2.0-dev libpng-dev");
        text(2,0,"GiteaPC installed. Please wait, while this app cleans up... ");
        std::this_thread::sleep_for(std::chrono::microseconds(90)); // just wait, system will do part here.
        // Lock mutex to update shared progress variable safely
        clear();
        refresh();
        std::lock_guard<std::mutex> lock(mtx);
        progress = i;
    }
    std::lock_guard<std::mutex> lock(mtx);
    finished = true;
}

int main() {
    initscr();
    text(0,0,"GiteaPC Installer v%s", VERSION);
    text(0,30,"This is a simple installer...");
    getch();

    text(1,0,"GiteaPC Installation commencing...");
    clear();
    refresh();

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    // Create a window for the progress bar at the bottom
    WINDOW* progress_win = newwin(3, max_x, max_y-3, 0);
    
    // Start the asynchronous task thread
    std::thread work_thread(installer);

    // Main loop for display and input
    while (true) {
        int current_progress;
        bool is_finished;
        
        // Lock mutex to read shared data safely
        {
            std::lock_guard<std::mutex> lock(mtx);
            current_progress = progress;
            is_finished = finished;
        }

        draw_progress_bar(progress_win, current_progress);
        
        if (is_finished) {
            break;
        }
        
        // Handle user input (e.g., check for 'q' to quit)
        int ch = getch();
        if (ch == 'q') {
            break;
        }

        // Small delay to prevent high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    endwin();
    return 0;
}