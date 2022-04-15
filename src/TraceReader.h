#pragma once

#include <fstream>
#include "Instruction.h"
#include "Global.h"

/*
* Reads file line by line and converts a line to an Instruction object.
*/
class TraceReader {
  private:
    std::ifstream file;
    int line_size = 2048;
    int index = 0;
  public:
    Global *global;
    // Opens file for reading
    TraceReader(std::string file_path, Global *global) {
      file.open(file_path);
      this->global = global;
      if(!file) { // file couldn't be opened
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
      }
    }

    // Reads next line and return a pointer to an Instruction object
    Instruction * getNextInst() {
      char line[line_size];
      
      if (file.eof()) return NULL; // return NULL if reaches EOF

      global->total_inst++;
      if(global->total_inst>=global->INSTRUCTION_COUNT) global->hault = true;

      file.getline(line, line_size); // get next line
      
      char id[64] = "";
      int type;
      char dep1[64] = "";
      char dep2[64] = "";
      char dep3[64] = "";
      char dep4[64] = "";

      sscanf(line,"%64[^,],%d,%64[^,],%64[^,],%64[^,],%64[^,]", id, &type, dep1, dep2, dep3, dep4); // scan line for values

      // Create object pointer and return
      return new Instruction(std::string(id), ++index, static_cast<INSTRUCTION_TYPE>(type), std::string(dep1), std::string(dep2), std::string(dep3), std::string(dep4));      
    }
};