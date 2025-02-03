#include <stdio.h>
#include <string.h>

#define MOD 26

// Function to get the 2x2 key matrix from a string
void getKeyMatrix(char key[], int keyMatrix[2][2]) {
    int k = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            keyMatrix[i][j] = key[k++] - 'A';
}

// Function to multiply 2x2 key matrix with a 2x1 message vector
void matrixMultiply(int keyMatrix[2][2], int messageVector[2], int result[2]) {
    result[0] = (keyMatrix[0][0] * messageVector[0] + keyMatrix[0][1] * messageVector[1]) % MOD;
    result[1] = (keyMatrix[1][0] * messageVector[0] + keyMatrix[1][1] * messageVector[1]) % MOD;
}

// Function to compute the modular inverse of determinant
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;
}

// Function to find the inverse of the 2x2 key matrix
void inverseKeyMatrix(int keyMatrix[2][2], int inverseMatrix[2][2]) {
    int det = keyMatrix[0][0] * keyMatrix[1][1] - keyMatrix[0][1] * keyMatrix[1][0];
    det = (det % MOD + MOD) % MOD; // Ensure positive determinant
    int detInverse = modInverse(det, MOD);

    if (detInverse == -1) {
        printf("Inverse does not exist, key is not valid.\n");
        return;
    }

    inverseMatrix[0][0] = (keyMatrix[1][1] * detInverse) % MOD;
    inverseMatrix[0][1] = (-keyMatrix[0][1] * detInverse + MOD) % MOD;
    inverseMatrix[1][0] = (-keyMatrix[1][0] * detInverse + MOD) % MOD;
    inverseMatrix[1][1] = (keyMatrix[0][0] * detInverse) % MOD;
}

// Function to encrypt a message using Hill Cipher
void encrypt(char message[], int keyMatrix[2][2]) {
    int n = strlen(message);
    if (n % 2 != 0) {
        printf("Message length should be even for 2x2 Hill Cipher\n");
        return;
    }

    int cipherText[n];
    for (int i = 0; i < n; i += 2) {
        int messageVector[2] = {message[i] - 'A', message[i + 1] - 'A'};
        int result[2];
        matrixMultiply(keyMatrix, messageVector, result);

        cipherText[i] = result[0];
        cipherText[i + 1] = result[1];
    }

    printf("Encrypted Text: ");
    for (int i = 0; i < n; i++)
        printf("%c", cipherText[i] + 'A');
    printf("\n");
}

// Function to decrypt a message using Hill Cipher
void decrypt(char cipherText[], int keyMatrix[2][2]) {
    int n = strlen(cipherText);
    int inverseMatrix[2][2];
    inverseKeyMatrix(keyMatrix, inverseMatrix);

    int decryptedText[n];
    for (int i = 0; i < n; i += 2) {
        int cipherVector[2] = {cipherText[i] - 'A', cipherText[i + 1] - 'A'};
        int result[2];
        matrixMultiply(inverseMatrix, cipherVector, result);

        decryptedText[i] = result[0];
        decryptedText[i + 1] = result[1];
    }

    printf("Decrypted Text: ");
    for (int i = 0; i < n; i++)
        printf("%c", decryptedText[i] + 'A');
    printf("\n");
}

int main() {
    char key[] = "GYBN"; // 2x2 key matrix
    char message[] = "HELP";

    int keyMatrix[2][2];
    getKeyMatrix(key, keyMatrix);

    encrypt(message, keyMatrix);
    decrypt("MODL", keyMatrix); // Replace "MODL" with the actual encrypted text

    return 0;
}
