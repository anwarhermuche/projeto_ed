#include <iostream>
#include <fstream>
#include "structs.hpp"

#ifndef BINARIO_H
#define BINARIO_H

using namespace std;

class Binario
{
private:
    string nomeArquivoBin;
    fstream arquivoBin;

public:
    // Construtor da classe Binario
    Binario(string nomeArquivoBinario) {
      nomeArquivoBin = nomeArquivoBinario;
    }

    // Destrutor da classe Binario
    ~Binario() {
      if (arquivoBin.is_open()) {
        arquivoBin.close();
      }
    }

    // Verifica se o arquivo binário existe
    inline bool Existe() {
      ifstream arqexiste(nomeArquivoBin);
      bool existe = arqexiste.good();
      arqexiste.close();
      return existe;
    }

    // Abre o arquivo binário
    inline bool Abrir() {
      Fechar();
      arquivoBin.open(nomeArquivoBin, ios::binary | ios::in | ios::out);
      if (!arquivoBin.good()) {
        throw runtime_error("Erro! O arquivo não existe, importe-o primeiro!");
      }

      return arquivoBin.is_open();
    }

    // Fecha o arquivo binário
    inline void Fechar() {
      if (Aberto()) {
        arquivoBin.close();
      }
    }

    // Verifica se o arquivo binário está aberto
    inline bool Aberto() {
      return arquivoBin.is_open();
    }

    // Retorna a quantidade de elementos no arquivo binário
    inline int QuantidadeDeElementos() {
      int qnt;
      if (Aberto()) {
        streampos posicaoOriginal = arquivoBin.tellg();
        ApontarNoFinal();
        qnt = arquivoBin.tellg() / sizeof(tabela);

        arquivoBin.seekg(posicaoOriginal);
      }
      else {
        Abrir();
        ApontarNoFinal();
        qnt = arquivoBin.tellg() / sizeof(tabela);
        Fechar();
      }
      return qnt;
    }

    // Move o cursor para o final do arquivo binário
    void ApontarNoFinal();

    // Mostra todos os elementos do arquivo binário
    void MostrarTodos();

    // Remove um elemento do arquivo binário com base na posição
    void RemoverElemento(const int &posicao);

    // Adiciona um elemento ao arquivo binário
    void AdicionarElemento();

    // Busca elementos por anzsic06
    void BuscarPorAnzsic06();

    // Busca elementos por área
    void BuscarPorArea();

    // Troca as posições de dois elementos no arquivo binário
    void TrocarPosicoes(int pos1, int pos2);

    // Exporta os dados para um arquivo CSV
    void ExportarParaCSV(string nomeArquivoSaidaCSV);

    // Importa dados de um arquivo CSV para o arquivo binário
    void ImportarDeCSVParaBinario(const string &nomeArquivoEntradaCSV);

    // Imprime o gap de elementos no arquivo binário
    void ImprimirGapDeElementos();

    // Adiciona um elemento em uma posição específica do arquivo binário
    void AdicinoarElementoEmPosicaoEspecifica(int posicao);

    // Altera os dados de um elemento em uma posição específica do arquivo binário
    void AlterarDadosEmPosicaoEspecifica(int posicao);

    // Ordena o arquivo binário
    void Ordenar();
};

#endif
