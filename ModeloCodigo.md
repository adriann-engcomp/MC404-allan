#define STDIN_FD  0
#define STDOUT_FD 1

int read(int __fd, const void *__buf, int __n) {

    int ret_val;

    __asm__ __volatile__(
        "mv a0, %1\n"
        "mv a1, %2\n"
        "mv a2, %3\n"
        "li a7, 63\n"
        "ecall\n"
        "mv %0, a0\n"

        : "=r"(ret_val)
        : "r"(__fd), "r"(__buf), "r"(__n)
        : "a0", "a1", "a2", "a7"
    );

    return ret_val;
}

void write(int __fd, const void *__buf, int __n) {

    __asm__ __volatile__(
        "mv a0, %0\n"
        "mv a1, %1\n"
        "mv a2, %2\n"
        "li a7, 64\n"
        "ecall\n"

        :
        : "r"(__fd), "r"(__buf), "r"(__n)
        : "a0", "a1", "a2", "a7"
    );
}

void exit(int code) {

    __asm__ __volatile__(
        "mv a0, %0\n"
        "li a7, 93\n"
        "ecall\n"

        :
        : "r"(code)
        : "a0", "a7"
    );
}

int main() {

    char entrada[33];

    read(STDIN_FD, entrada, 33);

    // Sinal
    int sinal;

    if (entrada[0] == '0') {
        sinal = 1;
    }
    else {
        sinal = -1;
    }

    // Expoente
    int exp = 0;
    int i;

    for (i = 1; i <= 8; i++) {

        exp = exp * 2;

        if (entrada[i] == '1') {
            exp = exp + 1;
        }
    }

    exp = exp - 127;

    // Mantissa
    int valormant;

    if (exp < 0) {
        valormant = 0;
    }
    else {

        valormant = 1;

        for (i = 9; i < 9 + exp; i++) {

            valormant = valormant * 2;

            if (entrada[i] == '1') {
                valormant = valormant + 1;
            }
        }
    }

    valormant = valormant * sinal;


    // Decimal para binario
    int numero;

    if (valormant < 0) {
        numero = -valormant;
    }
    else {
        numero = valormant;
    }

    int bin[32];

    for (i = 31; i >= 0; i--) {
        bin[i] = numero % 2;
        numero = numero / 2;
    }


    // Complemento de 2
    if (valormant < 0) {

        for (i = 0; i < 32; i++) {

            if (bin[i] == 0) {
                bin[i] = 1;
            }
            else {
                bin[i] = 0;
            }
        }

        for (i = 31; i >= 0; i--) {

            if (bin[i] == 0) {
                bin[i] = 1;
                break;
            }
            else {
                bin[i] = 0;
            }
        }
    }


    // Saida decimal
    char saida_decimal[12];
    char temp[12];

    int pos = 0;
    int tam = 0;
    numero = valormant;

    if (numero < 0) {
        saida_decimal[pos] = '-';
        pos++;
        numero = -numero;
    }

    if (numero == 0) {
        saida_decimal[pos] = '0';
        pos++;
    }
    else {

        while (numero > 0) {
            temp[tam] = (numero % 10) + '0';
            numero = numero / 10;
            tam++;
        }

        for (i = tam - 1; i >= 0; i--) {
            saida_decimal[pos] = temp[i];
            pos++;
        }
    }

    saida_decimal[pos] = '\n';
    pos++;

    int tam_decimal = pos;


    // Binario big endian
    char bin_big[35];

    bin_big[0] = '0';
    bin_big[1] = 'b';

    for (i = 0; i < 32; i++) {
        bin_big[i + 2] = bin[i] + '0';
    }

    bin_big[34] = '\n';


    // Binario little endian
    char bin_little[35];

    bin_little[0] = '0';
    bin_little[1] = 'b';

    pos = 2;

    int byte;

    for (byte = 3; byte >= 0; byte--) {

        for (i = byte * 8; i < byte * 8 + 8; i++) {
            bin_little[pos] = bin[i] + '0';
            pos++;
        }
    }

    bin_little[34] = '\n';


    // Binario para hexadecimal
    char hex[8];

    int grupo;
    int j;

    for (i = 0; i < 8; i++) {

        grupo = 0;

        for (j = 0; j < 4; j++) {
            grupo = grupo * 2;
            grupo = grupo + bin[i * 4 + j];
        }

        if (grupo < 10) {
            hex[i] = grupo + '0';
        }
        else {
            hex[i] = (grupo - 10) + 'a';
        }
    }


    // Hexadecimal big endian
    char hex_big[11];

    hex_big[0] = '0';
    hex_big[1] = 'x';

    for (i = 0; i < 8; i++) {
        hex_big[i + 2] = hex[i];
    }

    hex_big[10] = '\n';


    // Hexadecimal little endian
    char hex_little[11];

    hex_little[0] = '0';
    hex_little[1] = 'x';

    pos = 2;

    for (byte = 3; byte >= 0; byte--) {

        hex_little[pos] = hex[byte * 2];
        hex_little[pos + 1] = hex[byte * 2 + 1];

        pos = pos + 2;
    }

    hex_little[10] = '\n';


    // Prints no final
    write(STDOUT_FD, saida_decimal, tam_decimal);
    write(STDOUT_FD, bin_big, 35);
    write(STDOUT_FD, bin_little, 35);
    write(STDOUT_FD, hex_big, 11);
    write(STDOUT_FD, hex_little, 11);

    return 0;
}

void _start() {

    int ret_code = main();

    exit(ret_code);
}
