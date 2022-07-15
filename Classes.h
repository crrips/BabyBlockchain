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

	Account genAccount(Blockchain blockchain) {

		Account acc;

		KeyPair key;
		key.genKeyPair(key);
		//key.printDec();
		key.printHex();

		accountID = key.getPub();
		wallet[index] = key.getPub();
		wallet[index + 1] = key.getPriv();

		index += 2;

		blockchain.setAcc(acc);

		return acc;
	}

	unsigned long long getPub(int index) {
		return wallet[index];
	}

	unsigned long long getID() {
		return accountID;
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

	void setBalance(int bal) {
		balance = bal;
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

class Hash {

public:

	string toHash(string text) {

		hash<string> hash_str;
		string hash_text = to_string(hash_str(text));

		return hash_text;
	}

};

class Transaction {

private:

	string transactionID;
	Operation setOfOperations[0xFF];
	int nonce = 0;

public:

	string address2string(Transaction tx) {

		ostringstream oss;
		oss << &tx;
		string str = oss.str();

		return str;
	}

	Transaction createTransaction(Operation operation) {

		Transaction transaction;

		string tmp_str = address2string(transaction);
		Hash hash;
		transactionID = hash.toHash(tmp_str);

		transaction.setOfOperations[nonce] = operation;
		transaction.nonce++;

		return transaction;
	}

};

class Block {

private:

	string BlockID;
	string prevHash;
	Transaction setOfTransactions[0xFF];

public:

	string address2string(Block block) {

		ostringstream oss;
		oss << &block;
		string str = oss.str();

		return str;
	}

	Block createBlock(Transaction setOfTransactions[], string prevHash) {

		Block block;

		string tmp_str = address2string(block);
		Hash hash;
		BlockID = hash.toHash(tmp_str);

		block.prevHash = prevHash;

		for (int i = 0; i < 0xFF; i++)
		{
			block.setOfTransactions[i] = setOfTransactions[i];
		}

		return block;
	}

	Block genBlock(Transaction tx) {

		Block block;

		string tmp_str = address2string(block);
		Hash hash;
		BlockID = hash.toHash(tmp_str);

		block.setOfTransactions[0] = tx;

		return block;
	}

	string getPrevHash() {
		return prevHash;
	}

	string getBlockID() {
		return BlockID;
	}

};

class Blockchain {

private:

	Account coinDatabase[10000];
	Block blockHistory[10];
	int index_block = 0;
	int index_acc = 0;
	int faucetCoins = 10000;

public:

	Blockchain initBlockchain(Transaction tx) {

		Block block;
		block.genBlock(tx);

		blockHistory[index_block] = block;

		index_block++;

	}

	void getTokenFromFaucet(Account& account, int amount) {

		if (amount <= faucetCoins)
		{
			faucetCoins -= amount;
			int balance = account.getBalance() + amount;
			account.setBalance(balance);
		}
		else
		{
			cout << "In faucet remained " << faucetCoins << " coin(s)!!!" << endl;
		}

	}

	void validateBlock(Block block) {

		if (block.getPrevHash() == blockHistory[index_block - 1].getBlockID())
		{
			blockHistory[index_block] = block;
			index_block++;
		}

	}

	void setAcc(Account acc) {

		coinDatabase[index_acc] = acc;
		index_acc++;

	}

	void showCoinDatabase() {

		for (int i = 0; i < index_acc; i++)
		{
			cout << "Account ID: " << coinDatabase[i].getID() << "\n Balance: " << coinDatabase[i].getBalance() << endl << endl;
		}

	}

};