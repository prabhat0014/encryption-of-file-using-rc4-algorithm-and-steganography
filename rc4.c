#include <openssl/rand.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
char *xorTest(char *buf, int bytesRead)
{
    for (int i = 0; i < bytesRead; i++)
    {
        buf[i] = buf[i] ^ 0x0011;
    }
    return buf;
}
void main(int argc, char *argv[])
{
    const EVP_CIPHER *type = EVP_rc4();
    EVP_CIPHER_CTX *ctx;
    const EVP_MD *digest = EVP_sha256();
    int infd, outfd, bytesRead, bytesWritten, keyBytes, evpBytesRead;
    unsigned char buf[4096];
    unsigned char sBuf[8];
    const char magic[] = "Salted__";
    const char salt[] = "12345678";
    unsigned char salt2[8];
    bool eof = false;
    unsigned char hexkey[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_IV_LENGTH];
    unsigned char returnBuf[4096 + EVP_MAX_BLOCK_LENGTH];
    if (argc != 5)
    {
        printf("not enough arguments\nUsage:./rc4standalone.out <password> <infile> <outfile><e/d>\n");
        return;
    }
    const unsigned char *pw = argv[1];
    char *inpath = argv[2];
    char *outpath = argv[3];
    int tool;
    int pwLength = strlen(pw);
    printf("entered pw : %s\tlength : %d\n", pw, pwLength);
    RAND_bytes(salt2, sizeof(salt2));
    if (strcmp(argv[4], "e") == 0)
    {
        tool = 1;
        printf("set to encrypt.\n");
    }
    else
    {
        tool = 0;
        printf("set to decrypt.\n");
    }
    if ((infd = open(inpath, O_RDWR)) == -1)
    {
        printf("failed to open file.\n");
        return;
    }
    if ((outfd = creat(outpath, S_IRWXU)) == -1)
    {
        printf("outfile creation failed.\n");
        return;
    }
    if (tool == 1)
    {
        write(outfd, magic, 8);
        write(outfd, salt2, 8);
        if ((keyBytes = EVP_BytesToKey(type, digest, salt2, pw, pwLength, 1, hexkey, NULL)) == 0) //salt2
        {
            printf("EVP_BytesToKey has failed.\n");
            return;
        }
    }
    else if (tool == 0)
    {
        lseek(infd, 8, SEEK_SET);
        if ((read(infd, sBuf, 8)) == -1)
        {
            printf("Failed to read salt from encrypted file.");
        }
        printf("Salt from encrypted file: %s\n", sBuf);
        if ((keyBytes = EVP_BytesToKey(type, digest, (const unsigned char *)sBuf, pw, pwLength, 1, hexkey, NULL)) == 0)
        {
            printf("EVP_BytesToKey has failed.\n");
            return;
        }
    }
    printf("keyBytes: %d\n", keyBytes);
    if (keyBytes > 0)
    {
        printf("key=");
        for (int i = 0; i < keyBytes; i++)
            printf("%02X", hexkey[i]);
        printf("\n");
    }
    ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit_ex(ctx, EVP_rc4(), NULL, hexkey, NULL, tool);
    printf("Preparing to read bytes...\n");
    while (eof == false)
    {
        if ((bytesRead = read(infd, buf, 4096)) == -1)
        {
            printf("failed to read file.\n");
            return;
        }
        else if (bytesRead <= 0)
        {
            eof = true;
        }
        printf("bytesRead: %d\n", bytesRead);
        if (eof == false)
        {
            EVP_CipherUpdate(ctx, returnBuf, &evpBytesRead, buf, bytesRead);
        }
        else if (eof == true)
        {
            EVP_CipherFinal_ex(ctx, returnBuf, &evpBytesRead);
            printf("Finalizing...\n");
        }
        if ((bytesWritten = write(outfd, returnBuf, evpBytesRead)) == -1)
        {
            printf("failed to write to file\n");
            return;
        }
    }
    close(infd);
    close(outfd);
