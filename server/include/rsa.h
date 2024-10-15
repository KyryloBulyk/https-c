#ifndef RSA_H
#define RSA_H

// Структура для відкритого ключа RSA
typedef struct {
    int n;  // Модуль (n = p * q)
    int e;  // Відкритий експонент
} RSA_PublicKey;

// Структура для закритого ключа RSA
typedef struct {
    int n;  // Модуль (n = p * q)
    int d;  // Приватний експонент
} RSA_PrivateKey;

// Генерація відкритого і закритого ключів RSA
void rsa_generate_keys(RSA_PublicKey *pub, RSA_PrivateKey *priv);

// Функція шифрування повідомлення за допомогою відкритого ключа RSA
int rsa_encrypt(int message, RSA_PublicKey pub);

// Функція дешифрування повідомлення за допомогою закритого ключа RSA
int rsa_decrypt(int cipher, RSA_PrivateKey priv);

#endif // RSA_H
