#include <iostream>
#include "Classes.h"
#include "Functions.h"
using namespace std;

int main(){
	
	KeyPair key;
	key.genKeyPair(key);
	key.printDec(key);
	key.printHex(key);

	unsigned long long priv = key.getPriv();
	Signature sign;
	sign.signData(priv);

	return 0;
}