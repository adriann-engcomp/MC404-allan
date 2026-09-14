#include <stdio.h>

int main() {

    char entrada[33];

    int sinal;
    int exp = 0;
    int valormant = 0;
    int i;

    scanf("%32s", entrada);


    /* =========================
        PARTE 1
        IEEE-754 -> INTEIRO
       ========================= */


    // 1) Sinal
    if (entrada[0] == '0') {
        sinal = 1;
    }
    else {
        sinal = -1;
    }


    // 2) Expoente
    for (i = 1; i <= 8; i++) {

        exp = exp * 2;

        if (entrada[i] == '1') {
            exp = exp + 1;
        }
    }

    // Retira o bias
    exp = exp - 127;


    // 3) Mantissa
    if (exp < 0) {
        valormant = 0;
    }
    else {
        // 1 implícito
        valormant = 1;
        /*
           Constrói o número binário da parte inteira.

           Se ainda existirem bits da mantissa,
           usa entrada[9 + i].

           Se exp for maior que 23, os bits restantes
           são zeros.
        */
        for (i = 0; i < exp; i++) {

            valormant = valormant * 2;

            if (i < 23 && entrada[9 + i] == '1') {
                valormant = valormant + 1;
            }
        }
    }
    // Aplica o sinal
    valormant = valormant * sinal;


    printf("Valor inteiro = %d\n", valormant);



    /* =========================
       PARTE 2
       CONVERSOES
       ========================= */


    // Vetor que armazenará os 32 bits
    int binario[32];

    int numero;


    // Trabalhamos primeiro com o módulo do número
    if (valormant < 0) {
        numero = -valormant;
    }
    else {
        numero = valormant;
    }


    /* -------------------------
       1) BASE 10 -> BASE 2
       ------------------------- */

    for (i = 31; i >= 0; i--) {

        binario[i] = numero % 2;

        numero = numero / 2;
    }


    /* -------------------------
       2) COMPLEMENTO DE 2
       se o número for negativo
       ------------------------- */

    if (valormant < 0) {

        // Complemento de 1:
        // inverte todos os bits
        for (i = 0; i < 32; i++) {

            if (binario[i] == 0) {
                binario[i] = 1;
            }
            else {
                binario[i] = 0;
            }
        }


        // Soma 1
        int vai_um = 1;

        for (i = 31; i >= 0; i--) {

            if (vai_um == 1) {

                if (binario[i] == 0) {

                    binario[i] = 1;
                    vai_um = 0;

                }
                else {

                    binario[i] = 0;

                }
            }
        }
    }



    /* -------------------------
       3) BINÁRIO BIG-ENDIAN
       ------------------------- */

    printf("Binario Big-endian    = 0b");

    for (i = 0; i < 32; i++) {
        printf("%d", binario[i]);
    }

    printf("\n");



    /* -------------------------
       4) BINÁRIO LITTLE-ENDIAN
       ------------------------- */

    printf("Binario Little-endian = 0b");

    /*
       Big-endian:

       byte 0 | byte 1 | byte 2 | byte 3

       Little-endian:

       byte 3 | byte 2 | byte 1 | byte 0
    */

    int byte;

    for (byte = 3; byte >= 0; byte--) {

        for (i = byte * 8; i < byte * 8 + 8; i++) {

            printf("%d", binario[i]);

        }
    }

    printf("\n");



    /* =========================
       BASE 2 -> BASE 16
       ========================= */

    char hexadecimal[8];

    int grupo;
    int j;


    /*
       Cada 4 bits formam
       um dígito hexadecimal.
    */

    for (i = 0; i < 8; i++) {

        grupo = 0;

        for (j = 0; j < 4; j++) {

            grupo = grupo * 2;

            if (binario[i * 4 + j] == 1) {
                grupo = grupo + 1;
            }
        }


        /*
           0 até 9:
           vira '0' até '9'

           10 até 15:
           vira 'a' até 'f'
        */

        if (grupo < 10) {

            hexadecimal[i] = '0' + grupo;

        }
        else {

            hexadecimal[i] = 'a' + (grupo - 10);

        }
    }



    /* -------------------------
       5) HEXADECIMAL BIG-ENDIAN
       ------------------------- */

    printf("Hexadecimal Big-endian    = 0x");

    for (i = 0; i < 8; i++) {
        printf("%c", hexadecimal[i]);
    }

    printf("\n");



    /* -------------------------
       6) HEXADECIMAL LITTLE-ENDIAN
       ------------------------- */

    printf("Hexadecimal Little-endian = 0x");

    /*
       Cada byte hexadecimal possui
       dois caracteres.

       Big:
       01 | 23 | 45 | 67

       Little:
       67 | 45 | 23 | 01
    */

    for (byte = 3; byte >= 0; byte--) {

        printf("%c", hexadecimal[byte * 2]);
        printf("%c", hexadecimal[byte * 2 + 1]);

    }

    printf("\n");


    return 0;
}