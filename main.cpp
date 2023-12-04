#include <iostream>
#include <exception>

#include "utils.hpp"
#include "structs.hpp"
#include "Binario.hpp"

using namespace std;

// Integrantes do grupo: Anwar Martins Hermuche, Clarisse Lacerda Pimentel;
// Turma > 10A

// Campos: Anzsic06; Area; Year; GEO Count; EC Count

// Função principal
int main() {
  try {
    // Criação de um objeto Binario com o nome do arquivo binário
    Binario bin(NOME_ARQ_BINARIO);

    // Verifica se o arquivo binário já existe
    if (!bin.Existe()) {
      cout << "Aguarde enquanto importamos o arquivo CSV pela primeira vez!" << endl;
      bin.ImportarDeCSVParaBinario(NOME_ARQ_CSV);
      cout << "Arquivo importado com sucesso!" << endl;
    }

    bin.Fechar(); // Fecha o arquivo binário

    int opc; // Variável para a escolha do usuário

    do {
      try {
        mostrarOpcoes(); // Mostra o menu de opções

        cin >> opc;
        cout << "\n";

        int opt = 0; // Variável para a escolha secundária

        switch (opc) {
          case 0:
            break;
          case 1:
            int posicao;
            cout << "\nDigite a posição do elemento a ser excluído (1 - " << bin.QuantidadeDeElementos() << ")\n> ";
            cin >> posicao;
            bin.RemoverElemento(posicao); // Remove um elemento na posição especificada
            break;
          case 2:
            cout << "Cadastrar novo(s) elemento(s):\n"
                 << endl;
            bin.AdicionarElemento(); // Adiciona um novo elemento
            break;
          case 3: // Buscar
            cout << "1: Buscar por Anzsic06\nOu pressione qualquer número para voltar ao menu anterior\n> ";
            opt = retornaEscolha();
            if (opt == 1) {
              bin.BuscarPorAnzsic06(); // Busca por Anzsic06
            }
            bin.Fechar();
            break;
          case 4:
            cout << "Elementos cadastrados e ativos:" << endl
                 << endl;
            bin.MostrarTodos(); // Mostra todos os elementos
            break;
          case 5:
            bin.ExportarParaCSV(NOME_ARQ_SAIDA_CSV); // Exporta os dados para um arquivo CSV
            cout << endl
                 << "Arquivo CSV exportado!" << endl;
            break;
          case 6:
            bin.ImprimirGapDeElementos(); // Imprime gaps de elementos
            break;
          case 7:
            int pos1, pos2;
            cout << "Digite as 2 posições em sequência para efetuar a troca, de 1 a " << bin.QuantidadeDeElementos()
                 << ':'
                 << endl
                 << endl;
            cin >> pos1 >> pos2;
            bin.TrocarPosicoes(pos1, pos2); // Troca as posições de dois elementos
            break;
          case 8:
            int pos;
            cout << "Digite a posição que deseja efetuar a troca, de 1 a " << bin.QuantidadeDeElementos() << ':' << endl
                 << endl;
            cin >> pos;
            bin.AdicinoarElementoEmPosicaoEspecifica(pos); // Adiciona um elemento em uma posição específica
            break;
          case 9:
            int posEditar;
            cout << "Digite a posição que deseja efetuar a troca, de 1 a " << bin.QuantidadeDeElementos() << ':' << endl
                 << endl;
            cin >> posEditar;
            bin.AlterarDadosEmPosicaoEspecifica(posEditar); // Altera os dados de um elemento em uma posição específica
            break;
          case 10:
            bin.Ordenar(); // Ordena os elementos
            break;
          case 11:
            cout << endl << bin.QuantidadeDeElementos() << endl; // Exibe a quantidade de elementos
            break;
          default:
            cout << "Digite uma opção válida!\n";
            break;
        }
      } catch (exception &e) {
        cout << endl << e.what() << endl;
      }
    } while (opc != 0); // Continua o loop até que o usuário escolha a opção 0
  } catch (exception& e) {
    cout << endl << e.what() << endl; // Exibe mensagens de erro caso ocorram exceções
    return 1;
  }
  return 0;
}
