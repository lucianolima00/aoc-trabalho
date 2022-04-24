#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char memoria[154];
unsigned int mbr,
             mar,
             imm,
             pc = 0,
             reg[8];

unsigned char ir,
              ro0,
              ro1,
              e,
              l,
              g;

void busca() {
    mbr = memoria[mar++];
    mbr = mbr << 8;
    mbr = mbr | memoria[mar++];
    mbr = mbr << 8;
    mbr = mbr | memoria[mar++];
    mbr = mbr << 8;
    mbr = mbr | memoria[mar++];
}

void decodifica() {
    ir = mbr >> 24;
    ro0 = (mbr & 0x00E00000) >> 21;
    ro1 = (mbr & 0x001C0000) >> 18;
    mar = (mbr & 0x001FFFFF);
    imm = (mbr & 0x001FFFFF);
}

void executa() {
    switch (ir) {
        //hlt - 0b0000 0000
        case 0x00:
            pc = 0;
            break;
        //nop - 0x0000 0001
        case 0x01:
            pc++;
            break;
        //add - 0b0000 0010
        case 0x02:
            reg[ro0] += reg[ro1];
            break;
        //sub - 0b0000 0011
        case 0x03:
            reg[ro0] -= reg[ro1];
            break;
        //mul - 0b0000 0100
        case 0x04:
            reg[ro0] *= reg[ro1];
            break;
        //div - 0b0000 0101
        case 0x05:
            reg[ro0] /= reg[ro1];
        //cmp = 0b0000 0110
        case 0x06:
            e = reg[ro0] == reg[ro1];

            l = reg[ro0] < reg[ro1];

            g = reg[ro0] < reg[ro1];

            break;

        // movr - 0b0000 0111
        case 0x07:
            reg[ro0] = reg[ro1];
            break;

        // and - 0b0000 1000
        case 0x08:
            reg[ro0] = reg[ro0] & reg[ro1];
            break;
        // or - 0b0000 1001
        case 0x09:
            reg[ro0] = reg[ro0] | reg[ro1];
            break;
        // xor - 0b0000 1010
        case 0x0A:
            reg[ro0] = reg[ro0] ^ reg[ro1];
            break;
        // not - 0b0000 1011
        case 0x0B:
            reg[ro0] =! reg[ro1];
            break;
        // je - 0b0000 1100
        case 0x0C:
            if(e == 1)
                pc = mar;
            break;
        // jne - 0b0000 1110
        case 0x0D:
            if(e == 0)
                pc = mar;
            break;
        // jl - 0b0000 1110
        case 0x0E:
            if(l == 1)
                pc = mar;
            break;
        // jle - 0b0000 1111
        case 0x0F:
            if(l == 1 || e == 1)
                pc = mar;
            break;
        // jg - 0b0001 0000
        case 0x10:
            if(g == 1)
                pc = ro0;
            break;
        // jge - 0b0001 0001
        case 0x11:
            if(e == 1 || g == 1)
                pc = mar;
            break;
        // jmp - 0b0001 0010
        case 0x12:
            pc = mar;
            break;
        // ld - 0b0001 0011
        case 0x13:
            busca();
            reg[ro0] = mbr;
            break;
        // st - 0b0001 0100
        case 0x14:
            mbr = reg[ro0];
            memoria[mar++] = mbr & 0xFF000000 >> 24;
            memoria[mar++] = mbr & 0x00FF0000 >> 16;
            memoria[mar++] = mbr & 0x0000FF00 >> 8;
            memoria[mar++] = mbr & 0x000000FF;
            break;
        // movi - 0b0001 0101
        case 0x15:
            reg[ro0] = imm;
            break;
        // addi - 0b0001 0110
        case 0x16:
            reg[ro0] += imm;
            break;
        // subi - 0b0001 0111
        case 0x17:
            reg[ro0] -= imm;
            break;
        // muli - 0b0001 1000
        case 0x18:
            reg[ro0] *= imm;
            break;
        // divi - 0b0001 1001
        case 0x19:
            reg[ro0] /= imm;
            break;
        // lsh - 0b0001 1010
        case 0x1A:
            reg[ro0] = reg[ro0] << imm;
            break;
        // rsh - 0b0001 1011
        case 0x1B:
            reg[ro0] = reg[ro0] >> imm;
            break;
    }
}



unsigned int converter_opcode(char acumulador[]){
    char opcodes[28][4] = {"hlt", "nop", "add", "sub", "mul", "div", "cmp", "movr", "and", "our", "xor", "not", "je", "jne", "jl", "jle", "jg", "jge", "jmp", "ld", "st", "movi", "addi", "subi", "muli", "divi", "lsh", "rsh"};

    for (int i = 0; i < 27; i++) {
        if (strcmp(acumulador, opcodes[i]) == 0) {
            return i;
        }
    }
}

void memoriaWriter() {
    FILE *arq = NULL;
    char linha[100];
    char *line = NULL;
    size_t len = 0;
    char l;

    arq = fopen("example.txt", "rt");
    if (arq == NULL) {
        printf("File not found \n");
        exit(EXIT_FAILURE);
    }

    while ((getline(&line, &len, arq)) != -1){
        char acumulador[10];
        char acumuladorVazio[10];
        int flag = 1;
        char dadoInstrucao;
        unsigned int posMemo;
        unsigned int dados;
        unsigned int acumuladorHex;

        for(int i = 0 ;  i < strlen(line); i++){
            if(line[i] != ';' && line[i] != '\r' && line[i] != ' ' && line[i] != ','){
                strncat(acumulador, &line[i], 1);
            } else if (strcmp(acumulador, "") != 0) {
                if (flag == 1) { // Posição na memoria
                    posMemo = strtol(acumulador, NULL, 16);
                    flag = 2;
                    strcpy(acumulador, "");
                } else if (flag == 2) { // É dados ou instrução
                    dadoInstrucao = acumulador[0];
                    flag = 3;
                    strcpy(acumulador, "");
                } else if (flag == 3) {
                    printf("acumulador: %s dadoInstrucao: %c\n", acumulador, dadoInstrucao);
                    if (dadoInstrucao == 'd') { // Grava se for dado
                        memoria[posMemo] = strtol(acumulador, NULL, 16);
                        flag = 0;
                    }
                    if (dadoInstrucao == 'i') { // converte opcode
                        dados = converter_opcode(acumulador);
                        dados = dados << 24;
                    }

                    flag = 4;
                    strcpy(acumulador, "");
                } else if (flag == 4) { // converte reg
                    acumuladorHex = strtol(&acumulador[1], NULL, 16) << 21;
                    dados = dados | acumuladorHex;

                    flag = 5;
                    strcpy(acumulador, "");
                }
            }
        }

        if (flag == 5) { // converte reg
            if (acumulador[0] == 'r') {
                acumuladorHex = strtol(&acumulador[1], NULL, 16) << 18;
            } else {
                acumuladorHex = strtol(acumulador, NULL, 16);
            }
            dados = dados | acumuladorHex;
            printf("%08x\n", dados);
            flag = 6;
            strcpy(acumulador, "");
        }
    }

    fclose(arq);
    if (line)
        free(line);
}

void amostragem() {
    printf("CPU: \n");
    int r = 0;
    for(int j = 0; j < 5; j++) {
        for (int i = 0; i < 4; i++) {
            char regName[6];
            char text[21];
            char espace[] = " ";

            if (j <= 2) {
                unsigned int regValue;
                if (j <= 1) {
                    regValue = reg[r];
                    sprintf(regName, "R%i:", r++);
                } else if (j == 2) {
                    switch (i) {
                        case 0:
                            sprintf(regName, "MBR:");
                            regValue = mbr;
                            break;
                        case 1:
                            sprintf(regName, "MAR:");
                            regValue = mar;
                            break;
                        case 2:
                            sprintf(regName, "IMM:");
                            regValue = imm;
                            break;
                        case 3:
                            sprintf(regName, "PC:");
                            regValue = pc;
                            break;
                        default:
                            break;
                    }
                }

                int diff = 4 - strlen(regName);

                if (diff > 0) {
                    for (int l = 0; l < diff; l++) {
                        strcat(regName, espace);
                    }
                }

                sprintf(text, "%s 0x%08X", regName, regValue);
            } else {
                unsigned char regValue;

                if (j == 3) {
                    switch (i) {
                        case 0:
                            sprintf(regName, "IR:");
                            regValue = ir;
                            break;
                        case 1:
                            sprintf(regName, "RO0:");
                            regValue = ro0;
                            break;
                        case 2:
                            sprintf(regName, "RO1:");
                            regValue = ro1;
                            break;
                        default:
                            break;
                    }

                    int diff = 4 - strlen(regName);

                    if (diff > 0) {
                        for (int l = 0; l < diff; l++) {
                            strcat(regName, espace);
                        }
                    }

                    if (i == 3) {
                        sprintf(text, " ");
                    } else {
                        sprintf(text, "%s 0x%02X", regName, regValue);
                    }
                } else {

                    switch (i) {
                        case 0:
                            sprintf(regName, "E:");
                            regValue = e;
                            break;
                        case 1:
                            sprintf(regName, "L:");
                            regValue = l;
                            break;
                        case 2:
                            sprintf(regName, "G:");
                            regValue = g;
                            break;
                        default:
                            break;
                    }

                    int diff = 4 - strlen(regName);

                    if (diff > 0) {
                        for (int l = 0; l < diff; l++) {
                            strcat(regName, espace);
                        }
                    }

                    if (i == 3) {
                        sprintf(text, " ");
                    } else {
                        sprintf(text, "%s 0x%X", regName, regValue);
                    }
                }
            }

            int diff = 17 - strlen(text);

            printf("%s", text);
            if (i != 3) {
                if (diff > 0) {
                    for (int l = 0; l < diff; l++) {
                        printf("%s", " ");
                    }
                }
            } else {
                printf("\n");
            }
        }
    }

    printf("\n");

    r = 0;
    printf("Memória: \n");
    for (int i = 0; i < 39; i++) {
        for (int j = 0; j < 4; j++) {
            if (r <= 153) {
                printf("%02X: 0x%02X  ", r, memoria[r]);
                r++;
            }
            if (j == 3) {
                printf("\n");
            }
        }
    }
}

int main() {
    memoriaWriter();

    //busca();

    //decodifica();

    //executa();

    //amostragem();
    pc += 4;


    return 0;
}