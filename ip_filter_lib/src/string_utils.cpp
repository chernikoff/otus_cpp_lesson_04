#include "string_utils.h"

std::vector< std::string > split(std::string const & str, char d)
{
  std::vector< std::string > r;

  auto stop = str.find_first_of(d);
  decltype(stop) start = 0;
  while(stop != std::string::npos)
    {
      r.push_back(str.substr(start, stop - start));

      start = stop + 1;
      stop = str.find_first_of(d, start);
    }

  r.push_back(str.substr(start));

  return r;
}



