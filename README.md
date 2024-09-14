### Resoluções de Questões Variadas do Lab. de Micro

Esse repositório armazena minhas soluções para as problemáticas propostas do Laboratório de Microprocessadores, tanto das apostilas quanto de problemas avulsos de livros que tratam sobre a temática. 

### Compilando

Os códigos fontes foram compilados e exportados para hexadecimal utilizando o `PlatformIO` e, por isso, não estão em `.ino`. O jeito mais eficiente e prático de rodar esses códigos sem o arquivo de configuração é só copiando e colando todo o conteúdo abaixo do cabeçalho `Arduino.h` (que existe apenas pra deixar meu editor de texto feliz) para a IDE. É importante destacar que esse código pode falhar em versões mais antigas do IDE pelo uso de algumas palavras-chaves de `C++11` e posterior, os dois erros mais comuns é o uso do tipo `auto` e o uso de estruturas sem a keyword `struct`, se alguma coisa relacionada a isso falhar, troque `auto` por um tipo apropriado para a operação e use a keyword `struct` em todo lugar que a estrutura for citada.
