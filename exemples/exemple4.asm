DclVar b
DclVar salutation
Lambda 10
SetVar salutation
DclVar auxrevoirs
Lambda 12
SetVar auxrevoirs
DclVar a
GetVar salutation
StCall
Call
SetVar b
Jump 4
CstStr "Hello World!"
Print
CstBo Vrais
Return
Jump 4
CstStr "GoodBye World!"
Print
CstBo Vrais
Return
GetVar auxrevoirs
StCall
Call
SetVar a
GetVar a
Print
GetVar b
Print
Halt
