# Simple C RSA implementation

This is a simple implementation of the [RSA cryptosystem](https://en.wikipedia.org/wiki/RSA_(cryptosystem)) in C.

Please do not use this in any way as this is highly insecure as this is just a demonstration and nowhere near a safe implemetation.
The used primes are limited as integer overflows would be common without it, and I have no `big int` implementation as of now. However, I plan on creating one _just for fun_ and implement it here too.

This is my second C project after [CLI Snake in C](https://github.com/tiehm/c-snake), so please do not judge too hard.

## Dependencies

This uses `sodium` for secure random number generation. Which is odd, as everything in this project is technically insecure but this.

## Build

````
gcc main.c -std=c99 -lm -lsodium -o rsa
````

## Usage

### Generate a keypair

``./rsa generate`` will output a public and private keypair.

### Encrypt a message (number)

``./rsa encrypt e N toEncrypt`` will output the encrypted message/number.

### Decrypt an encrypted number

``./rsa decrypt toDecrypt d N`` will output the original unencrypted message/number.