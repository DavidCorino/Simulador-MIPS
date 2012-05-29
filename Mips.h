#include <iostream>
#include <stdlib.h>
#include <string>


using namespace std;

class Mips

{
    public:
    //M�todos principais.
           //Busca a instru��o e incrementa o contador.
           void fecth(); 
           //Extrai os campos da instru��o, OP, RS, RD, RT, SHAMT e FUNCT.
           void decode(); 
           //Executa as intru��es.
           void execute(); 
           //Executa uma-a-uma instru��o.
           void step(); 
           //Executa o c�digo at� o final.
           void run(); 
           
    //M�todos auxiliares.
           //Mostra o conte�do dos registradores.
           void dump_regs(); 
           //Mostra o conte�do da mem�ria incluindo os endere�os.
           void dump_mem(int start, int end, char format); 
           //Carrega as instru��es de um arquivo de texto para a mem�ria.
           void load(); 
           
    //Registradores.
           /*Contador de Programa, Opcode, Registrador fonte 1,
           Registrador fonte 2, Registrador destino, Shift Amont, Fun��o. */
           int pc; 
           char ir[32], op[5], rs[4], rt[4], rd[15], shamt[4], funct[5]; 
           //Registrador de instru��o.
           
    //Vari�vel auxilar para contagem de quantas instru��es tem o arquivo.
           int linhas;
};

