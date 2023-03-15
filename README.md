| ESP32 | 
| ----- |

# Projeto inicial

Projeto feito com intuito de acionar um relé de uma placa CPU criada nas aulas de eletrônica do curso, usando um ESP32, aonde a saída para ser ligada é necessário de uma configuração no ESP32 para ele dar pulsos em determinados terminais do registrador de dados 74HC595 para acionar a saída.



## Placa que foi usada com exemplo
![image_board](https://github.com/JoseWRPereira/esp32_io_ihm/blob/main/img/esp32_IO-top3D.PNG)

## Conteúdo das pastas

O projeto contém um arquivo em linguagem C sendo eles:
[main.c](main/main.c) esse arquivo é o arquivo que contém a função main e suas subsfunções
[acio_rele.c](main/acionamento_rele/acio_rele.c) já esse arquivo, contém toda parte de acionamento do relé, aonde é definido as saídas do ESP32 para acionar o relé
[wifi.c](main/wifi/wifi.c) toda biblioteca e configuração wifi está nesse arquivo. Os arquivos citados acima estão na pasta [main](main).

Os projetos ESP-IDF são construídos usando CMake. A configuração de compilação do projeto está contida nos arquivos CMakeLists.txt que fornecem um conjunto de diretivas e instruções que descrevem os arquivos de origem e destinos do projeto (executável, biblioteca ou ambos).
```
Além disso, o projeto de amostra contém os arquivos Makefile e component.mk, usados ​​para o sistema legado de compilação baseado em Make.
Eles não são usados ​​ou necessários ao compilar com CMake e idf.py.
