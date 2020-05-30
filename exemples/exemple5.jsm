Var a = 5;

Fonction fact(a){
	Si (a === 0){
		Retourner (1);
	}
	a *= fact(a-1);
	a = -a;
	Retourner (a);
}

Ecrire (fact(a));
Ecrire ("Fin de l'execution!");