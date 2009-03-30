        RSEG CODE:CODE:NOROOT(2)
        CODE32


    PUBLIC  Save_Task_SP
    PUBLIC  Restore_Task_SP

Save_Task_SP
   MOV R0,SP
   BX LR
  
Restore_Task_SP
   MOV SP,R0
   BX LR
   
   END