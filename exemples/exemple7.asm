DclVar a
DclVar b
CstStr "Bonjour "
SetVar a
CstStr "Darris"
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
GreStR
Jump 12
StToRe
GreStR
Jump 9
Swap
TypeOf
CstRe 2
Equal
ConJmp 2
GreStS
Jump 2
ReToSt
GreStS
ConJmp 57
CstStr "a > b"
Print
GetVar b
CstRe 10
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
SetVar b
CstStr "val de b : "
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
Print
Jump 2
CstStr "a < b"
Print
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
Print
GetVar b
CstStr "6"
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
Print
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
Print
GetVar b
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
Print
GetVar a
CstRe 9
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
Print
CstRe 2
CstRe 8
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
Print
Halt
