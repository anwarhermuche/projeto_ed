#include <iostream>
#include <fstream>
#include <cstring>

#ifndef STRUCTS_HPP
#define STRUCTS_HPP

using namespace std;

// Definição de uma estrutura chamada "tabela"
struct tabela {
    char anzsic06[100];
    char area[100];
    int year;
    int geo_count;
    int ec_count;
    int posicao = -1; // Posição inicializada com -1
    int valido = 1;   // Valor válido inicializado como 1

    // Construtor da estrutura
    tabela() {
      memset(anzsic06, 0, sizeof(anzsic06));
      memset(area, 0, sizeof(area));
      year = 0;
      geo_count = 0;
      ec_count = 0;
      posicao = -1;
      valido = 1;
    }

    // Função de partição usada no algoritmo de ordenação QuickSort
    int particao(int menor, int maior) {
      tabela pivo = this[maior]; // Seleciona o último elemento como pivo

      int i = (menor - 1);

      for (int j = menor; j <= maior; j++) {
        // Utilizando a área como ordenação primária
        if ((strcasecmp(this[j].area, pivo.area)) < 0) {
          i++;
          swap(this[i], this[j]);
        }
        // Utilizando o ec_count como ordenação secundária se as áreas forem iguais
        else if (((strcasecmp(this[j].area, pivo.area)) == 0) and (this[j].ec_count < pivo.ec_count)) {
          i++;
          swap(this[i], this[j]);
        }
      }

      swap(this[i + 1], this[maior]);

      return (i + 1);
    }

    // Função de ordenação QuickSort
    void qsort(int menor, int maior) {
      if (menor < maior) {
        int pi = particao(menor, maior);
        qsort(menor, pi - 1);
        qsort(pi + 1, maior);
      }
    }
};

#endif
