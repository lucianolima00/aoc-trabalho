#include <stdio.h>

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
        case: 0x03:
            reg[ro0] -= reg[ro1];
            break;
        //mul - 0b0000 0100
        case: 0x04:
            reg[ro0] *= reg[ro1];
            break;
        //div - 0b0000 0101
        case: 0x05:
            reg[ro0] /= reg[ro1];
        //cmp = 0b0000 0110
        case: 0x06:
            (reg[ro0] == reg[ro1]) ? e = 1 : e = 0;

            (reg[ro0] < reg[ro1]) ? l = 1 : l = 0;

            (reg[ro0] < reg[ro1]) ? g = 1 : g = 0;

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
            if(l == 1 || e = 1)
                pc = mar;
            break;
        // jg - 0b0001 0000
        case 0x10:
            if(g == 1)
                pc = ro0;
            break;
        // jge - 0b0001 0001
        case 0x11:
            if(e == 1 || g = 1)
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
        case 0x17
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

int main() {
    memoria[0] = 0x13;
    memoria[1] = 0x00;
    memoria[2] = 0x00;
    memoria[3] = 0x1E;
    memoria[4] = 0x13;
    memoria[5] = 0x20;
    memoria[6] = 0x00;
    memoria[7] = 0x24;
    busca();
    pc += 4;
    printf("%x\n", mbr);
    printf("%x\n", mar);
    printf("%x\n", pc);
    decodifica();
    printf("%x\n", mbr);
    printf("%x\n", ir);
    printf("%x\n", mar);
    printf("%x\n", imm);
    printf("%x\n", ro0);
    printf("%x\n", ro1);
    pc += 4;


    return 0;
}