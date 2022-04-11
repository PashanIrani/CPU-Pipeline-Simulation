#pragma once

#include <iostream>

enum INSTRUCTION_TYPE { INST_INT = 1, INST_FLOAT = 2, INST_BRANCH = 3, INST_LOAD = 4, INST_STORE = 5, INST_UNDEFINED = 6, INST_SUSPEND = 7 };

class Instruction {
  public:
    std::string id; // Instruction Id
    INSTRUCTION_TYPE type; // Type of instruction

    std::string dep1 = ""; // Dependency 1
    std::string dep2 = ""; // Dependency 2 
    std::string dep3 = ""; // Dependency 3
    std::string dep4 = ""; // Dependency 4

    Instruction() {
      this->id = "SUSPEND";
      this->type = INST_SUSPEND;

      this->dep1 = "";
      this->dep2 = "";
      this->dep3 = "";
      this->dep4 = "";
    }

    // Simply assigns values to class members
    Instruction(std::string id, INSTRUCTION_TYPE type, std::string dep1, std::string dep2, std::string dep3, std::string dep4) {
      this->id = id;
      this->type = type;
      
      this->dep1 = dep1;
      this->dep2 = dep2;
      this->dep3 = dep3;
      this->dep4 = dep4;
    }

    // Prints class
    void print() {
      std::cout << "[id: " << id 
      << ", Type: " << getTypeAsString() 
      << ", Dep1: " << dep1 
      << ", Dep2: " << dep2 
      << ", Dep3: " << dep3 
      << ", Dep4: " << dep4 << "]" << std::endl;
    }

    std::string getTypeAsString() {
      switch (type)
      {
      case INST_INT:
        return "INT";
        break;
      case INST_FLOAT:
        return "FLOAT";
        break;
      case INST_BRANCH:
        return "BRANCH";
        break;
      case INST_LOAD:
        return "LOAD";
        break;
      case INST_STORE:
        return "STORE";
        break; 
      case INST_SUSPEND:
        return "SUSPEND";
        break; 
      default:
        return "UNDEFINED";
        break;
      }
    }
};