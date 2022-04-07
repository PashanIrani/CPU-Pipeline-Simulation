# CMPT305-Project2

## Rough Plan

### 📥 IF: 
  * Send current INST to **ID**
  * Grab next INST and make it current
  * If current INST is a branch; hault.

### 🔎 ID:
  * Determine if and what arithmetic (ALU, Floating point) the current INST needs
    * If arithmetic is available, send to any **EX**, and set **EX**'s arithmetic.
    * If arithmetic is **NOT** available, send to queue of the EX which is currently using the needed function (this will ensure that the INST that needs it next, can run right away) 
  * If INST is not a arithmetic, add INST to available **EX**

### ⚙️ EX:
  * Send current INST to **MEM**, then set next INST in queue to current
  * If current INST needs to access memory, and another INST is already accessing memory in MEM, then add current to the queue of that **MEM**

### 🧠 MEM:
  * Send current INST to **WB** then set next INST in queue to current
  
### ✍️ WB:
  * "Complete" current INST, and then mark self available.
