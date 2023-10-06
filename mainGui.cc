

#include "mainGui.h"


MainGui::MainGui(std::string file_name)
{
  SetupMainGui(file_name);
}

MainGui::~MainGui()
{
}

static std::vector<std::string> pic_list;
static int pic_list_index;


void MainGui::SetupMainGui(std::string file_name)
{
  
  this->add(m_image);
  
  get_file_list(file_name);

  m_image.show(); 
  
  this->signal_key_press_event().connect( sigc::mem_fun( *this, &MainGui::on_key_press) );

}


bool MainGui::on_key_press(GdkEventKey* event) {
  if (event->keyval == 'q') {
    quit_program(true);
  } else  if ((event->keyval == 0xffff) || (event->keyval == 0xff08)) {
	  delete_pic();
  } else  if (event->keyval == 0x2C) {
	  goto_previouspic();
  } else  if (event->keyval == 0x2E) {
	 goto_nextpic();
  } else  if (event->keyval == 'u') {
	  undelete_last_pic();
  } else {
	  std::cout << event->keyval << std::endl;	
  }
  return true;
  // return false; // to propagate the event further  
}


void MainGui::quit_program(bool success) {
	exit(success);
}

 void MainGui::goto_nextpic() {
	 if (pic_list_index == pic_list.size() - 1) {
		 pic_list_index = 0;
	 } else {
		 pic_list_index++;
	 }
	 set_image();
}


void MainGui::set_image() {
	this->resize(200,200);
	m_image.set(pic_list[pic_list_index]); 
	this->set_title(pic_list[pic_list_index]);
}

 
void MainGui::goto_previouspic() {
 	 if (pic_list_index == 0) {
		 pic_list_index = pic_list.size() - 1;
	 } else {
		 pic_list_index--;
	 }
	 set_image();
}

std::string get_home_dir() {
	std::string home_dir;
	 if(const char* env_p = std::getenv("HOME"))
	   home_dir = env_p; 
	 return home_dir;
}

void MainGui::delete_pic() {
	
	 std::string home_dir = get_home_dir();
	 std::string file_name_without_dir = pic_list[pic_list_index].substr(pic_list[pic_list_index].rfind("/"), pic_list[pic_list_index].length()-1);
	 std::string trash_dir = home_dir + "/.trash" ;
	 std::string trash_file = trash_dir + file_name_without_dir;
		 
	if (std::filesystem::exists(trash_dir)) {	
		if (! std::filesystem::is_directory(trash_dir)) {			
			std::cout << "unable to move file to " + trash_dir << std::endl;
			return;
		}			
	} else {
		std::filesystem::create_directory(trash_dir);
	}		
	
	 try {
        int index_to_delete = pic_list_index;  
                       
        if (pic_list.size() == 1) { 
			std::cout << "only one left" << std::endl;
			std::filesystem::rename(pic_list[pic_list_index], trash_file);
			std::cout << "no more pictures left.  closing." << std::endl;
			quit_program(true);
		} else {
			// std::cout << "current count is: " << pic_list.size() << std::endl;
		}		
               
        std::filesystem::rename(pic_list[index_to_delete], trash_file);
                 
        pic_list.erase(pic_list.begin() + index_to_delete);
        pic_list.shrink_to_fit();
       
        if (pic_list_index >= pic_list.size()) {
			pic_list_index = 0;
		}
		set_image();
        
    } catch (std::filesystem::filesystem_error& e) {
      std::cout << e.what() << '\n';
   }
   
}

void MainGui::undelete_last_pic() {
	std::cout << "u: still needs implelented" << std::endl;
}

void MainGui::get_file_list(std::string path) {
	std::string requested_pic;
	std::string dir;
	if (std::filesystem::exists(path)) {				
		if (! std::filesystem::is_directory(path)) {		
		   if (is_picture(path)) {	
			 requested_pic = path;
			 dir = get_dir_of_picture(path);
		   }
	   } else {
		   dir = path;
	   }
	   
	   populate_picture_list(dir);
	   if (pic_list.size() == 0) { 
		 std::cout << "no pictures found in this directory location." << std::endl;
		 quit_program(false); 
	   }
	   
	   if (requested_pic.length() > 0) {
		   pic_list_index = get_pic_index_in_piclist(path);
	   } else {
		   pic_list_index = 0;
	   }
	   set_image();
	    
	} else { 
		std::cout << "file or directory does not exist" << std::endl;
		quit_program(false);
	}	
}

bool MainGui::is_picture(const std::string& path_str) {
	if (ends_with(path_str, ".jpg") || 
	ends_with(path_str, ".jpeg") ||
	ends_with(path_str, ".png") || 
	ends_with(path_str, ".webp")) {
		return true;
	}
	return false;
}

std::string MainGui::get_dir_of_picture(const std::string& s) {
	 return s.substr(0, s.rfind("/"));
}

bool MainGui::ends_with(const std::string& s, const std::string& suffix) {
	// ignoring case 
	std::string s1 = s;
	std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
	std::string suffix1 = suffix;
	std::transform(suffix1.begin(), suffix1.end(), suffix1.begin(), ::toupper);
    return s1.rfind(suffix1) == (s1.size()-suffix1.size());
}

void MainGui::populate_picture_list(const std::string& path) {
	for (const auto & entry : std::filesystem::directory_iterator(path)) {
	  if (is_picture(entry.path())) {			 
		 pic_list.push_back(entry.path());
	  }
	}
}

int MainGui::get_pic_index_in_piclist(const std::string path) {
	auto it = find(pic_list.begin(), pic_list.end(), path);
    if (it != pic_list.end()) {
        return it - pic_list.begin(); 
    }
    else {
       return -1;
    }
}
