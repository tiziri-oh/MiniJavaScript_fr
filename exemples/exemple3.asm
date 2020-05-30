DclVar str
DclVar a
DclVar b
DclVar fact
Lambda 17
DclArg a
SetVar fact
CstStr "Salut le monde !"
SetVar str
GetVar str
Print
CstRe 5
SetVar a
GetVar fact
StCall
GetVar a
SetArg
Call
SetVar b
GetVar b
Print
Jump 67
DclVar resultat
DclVar i
CstRe 1
SetVar resultat
CstRe 1
SetVar i
GetVar i
GetVar a
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
LowStR
Jump 12
StToRe
LowStR
Jump 9
Swap
TypeOf
CstRe 2
Equal
ConJmp 2
LowStS
Jump 2
ReToSt
LowStS
ConJmp 32
GetVar resultat
GetVar a
MultRe
SetVar resultat
GetVar i
CstRe 1
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
SetVar i
Jump -59
GetVar a
Return
Halt
