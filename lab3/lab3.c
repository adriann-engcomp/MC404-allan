#define STDIN_FD  0
#define STDOUT_FD 1

int read(int __fd, const void *__buf, int __n){
    int ret_val;
    __asm__ __volatile__(
    "mv a0, %1           # file descriptor\n"
    "mv a1, %2           # buffer \n"
    "mv a2, %3           # size \n"
    "li a7, 63           # syscall write code (63) \n"
    "ecall               # invoke syscall \n"
    "mv %0, a0           # move return value to ret_val\n"
    : "=r"(ret_val)  // Output list
    : "r"(__fd), "r"(__buf), "r"(__n)    // Input list
    : "a0", "a1", "a2", "a7"
);
    return ret_val;
}

void write(int __fd, const void *__buf, int __n)
{
    __asm__ __volatile__(
    "mv a0, %0           # file descriptor\n"
    "mv a1, %1           # buffer \n"
    "mv a2, %2           # size \n"
    "li a7, 64           # syscall write (64) \n"
    "ecall"
    :   // Output list
    :"r"(__fd), "r"(__buf), "r"(__n)    // Input list
    : "a0", "a1", "a2", "a7"
    );
}

void exit(int code)
{
    __asm__ __volatile__(
    "mv a0, %0           # return code\n"
    "li a7, 93           # syscall exit (64) \n"
    "ecall"
    :   // Output list
    :"r"(code)    // Input list
    : "a0", "a7"
    );
}

int main(){

    // Defino o tam da entrada e leio
    char entrada[33];
    read(STDIN_FD, entrada, 33);

    // Defino o sinal
    int sinal;

    if (entrada[0] == '0') {
        sinal = 1;
    }
    else {
        sinal = -1;
    }

    // Defino o expoente
    int exp = 0;
    int i;

    for (i = 1; i <= 8; i++) {

        exp = exp * 2;

        if (entrada[i] == '1') {
            exp = exp + 1;
        }
    }

    exp = exp - 127;

    // Defino a Mantissa
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

    // Imprimo como char
    char print_int[12]; // Pois, o menor int de 32 bits como char é: -2147483648\n
    char temp[12];

    int pos = 0;
    int tam = 0;
    numero = valormant;

    if (numero < 0) {
        print_int[pos] = '-';
        pos++;
        numero = -numero;
    }
    if (numero == 0) {
        print_int[pos] = '0';
        pos++;
    }
    else {
        while (numero > 0) {
            temp[tam] = (numero % 10) + '0';
            numero = numero / 10;
            tam++;
        }
        for (i = tam - 1; i >= 0; i--) {
            print_int[pos] = temp[i];
            pos++;
        }
    }

    print_int[pos] = '\n';
    pos++;
    int tam_int = pos;
    write(STDOUT_FD, print_int, tam_int);

    // Converto de int para b2
    int numero;
    if (valormant < 0) {
        numero = -valormant;
    }
    else {
        numero = valormant;
    }

    int bin[32];
    // Fixo
    bin[0] = '0';
    bin[1] = 'b';
    bin[31] = '\n';

    for (i = 31; i >= 0; i--) {
        bin[i] = numero % 2;
        numero = numero / 2;
    }

    // Caso numero seja negativo, faço o complemento de 2
    if (valormant < 0) {
        // Inverto os bits
        for (i = 0; i < 32; i++) {
            if (bin[i] == 0) {
                bin[i] = 1;
            }
            else {
                bin[i] = 0;
            }
        }
        // Somo 1
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

    // Binario big endian
    char bin_big[35];

    bin_big[0] = '0';
    bin_big[1] = 'b';
    bin_big[34] = '\n';

    for (i = 0; i < 32; i++) {
        bin_big[i + 2] = bin[i] + '0';
    }
    write(STDOUT_FD, bin_big, 35);

    // Binario little endian
    char bin_lit[35];

    bin_lit[0] = '0';
    bin_lit[1] = 'b';
    bin_lit[34] = '\n';

    pos = 2;
    int byte;

    for (byte = 3; byte >= 0; byte--) {
        for (i = byte * 8; i < byte * 8 + 8; i++) {
            bin_lit[pos] = bin[i] + '0';
            pos++;
        }
    }
    write(STDOUT_FD, bin_lit, 35);

    // Converto de binario para hexadecimal
    char hex[8];
    int grupo_de_4;
    int j;

    for (i = 0; i < 8; i++) {
        grupo_de_4 = 0;
        for (j = 0; j < 4; j++) {
            grupo_de_4 = grupo_de_4 * 2;
            grupo_de_4 = grupo_de_4 + bin[i * 4 + j];
        }
        if (grupo_de_4 < 10) {
            hex[i] = grupo_de_4 + '0';
        }
        else {
            hex[i] = (grupo_de_4 - 10) + 'a';
        }
    }

    // Hexadecimal big endian
    char hex_big[11];

    hex_big[0] = '0';
    hex_big[1] = 'x';
    hex_big[10] = '\n';

    for (i = 0; i < 8; i++) {
        hex_big[i + 2] = hex[i];
    }
    write(STDOUT_FD, hex_big, 11);

    // Hexadecimal little endian
    char hex_lit[11];
    hex_lit[0] = '0';
    hex_lit[1] = 'x';
    hex_lit[10] = '\n';

    pos = 2;
    for (byte = 3; byte >= 0; byte--) {
        hex_lit[pos] = hex[byte * 2];
        hex_lit[pos + 1] = hex[byte * 2 + 1];
        pos = pos + 2;
    }
    write(STDOUT_FD, hex_lit, 11);

    return 0;
}

void _start()
{
    int ret_code = main();
    exit(ret_code);
}