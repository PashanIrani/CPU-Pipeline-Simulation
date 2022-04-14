#pragma once

#include <unordered_map>
#include <iostream>
#include "Instruction.h"

class DependencyManager {
  // A HashMap keeps track of which dependencies have been completed 
  std::unordered_map<std::string, bool> instStatus;
  std::unordered_map<std::string, bool> duplicate;

  // gets status of an instruction. If id is an empty string, it returns true as an empty string would be equal to "is no instruction been completed"
  bool getStatus(std::string id) {
    if (instStatus.find(id) != instStatus.end()){ // if id is in instStatus give status
      return id.compare("") == 0 ? true : instStatus[id];
    }
    return id.compare("") == 0 ? true : duplicate[id];
    
  }

  // sets status of an instruction
  void set(std::string id, unsigned int index, bool value) {
    index_to_inst[index] = id;
    instStatus[index_to_inst[index]] = value;

  }
  
  // sets status of an instruction
  void setDuplicate(std::string id, bool value) {
    duplicate[id] = value;
  }

  public:
  DependencyManager() {
    instStatus = {};
    duplicate = {};
  }

  // Add instruction to hashmap, with default status being false
  void add(Instruction * inst) {
    if (inst == NULL) return;
    if (instStatus.count(inst->id)==0){
      set(inst->id, false);
      return;
    }
    setDuplicate(inst->id, false);
    return;
  } 

  // Mark an instruction to be complete
  void markComplete(Instruction * inst) {
    if (inst == NULL) return;
    if (instStatus.find(inst->id) != instStatus.end()){
      set(inst->id, true);
      return;
    }
    setDuplicate(inst->id, true);
    return;
  }

  // Print HashMaps contents
  void print() {
    std::cout << "DM:" << std::endl; 
    for(auto iter = instStatus.begin(); iter != instStatus.end(); ++iter){ 
      std::cout << iter->first << ": " << instStatus[iter->first] << std::endl;
    }
  }

  // Returns true if all the dependenciese of the inst have been met, false otherwise.
  bool dependenciesMet(Instruction * inst) {
    if (inst == NULL) return true;
    return getStatus(inst->dep1) && getStatus(inst->dep2) && getStatus(inst->dep3) && getStatus(inst->dep4); 
  }
};