#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <random>
using namespace std;

void little2hex(unsigned long long dec) {

    string digits[16] = { "0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F" };
    string hex;

    while (dec != 0) {
        hex.insert(0, digits[dec % 16]);
        dec /= 16;
    }

    int bytes = hex.length() / 2;

    int left = 0;
    int right = hex.length() - 1;

    for (int i = 0; i < bytes / 2; i++) {

        swap(hex[left], hex[right - 1]);
        swap(hex[right], hex[left + 1]);

        left += 2; right -= 2;

    }

    cout << hex << endl;

}

void genPrivateKey(unsigned long long &privkey) {

    mt19937_64 gen(random_device{}());
    unsigned long long dec = gen() % 0xFFFFFFFFFFFFFFF + 0x0;
    privkey = dec;

}

void genPublicKey(unsigned long long &pubkey, unsigned long long privkey) {

    mt19937_64 gen(random_device{}());
    unsigned long long dec = gen() % 0xFFFFFFFFFFFFFFF + 0x0;

    if (dec > privkey) {
        pubkey = dec - privkey;
    }
    else
    {
        pubkey = privkey - dec;
    }
    
}

//////////////////////////////////

// find gcd
int gcd(unsigned long long a, unsigned long long b) {
    unsigned long long t;
    while (1) {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}

void sign(unsigned long long key) {

    //2 random prime numbers
    unsigned long long p = 11;
    unsigned long long q = 22;
    unsigned long long n = p * q;//calculate n
    unsigned long long track;
    double phi = (p - 1) * (q - 1);//calculate phi
    //public key
    //e stands for encrypt
    double e = key;
    //for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
    while (e < phi) {
        track = gcd(e, phi);
        if (track == 1)
            break;
        else
            e++;
    }
    unsigned long long message = 12;
    unsigned long long c = pow(message, e); //encrypt the message
    c = fmod(c, n);
    cout << "\nOriginal Message = " << message;
    cout << "\n" << "Encrypted message = " << c << endl;

}

/*
//2 random prime numbers
    unsigned long long p = 11;
    unsigned long long q = 22;
    unsigned long long n = p * q;//calculate n
    unsigned long long track;
    double phi = (p - 1) * (q - 1);//calculate phi
    //public key
    //e stands for encrypt
    double e = key;
    //for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
    while (e < phi) {
        track = gcd(e, phi);
        if (track == 1)
            break;
        else
            e++;
    }
    //private key
    //d stands for decrypt
    //choosing d such that it satisfies d*e = 1 mod phi
    double d1 = 1 / e;
    double d = fmod(d1, phi);
    unsigned long long message = 12;
    unsigned long long c = pow(message, e); //encrypt the message
    double m = pow(c, d);
    c = fmod(c, n);
    m = fmod(m, n);
    cout << "\nOriginal Message = " << message;
    cout << "\n" << "e = " << e;
    cout << "\n" << "d = " << d;
    cout << "\n" << "Encrypted message = " << c;
    cout << "\n" << "Decrypted message = " << m << endl;
*/