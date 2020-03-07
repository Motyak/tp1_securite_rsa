//p = 47
//q = 31
//n = p * q = 1457
//phi(n) = (p-1)(q-1) = 1380
//e = 11	#tel que pgcd(e,phi(n)) = 1
//d = 251		#tel que d * e mod phi(n) = 1

//clé publique = (e,n) = (11,1457)
//clé privée = (d,n) = (251,1457)

//Soit le message clair m = 1000,
//Soit x le message chiffré, 
//x = m^e mod n = 1000^11 mod 1457 = 1248
//m = x^d mod n = 1248^251 mod 1457 = 1000

#include <iostream>
#include <random>
#include <utility>	//std::pair
#include <vector>
#include <cmath>	//sqrt

// #define VALEUR_P 47
// #define VALEUR_Q 31
// #define VALEUR_E 11

using Primes = std::pair<long long,long long>;

//a^b mod n
int modExp(long long a, long long b, long long n)
{
	//algo expo rapide/exponential squaring
	
	if (b == 0) 
        return 1; 
  
    if (b == 1) 
        return a % n; 
  
    long long t = modExp(a, b / 2, n); 
    t = (t * t) % n; 
  
    // if exponent is even value 
    if (b % 2 == 0) 
        return t; 
  
    // if exponent is odd value 
    else
        return ((a % n) * t) % n; 
}

//test de Miller-Rabin
bool millerTest(int d, int n) 
{ 
	std::random_device random;
    int a = 2 + random() % (n - 4); 
    int x = modExp(a, d, n); 
  
    if (x == 1  || x == n-1) 
       return true; 

    while (d != n-1) 
    { 
        x = (x * x) % n; 
        d *= 2; 
  
        if (x == 1)
			return false;
			
        if (x == n-1)
			return true; 
    }

    return false; 
}

//test de primalité utilisant le test de miller
bool isPrime(int n, int accuracy=2) 
{ 
    if (n <= 1 || n == 4)  
		return false; 
		
    if (n <= 3) 
		return true; 
  
    int d = n - 1; 
    while (d % 2 == 0) 
        d /= 2; 
  
    for (int i = 0; i < accuracy; i++) 
         if (!millerTest(d, n)) 
              return false; 
  
    return true; 
}

//generate n bits prime
long long generatePrime(int nbOfBits)
{
	bool prime = false;
	long long number;
	std::random_device random;
	while(!prime)
	{
		//on genere un nombre aléatoire, composé de n bits
		number = random() % (1<<nbOfBits);
		
		//on test si il est premier
		prime = isPrime(number);
	}
	return number;
}

int modInverse(int a, int b) 
{ 
    a = a%b; 
    for (int x=1;x<b;++x) 
       if ((a*x) % b == 1) 
          return x; 
}

Primes generatePrimes(int nbOfBits)
{
	//hardcodé pour l'instant
	// return std::make_pair(VALEUR_P,VALEUR_Q);
	
	return std::make_pair(generatePrime(nbOfBits),
		generatePrime(nbOfBits));
}

int chiffrer(int m, int e, int n)
{
	return modExp(m,e,n);
} 

int dechiffrer(int x, int d, int n)
{
	return modExp(x,d,n);
}

std::vector<int> calculateCandidates(int phin)
{
	const int NB_OF_CANDIDATES = 20;
	std::vector<int> candidates;
	for(int i=1 ; i<=NB_OF_CANDIDATES ; ++i)
		candidates.push_back(i*phin+1);

	return candidates;
}

std::vector<int> calculatePrimeFactors(int n) 
{
	std::vector<int> factors;

	while (n%2 == 0)
		n = n/2;

	for (int i = 3; i <= sqrt(n); i = i+2)
		while (n%i == 0)
		{
			factors.push_back(i);
			n = n/i;
		}
         
	if (n > 2)
	{
		factors.push_back(n);
		return factors;
	}
		
}

std::pair<int,int> getfirstPairFactorization(std::vector<int> candidates)
{
	for(int c : candidates)
	{
		std::vector<int> factorization;
		if(!isPrime(c))
		{
			factorization = calculatePrimeFactors(c);
			if(factorization.size() == 2 && factorization.at(0) != factorization.at(1))
				return std::make_pair(factorization.at(0),factorization.at(1));
		}
			
	}
}

//calculate smallest possible values
std::pair<int,int> calculate_e_and_d(int phin)
{
	std::vector<int> candidates = calculateCandidates(phin);
	return getfirstPairFactorization(candidates);
}

//g++ --std=c++11 -o bin/asym src/asym.cpp
int main()
{	
	int p,q,n,phin,e,d,m;
	
	Primes primes = generatePrimes(6);
	
	p = primes.first;
	q = primes.second;
	n = p*q;
	phin = (p-1) * (q-1);
	std::pair<int,int> factorization = calculate_e_and_d(phin);
	e = factorization.first;
	// d = modInverse(e,phin);
	d = factorization.second;
	
	std::cout<<"p="<<p<<"\tq="<<q<<"\tn="<<n<<std::endl;
	std::cout<<"phi(n)="<<phin<<"\te="<<e<<"\td="<<d<<std::endl;

	do
	{
		std::cout<<"\nEcrivez le message (nombre) à chiffrer (doit être inférieur à n="
			<<n<<") :"<<std::endl;
		std::cin>>m;
	}
	while(m > n);
	
	std::cout<<"-----------------------"<<std::endl;
	std::cout<<"Le message clair est : "<<m<<std::endl;
	
	std::cout<<"\nChiffrement du message "<<m
		<<" avec la clé publique ("<<e<<","<<n<<") :"<<std::endl;
	int encrypted = chiffrer(m,e,n);
	std::cout<<"x="<<encrypted<<std::endl;
	
	std::cout<<"\nDechiffrement du message crypté "<<encrypted
		<<" avec la clé privée ("<<d<<","<<n<<") :"<<std::endl;
	int decrypted = dechiffrer(encrypted,d,n);
	std::cout<<"m="<<decrypted<<std::endl;
	
	
	return 0;
}

