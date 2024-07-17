#include "client_ss.h"

std::string
read_file(std::string path)
{
  std::ifstream inFile(path);
  std::string content;

  if (!inFile.is_open()) {
    std::cerr << "Error opening file: " << path << std::endl;
    return "Err :("; 
  }


  std::string line;
  while (std::getline(inFile, line)) {
    content += line + "\n";
  }

  inFile.close();
  return content;
}

int 
write_file(std::vector<std::string> query)
{
  std::string path = query[1];
  std::string content = query[2];
  std::ofstream outFile(path);

  if (!outFile.is_open()) {
    std::cerr << "Error opening file for writing: " << path << std::endl;
    return -1; 
  }

  outFile << content;
  outFile.close();

  return 0;
}

std::string 
get_permissions(std::string path) {
  struct stat fileInfo;
  if (stat(path.c_str(), &fileInfo) != 0) {
      std::cerr << "Error accessing file: " << strerror(errno) << std::endl;
      return "Error";
  }

  std::string permissions = "";
  permissions += (fileInfo.st_mode & S_IRUSR) ? "r" : "-";
  permissions += (fileInfo.st_mode & S_IWUSR) ? "w" : "-";
  permissions += (fileInfo.st_mode & S_IXUSR) ? "x" : "-";
  permissions += (fileInfo.st_mode & S_IRGRP) ? "r" : "-";
  permissions += (fileInfo.st_mode & S_IWGRP) ? "w" : "-";
  permissions += (fileInfo.st_mode & S_IXGRP) ? "x" : "-";
  permissions += (fileInfo.st_mode & S_IROTH) ? "r" : "-";
  permissions += (fileInfo.st_mode & S_IWOTH) ? "w" : "-";
  permissions += (fileInfo.st_mode & S_IXOTH) ? "x" : "-";
  return permissions;
}


int 
checkQuery(std::vector<std::string>& words)
{
  if (words[0] == "read" or words[0] == "getperm")
  {
    if (words.size() > 2 or words.size() < 2)
    {
      std::cout << "Either more or less arguments provided" << std::endl;
      return -1;
    }
    else
    {
      return 1;
    }
  }
  else if(words[0]=="write")
  {
    if (words.size() <= 2)
    {
      std::cout << "Insufficient Arguments" << std::endl;
      return -1;
    }
    if (words.size() > 3)
    {
      std::string g = words[2];
      std::vector<std::string> temp ;
      temp.push_back(words[0]);
      temp.push_back(words[1]);
      for(int i = 3; i< words.size(); i++){
        g+=' ';
        g += words[i];
      }
      temp.push_back(g);
      words = temp;

      return 1;
    }
    return 1;
  }
  else
    {
      std::cout << "Sorry What?" << std::endl;
      return -1;
    }
}