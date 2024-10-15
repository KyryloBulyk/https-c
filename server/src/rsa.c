#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"

// A simple example of generating public and private keys
void rsa_generate_keys(RSA_PublicKey *pub, RSA_PrivateKey *priv) {
    // Fixed values for demonstration purposes (not to be used in real cryptography)
    int p = 61;  // A prime number
    int q = 53;  // A prime number
    int n = p * q;
    int e = 17;  // Choosing a public exhibitor
    int d = 2753;  // Private exhibitor (calculated manually for example)

    pub->n = n;
    pub->e = e;

    priv->n = n;
    priv->d = d;
}

// Function for message encryption
int rsa_encrypt(int message, RSA_PublicKey pub) {
    int result = 1;
    for (int i = 0; i < pub.e; i++) {
        result = (result * message) % pub.n;
    }
    return result;
}

// Function to decrypt a message
int rsa_decrypt(int cipher, RSA_PrivateKey priv) {
    int result = 1;
    for (int i = 0; i < priv.d; i++) {
        result = (result * cipher) % priv.n;
    }
    return result;
}
