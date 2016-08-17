#ifndef TEXTGENERATOR_HH_
# define TEXTGENERATOR_HH_

# include <iostream>
# include <vector>
# include <map>
# include <utility>
# include <fstream>

namespace Markov
{
  class TextGenerator
  {
  public:
    typedef std::map<std::pair<std::string, std::string>, std::vector<std::string> > chainType;
    typedef chainType::const_iterator miter;
    typedef std::vector<std::string>::const_iterator iter;

  public:
    TextGenerator();
    ~TextGenerator();
    void learn(const std::string& filename);
    void showDebug() const;
    void generate(unsigned int character = 0,
		  unsigned int word = 0,
		  unsigned int line = 0);

  private:
    void initFirstWords(iter& it, iter end);

  private:
    chainType	_chain;
    std::string _previousWord;
    std::string _currentWord;
    std::string _nextWord;
  };
}

#endif /* !TEXTGENERATOR_HH_ */
