from telegram.ext import *
from io import BytesIO
import cv2
import asyncio
import numpy as np
import tensorflow as tf


(x_train, y_train), (x_test, y_test) = tf.keras.datasets.cifar10.load_data()

x_train, x_test = x_train / 255, x_test / 255

class_names = ['Plane', 'Car', 'Bird', 'Cat', 'Deer', 'Dog', 'Frog', 'Horse', 'Ship', 'Truck'] 

model = tf.keras.models.Sequential()
model.add(tf.keras.layers.Conv2D(32, (3,3), activation='relu', input_shape=(32, 32, 3)))
model.add(tf.keras.layers.MaxPooling2D((2,2)))
model.add(tf.keras.layers.Conv2D(64, (3,3), activation='relu'))
model.add(tf.keras.layers.MaxPooling2D((2,2)))
model.add(tf.keras.layers.Conv2D(64, (3,3), activation='relu'))
model.add(tf.keras.layers.Flatten())
model.add(tf.keras.layers.Dense(64, activation='relu'))
model.add(tf.keras.layers.Dense(10, activation='softmax'))

async def iniciar(update, context):
    await update.message.reply_text("Seja bem-vindo!")
    
async def ajuda(update, context):
    await update.message.reply_text("""
                              /iniciar - Inicia a conversa
                    /ajuda - Mostra essa mensagem
                    /treinar - Treinar a rede neural
                              """)


async def treinar(update, context):
    update.message.reply_text("Modelo está sendo treinado!")
    model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
    model.fit(x_train, y_train, epochs=10, validation_data=(x_test, y_test))
    model.save('cifar_classifier.model')
    await update.message.reply_text("Pronto! Agora você pode mandar uma imagem")

        
async def handle_message(update, context):
    await update.message.reply_text("Por favor treine o modelo com uma imagem!")
    

async def handle_photo(update, context):
    file = await context.bot.get_file(update.message.photo[-1].file_id)
    f = BytesIO(await file.download_as_bytearray())
    file_bytes = np.asarray(bytearray(f.read()), dtype=np.uint8)
    
    img = cv2.imdecode(file_bytes, cv2.IMREAD_COLOR)
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    img = cv2.resize(img, (32, 32), interpolation=cv2.INTER_AREA)
    
    prediction = model.predict(np.array([img/255]))
    await update.message.reply_text(f"Na imagem eu vejo um(a) {class_names[np.argmax(prediction)]}")


dp = ApplicationBuilder().token("6183521273:AAGeaXYjP5kbSry5sxp-GuhyzQR4V9jh-p4").build()

dp.add_handler(CommandHandler("iniciar", iniciar))
dp.add_handler(CommandHandler("ajuda", ajuda))
dp.add_handler(CommandHandler("treinar", treinar))
dp.add_handler(MessageHandler(filters.TEXT, handle_message))
dp.add_handler(MessageHandler(filters.PHOTO, handle_photo))

dp.run_polling()
dp.idle()
