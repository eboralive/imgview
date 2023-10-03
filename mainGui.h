

#ifndef GTKMM_MAINGUI_H
#define GTKMM_MAINGUI_H

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdio>
#include <vector>
#include <filesystem>

class MainGui : public Gtk::Window
{

public:
  MainGui(std::string path);
  virtual ~MainGui();

protected:
 
  bool on_key_press(GdkEventKey* event);
  void quit_program(bool success);
  void goto_nextpic();
  void goto_previouspic();
  void delete_pic();
  void undelete_last_pic();
  void get_file_list(std::string path);
  bool ends_with(const std::string& s, const std::string& suffix);
  bool is_picture(const std::string& s);
  std::string get_dir_of_picture(const std::string& s);
  void populate_picture_list(const std::string& dir);
  int get_pic_index_in_piclist(const std::string path);
  void set_image();
			  
  void SetupMainGui(std::string path);
  void (MainGui::*acceptHandler)();


  //Member widgets:

  Gtk::Image m_image;
    
};

#endif
