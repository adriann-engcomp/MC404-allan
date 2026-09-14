/* Programa que retorna a resposta para a Grande Pergunta sobre a 
 * Vida, o Universo e Tudo o Mais */

// 1. Compilo:
// clang -S prog.c -o prog.s para asm da maquina;
// clang-15 --target=riscv32 -march=rv32g -mabi=ilp32d -mno-relax prog.c -S -o prog.s para riscv-32

// 2. Monto:
// as prog.s -o prog.o
// clang-15 --target=riscv32 -march=rv32g -mabi=ilp32d -mno-relax prog.s -c -o prog.o

// 3. Ligador:
// Quando tenho que ligar o bibliotecas (modulos) com o .x
// ld.lld prog.o module1.o module2.o -o prog.x
// Quando tenho so o .x:
// ld.lld prog.o -o prog.x

// 4. Desmonto:
// llvm-objdump -D prog.o

// 5. Makefile = 1+2+3+4


int main(void) {
    return 42;
}