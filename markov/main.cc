#include "TextGenerator.hh"

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <list>

namespace
{
  /*!
  ** Try to convert a string to an int.
  **
  ** @param str The expression to convert
  ** @param dest The converted expression
  **
  ** @return If conversion succeed
  */
  template<typename T>
  inline bool
  fromString(const std::string& str, T& dest)
  {
    std::istringstream iss(str);
    const bool res = iss >> dest != 0;
    if (!res)
      std::cerr << "Unable to convert \"" << str << "\" to int" << std::endl;
    return res;
  }
}

/*!
** The entry point. This program take in option:
** - files
** - option
**
** The option are:
** - c: Number of characters to generate (0 == no limit)
** - w: Number of words to generate (0 == no limit)
** - l: Number of lines to generate (0 == no limit)
**
** @param argc Number of argument
** @param argv Arguments
**
** @return Error code (0 means no error)
*/
int main(int argc, char** argv)
{
  srand(time(0));
  if (argc <= 1)
  {
    std::cerr << "Usage: " << argv[0] << " [option] <files>" << std::endl
	      << "\t-c: Characters limit (0 for no limit)" << std::endl
	      << "\t-w: Words limit (0 for no limit)" << std::endl
	      << "\t-l: Lines limit (0 for no limit)" << std::endl;
    return 1;
  }

  unsigned int c = 0;
  unsigned int w = 0;
  unsigned int l = 0;
  std::list<std::string> files;
  for (int i = 1; i < argc; ++i)
  {
    if (argv[i][0] == '-' && argv[i][1] && !argv[i][2] && argv[i + 1])
    {
      std::istringstream ss;
      switch (argv[i][1])
      {
	case 'c':
	  if (!fromString(argv[i + 1], c))
	    return 3;
	  ++i;
	  break;
	case 'w':
	  if (!fromString(argv[i + 1], w))
	    return 3;
	  ++i;
	  break;
	case 'l':
	  if (!fromString(argv[i + 1], l))
	    return 3;
	  ++i;
	  break;
	default:
	  std::cerr << "Unknow option: -" << argv[i][1] << std::endl;
	  return 2;
      }
    }
    else
      files.push_back(argv[i]);
  }

  Markov::TextGenerator generator;
  typedef std::list<std::string>::const_iterator iter;
  for (iter it = files.begin(); it != files.end(); ++it)
    generator.learn(*it);

  //generator.showDebug();

  generator.generate(c, w, l);

  return 0;
}
