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

        char entrada[33];
        read(STDIN_FD, entrada, 33);

        // Parte 1 do problema

        // Sinal (bit MSB)
        int sinal;
        if(entrada[0] == '0'){
            sinal = 1;
        } else {
            sinal = -1;
        }

        // Exp (expmax - 127)
        int exp = 0;
        int i;
        for (i = 1; i <= 8; i++){
            exp = exp * 2;
            if (entrada[i] == '1'){
                exp = exp + 1;
            }
        }
        exp = exp - 127;

        // Mantissa
        int valormant;
        if (exp < 0){
            // não considero valores negativos
            valormant = 0;
        }
        else{
            // 1 MSB
            valormant = 1;
            for (i = 9; i < 9 + exp; i++){
                valormant = valormant * 2;
                
                if (entrada[i] == '1'){
                    valormant = valormant + 1;
                }
            }
        }
        valormant = valormant * sinal;
    
        // Parte 2: Conversoes

        int numero;
        if (valormant < 0){
            numero = -valormant;
        }
        else{
            numero = valormant;
        }

        // B10 -> b2 
        // saindo do lsb para msb
        int bin[32];
        for (i = 31; i >= 0; i--){
            bin[i] = numero % 2;
            numero = numero / 2;
        }

        // Complemento de 2 = c1 + 1
        // c1
        if (valormant < 0){
            for (i = 0; i < 32; i++){
                if (bin[i] == 0){
                    bin[i] = 1;
                }
                else{
                    bin[i] = 0;
                }
            }
        }
        // Somo 1
        for (i = 31; i >= 0; i--){
            if (bin[i] == 0){
                bin[i] = 1;
                break;
            }
            else{
                bin[i] = 0;
            }
        }

        // Saida decimal
        char SaidaDecimal[12];
        char temp[12];

        int pos = 0;
        int tam = 0;
        numero = valormant;

        if (numero < 0){
            SaidaDecimal[pos] = '-';
            pos++;
            numero = -numero;
        }
        if (numero == 0){
            SaidaDecimal[pos] = '0';
            pos++;
        }
        else{
            while (numero > 0){
                temp[tam] = (numero % 10) + '0';
                numero = numero / 10;
                tam++;
            }
            for (i= tam -1; i >= 0; i--){
                SaidaDecimal[pos] = temp[i];
                pos++;
            }
        }

        SaidaDecimal[pos] = '\n';
        pos++;

        int TamDecimal = pos;

        // Binário big endian
        char BinBig[35];
        BinBig[0] = '0';
        BinBig[1] = 'b';
        BinBig[35] = '\n';

        for (i = 0; i < 32; i++){
            BinBig[i+2] = bin[i] + '0';
        }

        // Binário little endian
        char BinLittle[35];
        BinLittle[0] = '0';
        BinLittle[1] = 'b';
        BinLittle[35] = '\n';
        pos = 2;
        int byte;

        for (byte = 3; byte >= 0; byte--){
            for (i = byte * 8; i < byte * 8 + 8; i++){
                bin_little[pos] = bin[i] + 
            }
        }

    return 0;
}































void _start()
    {
    int ret_code = main();
    exit(ret_code);
    }