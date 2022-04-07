# CMPT305-Project2

## Rough Plan

### 📥 IF: 
  * Send current INST to **ID**
  * Grab next INST
  * If INST is dependant on another INST, hault all future sends to **ID**, until it's dependency is completed. Possible create a queue? to hold the "haulted" sends because fetches of new instructions will still happen, just won't send to **ID**

### 🔎 ID:
  * Determine what unit (ALU, Integer, Floating point, etc) the current INST needs
    * If unit is available, send to **EX**
    * If unit is **NOT** available, send to queue of the EX which is currently using the needed unit (this will ensure that the INST that needs it next, can run right away) 

### ⚙️ EX:
  * Send current INST to **MEM**, then set next INST in queue to current
  * If current INST needs to access memory, and another INST is already accessing memory in MEM, then add current to the queue of that **MEM**

### 🧠 MEM:
  * Send current INST to **WB** then set next INST in queue to current
  
### ✍️ WB:
  * "Complete" current INST and set next INST in queue to current
