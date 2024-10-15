#ifndef RSA_H
#define RSA_H

// Structure for the RSA public key
typedef struct {
    int n;  // Module (n = p * q)
    int e;  // Open exhibitor
} RSA_PublicKey;

// Structure for the RSA private key
typedef struct {
    int n;  // Module (n = p * q)
    int d;  // Private exhibitor
} RSA_PrivateKey;

// Generate public and private RSA keys
void rsa_generate_keys(RSA_PublicKey *pub, RSA_PrivateKey *priv);

// Function of encrypting a message using the RSA public key
int rsa_encrypt(int message, RSA_PublicKey pub);

// The function of decrypting a message using the RSA private key
int rsa_decrypt(int cipher, RSA_PrivateKey priv);

#endif // RSA_H
