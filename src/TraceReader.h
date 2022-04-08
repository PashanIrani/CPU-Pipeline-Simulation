#include <fstream>
#include "Instruction.h"

class TraceReader {
  private:
    std::ifstream file;
    int line_size = 2048;

  public:
    TraceReader(std::string file_path) {
      file.open(file_path);

      if(!file) { // file couldn't be opened
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
      }

        
    }

    Instruction * getNextInst() {
      char line[line_size];
      
      if (file.eof()) return NULL;
      

      file.getline(line, line_size);
      
      // std::cout << line << std::endl;
      char id[64] = "";
      int type;
      char dep1[64] = "";
      char dep2[64] = "";
      char dep3[64] = "";
      char dep4[64] = "";

      sscanf(line,"%64[^,],%d,%64[^,],%64[^,],%64[^,],%64[^,]", id, &type, dep1, dep2, dep3, dep4);


      return new Instruction(std::string(id), static_cast<INSTRUCTION_TYPE>(type), std::string(dep1), std::string(dep2), std::string(dep3), std::string(dep4));      
    }
};