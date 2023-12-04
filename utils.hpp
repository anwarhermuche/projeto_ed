#include <iostream>
#include <cstring>
#include "structs.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP

using namespace std;

// Definição de constantes para nomes de arquivos
const string NOME_ARQ_BINARIO = "arqBinario";
const string NOME_ARQ_CSV = "Data7602DescendingYearOrder.csv";
const string NOME_ARQ_SAIDA_CSV = "binarioParaCSV.csv";

// Função para mostrar as opções do programa
inline void mostrarOpcoes()
{
  cout << "0: Encerrar programa\n";
  cout << "1: Remover elemento\n";
  cout << "2: Adicionar elemento(s)\n";
  cout << "3: Buscar um determinado elemento\n";
  cout << "4: Imprimir elementos cadastrados\n";
  cout << "5: Exportar para um arquivo .csv\n";
  cout << "6: Imprimir um intervalo de elementos\n";
  cout << "7: Trocar 2 elementos de posição\n";
  cout << "8: Adicionar elemento em uma posição específica\n";
  cout << "9: Alterar dados de um determinado elemento\n";
  cout << "10: Realizar ordenação\n";
  cout << "\nEscolha uma opção! > ";
}

// Função para retornar a escolha do usuário
inline int retornaEscolha()
{
  int n;
  cin >> n;
  return n;
}

// Função para converter um vetor de strings em uma estrutura "tabela"
inline tabela convertVetor(string vetor[])
{
  tabela elemento;

  // Copia os valores do vetor de strings para a estrutura "elemento"
  strcpy(elemento.anzsic06, vetor[0].c_str());
  strcpy(elemento.area, vetor[1].c_str());
  elemento.year = int(stof(vetor[2]));
  elemento.geo_count = int(stof(vetor[3]));
  elemento.ec_count = int(stof(vetor[4]));

  return elemento;
}

#endif
