#ifndef _PROMPT_H_
#define _PROMPT_H_

#include <string>
#include <vector>
#include <map>

struct Command
{
  int (*func)(const std::vector<std::string>& argv);  /* Function to call to do the job. */
  const std::string doc;  /* Documentation for this function.  */
  Command(int (*func)(const std::vector<std::string>& argv),const std::string doc) : func(func), doc(doc) {};
};

class Prompt
{
  public:
  Prompt();
  virtual ~Prompt();
  int start(const char* prompt);
  void addComand(const std::string name, int (*func)(const std::vector<std::string>&), const std::string doc);

  private:
  bool quit;
  Prompt(const Prompt&);
  const Prompt& operator=(const Prompt&);
  int Execute(char*);
  static char** CommandCompletion(const char* text, int start, int end);
  static char* CommandGenerator(const char* text, int state);
  char* SkipLeadingWhitespaces(char* string);
  static int Help(const std::vector<std::string>& argv);
  static int Exit(const std::vector<std::string>& argv);

  std::map<const std::string, Command&> commands;
  
};

#endif
 
