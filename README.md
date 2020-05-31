Compilateur MiniJavaScript en français

Pour compiler il faut executer la commande:
	- make

Pour lancer le programme:
	- ./bin/myJSCompiler <source.jsm> <destination.asm>

On peut lancer des exemples prédéfini, utilisé pour débugué l'application. Les fichiers de test associés sont stockés dans exemples/exemple<n>.jsm:
	- make exp num=<n>
	
Pour lancer le programme en mode debug avec une execution dans la machine virtuelle miniJS:
	- ./bin/myJSCompiler <source.jsm> <destination.asm> 1
	- make debug num=<n>
