#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "utils.hpp"
#include "Binario.hpp"
#include "structs.hpp"
#include "ordenacao.hpp"

using namespace std;

const int LINHAS_POR_VEZ = 150;

// Função para posicionar o cursor no final do arquivo binário
void Binario::ApontarNoFinal() {
  if (this->Aberto())
  {
    this->arquivoBin.seekg(0, ios::end);
  }
  else
  {
    this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::ate);
  }
}

// Função para mostrar todos os elementos válidos do arquivo binário
void Binario::MostrarTodos()
{
  this->Abrir();
  int qtdElementos = this->QuantidadeDeElementos();
  this->arquivoBin.seekg(0, ios::beg);

  tabela elementoAux;
  for (int i = 0; i < qtdElementos; i++)
  {
    this->arquivoBin.seekg(i * sizeof(tabela));
    this->arquivoBin.read((char *)&elementoAux, sizeof(tabela));
    if (elementoAux.valido == 1)
    {
      cout << elementoAux.anzsic06 << "  " 
           << elementoAux.area << "  " 
           << elementoAux.year << "  " 
           << elementoAux.geo_count << "  " 
           << elementoAux.ec_count << endl;
    }
  }
  this->Fechar();
}

// Função para remover um elemento na posição especificada
void Binario::RemoverElemento(const int &posicao)
{
  this->Abrir();
  int qtdElementos = this->QuantidadeDeElementos();

  this->arquivoBin.seekg(0, this->arquivoBin.beg); 
  int cont = posicao - 1;                          
  tabela elementoAux;
  if (cont >= 0 and cont < qtdElementos)
  {
    this->arquivoBin.seekg(cont * sizeof(tabela));
    this->arquivoBin.read((char *)&elementoAux, sizeof(tabela));
    if (elementoAux.valido == 1)
    {
      cout << elementoAux.area << endl;
      elementoAux.valido = 0;
      this->arquivoBin.seekp(cont * sizeof(tabela));
      this->arquivoBin.write((char *)&elementoAux, sizeof(tabela));
    }
    else if (elementoAux.valido == 0)
    {
      cout << endl
           << "Elemento já excluído!" << endl
           << endl;
    }
  }
  else
  {
    cout << "\nNão há elemento nesta posição!\n";
  }
  this->Fechar();
}

// Função para importar dados de um arquivo CSV para o arquivo binário
void Binario::ImportarDeCSVParaBinario(const string &nomeArquivoEntradaCSV)
{
  ifstream arqCSV(nomeArquivoEntradaCSV);
  if (!arqCSV.good()) {
    throw runtime_error("Você precisa importar a base de dados antes de utilizar o programa!\nUtilize `make download_csv` e tente novamente.");
  }
  cout << endl << "Importando de "<< nomeArquivoEntradaCSV << " para " << nomeArquivoBin << endl;
  arquivoBin.open(nomeArquivoBin, ios::binary | ios::out);
  arquivoBin.seekp(0, ios::end);

  string vetor[7];
  string coluna;
  string aux;
  tabela elemento;

  getline(arqCSV, aux); 

  while (getline(arqCSV, aux))
  {
    stringstream linha(aux);

    for (int i = 0; i < 7; i++)
    {
      getline(linha, coluna, ',');
      if (coluna.front() == '"' and coluna.back() != '"')
      {
        string colunaTemp;
        while (coluna.back() != '"')
        {
          getline(linha, colunaTemp, ',');
          coluna += ',' + colunaTemp;
        }
      }
      vetor[i] = coluna;
    }

    elemento = convertVetor(vetor); 
    arquivoBin.write((char *)&elemento, sizeof(tabela));
  }
  arquivoBin.close();
  arqCSV.close();
}

// Função para exportar dados do arquivo binário para um arquivo CSV
void Binario::ExportarParaCSV(string nomeArquivoSaidaCSV)
{
  this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::out | ios::ate);

  ofstream arqCSV(nomeArquivoSaidaCSV);
  arqCSV << "anzsic06,Area,year,geo_count,ec_count" << endl;

  int qtdElementos = this->QuantidadeDeElementos();
  tabela elementoAux;

  for (int i = 0; i < qtdElementos; i++)
  {
    this->arquivoBin.seekg(i * sizeof(tabela));
    this->arquivoBin.read((char *)&elementoAux, sizeof(tabela));

    if (elementoAux.valido != 0)
    {
      arqCSV << elementoAux.anzsic06 << ";" 
             << elementoAux.area << ";" 
             << elementoAux.year << ";" 
             << elementoAux.geo_count << ";" 
             << elementoAux.ec_count << endl;
    }
  }

  arqCSV.close();
  this->Fechar();
}

// Função para adicionar um ou mais elementos ao arquivo binário
void Binario::AdicionarElemento()
{
  tabela elementoAux;
  int opc;
  cout << "Quantos Elementos deseja cadastrar?" << endl
       << endl;
  cout << "1: Cadastrar apenas 1." << endl
       << "2: Mais de um." << endl;
  cin >> opc;
  cout << endl;

  this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::out | ios::in | ios::app);

  switch (opc)
  {
    case 1:
      cin.ignore();
      cout << "Anzsic06: ";
      cin.getline(elementoAux.anzsic06, 255);
      cout << "Area: ";
      cin.getline(elementoAux.area, 255);
      cout << "Year: ";
      cin >> elementoAux.year;
      cin.ignore();
      cout << "Geo Count: ";
      cin >> elementoAux.geo_count;
      cin.ignore();
      cout << "EC Count: ";
      cin >> elementoAux.ec_count;
      cin.ignore();

      arquivoBin.write((char *)&elementoAux, sizeof(tabela));
      break;
    case 2:
      cout << "Quantos elementos ao todo você deseja cadastrar? ";
      int segOpc;
      cin >> segOpc;
      cout << endl;

      for (int i = 0; i < segOpc; i++)
      { 
		cin.ignore();
        cout << "Anzsic06: ";
        cin.getline(elementoAux.anzsic06, 255);
        cout << "Area: ";
        cin.getline(elementoAux.area, 255);
        cout << "Year: ";
        cin >> elementoAux.year;
        cin.ignore();
        cout << "Geo Count: ";
        cin >> elementoAux.geo_count;
        cin.ignore();
        cout << "EC Count: ";
        cin >> elementoAux.ec_count;
        cin.ignore();

        arquivoBin.write((char *)&elementoAux, sizeof(tabela));

        cout << endl;
      }
      break;
    default:
      cout << "Opção Inválida";
      break;
  }

  this->Fechar();
}

// Função para buscar elementos por anzsic06
void Binario::BuscarPorArea()
{
  this->arquivoBin.open(NOME_ARQ_BINARIO, ios::in | ios::binary | ios::ate);

  int qtdElementos = this->QuantidadeDeElementos();

  cout << "\nDigite a Area desejada > ";
  cin.clear();
  cin.ignore();
  char area[255];
  cin.getline(area, 255);
  cout << endl;

  int cont = 0;
  int aux = 0;
  tabela elemento;
  this->arquivoBin.seekg(0, this->arquivoBin.beg);

  while (cont < qtdElementos)
  {
    this->arquivoBin.seekg(cont * sizeof(tabela));
    this->arquivoBin.read((char *)&elemento, sizeof(tabela));
    if (strcmp(area, elemento.area) == 0)
    {
      if (elemento.valido == 1)
      {
        cout << elemento.anzsic06 << "  " 
             << elemento.area << "  "
             << elemento.year << "  " 
             << elemento.geo_count << "  "
             << elemento.ec_count << endl;
        aux++;
      }
    }
    cont++;
  }

  if (aux == 0)
    cout << "Não há elementos cadastrados com essa Area!\n";

  this->Fechar();
}

// Função para imprimir um intervalo de elementos do arquivo binário
void Binario::ImprimirGapDeElementos()
{
  this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::out | ios::ate);
  int tamArq = static_cast<int>(this->arquivoBin.tellg());
  int qtdElementos = int((tamArq / sizeof(tabela)));

  cout << "Escolha o intervalo que você quer imprimir no Arquivo, de 1 a " << qtdElementos << ':' << endl
       << endl;
  tabela elemento;
  int gapInicio, gapFim;
  cin >> gapInicio >> gapFim;
  cin.ignore();
  int cont_el_validos = (gapFim) - (gapInicio - 1);
  bool verificacao = false;

  if ((gapInicio > 0) and (gapInicio < qtdElementos) and (gapFim > 0) and (gapFim < qtdElementos))
  {
    cout << endl
         << "Elementos Cadastrados e Ativos entre as linhas " << gapInicio << " - " << gapFim << ":" << endl
         << endl;
    while (gapInicio <= gapFim and cont_el_validos <= gapFim)
    {
      this->arquivoBin.seekg((gapInicio - 1) * sizeof(tabela)); //
      this->arquivoBin.read((char *)&elemento, sizeof(tabela));
      if (elemento.valido == 1)
      {
        cout << elemento.anzsic06 << "  " 
             << elemento.area << "  " 
             << elemento.year << "  " 
             << elemento.geo_count << "  " 
             << elemento.ec_count << endl;
        verificacao = true;
      }
      if (verificacao == false)
      {
        cont_el_validos--; 
        gapFim++;  
      }
      gapInicio++;
      verificacao = false;
    }
  }
  else
    cout << "Intervalo inválido!\n";

  this->Fechar();
}

// Função para trocar as posições de dois elementos no arquivo binário
void Binario::TrocarPosicoes(int pos1, int pos2) {
  this->arquivoBin.open(NOME_ARQ_BINARIO, ios::in | ios::binary | ios::out);
  tabela aux1, aux2;

  this->arquivoBin.seekg(pos1 * sizeof(tabela), ios::beg);
  this->arquivoBin.read((char *)&aux1, sizeof(tabela));
  this->arquivoBin.seekg(pos2 * sizeof(tabela), ios::beg);
  this->arquivoBin.read((char *)&aux2, sizeof(tabela));

  this->arquivoBin.seekp(pos2 * sizeof(tabela), ios::beg);
  this->arquivoBin.write((char *)&aux1, sizeof(tabela));
  this->arquivoBin.seekp(pos1 * sizeof(tabela), ios::beg);
  this->arquivoBin.write((char *)&aux2, sizeof(tabela));

  this->Fechar();
}

// Função para adicionar um elemento em uma posição específica no arquivo binário
void Binario::AdicinoarElementoEmPosicaoEspecifica(int posicao) {
  this->Abrir();

  int qtdElementos = this->QuantidadeDeElementos();
  if (posicao < 0 or posicao > qtdElementos)
  {
    cout << "Posição inválida." << endl;
    return;
  }

  tabela elementoAux;
  cin.ignore();
  cout << "Anzsic06: ";
  cin.getline(elementoAux.anzsic06, 255);
  cout << "Area: ";
  cin.getline(elementoAux.area, 255);
  cout << "Year: ";
  cin >> elementoAux.year;
  cin.ignore();
  cout << "Geo Count: ";
  cin >> elementoAux.geo_count;
  cin.ignore();
  cout << "EC Count: ";
  cin >> elementoAux.ec_count;
  cin.ignore();

  int tamanhoElemento = sizeof(tabela);
  streampos posicaoInserir = posicao * tamanhoElemento;
  this->arquivoBin.seekp(posicaoInserir);

  for (int i = qtdElementos - 1; i >= posicao; i--)
  {
    streampos posicaoAtual = i * tamanhoElemento;
    this->arquivoBin.seekg(posicaoAtual);
    tabela elemento;
    this->arquivoBin.read((char *)&elemento, tamanhoElemento);

    streampos proximaPosicao = (i + 1) * tamanhoElemento;
    this->arquivoBin.seekp(proximaPosicao);
    this->arquivoBin.write((char *)&elemento, tamanhoElemento);
  }

  this->arquivoBin.seekp(posicaoInserir);
  this->arquivoBin.write((char *)&elementoAux, tamanhoElemento);

  this->Fechar();
}

// Função para alterar os dados de um elemento em uma posição específica no arquivo binário
void Binario::AlterarDadosEmPosicaoEspecifica(int posicao) {
  this->Abrir();

  int qtdElementos = this->QuantidadeDeElementos();
  if (posicao < 0 or posicao >= qtdElementos)
  {
    cout << "Posição inválida." << endl;
    return;
  }

  tabela elementoAux;
  cin.ignore();
  cout << "Anzsic06: ";
  cin.getline(elementoAux.anzsic06, 255);
  cout << "Area: ";
  cin.getline(elementoAux.area, 255);
  cout << "Year: ";
  cin >> elementoAux.year;
  cin.ignore();
  cout << "Geo Count: ";
  cin >> elementoAux.geo_count;
  cin.ignore();
  cout << "EC Count: ";
  cin >> elementoAux.ec_count;
  cin.ignore();

  int tamanhoElemento = sizeof(tabela);
  
  streampos posicaoAlterar = posicao * tamanhoElemento;
  this->arquivoBin.seekp(posicaoAlterar);

  this->arquivoBin.write((char *)&elementoAux, tamanhoElemento);

  this->Fechar();
}

// Função para ordenar o arquivo binário externamente
void Binario::Ordenar() {
  if(Existe()) {
    string nomeArqBinOrdenado = "dados_ordenados.bin";
    sortExterno(this->nomeArquivoBin, nomeArqBinOrdenado, LINHAS_POR_VEZ);
    remove(this->nomeArquivoBin.c_str());
    rename(nomeArqBinOrdenado.c_str(), this->nomeArquivoBin.c_str());
    } else {
    throw runtime_error("Arquivo não existe! Importe-o primeiro");
  }
}
