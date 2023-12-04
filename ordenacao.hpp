#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <limits>
#include <limits.h>
#include "structs.hpp"
using namespace std;

#ifndef ORDENACAO_HPP
#define ORDENACAO_HPP

// Função para calcular a quantidade de elementos em um arquivo binário
int quantidadeElementos(ifstream &arq) {
  streampos posInicial = arq.tellg();
  arq.seekg(0, arq.end);
  int qntElementos = static_cast<int>(arq.tellg() / sizeof(tabela));
  arq.seekg(posInicial); 
  return qntElementos;
}

// Função para salvar um array de estruturas em um arquivo binário
void salvarArquivo(string nome, tabela arr[], int tam) {
  ofstream arq(nome, ios::binary);
  for (int i = 0; i < tam; i++) {
    arq.write((char *) (&arr[i]), sizeof(tabela));
  }
  arq.close();
}

// Classe que representa um nó em um heap mínimo personalizado
class MinHeapNoh {
public:
    tabela elemento;
    int i;    
    int j;    

    // Função para alterar a área do elemento para o máximo possível
    void alterarArea() {
      elemento.area[0] = numeric_limits<char>::max();
    }

    // Função para alterar a posição do elemento
    void alterarPosicao(int &pos) {
      elemento.posicao = pos++;
    };
};

// Função para realizar uma troca entre dois nós de heap mínimo personalizado
void swap(MinHeapNoh *x, MinHeapNoh *y) {
  MinHeapNoh temp = *x;
  *x = *y;
  *y = temp;
}

// Classe que implementa um heap mínimo personalizado
class MinHeap {
private:
    MinHeapNoh *dados;
    int tamanho;

    int esquerdo(int i) {
      return 2 * i + 1;
    }

    int direito(int i) {
      return 2 * i + 2;
    }

    // Função para corrigir um heap mínimo descendo a partir de um índice dado
    void corrigeDescendo(int i) {
      int esq = esquerdo(i);
      int dir = direito(i);

      int menor = i;

      if ((esq < tamanho) && ((strcasecmp(dados[esq].elemento.area, dados[i].elemento.area)) < 0)) {
        menor = esq;
      } else if ((esq < tamanho) && ((strcasecmp(dados[esq].elemento.area, dados[i].elemento.area)) == 0) &&
                 (dados[esq].elemento.ec_count < dados[i].elemento.ec_count)) {
        menor = esq;
      }

      if ((dir < tamanho) && ((strcasecmp(dados[dir].elemento.area, dados[menor].elemento.area)) < 0)) {
        menor = dir;
      } else if ((dir < tamanho) && ((strcasecmp(dados[dir].elemento.area, dados[menor].elemento.area)) == 0) &&
                 (dados[dir].elemento.ec_count < dados[menor].elemento.ec_count)) {
        menor = dir;
      }

      if (menor != i) {
        swap(&dados[i], &dados[menor]);
        corrigeDescendo(menor);
      }
    }

public:
    MinHeap(MinHeapNoh arr[], int tam) {
      tamanho = tam;
      dados = arr;
      int i = (tamanho - 1) / 2;
      while (i >= 0) {
        corrigeDescendo(i);
        i--;
      }
    }

    ~MinHeap() {
      delete[] dados;
    };

    MinHeapNoh getMin() {
      return dados[0];
    }

    void inserirMin(MinHeapNoh x) {
      dados[0] = x;
      corrigeDescendo(0);
    }
};

// Função para realizar a mesclagem de arquivos binários ordenados
void mergeArquivos(const string& nomeArqInput, const string& nomeArqOutput, int n, int k) {
  MinHeapNoh *nohs = new MinHeapNoh[k];
  tabela *leitura = new tabela[1];
  stringstream novoNomeArquivo;
  
  for (int i = 0; i < k; i++) {
    novoNomeArquivo.str("");
    novoNomeArquivo.clear();
    novoNomeArquivo << "temp_";
    novoNomeArquivo << to_string(i + 1);
    novoNomeArquivo << ".bin";

    ifstream arqBinLeitura(novoNomeArquivo.str(), ios::binary);

    arqBinLeitura.read((char *) (&leitura[0]), sizeof(tabela));

    nohs[i].elemento = leitura[0];
    nohs[i].i = i;
    nohs[i].j = 1;

    arqBinLeitura.close();
  }

  MinHeap hp(nohs, k);

  int qntArquivos = 0;
  int posInsercaoFinal = 0;
  ifstream arqBinEntrada(nomeArqInput, ios::binary);
  arqBinEntrada.close();

  ofstream arqBinResult(nomeArqOutput, ios::binary);

  while (qntArquivos != k) {
    MinHeapNoh raiz = hp.getMin();
    raiz.alterarPosicao(posInsercaoFinal);
    arqBinResult.write((char *) (&raiz.elemento), sizeof(tabela));

    novoNomeArquivo.str("");
    novoNomeArquivo.clear();
    novoNomeArquivo << "temp_";
    novoNomeArquivo << to_string(raiz.i + 1);
    novoNomeArquivo << ".bin";

    ifstream arqBinInput(novoNomeArquivo.str(), ios::binary);
    int qntElementos = quantidadeElementos(arqBinInput);

    if (raiz.j < qntElementos) { 
      arqBinInput.seekg(raiz.j * sizeof(tabela), arqBinInput.beg);
      arqBinInput.read((char *) (&leitura[0]), sizeof(tabela));
      raiz.elemento = leitura[0];
      raiz.j++;
    } else { 
      raiz.alterarArea();
      qntArquivos++;
    }
    arqBinInput.close();
    hp.inserirMin(raiz);
  }

  arqBinResult.close();
  delete[] leitura;
}

// Função para criar arquivos temporários ordenados
int criarArquivosOrdenados(const string& nomeArqInput, int tamParticao) {
  ifstream arqBinEntrada(nomeArqInput, ios::binary);

  if (arqBinEntrada) {
    int qntElementos = quantidadeElementos(arqBinEntrada);
    arqBinEntrada.seekg(0, arqBinEntrada.beg);
    stringstream nomeArqAux;

    tabela *arrBin = new tabela[tamParticao];
    tabela *leitura = new tabela[1];

    int linhaFim = qntElementos - (qntElementos % tamParticao);
    int linhaAtual = 0, cont = 0, total = 0;

    while (linhaAtual < linhaFim) {
      arqBinEntrada.read((char *) (&leitura[0]), sizeof(tabela));
      arrBin[total] = leitura[0];
      total++;

      if (total == tamParticao) {
        cont++;
        nomeArqAux.str("");
        nomeArqAux.clear();
        nomeArqAux << "temp_";
        nomeArqAux << to_string(cont);
        nomeArqAux << ".bin";

        arrBin->qsort(0, total - 1);
        salvarArquivo(nomeArqAux.str(), arrBin, tamParticao);
        total = 0;
      }

      linhaAtual++;
    }

    if (linhaAtual != qntElementos) {
      tabela *vetorDadosRestantes = new tabela[qntElementos - linhaAtual];

      while (linhaAtual < qntElementos) {
        arqBinEntrada.read((char *) (&leitura[0]), sizeof(tabela));
        vetorDadosRestantes[total] = leitura[0];
        total++;
        linhaAtual++;
      }

      cont++;
      nomeArqAux.str("");
      nomeArqAux.clear();
      nomeArqAux << "temp_";
      nomeArqAux << to_string(cont);
      nomeArqAux << ".bin";

      vetorDadosRestantes->qsort(0, total - 1);
      salvarArquivo(nomeArqAux.str(), vetorDadosRestantes, total);

      delete[] vetorDadosRestantes;
    }

    arqBinEntrada.close();
    delete[] leitura;
    delete[] arrBin;
    return cont;
  } else {
    throw runtime_error("Erro! Arquivo não encontrado");
  }
}

// Função principal que realiza a ordenação externa
void sortExterno(const string& nomeArqInput, const string& nomeArqOutput, int tamParticao) {
  int qntArquivosOrdenados = -1;

  try {
    qntArquivosOrdenados = criarArquivosOrdenados(nomeArqInput, tamParticao);

    cout << "Quantidade de arquivos temporários: " << qntArquivosOrdenados << endl;

    mergeArquivos(nomeArqInput, nomeArqOutput, tamParticao, qntArquivosOrdenados);
  }
  catch(exception& e){
    cout << e.what() << endl;
  }

  if (qntArquivosOrdenados != -1) {
    stringstream nomeArqAux;
    for(int i = 0; i < qntArquivosOrdenados; i++){
      nomeArqAux.str("");
      nomeArqAux.clear();
      nomeArqAux << "temp_";
      nomeArqAux << to_string(i + 1);
      nomeArqAux << ".bin";
      remove(nomeArqAux.str().c_str());
    }
    cout << "\nArquivo principal ordenado com sucesso!\n";
  }
}

#endif
