# Encryption of File using RC4 algorithm & Steganography
The main aim of this project is to provide a better security to the file that
consists of sensitive information. This algorithm provides better security than
that provided by existing file encryption algorithms.

### STEPS
- Encrypting a file
- Storing the cipher text in a bitmap
- Encrypting the cipher text that is to be stored in the bitmap image
- Decrypting the text that is stored in the bitmap image
- Getting the cipher text back from the bitmap
- decrypting the cipher text

Even if a cryptanalyst tries to attack the algorithm, he/she should decipher the
encrypted bitmap image first and then finding the cipher text of the encrypted file
and then deciphering that text. As this is too tough to achieve, the encrypted file is
secured to a better extent.

### EXECUTION
You need to have linux terminal to execute this code using the following commands <br>
gcc -o a rc4.c -L/local/lib/ -lssl -lcrypto <br>
gcc -b b steganography.c <br>

ENCRYPTION <br>
./a 'password' 'plaintext' 'cipher to bit file' e <br>
./b -e 'input bitmap' 'output bitmap' 'cipher to bit file' <br>

DECRYPTION <br>
./b -d 'output bitmap' 'intermediate bitmap' 'bit to cipher file' <br>
./a 'password' 'bit to cipher file' 'output file' d

EXAMPLE: <br>
gcc -o a rc4.c -L/local/lib/ -lssl -lcrypto <br>
gcc -b b steganography.c <br>
./a pass plaintext.txt ciphertobit.txt e <br>
./b -e input.bmp output.bmp ciphertobit.txt <br>
./b -d output.bmp intermediate.bmp bittocipher.txt <br>
./a pass bittocipher.txt output.txt d <br>

### CONCLUSION
This project provides confidential communication and secret data storing and protection
of data alteration and access control system for digital content distribution and
media databases. <br>
It also provides potential capability to hide the existence of confidential data and
hardness of detecting the hidden data and also enhance the secrecy of the encrypted data.
