#include "Memory.h"
#include "Mips.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <locale.h>
#include <fstream>
#include <conio.h>

using namespace std;
using std::getline;
using std::string;

// Declara��es de objetos.

Memory objMemory; 
Mips objIR;
Mips objOP;
Mips objRS;
Mips objRT;
Mips objRD;
Mips objSHAMT;
Mips objFUNCT;
Mips objPC;
Mips objLINHAS;

int pc = 0;

char line[] = "\n========================================================================= \n";

// enums.

enum opcode

{
    LW = 35, SW = 43
};

enum function

{
    ADD = 32, SUB = 34, MUL = 24, DIV = 26
};

// M�todo da classe Memory.

int Memory::read(int address)

{
    return m_array[address];
}

void Memory::write(int address, int data)

{

    m_array[address] = data;
}

// M�todo para mostrar os valores finais armazenados nos registradores.

void dump_regs()

{
    cout << "Registrador RS: " << objRS.rs << "\nRegistrador RT: " 
    << objRT.rt << "\nRegistrador RD: " << objRD.rd
    << "\nRegistrador funct: " << objFUNCT.funct << "\n" << endl;
}

// M�todo para mostrar o cont�do da mem�ria.

void dump_mem(int start, int end, char format)

{
    int x;
    unsigned long int aux1;
    x = start;
    cout << "\n" << "Posi��o   Conte�do \n\n";
    while(x <= end)
    {
        switch(format)
        {
            case 'd':
            case 'D':
                 aux1 = objMemory.read(start);
                 printf("   %.3ld -- %ld \n", start, aux1);
                 break;
            case 'h':
            case 'H':
                 aux1 = objMemory.read(start);
                 printf("   %.3ld -- %X \n", start, aux1);
                 break;
            default:
                 cout << line 
                 << "ATEN��O!!! \nVoc� digitou uma op��o n�o compat�vel. "
                 << "O programa sera encerrado! \n" << endl;
                 getch();
                 exit(0);
        }
        start++;
        x++;
    }
    cout << "\n";
}

// M�todos de execu��o das instru��es (palavras).

void execute()

{
    unsigned long int funcao, opcode=0, s0=0, s1=0, s2=0, t0=0, t1=0, t2=0;
    unsigned long int i, HI, LO, C;
    
    opcode = strtoul(objOP.op, NULL, 2);
    funcao = strtoul(objFUNCT.funct, NULL, 2);
    
    switch(opcode)
    {
        case SW:
             t0 = strtoul(objRS.rs, NULL, 2);
             C = strtoul(objRD.rd, NULL, 2);
             objMemory.write(C, t0);
             itoa(t0, objRD.rd, 2);
             break;
             
        case 0:
             switch(funcao)
             {
                 case ADD: //Fun��o de adi��o.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      s0 = s2 + s1;
                      itoa(s0, objRD.rd, 2);
                      break;
             
                 case SUB: //Fun��o de subtra��o.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      s0 = s2 - s1;
                      itoa(s0, objRD.rd, 2); 
                      break;
             
                 case MUL: //Fun��o de multiplica��o.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      i = 1;
                      t0 = s2;
                      while(i < s1)
                      {
                          t0 = t0 + s2;
                          i++;
                      }
                      itoa(t0, objRD.rd, 2);
                      break;
             
                 case DIV: //Fun��o de divis�o.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      t0 = s2;
                      HI=1;
                      while(1)
                      {
                          t0-=s1;
                          if(t0 <= 1)
                              break;
                          HI++;
                      }
                      s0 = HI;
                      itoa(s0, objRD.rd, 2);
                      break;
             }
    }
}

/* M�todo auxiliar para complementar as instru��es convertidas de DEC para BIN 
com 'zeros' a esquerda. */

void complemento(int quantidade01)

{
     int k = 0;
     char zero[100];
     while(quantidade01 < 32)
     {
         zero[k] = '0';
         quantidade01++;
         k++;
     }
     zero[k] = '\0';
     strcat(zero, objIR.ir);
     strcpy(objIR.ir, zero);
}

// M�todos de decodifica��o das instru��es para os registradores(palavras).

void decode()

{
    
    int quantidade;
    quantidade = strlen(objIR.ir);
    if(quantidade < 32);
        complemento(quantidade);

    string s = objIR.ir;
    unsigned long int aux;
    
    s.copy(objOP.op, 6, 0); 
    aux = strtoul(objOP.op, NULL, 2);
    
    switch(aux)
    {
        case LW:
             s.copy(objRS.rs, 5, 6); 
             s.copy(objRT.rt, 5, 11); 
             s.copy(objRD.rd, 16, 16);
             break;
        case SW:
             s.copy(objRS.rs, 5, 6); 
             s.copy(objRT.rt, 5, 11); 
             s.copy(objRD.rd, 16, 16);
             break;
        default:
                s.copy(objRS.rs, 5, 6); 
                s.copy(objRT.rt, 5, 11); 
                s.copy(objRD.rd, 5, 16); 
                s.copy(objSHAMT.shamt, 5, 21); 
                s.copy(objFUNCT.funct, 6, 26);
    }
}

// M�todo de busca das instru��es.

void fetch()

{
    unsigned long int instrucao1;
    instrucao1 = objMemory.read(pc);
    itoa(instrucao1, objIR.ir, 2);
    pc++;
}

// M�todo para carregar as instru��es (palavras) de um arquivo para a mem�ria.

void load()

{
    setlocale(LC_ALL, "");
    char caminho[300], inst[32]; 
    unsigned long int instrucao;
    
    cout << line << "Digite o endere�o (caminho) onde est� seu documento "
    << "com as instru��es."
    << "\nExemplo: \"C:\\Arquivos de programas\\MIPS\\mips.txt\" \n" << endl;
    cin >> caminho;
    
    ifstream arquivo;
    arquivo.open(caminho);
    if(arquivo.fail())
    {
        cout << "\n\nErro na abertura do arquivo!!! \n\n" << "Verifique se o "
        << "caminho digitado est� correto. \n"
        << "Verifique se o nome do arquivo est� correto. \nVerifique se a "
        << "extens�o do arquivo "
        << "esta correta." << "\n\n\tPressione qualquer tecla para sair. ";
        getch();
        exit(0);
    }
    
    int i = 0; //a vari�vel "i" far� a fun��o do PC nesse m�todo.
    while(arquivo.getline(inst, 33))
    {
        instrucao = strtoul(inst, NULL, 2);
        objMemory.write(i, instrucao);
        objLINHAS.linhas = i;
        i+=4;
    }
    arquivo.close();
}

// M�todos para realizar busca, decidifica��o e execu��o das instru��es.

void step()

{
    fetch();
    decode();
    execute();
}

/* M�todo para realizar os passos do 'step' at� que n�o tenha mais intru��es 
em mem�ria. */

void run()

{
    step();
}

// M�todo auxiliar para mostrar o menu na tela.

void dump()

{
    int valor_ini, valor_fin;
    char escolha, formato;
    system("cls");
    cout << line << "Escolha uma das op��es abaixo: \n\nDigite (1) para ver o "
    << "conte�do dos registradores."
    << "\nDigite (2) para ver o conte�do da mem�ria." 
    << "\nDigite (3) para sair do programa. \n\nDigite sua op��o: ";
    cin >> escolha;
    
    switch(escolha)
    {
        case '1':
             system("cls");
             cout << line << "Op��o escolhida: " << escolha << "\n" << endl;
             dump_regs();
             break;
        case '2':
             system("cls");
             cout << line << "Op��o escolhida: " << escolha << "\n" << endl;
             cout << "Digite a posi��o inicial da mem�ria a ser lida: ";
             cin >> valor_ini;
             cout << "\nDigite o valor final da mem�ria a ser lida: ";
             cin >> valor_fin;
             cout << "\nDigite o tipo a ser mostrado \n(D) para decimal ou (H) "
             << "para hexadecimal: ";
             cin >> formato;
             cout << line;
             dump_mem(valor_ini, valor_fin, formato);
             break;
        case '3':
             exit(0);
        default:
                system("cls");
                dump();
    }
}

// M�todo principal.

int main()

{
    load();
    int z = 0;
    do
    {
        run();
        z++;
    }
    while(z <= objLINHAS.linhas);
    dump();
    system("pause");
    return 0;
}
