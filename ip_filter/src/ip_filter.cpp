#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <ip_filter/string_utils.h>
#include <ip_filter/ip_pool.h>

int main(int /*argc*/, char const * /*argv*/[])
{
    try
    {
        ip_pool pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector< std::string > v = split(line, '\t');
            pool.push_back(ip_v4::make(v.at(0)));
        }

        pool = pool.reverse_sort();

        std::cout << pool << std::endl;

        std::cout << pool.filter(1) << std::endl;

        std::cout << pool.filter(46, 70) << std::endl;

        std::cout << pool.filter_any(46) <<  std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
