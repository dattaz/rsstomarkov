#include "TextGenerator.hh"
#include <iterator>
#include <list>
#include <cstdlib>

namespace Markov
{
  namespace
  {
    /*!
    ** Just an helper function which tokenize a string, ie
    ** split a string into words depending on given delimiters.
    **
    ** @param str The string to split
    ** @param tokens The word container to fill with
    */
    void tokenize(const std::string& str,
		  std::vector<std::string>& tokens,
		  const std::string& delimiters = " \n\t\r\f\v")
    {
      std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
      std::string::size_type pos = str.find_first_of(delimiters, lastPos);

      while (std::string::npos != pos || std::string::npos != lastPos)
      {
	tokens.push_back(str.substr(lastPos, pos - lastPos));
	lastPos = str.find_first_not_of(delimiters, pos);
	pos = str.find_first_of(delimiters, lastPos);
      }
    }

    /*!
    ** Just generate a random number from 0 to size.
    **
    ** @param size The limit of the random number to generate
    **
    ** @return A random number
    */
    unsigned int
    randomAccess(unsigned int size)
    {
      return rand() % size;
    }

    /*!
    ** Get a random item from a map.
    **
    ** @param chain The map
    **
    ** @return A random item
    */
    TextGenerator::miter
    randomItem(TextGenerator::chainType& chain)
    {
      TextGenerator::miter item = chain.begin();
      std::advance(item, randomAccess(chain.size()));
      return item;
    }
  }

  /*!
  ** Nothing special to do in constructor
  */
  TextGenerator::TextGenerator()
  {
  }

  /*!
  ** Nothing special to do in destructor
  */
  TextGenerator::~TextGenerator()
  {
  }

  /*!
  ** Just an helper function to initialize the 3 words
  ** used in the main algorithm.
  **
  ** @param it Begining of the word list.
  ** @param end End of the word list.
  */
  void
  TextGenerator::initFirstWords(iter& it, iter end)
  {
    _previousWord = *it;
    ++it;
    if (it == end)
      return;
    _currentWord = *it;
    ++it;
    if (it == end)
      return;
    _nextWord = *it;
    ++it;
  }

  /*!
  ** This function help to learn a file.
  **
  ** Open a file, and extract all lines.
  ** The lines are tokenized.
  **
  ** Now, apply this algorithm:
  ** 1) Associate (w1, w2) with w3.
  ** 2) Set w1 = w2.
  ** 3) Set w2 = w3.
  ** 4) Set w3 with the next word in word list.
  ** 5) Redo from step 1 until there is no word remaining.
  **
  ** @param filename The name of the file to learn.
  */
  void
  TextGenerator::learn(const std::string& filename)
  {
    std::ifstream file(filename.c_str());
    if (!file)
    {
      std::cerr << "Unable to read the file " << filename << std::endl;
      return;
    }

    _previousWord = "";
    _currentWord = "";
    _nextWord = "";

    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(file, line))
      tokenize(line, tokens);

    iter it = tokens.begin();
    iter end = tokens.end();
    if (_previousWord.empty())
      initFirstWords(it, end);

    do
    {
      _chain[std::make_pair(_previousWord, _currentWord)].push_back(_nextWord);
      _previousWord = _currentWord;
      _currentWord = _nextWord;
      if (it != end)
      {
	_nextWord = *it;
	++it;
      }
    }
    while (it != end);

    if (!_previousWord.empty() && !_currentWord.empty() && !_nextWord.empty())
      _chain[std::make_pair(_previousWord, _currentWord)].push_back(_nextWord);
  }

  /*!
  ** Show the association table. Very useful for debugging purpose
  ** or simply to understand better the algorithm.
  */
  void
  TextGenerator::showDebug() const
  {
    for (miter it = _chain.begin(); it != _chain.end(); ++it)
    {
      std::cout << "[" << it->first.first << "]["
		<<  it->first.second << "] => ";
      std::copy(it->second.begin(), it->second.end(),
		std::ostream_iterator<std::string>(std::cout, " "));
      std::cout << std::endl;
    }
  }

  /*!
  ** Randomly generate a text from other text learn previously.
  **
  ** We start at the first word, then apply this algorithm:
  ** 1) Search list of word associated with (w1, w2).
  ** 2) Set w3 = random value choose in the list (w1, w2).
  ** 3) Print the value of w3.
  ** 4) Set w1 to w2.
  ** 5) Set w2 to w3.
  ** 6) Redo from step 1.
  **
  ** If no limit is given (all option to 0), then
  ** the generation is infinite.
  **
  ** @param charLimit Number of characters to display (0 no limit)
  ** @param wordLimit Number of words to display (0 no limit)
  ** @param lineLimit Number of lines to display (0 no limit)
  */
  void
  TextGenerator::generate(unsigned int charLimit,
			  unsigned int wordLimit,
			  unsigned int lineLimit)
  {
    if (_chain.size() <= 0)
      return;
    miter it = randomItem(_chain);
    std::string w1 = it->first.first;
    std::string w2 = it->first.second;
    unsigned int character = 0;
    unsigned int word = 0;
    unsigned int line = 0;

    while (line < lineLimit || !lineLimit)
    {
      const std::vector<std::string>& list = _chain[std::make_pair(w1, w2)];
      // If list.size <= 0, then this a line end with no association
      if (list.size() <= 0)
      {
	++line;
	std::cout << std::endl;
	it = randomItem(_chain);
	w1 = it->first.first;
	w2 = it->first.second;
      }
      else
      {
	w1 = w2;
	w2 = list[rand() % list.size()];
	++word;
	if (character + w2.size() >= charLimit && charLimit)
	  w2.resize(charLimit - character);
	character += w2.size();
	std::cout << w2 << " ";
	if ((character >= charLimit && charLimit) || (word >= wordLimit && wordLimit))
	{
	  std::cout << std::endl;
	  return;
	}
      }
    }
  }
}
