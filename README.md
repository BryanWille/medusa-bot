# Medusa Bot
O Medusa bot é um projeto feito em C++ utilizando um ESP32-CAM para rotacioanr, tirar fotos, identificar o que está na imagem e traduzir os rótulos atráves do Google Cloud e do Telegram.

## Tecnologias Usadas
- Arduino IDE
- ESP32-CAM
- Servo Motor
- Conversor USB
- Telegram
- Google Vision
- Google Tradutor

## 🔧 Como funciona?
O usuário chama o bot no telegram usando o comando /photo, o bot então vai tirar três fotos, fazendo a seguinte rotina por foto:
1. Tirar foto
2. Enviar foto para Google Vision classificar imagem
3. Traduzir os rótulos classificados usando o Google Tradutor
4. Enviar a foto tirada para o grupo do Telegram
5. Enviar a mensagem com os rótulos traduzidos para o grupo do Telegram.
6. Girar 90°

### Resultados
![image](https://github.com/BryanWille/medusa-bot/assets/84272231/5184e40c-2b09-4058-8ba3-a11cd4280e91)
![image](https://github.com/BryanWille/medusa-bot/assets/84272231/383ad283-ab3b-4186-80bc-831333eba17c)


## Dificuldades 
- Utilização de API no C++
Tivemos dificuldades com a API em C++ por conta da manipulação dos tipos de dados JSON, que é diferente de outras linguagens como Python e Javascript, onde é mais fácil gerenciar os dados.

- Fazer o upload do código para o ESP32-CAM
Algumas bibliotecas necessárias o Arduino IDE não possuia compatibilidade o que resulto em atrasos, e também uma limitação física do ESP32-CAM nos obrigou a ter que usar um conversor USB-Serial para transmitir corretamente os dados dos pinos RX e TX

- Erro da Biblioteca CameraWebServer
O problema específico ocorre quando o ESP32-CAM estava configurado para capturar fotos e enviá-las em tempo real para o Telegram. No entanto, em vez de enviar as fotos em tempo real, o módulo estava salvando as imagens localmente e enviando-as de forma aleatória, resultando em uma experiência de transmissão não satisfatória.

- Bilbioteca base64
Para enviar os dados para API do Google Cloud as imagens precisam estar em base64, por não conseguirmos instalar nenhuma biblioteca em C++ pronta para isso, tivemos que escrever direto no código o conversor.

## 👷Feito por
- [Bryan Wille](https://github.com/BryanWille)
- [Gustavo Pedrosa](https://github.com/Gustavo-hgp)
- [João Renato](https://github.com/JoaoRenato2)
- [Pedro Gabriel Sad](https://github.com/PedroSad1)

## 📜 Licença
[MIT](https://choosealicense.com/licenses/mit/)
