#include <iostream> //std::cout
#include <utility>	//std::pair
#include <random>   //std::random_device

using llong = long long;

std::pair<llong,llong> generatePrimes(int nbOfBits);
    llong generatePrime(int nbOfBits);
    bool isPrime(int n, int accuracy=2);
    bool millerTest(int d, int n);
    int modExp(int a, int b, int n);

std::pair<int,int> getFactorization(int n);
    std::vector<int> getAllPrimes(int from, int to);

int main()
{
    //generer deux nombres premiers p et q et calculer leur produit n
    int p, q, n;
    const int NB_BITS = 11;
    std::cout << "Génération de p et q.." << std::endl;
    std::pair<int, llong> primes = generatePrimes(NB_BITS);
    p = primes.first;
    q = primes.second;
    n = p * q;
    std::cout << "p et q =" << p << "," << q << " et n = " << n << std::endl;
    std::cout << "Recherche de la factorisation à partir de n.." << std::endl;

    //Trouver la factorisation
    try {
        std::pair<int,int> factorization = getFactorization(n);
        std::cout << "Factorisation trouvée : " 
            << factorization.first << "," << factorization.second << std::endl;
    } catch(std::string s) {
        std::cout << "Erreur : " << s << std::endl;
    }

    return 0;
}

//a^b mod n
int modExp(int a, int b, int n)
{
	//algo expo rapide/exponential squaring
	
	if (b == 0) 
        return 1; 
  
    if (b == 1) 
        return a % n; 
  
    llong t = modExp(a, b / 2, n); 
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
bool isPrime(int n, int accuracy) 
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
llong generatePrime(int nbOfBits)
{
	bool prime = false;
	llong number;
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

std::pair<llong,llong> generatePrimes(int nbOfBits)
{
	return std::make_pair(generatePrime(nbOfBits),
		generatePrime(nbOfBits));
}

std::vector<int> getAllPrimes(int from, int to)
{
    std::vector<int> v;
    for(int i = from ; i <= to ; ++i)
    {
        if(isPrime(i))
            v.push_back(i);
    }
    return v;
}

std::pair<int,int> getFactorization(int n)
{
    std::vector<int> primes = getAllPrimes(1, n);
    for(int p1 : primes)
    {
        for(int p2 : primes)
        {
            int produit = p1 * p2;
            if(produit == n)
                return std::make_pair(p1, p2);
        }
    }
    throw "No factorization found!";
}