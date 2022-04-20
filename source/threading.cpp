#include <vector>
#include <future>
#include "..\include\numtypes.h"
#include "..\include\bbp.h"
#include "..\include\threading.h"

using std::string;
using std::vector;
using std::future;
using std::async;

namespace pimath
{
    string pihexthread(fast64 n, fast32 threads)
    {
        vector<future<char>> futures;
        futures.reserve(threads);
        for (int i = 0; i < threads; ++i)
        {
            futures.push_back(async(std::launch::async, pihex, n));
            ++n;
        }

        string results;
        results.reserve(threads);
        for (future<char>& f : futures)
        {
            results.push_back(f.get());
        }
        
        return results;
    }
}