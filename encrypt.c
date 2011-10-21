#include <stdio.h>
#define MAX_CHAR_LEN 10000

void produceKeystream(int textlength, unsigned char key[], 
		      int keylength, unsigned char keystream[])
{
  unsigned int S[256];
  int i, j = 0, k;
  unsigned char tmp;

  for (i=0; i<256; i++)
    S[i] = i;
  for (i=0; i<256; i++) {
    j = (j + S[i] + key[i % keylength]) % 256;
    tmp = S[i];
    S[i] = S[j];
    S[j] = tmp;
  }

  i = j = k = 0;
  while (k < textlength) {
    i = (i + 1) % 256;
    j = (j + S[i]) % 256;
    tmp = S[i];
    S[i] = S[j];
    S[j] = tmp;
    keystream[k++] = S[(S[i] + S[j]) % 256];
  }
}

void encrypt(int textlength, unsigned char plaintext[], 
	     unsigned char keystream[],
	     unsigned char ciphertext[])
{
  int i;
  for (i=0; i<textlength; i++)
    ciphertext[i] = keystream[i] ^ plaintext[i];
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Please use one and only one filename as a parameter\n");
    return 0;
  }

  unsigned char plaintext[MAX_CHAR_LEN];
  unsigned char key[32];
  unsigned char keystream[MAX_CHAR_LEN];
  unsigned char ciphertext[MAX_CHAR_LEN];
  unsigned c;
  int i = 0, textlength, keylength;
  FILE *fp;

  if ((fp = fopen(*++argv, "r")) == NULL) {
    printf("file \"%s\" not found!\n", *argv);
    return 0;
  }

  while ((c = getc(fp)) != EOF)
    plaintext[i++] = c;
  textlength = i;
  fclose(fp);

  /* input a key */
  printf("passwd: ");
  for (i=0; (c = getchar())!='\n'; i++)
    key[i] = c;
  key[i] = '\0';
  keylength = i;

  /* use key to generate a keystream */
  produceKeystream(textlength, key, keylength, keystream);

  /* use the keystream and plaintext to generate ciphertext */
  encrypt(textlength, plaintext, keystream, ciphertext);

  fp = fopen(*argv, "w");
  for (int i=0; i<textlength; i++)
    putc(ciphertext[i], fp);
  fclose(fp);
  return 0;
}
