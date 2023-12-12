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

## Dificuldades 
Apresentamos algumas dificuldades durante o projeto, porém 

## 👷Feito por
- [Bryan Wille](https://github.com/BryanWille)
- [Gustavo Pedrosa](https://github.com/Gustavo-hgp)
- [João Renato](https://github.com/JoaoRenato2)
- [Pedro Gabriel Sad](https://github.com/PedroSad1)

## 📜 Licença
[MIT](https://choosealicense.com/licenses/mit/)
