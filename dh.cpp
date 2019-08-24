// DeffieHellman.cpp
//8-13-19
//Tyler Perkins
//Basic client server implementation of DH Key exchange
//Not secure in the slightest, just a POC
//Despagettified 8-22-19

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <time.h>

using namespace std;

class server {
//this is the server, he just genorates the public values and thats it
public:
	int g, n;
	int otherKey;

	server() {
		//make the public values g and n, and create our private value
		cout << "Constructing Server..." << endl;
		//make n
		srand(time(NULL));
		n = rand();
		cout << "n is " << n << endl;

		//make g
		g = primeGen();
		cout << "g is " << g << endl;

		//genorate a non zero private value
		while (true) {
			privateServerKey = rand() % n;
			if (privateServerKey != 0) {
				break;
			}
		}
		cout << "SERVER private value is " << privateServerKey << endl;
	}

	~server() {
		cout << "Deconstructing Server..." << endl;

	}
	
	//retreive values from the client
	void getValue(int type, int value) {
		//key: 1 - otherprivatekey
		//2 - compiled key

		switch (type) {
		case 1:
			//otherprivatekey
			forignServerValue = value;
		case 2:
			//final value
			finalValue = value;
		}
		cout << "SERVER received value" << endl;
	}

	//compute first value "otherkey"
	int findOtherKey() {
		otherKey = (g ^ privateServerKey) % n;
		cout << "SERVER otherkey is " << otherKey << endl;
		return otherKey;
	}

	int findFinalValue() {
		finalValue = (forignServerValue ^ privateServerKey) % n;
		cout << "SERVER final shared secret is " << finalValue << endl;
		return finalValue;
	}

private:
	int maxKey = 200;
	int privateServerKey;
	int finalValue;
	int forignServerValue;

	int primeGen() {

		while (true) {
			srand(time(NULL));
			int randomNumber = (rand() % maxKey);
			bool isPrime = true;

			//check for prime number
			for (int i = 2; i <= randomNumber / 2; ++i) {

				//this will catch nonprimes
				if (randomNumber % i == 0 || randomNumber == 0 ) {

					isPrime = false;
				}
				//all else will loop till the for loop ends which confirms a prime number
			}

			if (isPrime) {
				return randomNumber;
			}
			
		}
		//this should never be reached
		return 0;
	}
};

class client {
//this is the client machine will be not genorating the numbers but just connects to the server
public:
	int g, n;
	int otherKey;
	client(int g1, int n1) {
		//receive the 2 public numbers, g and n, and create our private key
		srand(time(NULL));
		cout << "Constructing Client..." << endl;
		g = g1;
		n = n1;
		//cout << "client received g as " << g << endl;
		//cout << "client received n as " << n << endl;

		//genorate a non zero private value
		while (true) {
			privateServerKey = rand() % n;
			if (privateServerKey != 0) {
				break;
			}
		}
		cout << "CLIENT private value is " << privateServerKey << endl;
	}

	~client() {
		cout << "Deconstructing Client..." << endl;

	}

	//retreive values from server
	void getValue(int type, int value) {
		//key: 1 - otherprivatekey
		//2 - compiled key

		switch (type) {
			case 1:
				//otherprivatekey
				forignServerValue = value;
			case 2:
				//final value
				finalValue = value;
		}
		cout << "CLIENT received value" << endl;
	}
	
	//compute first value "otherkey"
	int findOtherKey() {
		otherKey = (g ^ privateServerKey) % n;
		cout << "CLIENT othervalue is " << otherKey << endl;
		return otherKey;
	}

	int findFinalValue() {
		finalValue = (forignServerValue ^ privateServerKey) % n;
		cout << "CLIENT final shared secret is " << finalValue << endl;
		return finalValue;
	}
	

private:
	int privateServerKey;
	int forignServerValue;
	int finalValue;
};

int main(int argc, char **argv)
{
    //declare vars

	//create 2 objects, client and server
	server testServer;
	//pass g and n to the client. these are public information
	client testClient(testServer.g,testServer.n);

	//g, n, a, and b are all made; calculate otherkey, or A and B, then send them
	//give client its otherkey
	testClient.getValue(1, testServer.findOtherKey());
	testServer.getValue(1, testClient.findOtherKey());

	//calculate shared secret
	testClient.findFinalValue();
	testServer.findFinalValue();

	return 0;
}
