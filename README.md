# Medusa Bot
O Medusa bot é um projeto feito em C++ utilizando um ESP32-CAM para tirar fotos, identificar o que está na imagem e traduzir os rótulos atráves do Google Cloud e do Telegram.

## 🔧 Como funciona?
O usuário chama o bot no telegram usando o comando /photo, o bot então vai tirar três fotos, fazendo a seguinte rotina por foto:
- Tirar foto
- Enviar foto para Google Vision classificar imagem
- Traduzir os rótulos classificados usando o Google Tradutor
- Enviar a foto tirada para o grupo do Telegram
- Enviar a mensagem com os rótulos traduzidos para o grupo do Telegram.
- Girar 90°

## Tecnologias Usadas
- ESP32-CAM
- Servo Motor
- Conversor USB
- Telegram
- Google Vision
- Google Tradutor

## 👪 Contribuições
Ideias de melhorias são sempre aceitas!

## 👷Feito por
- [Bryan Wille](https://github.com/BryanWille)
- [Gustavo Pedrosa](https://github.com/Gustavo-hgp)
- [João Renato](https://github.com/JoaoRenato2)
- [Pedro Gabriel Sad](https://github.com/PedroSad1)

## 📜 Licença
[MIT](https://choosealicense.com/licenses/mit/)
