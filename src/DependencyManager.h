#pragma once

#include <unordered_map>
#include <iostream>
#include "Instruction.h"
#include <vector>

class DependencyManager {
  // A HashMap keeps track of which dependencies have been completed 
  std::unordered_map<size_t, bool> instStatus;
  std::unordered_map<std::string,  std::vector<size_t>> idToIndex;

  // gets status of an instruction. If id is an empty string, it returns true as an empty string would be equal to "is no instruction been completed"
  bool getStatus(std::string id, size_t index) {
    if (id.compare("") == 0) return true;


    return instStatus[getIndex(id, index)];
  }

  // sets status of an instruction
  void set(size_t index, bool value) {
    instStatus[index] = value;
  }

  public:
  DependencyManager() {
    instStatus = {};
  }

  // Returns the index for the provided instruction ID. It returns the closest index (most recently arrived) to itself, and lower than itself.
  // As that would the logical index to use
  size_t getIndex(std::string id, size_t rIndex) {
    std::vector<size_t> ids = idToIndex[id];

    for(auto iter = ids.rbegin(); iter != ids.rend(); ++iter){ 
      if (*iter < rIndex) {
        return *iter;
      }
    }

    std::cout << "Index for " << id << "not found." << std::endl;
    exit(1);
  }

  void logIndex(Instruction * inst) {
    if (idToIndex.count(inst->id) == 0) {
      std::vector<size_t> ids;
      idToIndex[inst->id] = ids;
    }

    // Only keep log of the newly arriving index and the most recent one, as they are the only ones that matter
    std::vector<size_t> list = idToIndex[inst->id]; // get current vector
    std::vector<size_t> ids; // new vector that will replace current vector
    
    // If list is not empty, grab the most recent index and add to new vector
    if (list.size() > 0) {
      size_t lastId = list[list.size() - 1];
      ids.push_back(lastId);
    }

    // add the new index to the array
    ids.push_back(inst->index);
    idToIndex[inst->id] = ids; // replace old array
  }
  // Add instruction to hashmap, with default status being false
  void add(Instruction * inst) {
    logIndex(inst); // log index

    set(inst->index, false); // set instruction completetion status to false
  } 

  // Mark an instruction to be complete
  void markComplete(Instruction * inst) {
    set(inst->index, true); // set instruction completetion stattus to true
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
    return getStatus(inst->dep1, inst->index) && getStatus(inst->dep2, inst->index) && getStatus(inst->dep3, inst->index) && getStatus(inst->dep4, inst->index); 
  }
};