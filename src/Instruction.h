#include <iostream>

enum INSTRUCTION_TYPE { INST_INT = 1, INST_FLOAT = 2, INST_BRANCH = 3, INST_LOAD = 4, INST_STORE = 5 };

class Instruction {
  private:
    std::string id;
    INSTRUCTION_TYPE type;

    std::string dep1 = ""; 
    std::string dep2 = ""; 
    std::string dep3 = ""; 
    std::string dep4 = ""; 

  public:
    Instruction(std::string id, INSTRUCTION_TYPE type, std::string dep1, std::string dep2, std::string dep3, std::string dep4) {
      this->id = id;
      this->type = type;
      
      this->dep1 = dep1;
      this->dep2 = dep2;
      this->dep3 = dep3;
      this->dep4 = dep4;
    }

    void print() {
      std::cout << "[id:" << id << ", Type:" << type << ", Dep1: " << dep1 << ", Dep2: " << dep2 << ", Dep3: " << dep3 << ", Dep4: " << dep4 << "]" << std::endl;
    }
};