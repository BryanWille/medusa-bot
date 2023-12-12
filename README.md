# Medusa Bot
O Medusa bot é um projeto feito em C++ utilizando um ESP32-CAM para tirar fotos, identificar o que está na imagem e traduzir os rótulos atráves do Google Cloud e do Telegram.

### 👷Feito por
- [Bryan Wille](https://github.com/BryanWille)
- [Gustavo Pedrosa](https://github.com/Gustavo-hgp)
- [João Renato](https://github.com/JoaoRenato2)
- [Pedro Gabriel Sad](https://github.com/PedroSad1)

## 🔧 Como funciona?
O ESP32-CAM é feito para tirar fotos, então adaptamos ele com um motor de rotação, a ação de tirar foto é controlada a partir do Telegram, onde pedimos para tirar foto, ele tira quatro fotos por comando, sendo uma foto e o motor gira 90°, então a foto é enviada para o Google Cloud que utiliza o Google Vision para fazer os rótulos e o Google Tradutor para traduzir os mesmos e enviar para o Telegram o que está na imagem.  
![1638415974659](https://user-images.githubusercontent.com/84272231/180613322-8e90fa5a-5691-479d-99b0-4da9fe9813e6.png)

## 👪 Contribuições
Ideias de melhorias são sempre aceitas!

## 📜 Licença
[MIT](https://choosealicense.com/licenses/mit/)
