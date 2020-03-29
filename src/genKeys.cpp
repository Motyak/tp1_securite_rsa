#include <iostream> //std::cout
#include <utility>	//std::pair
#include <random>   //std::random_device
#include <array>    //std::array
#include <vector>   //std::vector
#include <fstream> //std::ofstream

using llong = long long;
enum Key {PUB, PRIV};   //automatically cast as an int /= enum class

std::array<std::pair<int,llong>,2> generateKeys();

    std::pair<llong,llong> generatePrimes(int nbOfBits);
        llong generatePrime(int nbOfBits);
        bool isPrime(int n, int accuracy=2);
        bool millerTest(int d, int n);
        int modExp(int a, int b, int n);

    std::pair<int,int> calculate_e_and_d(int phin);
        std::pair<int,int> getfirstPairFactorization(std::vector<int> candidates);
        std::vector<int> calculatePrimeFactors(int n);
        std::vector<int> calculateCandidates(int phin);

void saveKeysToFiles(std::array<std::pair<int,llong>,2> keys);

//g++ -o bin/genKeys src/genKeys.cpp && bin/genKeys
int main()
{
    std::cout << "Generatings keys..." << std::endl;
    std::array<std::pair<int,llong>,2> keys = generateKeys();
    saveKeysToFiles(keys);
    std::cout << "Keys saved to 'pub.key' and 'priv.key' !" << std::endl;

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

std::array<std::pair<int,llong>,2> generateKeys()
{
    const int NB_BITS = 10;

    std::pair<llong, llong> primes = generatePrimes(NB_BITS);
    int p = primes.first, q = primes.second;
    int n = p * q, phin = (p - 1) * (q - 1);

    std::pair<int,int> factorization = calculate_e_and_d(phin);
    int e = factorization.first, d = factorization.second;

    return {std::make_pair(e, n), std::make_pair(d, n)};
}

void saveKeysToFiles(std::array<std::pair<int,llong>,2> keys)
{
    const std::string PUB_KEY_FILENAME = "pub.key";
    const std::string PRIV_KEY_FILENAME = "priv.key";

    std::ofstream ofs(PUB_KEY_FILENAME);
    ofs << keys[Key::PUB].first << "," << keys[Key::PUB].second << std::endl;
    ofs.close();

    ofs.open(PRIV_KEY_FILENAME);
    ofs << keys[Key::PRIV].first << "," << keys[Key::PRIV].second << std::endl;
    ofs.close();
}