Pour construire les executables, faire un 'make' à la racine.

1. Implémentation :

    Fichiers utilisés :
        - src/genKeys.cpp : pour la génération de clés publiques et privées
        - src/numbers/encrypt.cpp : pour chiffrer un nombre avec une clé publique
        - src/numbers/decrypt.cpp : pour déchiffrer un nombre avec une clé privée

    Explication du code :

        -Pour générer les clés :
        1- Je génère deux nombres premiers, p et q.
            Pour générer un nombre premier, je génère des nombres composés de 10 bits aléatoires
            jusqu'à trouver un nombre premier.
                Pour tester la primalité d'un nombre, j'utilise le test de Miller-Rabin qui inclut
                un calcul d'exponentiation modulaire.
                    Pour le calcul d'exponentiation modulaire j'utilise l'algorithme
                    d'exponentiation rapide (utilisant les carrés).
        2- Je calcule n en faisant le produit de p et q, je calcule phi(n) en faisait (p-1).(q-1).
        3- Je génère un couple e et d à partir de phi(n).
            Je commence par calculer une liste de 20 candidats de cette manière :
            {1*phi(n)+1, 2*phi(n)+2, ... 20*phi(n)+20}
                Je cherche parmi la liste de candidats un nombre factorisable en exactement 
                deux nombres distincts.  Ca sera e et d.
        4- La clé publique sera (e,n), tandis que la clé privée sera (d,n).
            Je sauvegarde ces clés dans deux fichiers nommés 'pub.key' et 'priv.key' au format "nb1,nb2".


        -Pour chiffrer un nombre (avec une clé publique) :
        1- Je récupère e et n à partir de la clé publique.
        2- Je procède à un calcul d'exponentiation modulaire (algorithme d'exponentiation rapide) afin
        de calculer le message chiffré à partir du message clair et de la clé publique.
        Soit m le message en clair et x le message chiffré, x = m^e mod n.


        -Pour déchiffrer un nombre (avec une clé privée) :
        1- Je récupère d et n à partir de la clé privée.
        2- Je procède à un calcul d'exponentiation modulaire (algo exponentiation rapide) afin
        de calculer le message en clair à partir du message chiffré et de la clé privée.
        Soit x le message chiffré et m le message en clair, m = x^d mod n.


2.Test local :

    1)Générer une paires de clés publiques et privés en executant : 
        '$bin/genKeys'.

    2)Chiffrer un message m avec votre clé publique générée en executant :
        '$bin/numbers/encrypt m $(< pub.key)'

    3)Dechiffrer le message chiffré x, résultat de l'execution précédente, en executant :
        '$bin/numbers/decrypt x $(< priv.key)'

    Si tout fonctionne correctement, on devrait retrouver en résultat du déchiffrement le
    message en clair initial.

3.Test distant (à 2) :

    1)Chaque personne génère une paires de clés publiques et privés en executant : 
        '$bin/genKeys'.

    2)Les deux personnes s'échangent leurs clés publiques.

    3)Une personne A chiffre un message m avec la clé publique du destinataire en executant :
        '$bin/numbers/encrypt m cle'

    4)Cette même personne transmet le message chiffré résultant de l'execution précédente 
    au destinataire.

    5)Le destinataire déchiffre le message chiffré x avec sa clé privée en executant :
        '$bin/numbers/decrypt x cle'
    
    Si tout fonctionne, la personne B devrait retrouver le message clair envoyé par la personne A.

4.Taille petite :

...

5.Chiffrement d'un texte :

    Fichiers utilisés :
        - src/genKeys.cpp : pour la génération de clés publiques et privées
        - src/text/encrypt.cpp : pour chiffrer un texte ASCII avec une clé publique
        - src/text/decrypt.cpp : pour déchiffrer un texte ASCII avec une clé privée

    Explication du code :

        -Pour générer les clés : même méthode que pour l'exo1

        -Pour chiffrer un texte ASCII (avec une clé publique) :
        1- Je récupère e et n à partir de la clé publique.
        2- Je convertis chaque caractère du message en clair par son numéro ASCII correspondant,
        je fais en sorte qu'ils aient tous une taille de 3.
        3- Puis je sépare la chaîne en bloc de 4, en partant de la droite.
        4- On chiffre chaque bloc, correspondant à un nombre, avec la clé publique
        en utilisant la méthode de l'exo 1, on les sépare d'un espace.

        -Pour déchiffrer un texte ASCII (avec une clé privée) :
        1- Je récupère d et n à partir de la clé privée.
        2- Je déchiffre chaque bloc, correspondant à un nombre, avec la clé privée
        en utilisant la méthode de l'exo 1.
        3- Je sépare la chaîne en bloc de 3 (parce qu'on veut des codes ascii [0;127]), 
        en partant de la droite.
        4- Je convertis chaque bloc, correspondant à un code ASCII, en son caractère correspondant.

