#pragma once
#include "Functions.h"

class KeyPair {

private:

	unsigned long long privateKey;
	unsigned long long publicKey;

public:

	KeyPair genKeyPair(KeyPair &key) {
				
		genPrivateKey(privateKey);
		genPublicKey(publicKey, privateKey);

		return key;
	}

	void printDec() {
		cout << "Private key: " << privateKey << endl
			<< "Public \tkey: " << publicKey << endl << endl;
	}

	void printHex() {
		cout << "Private key: 0x"; dec2hex(privateKey);
		cout << "Public \tkey: 0x"; dec2hex(publicKey);
	}
	
	unsigned long long getPriv() {
		return privateKey;
	}

	unsigned long long getPub() {
		return publicKey;
	}

};

class Signature {

	int message;

public:

	int signData(int msg, unsigned long long key) {

		//2 random prime numbers
		double p = 11;
		double q = 22;
		double n = p * q;//calculate n
		unsigned long long track;
		double phi = (p - 1) * (q - 1);//calculate phi

		double e = key;
		//for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
		while (e < phi) {
			track = gcd(e, phi);
			if (track == 1)
				break;
			else
				e++;
		}
		unsigned long long c = pow(msg, e); //encrypt the message
		c = fmod(c, n);

		message = msg;

		return c;
	}

	bool verifySignature(int sign, int msg, unsigned long long key) {

		//2 random prime numbers
		double p = 11;
		double q = 22;
		double n = p * q;//calculate n
		unsigned long long track;
		double phi = (p - 1) * (q - 1);//calculate phi

		double e = key;
		//for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
		while (e < phi) {
			track = gcd(e, phi);
			if (track == 1)
				break;
			else
				e++;
		}

		double d1 = 1 / e;
		double d = fmod(d1, phi);
		double c = pow(msg, e); //encrypt the message
		unsigned long long m = pow(c, d);
		m = fmod(m, n);

		if (m == sign && msg == message)
		{
			//cout << "\nTrue" << endl;
			return true;
		}
		else
		{
			//cout << "\nFalse" << endl;
			return false;
		}
	}

};

class Account {

private:

	unsigned long long accountID;
	unsigned long long wallet[100];
	int balance = 0;
	int index = 0;
	int money = 0;

public:

	Account genAccount(Account &acc) {

		KeyPair key;
		key.genKeyPair(key);
		//key.printDec();
		key.printHex();

		accountID = key.getPub();
		wallet[index] = key.getPub();
		wallet[index + 1] = key.getPriv();

		index += 2;

		return acc;
	}

	unsigned long long getPub(int index) {
		return wallet[index];
	}

	void addKeyPairToWallet() {

		KeyPair key;
		key.genKeyPair(key);

		wallet[index] = key.getPub();
		wallet[index + 1] = key.getPriv();

		index += 2;

	}

	void updateBalance(int &balance) {
		
		balance += money;
		money = 0;

	}

	int getBalance() {
		return balance;
	}

	void printBalance() {
		cout << "\nBalance: " << balance << endl;
	}

	void createPaymentOp(Account receiver, int amount, int index) {
	
		balance -= amount;
		receiver.money = amount;

		updateBalance(receiver.balance);
		
	}

	int signData(int msg, int index) {

		Signature signature;
		int sign = signature.signData(msg, wallet[index * 2 - 1]);
		
		return sign;
	}

};

class Operation {

private:

	Account sender;
	Account receiver;
	int amount;
	int sign;

public:

	Operation createOperation(Account sender, Account receiver, int amount, int signature) {
	
		Operation operation;

		operation.sender = sender;
		operation.receiver = receiver;
		operation.amount = amount;
		operation.sign = signature;

		if (&Operation::verifyOperation) {
			operation.sender.createPaymentOp(operation.receiver, operation.amount, 1);
		}

		return operation;
	}

	bool verifyOperation(Operation operation) {

		Signature sign;

		if (sign.verifySignature(operation.sign, 10, operation.sender.getPub(0))) {

			if (operation.amount <= operation.sender.getBalance()) {

				return true;

			}

		}

	}

};

class Transaction {

private:

	string transactionID;
	Operation setOfOperations[0xFFFFF];
	int nonce = 0;

public:

	Transaction createTransaction(Operation operation) {

		Transaction transaction;

		transaction.setOfOperations[nonce] = operation;
		transaction.nonce++;

		return transaction;
	}

};