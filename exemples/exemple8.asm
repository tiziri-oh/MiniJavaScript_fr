DclVar a
DclVar b
CstRe 1
SetVar a
CstRe 2
SetVar b
GetVar a
GetVar b
Swap
TypeOf
CstRe 1
Equal
ConJmp 10
Swap
TypeOf
CstRe 1
Equal
ConJmp 2
AddRe
Jump 12
StToRe
AddRe
Jump 9
Swap
TypeOf
CstRe 2
Equal
ConJmp 2
Concat
Jump 2
ReToSt
Concat
SetVar a
GetVar a
Print
GetVar b
Print
Halt
