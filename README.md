# Estrutura de dados - Projeto Prático de Ordenação Externa

Este projeto foi feito com durante o segundo semestre de 2023 para a disciplina Estrutura de Dados, do prof. Renato.

Os dados tem campos Anzsic06, Area, Year, Geo Count e EC Count. Além disso, a ordenação primária é feita através do campo Area e, secundariamente, através do campo EC Count. De forma DECRESCENTE.

## Como executar?
Primeiramente, se não tiver o `g++` e o `curl`, instale-os com o seguinte comando:
```bash
sudo apt install g++ curl
```

Agora, execute `make baixar_dados` para baixar os dados que estão no servidor do DropBox. Dessa forma, facilitamos a importação da base de dados.

Depois disso, basta executar `make`para fazer a compilação do projeto e, após isso, `./programa` para executar o projeto.

## Integrantes do grupo
- Anwar Martins Hermuche
- Clarisse Lacerda Pimentel
