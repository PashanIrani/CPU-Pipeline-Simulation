#pragma once

#include <unordered_map>
#include <iostream>
#include "Instruction.h"

class DependencyManager {
  // A HashMap keeps track of which dependencies have been completed 
  std::unordered_map<std::string, bool> instStatus;

  // gets status of an instruction. If id is an empty string, it returns true as an empty string would be equal to "is no instruction been completed"
  bool getStatus(std::string id) {
    return id.compare("") == 0 ? true : instStatus[id];
  }

  // sets status of an instruction
  void set(std::string id, bool value) {
    instStatus[id] = value;
  }
  
  public:
  DependencyManager() {
    instStatus = {};
  }

  // Add instruction to hashmap, with default status being false
  void add(Instruction * inst) {
    if (inst == NULL) return;
    set(inst->id, false);
  } 

  // Mark an instruction to be complete
  void markComplete(Instruction * inst) {
    if (inst == NULL) return;
    set(inst->id, true);
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