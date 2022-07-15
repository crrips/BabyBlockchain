#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <random>
#include <sstream>
using namespace std;

//////////////////////////////////

void dec2hex(unsigned long long dec) {

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

int gcd(int a, int b) {
    double t;
    while (1) {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}

//////////////////////////////////
