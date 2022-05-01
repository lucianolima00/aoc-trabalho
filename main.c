#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char memoria[154];
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
    if (strcmp(acumulador, "hlt") == 0) {
        return 0;
    } else if (strcmp(acumulador, "nop") == 0) {
        return 1;
    } else if (strcmp(acumulador, "add") == 0) {
        return 2;
    } else if (strcmp(acumulador, "sub") == 0) {
        return 3;
    } else if (strcmp(acumulador, "mul") == 0) {
        return 4;
    } else if (strcmp(acumulador, "div") == 0) {
        return 5;
    } else if (strcmp(acumulador, "cmp") == 0) {
        return 6;
    } else if (strcmp(acumulador, "movr") == 0) {
        return 7;
    } else if (strcmp(acumulador, "and") == 0) {
        return 8;
    } else if (strcmp(acumulador, "or") == 0) {
        return 9;
    } else if (strcmp(acumulador, "xor") == 0) {
        return 10;
    } else if (strcmp(acumulador, "not") == 0) {
        return 11;
    } else if (strcmp(acumulador, "je") == 0) {
        return 12;
    } else if (strcmp(acumulador, "jne") == 0) {
        return 13;
    } else if (strcmp(acumulador, "jl") == 0) {
        return 14;
    } else if (strcmp(acumulador, "jle") == 0) {
        return 15;
    } else if (strcmp(acumulador, "jg") == 0) {
        return 16;
    } else if (strcmp(acumulador, "jge") == 0) {
        return 17;
    } else if (strcmp(acumulador, "jmp") == 0) {
        return 18;
    } else if (strcmp(acumulador, "ld") == 0) {
        return 19;
    } else if (strcmp(acumulador, "st") == 0) {
        return 20;
    } else if (strcmp(acumulador, "movi") == 0) {
        return 21;
    } else if (strcmp(acumulador, "addi") == 0) {
        return 22;
    } else if (strcmp(acumulador, "subi") == 0) {
        return 23;
    } else if (strcmp(acumulador, "muli") == 0) {
        return 24;
    } else if (strcmp(acumulador, "divi") == 0) {
        return 25;
    } else if (strcmp(acumulador, "lsh") == 0) {
        return 26;
    } else if (strcmp(acumulador, "rsh") == 0) {
        return 27;
    } else {
        return 0;
    }
}

void memoriaWriter() {
    FILE *arq = NULL;
    char line[999];
    size_t len = 0;
    char l;

    arq = fopen("example.txt", "rt");
    if (arq == NULL) {
        printf("File not found \n");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(line, sizeof(line), arq) != NULL) {
        char instrucao[12];
        int flag = 1;
        char *token;
        char dadoInstrucao[5];
        unsigned int posMemo;
        unsigned int dados;
        unsigned int opcode;
        unsigned int dadosPos;
        unsigned int acumuladorHex;

        printf("linha: %s\n", line);

        token = strtok(line, ";");
        
        posMemo = strtol(token, NULL, 16);
        printf("%x\n", posMemo);

        token = strtok(NULL, ";");

        printf("%s\n", token);

        if (strcmp(token, "d") == 0) { // Grava se for dado
            token = strtok(NULL, ";");
            dados = strtol(token, NULL, 16);
            printf("dados: %x\n", dados);

            flag = 6;
        } else if (strcmp(token, "i") == 0) {
            token = strtok(NULL, ";");
            strcpy(instrucao, token);
            token = strtok(instrucao, " ");

            // converte opcode
            opcode = converter_opcode(token);
            dados = opcode << 24;
            token = strtok(NULL, " ");
            printf("opcode %i\n", opcode);
            if (opcode != 0){
                // converte reg0
                strcpy(dadoInstrucao, token);
                dadoInstrucao[strlen(token)-1] = '\0';

                acumuladorHex = strtol(&dadoInstrucao[1], NULL, 16) << 21;
                dados = dados | acumuladorHex;
                
                token = strtok(NULL, " ");
                // converte reg1 ou IMM e MAR
                strcpy(dadoInstrucao, token);
                printf("instrucao %s\n", dadoInstrucao);

                if (dadoInstrucao[0] == 'r') {
                    acumuladorHex = strtol(&dadoInstrucao[1], NULL, 16) << 18;
                } else {
                    acumuladorHex = strtol(dadoInstrucao, NULL, 16);
                }
                dados = dados | acumuladorHex;
                printf("token: %s\n", token);
                printf("dados: %x\n", dados);
            }
        }

        /*

        for(int i = 0 ;  i <= strlen(line); i++){
            if(line[i] != ';' && line[i] != '\r' && line[i] != '\n' && line[i] != ' ' && line[i] != ','){
                strncat(acumulador, &line[i], 1);
            } else if (strcmp(acumulador, "") != 0) {
                if (flag == 1) { // Posição na memoriat                    
                    posMemo = strtol(acumulador, NULL, 16);
                    flag = 2;
                    strcpy(acumulador, "");
                } else if (flag == 2) { // É dados ou instrução
                    dadoInstrucao = acumulador[0];
                    flag = 3;
                    strcpy(acumulador, "");
                } else if (flag == 3) {
                    if (dadoInstrucao == 'd') { // Grava se for dado
                        dados = strtol(acumulador, NULL, 16);

                        flag = 6;
                    } else if (dadoInstrucao == 'i') { // converte opcode
                        dados = converter_opcode(acumulador);
                        dados = dados << 24;

                        flag = 4;
                    }

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
            flag = 6;
            strcpy(acumulador, "");
        }*/

        if (flag == 6) {
            dadosPos = dados;
            memoria[posMemo] = dadosPos >> 24;
            dadosPos = dados & 0x00FF0000;
            memoria[posMemo+1] = dadosPos >> 16;
            dadosPos = dados & 0x0000FF00;
            memoria[posMemo+2] = dadosPos >> 8;
            dadosPos = dados & 0x000000FF;
            memoria[posMemo+3] = dadosPos;
            flag = 0;
        }
    }

    fclose(arq);
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

    /*
    while (1) {
        busca();
        decodifica();
        executa();

        amostragem();
        pc += 4;
        printf("Pressione uma tecla para iniciar o próximo ciclo de máquina ou aperte CTRL+C para finalizar a execução do trabalho.\n");  
        getchar();    
    }*/


    return 0;
}