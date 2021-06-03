#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sodium.h>

typedef struct {
    unsigned int d;
    unsigned int N;
} private_key;

typedef struct {
    unsigned int e;
    unsigned int N;
} public_key;

typedef struct {
    private_key privateKey;
    public_key publicKey;
} keypair;

// checks if a number is prime
// uses inefficient loop method to find out as I am way to stupid for other implementations
int is_prime(unsigned int i) {
    if (i < 3) {
        return 1;
    }
    if (i % 2 == 0) {
        return 0;
    }
    int n = ceil(sqrt((double)i));
    for (int j = 3; j < n; ++j) {
        if (i % j == 0) {
            return 0;
        }
    }
    return 1;
}

// generate a prime number between min and max
unsigned int generate_prime(unsigned int min, unsigned int max) {
    int found_prime = 0;
    unsigned int generated;
    while (found_prime == 0) {
        generated = randombytes_uniform(max - min) + min;
        if (is_prime(generated) == 1 && generated <= max) {
            found_prime = 1;
        }
    }
    return generated;
}

// gcd
unsigned int greatest_common_divisor(unsigned int a, unsigned int b) {
    if (b == 0) {
        return a;
    }
    return greatest_common_divisor(b, a % b);
}

// b^e % m
unsigned int mod_pow(unsigned int b, unsigned int e, unsigned int m) {
    unsigned int r = 1;
    b = b % m;
    while (e > 0) {
        if (e % 2 == 1) {
            r = (r * b) % m;
        }
        e = e >> 1;
        b = (b*b) % m;
    }
    return r;
}
// Maximum and minimum prime numbers to generate as we have integer overflows quite fast without a big int implementation
const unsigned int max_prime = 17;
const unsigned int min_prime = 11;
keypair generate_keypair() {
    keypair kp;

    unsigned int p = generate_prime(min_prime, max_prime);
    // set the minimum to be p + 1 as we need 2 different prime numbers
    unsigned int q = generate_prime(p+1, max_prime+1);

    unsigned int n = p*q;
    unsigned int phi = (p - 1) * (q - 1);

    // e has to be a co-prime of phi
    unsigned int e;
    while (1) {
        e = randombytes_uniform(n-1) + 2;
        if (greatest_common_divisor(phi, e) == 1) {
            break;
        }
    }

    int d = 0;
    // The maximum of runs < phi is completely random, I am not sure what limit to set here so I just chose phi
    for (int i = 1; i < phi; ++i) {
        if ((e * i) % phi == 1 % phi) {
            d = i;
            break;
        }
    }

    private_key privateKey;
    privateKey.N = n;
    privateKey.d = d;

    public_key publicKey;
    publicKey.N = n;
    publicKey.e = e;

    kp.publicKey = publicKey;
    kp.privateKey = privateKey;

    return kp;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("invalid arguments supplied\n");
        return 1;
    }
    char *action = argv[1];
    if (strncmp(action, "generate", 8) == 0) {
        if (sodium_init() < 0) {
            return 1;
        }
        keypair kp = generate_keypair();
        printf("Private Key: (%d,%d); Public Key: (%d, %d)\n", kp.privateKey.d, kp.privateKey.N, kp.publicKey.e, kp.publicKey.N);
    } else if (strncmp(action, "encrypt", 7) == 0) {
        unsigned long e = strtoul(argv[2], NULL, 0);
        unsigned long N = strtoul(argv[3], NULL, 0);
        unsigned long dec_message = strtoul(argv[4], NULL, 0);

        unsigned int encrypted = mod_pow(dec_message, e, N);
        printf("encrypted: %d\n", encrypted);
    } else if (strncmp(action, "decrypt", 7) == 0) {
        unsigned long c = strtoul(argv[2], NULL, 0);
        unsigned long d = strtoul(argv[3], NULL, 0);
        unsigned long N = strtoul(argv[4], NULL, 0);

        unsigned int decrypted = mod_pow(c, d, N);
        printf("decrypted %d\n", decrypted);
    } else {
        printf("invalid arguments supplied\n");
        return 1;
    }
    return 0;
}
