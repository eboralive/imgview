// File: main.cc

#include "mainGui.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[], char *envp[])
{
  auto app = Gtk::Application::create();
  
  if (argc < 2) {
	  std::string path = std::filesystem::current_path();  
	  MainGui mainGui(path);
	  return app->run(mainGui); //Shows the window and returns when it is closed.
  } else {
	  std::string argv1 = argv[1];	  
	  if (argv1.rfind("--help", 0) == 0) {
		  std::cout << "q: quit" << std::endl;
		  std::cout << ",: previous picture" << std::endl;
		  std::cout << ".: next picture" << std::endl;
		  std::cout << "delete or backspace: move file to $HOME/.local/trash/" << std::endl;
		  std::cout << "u: restore previous delete from trash." << std::endl;	
		  return true;	  
	  }
	  
	  std::string path(argv[1]);
	  MainGui mainGui(path);
	  return app->run(mainGui); //Shows the window and returns when it is closed.
  }
  
  
}

