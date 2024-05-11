#include "nameserver_ss.h"
void 
create_file(std::string path) {
  std::ofstream outfile(path);
  if (outfile.is_open()) {
    std::cout << "File created successfully: " << path << std::endl;
    outfile.close();
  } else {
    std::cerr << "Failed to create file: " << path << std::endl;
  }
}

void 
delete_file(std::string path) {
  std::ifstream infile(path);
  if (!infile.good()) {
    std::cerr << "File does not exist: " << path << std::endl;
    return;
  }

  if (std::remove(path.c_str()) == 0) {
    std::cout << "File deleted successfully: " << path << std::endl;
  } else {
    std::cerr << "Failed to delete file: " << path << std::endl;
  }
}

void
copy_file(std::string source, std::string destination){

}