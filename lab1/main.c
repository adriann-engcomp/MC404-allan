
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

void write(int __fd, const void *__buf, int __n){
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

void exit(int code){
  __asm__ __volatile__(
    "mv a0, %0           # return code\n"
    "li a7, 93           # syscall exit (64) \n"
    "ecall"
    :   // Output list
    :"r"(code)    // Input list
    : "a0", "a7"
  );
}

void _start()
{
  int ret_code = main();
  exit(ret_code);
}

#define STDIN_FD  0
#define STDOUT_FD 1

/* Aloca um vetor com 8 bytes.*/
char vetor[8];

  int main(){
  /* Lê uma string da entrada padrão */
  int num = read(STDIN_FD, (void*) vetor, 8);

  // Converto char do vetor para int
  // num1
  int num1 = (vetor[0] - '0') * 10 + vetor[1] - '0';
  // espaco
  char op = vetor[3];
  // espaço
  int num2 = (vetor[5] - '0') * 10 + vetor[6] - '0';

  // resultado
  int resultado = 0;

  // depois defino as 4 operacoes entre num1 e num2
  if (op == '+'){
    resultado = num1 + num2;
  }
  else if (op == '-'){
    resultado = num1 - num2;
  }
  else if (op == '*'){
    resultado = num1 * num2;
  }
  else if(op == '/'){
    resultado = num1 / num2;
  }

  // Converto int para char que ficara no vetor
  vetor[0] = (resultado / 10000) % 10 + '0';
  vetor[1] = (resultado / 1000) % 10 + '0';
  vetor[2] = (resultado / 100) % 10 + '0';
  vetor[3] = (resultado / 10) % 10 + '0';
  vetor[4] = resultado % 10 + '0';
  vetor[5] = '\n';

  // Imprime o vetor com tamanho 6 que contem a operacao entre 2 num
  write(STDOUT_FD, (void*) vetor, 6);

  return 0;
  }