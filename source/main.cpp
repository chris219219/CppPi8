#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include "..\include\numtypes.h"
#include "..\include\bbp.h"
#include "..\include\threading.h"

using namespace pimath;
using std::cout;
using std::cin;
using std::getline;
using std::string;
using std::thread;
using std::fstream;
using std::ofstream;
using std::ifstream;

bool file_exists(const char* file_name);

template <typename T>
bool retrieve(const char* file_name, T& res);

template <typename T>
bool store(const char* file_name, const T& val);

template <typename T>
void store(fstream& strm, const T& val);

void run_calc();
void start_pause_cmd();
void save_cmd();
void quit_cmd();

const fast32 MACHINE_THREADS = std::thread::hardware_concurrency();
const char* PIHEX_TXT = "pihex.txt";
const char* ITER_TXT = "iter.txt";

bool pause = true;
bool stop = false;
bool paused = true;
bool stopped = false;

fast64 iterations = 0;
string pihexdata{};

fstream pihexstream;

int main()
{
    cout
        << "--------------------------------------\n"
        << " Welcome to CppPi8, a program made to \n"
        << "  calculate hexadecimaldigits of pi!  \n"
        << "--------------------------------------\n";

    cout << '\n';

    cout
        << "NOTES:\n"
        << "1. Calculated digits will be stored in memory until saved.\n"
        << "2. Digits are auto-saved if the number of digits calculated\n"
        << "   in this run hits the 32 bit signed integer limit.\n"
        << "3. Digits are saved in pihex.txt, the number of iterations\n"
        << "   the program has completed in total is saved in iter.txt.\n";

    cout << '\n';

    cout << "Attempting to load previous saved data...\n";

    if (retrieve(ITER_TXT, iterations))
        cout << "Loaded previous iterations.\n";
    else
        cout << "WARNING: Couldn't load previous iterations!\n";
    
    if (file_exists(PIHEX_TXT))
        cout << "Found pihex file.\n";
    else
        cout << "WARNING: Couldn't find pihex file!\n";

    pihexstream.open(PIHEX_TXT, fstream::in | fstream::out | fstream::app);
    
    cout << '\n';

    cout << "NOTE: program starts paused!!!\n";
    thread th(run_calc);
    th.detach();

    cout << '\n';

    cout
        << "Commands:\n"
        << "1 = start/pause\n"
        << "2 = save\n"
        << "3 = quit\n";

    string cmd;
    while (!stopped)
    {
        cout << '\n';
        cout << "> ";

        getline(cin, cmd);

        if (cmd == "1")
        {
            start_pause_cmd();
        }
        else if (cmd == "2")
        {
            save_cmd();
        }
        else if (cmd == "3")
        {
            quit_cmd();
        }
        else
        {
            cout << "Invalid command.\n";
        }
    }

    cout << '\n';
    cout << "Press [ENTER] to continue...";
    cin.ignore();
}

void run_calc()
{
    while (!stop)
    {
        if (stopped) stopped = false;
        while (!pause)
        {
            if (paused) paused = false;
            
            if (pihexdata.size() + MACHINE_THREADS >= INT_FAST32_MAX)
            {
                store(pihexstream, pihexdata);
                pihexdata.clear();
            }

            string result = pihexthread(iterations, MACHINE_THREADS);
            pihexdata += result;
            iterations += MACHINE_THREADS;
        }
        if (!paused) paused = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!stopped) stopped = true;
}

void start_pause_cmd()
{
    fast64 tempiterations = iterations;
    switch (paused)
    {
        case true:
            cout << "Starting...\n";
            pause = false;
            while (paused)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cout << "Started at " << tempiterations << " iterations.\n";
            break;

        case false:
            cout << "Pausing...\n";
            pause = true;
            while (!paused)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cout << "Paused at " << iterations << " iterations.\n";
            break;
    }
}

void save_cmd()
{
    if (!paused)
    {
        cout << "Must be paused to save.\n";
        return;
    }

    cout << "Saving...\n";
    store(ITER_TXT, iterations);
    store(pihexstream, pihexdata);
    cout << "Saved.\n";
}

void quit_cmd()
{
    cout << "Quitting...\n";
    pause = true;
    stop = true;
    while (!stopped)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cout << "Quit at " << iterations << " iterations.\n";
}

bool file_exists(const char* file_name)
{
    ifstream strm(file_name);
    if (!strm.is_open())
        return false;
    strm.close();
    return true;
}

template <typename T>
bool retrieve(const char* file_name, T& res)
{
    ifstream strm(file_name);
    if (!strm.is_open())
        return false;
    strm >> res;
    strm.close();
    return true;
}

template <typename T>
bool store(const char* file_name, const T& val)
{
    ofstream strm(file_name);
    if (!strm.is_open())
        return false;
    strm << val;
    strm.close();
    return true;
}

template <typename T>
void store(fstream& strm, const T& val)
{
    strm << val;
}