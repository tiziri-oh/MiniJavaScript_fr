Fonction main()
{
	Var a = 5;

	Fonction fact(a){
		Si (a === 0){
			Retourner (1);
		}
		a *= fact(a-1);
		Retourner (a);
	}

	Ecrire (fact(a));
	Retourner(0);
}

main();