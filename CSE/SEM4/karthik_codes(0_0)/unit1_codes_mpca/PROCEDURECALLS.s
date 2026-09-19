.TEXT
MAIN: MOV R1,#3
      BL FOO
      ADD R2,R0,R1
      SWI 0X11
FOO:
    MOV R0,#2
    MOV PC,LR