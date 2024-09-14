### Resoluções de Questões Variadas do Lab. de Micro

Esse repositório armazena minhas soluções para as problemáticas propostas do Laboratório de Microprocessadores, tanto das apostilas quanto de problemas avulsos de livros que tratam sobre a temática. Note que os algoritmos desenvolvidos podem não ser a forma mais otimizada possível de resolver as questões.

### Compilando

O código pode ser compilado diretamente com o Arduino IDE ou alguma toolkit oficial, como o `arduino-cli` para Linux. É importante destacar que esse código pode falhar em versões mais antigas do IDE pelo uso de algumas palavras-chaves de `C++11` e posterior, os dois erros mais comuns é o uso do tipo `auto` e o uso de estruturas sem a keyword `struct`, se alguma coisa relacionada a isso falhar, troque `auto` por um tipo apropriado para a operação e use a keyword `struct` em todo lugar que a estrutura for citada.
