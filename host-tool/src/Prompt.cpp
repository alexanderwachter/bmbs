#include "Prompt.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <iterator>

Prompt* currentPromt;

Prompt::Prompt() :
quit(0), commands()
{
  Command* help = new Command(&Prompt::Help, "Print help for command\nhelp [command]");
  Command* exit = new Command(&Prompt::Exit, "Exit this programm\nexit");
  commands.insert(std::pair<std::string, Command&>("help", *help));
  commands.insert(std::pair<std::string, Command&>("exit", *exit));
}

int Prompt::start(const char* prompt)
{
  char* buf;
  rl_initialize();
  rl_attempted_completion_function = &Prompt::CommandCompletion;
  currentPromt = this;
  Help(std::vector<std::string>());
  while((buf = readline(prompt)) != NULL) 
  {
    char* line = SkipLeadingWhitespaces(buf);
    if(*line) //Only save non-empty commands
    {
      add_history(buf);
      Execute(line);
    }
    free(buf);
    buf = NULL;
  }
  free(buf);
  return EXIT_SUCCESS;
}

Prompt::~Prompt()
{
  for(auto &cmd : commands)
    delete &(cmd.second);
}

int Prompt::Execute(char* line)
{
  std::vector<std::string> argv;
  std::istringstream iss(line);
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::back_inserter(argv));
  if(argv.size() == 0)
    return EXIT_SUCCESS;
  auto it = commands.find(argv[0]);
  if(it != commands.end())
  {
    return it->second.func(argv);
  }

  std::cout << "Command not found!" << std::endl;
  return EXIT_FAILURE;
}

void Prompt::addComand(const std::string name, int (*func)(const std::vector<std::string>&),const std::string doc)
{
  Command* cmd = new Command(func,doc);
  commands.insert(std::pair<std::string, Command&>(name, *cmd));
}

char** Prompt::CommandCompletion(const char* text, int start, int end)
{
  char ** completionList = nullptr;

  if(start == 0)
    completionList = rl_completion_matches(text, &Prompt::CommandGenerator);

  return completionList;
}

char* Prompt::CommandGenerator(const char* text, int state)
{
  static std::map<std::string, Command&>::iterator it;
  int len = strlen(text);

  if(state == 0)
    it = currentPromt->commands.begin();

  while(it != currentPromt->commands.end())
  {
    const char* name = it->first.c_str();
    it++;
    if(strncmp(name, text, len) == 0)
    {
      char* name_dup = new char[it->first.length() +1 ];
      std::strcpy (name_dup, name);
      return name_dup;
    }
  }
  return nullptr;
}

char* Prompt::SkipLeadingWhitespaces(char* string)
{
  register char* s;
  for (s = string; *s == ' '; s++);
  return s;
}

int Prompt::Help(const std::vector<std::string>& argv)
{
  if(argv.size() < 2)
  {
    std::cout << "Available commands:" << std::endl;

    for (auto &cmd : currentPromt->commands)
    {
      std::cout << cmd.first << std::endl;
    }
    std::cout << "for more inforamtion, enter \"help <command>\"" << std::endl;
    return 0;

  }
  std::map<std::string, Command&>::iterator cmd = currentPromt->commands.find(argv[1]);
  if(cmd != currentPromt->commands.end())
    std::cout << cmd->second.doc << std::endl;
  else 
    std::cout << "Unknown command " << argv[1] << std::endl;
  return EXIT_SUCCESS;
}

int Prompt::Exit(const std::vector<std::string>& argv)
{
  exit(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}
