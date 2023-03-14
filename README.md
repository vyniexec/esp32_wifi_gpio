| ESP32 | 
| ----- |

# Projeto inicial

Projeto feito com intuito de acionar um relé de uma placa CPU criada nas aulas de eletrônica do curso, usando um ESP32, aonde a saída para ser ligada é necessário de uma configuração no ESP32 para ele dar pulsos em determinados terminais do registrador de dados 74HC595 para acionar a saída.



## Placa que foi usada com exemplo
![image_board](https://github.com/JoseWRPereira/esp32_io_ihm/blob/main/img/esp32_IO-top3D.PNG)

## Conteúdo das pastas

O projeto contém um arquivo em linguagem C [main.c](main/main.c). O arquivo está na pasta [main](main).

Os projetos ESP-IDF são construídos usando CMake. A configuração de compilação do projeto está contida nos arquivos CMakeLists.txt que fornecem um conjunto de diretivas e instruções que descrevem os arquivos de origem e destinos do projeto (executável, biblioteca ou ambos).
```
Além disso, o projeto de amostra contém os arquivos Makefile e component.mk, usados ​​para o sistema legado de compilação baseado em Make.
Eles não são usados ​​ou necessários ao compilar com CMake e idf.py.