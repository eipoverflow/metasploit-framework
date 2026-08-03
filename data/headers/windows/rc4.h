//
// License:
// https://github.com/rapid7/metasploit-framework/blob/master/LICENSE
//

// This code was originally obtained and modified from the following source
// by Bobin Verton:
// https://gist.github.com/rverton/a44fc8ca67ab9ec32089

#define MODULO_N 256   // 2^8

void swap(unsigned char *a, unsigned char *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int KSA(char *key, unsigned char *S) {
  int len = strlen(key);
  int j = 0;

  for (int i = 0; i < MODULO_N; i++) {
    S[i] = i;
  }

  for (int i = 0; i < MODULO_N; i++) {
    j = (j + S[i] + key[i % len]) % MODULO_N;
    swap(&S[i], &S[j]);
  }

  return 0;
}

int PRGA(unsigned char *S, char *plaintext, unsigned char *ciphertext, int plainTextSize) {
  int i = 0;
  int j = 0;

  for (size_t n = 0, len = plainTextSize; n < len; n++) {
    i = (i + 1) % MODULO_N;
    j = (j + S[i]) % MODULO_N;
    swap(&S[i], &S[j]);
    int rnd = S[(S[i] + S[j]) % MODULO_N];
    ciphertext[n] = rnd ^ plaintext[n];
  }

  return 0;
}

int RC4(char *key, char *plaintext, unsigned char *ciphertext, int plainTextSize) {
  unsigned char S[MODULO_N];
  KSA(key, S);
  PRGA(S, plaintext, ciphertext, plainTextSize);
  return 0;
}
