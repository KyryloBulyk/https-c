#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"

// Простенький приклад генерації відкритих і закритих ключів
void rsa_generate_keys(RSA_PublicKey *pub, RSA_PrivateKey *priv) {
    // Фіксовані значення для демонстрації (не використовувати в реальній криптографії)
    int p = 61;  // Просте число
    int q = 53;  // Просте число
    int n = p * q;
    int e = 17;  // Вибір публічного експонента
    int d = 2753;  // Приватний експонент (вирахувано вручну для прикладу)

    pub->n = n;
    pub->e = e;

    priv->n = n;
    priv->d = d;
}

// Функція для шифрування повідомлення
int rsa_encrypt(int message, RSA_PublicKey pub) {
    int result = 1;
    for (int i = 0; i < pub.e; i++) {
        result = (result * message) % pub.n;
    }
    return result;
}

// Функція для дешифрування повідомлення
int rsa_decrypt(int cipher, RSA_PrivateKey priv) {
    int result = 1;
    for (int i = 0; i < priv.d; i++) {
        result = (result * cipher) % priv.n;
    }
    return result;
}
