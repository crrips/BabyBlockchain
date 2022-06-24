#pragma once
#include <iostream>
#include "Functions.h"
using namespace std;

class KeyPair {

private:

	unsigned long long privateKey;
	unsigned long long publicKey;

public:

	KeyPair genKeyPair(KeyPair &key) {
				
		genPrivateKey(key.privateKey);
		genPublicKey(key.publicKey, key.privateKey);

		return key;
	}

	void printDec(KeyPair key) {
		cout << "Private key: " << key.privateKey << endl
			<< "Public \tkey: " << key.publicKey << endl << endl;
	}

	void printHex(KeyPair key) {
		cout << "Private key: 0x"; little2hex(key.privateKey);
		cout << "Public \tkey: 0x"; little2hex(key.publicKey);
	}
	
	unsigned long long getPriv() {
		return privateKey;
	}

	unsigned long long getPub() {
		return publicKey;
	}

};

class Signature {

public:

	void signData(unsigned long long key) {
		sign(key);
	}

	bool verifySignature() {

	}

};