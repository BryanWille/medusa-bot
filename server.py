from telegram.ext import *
from io import BytesIO
from google.cloud import vision
from google.oauth2.service_account import Credentials
from textblob import TextBlob
import json

async def new_message_handler(update, context):
    message_content = update['message']['photo'][0].file_id
    print(update['message'])
    await handle_photo(update=update, context=context, fileid=message_content)

def analyze_image(content):
    credentials = Credentials.from_service_account_file('credentials.json')
    client = vision.ImageAnnotatorClient(credentials=credentials)
    image = vision.Image(content=content)
    response = client.label_detection(image=image)
    labels = response.label_annotations
    return [label.description for label in labels]

async def handle_photo(update, context, fileid):
    file = await context.bot.get_file(fileid)
    image_content = BytesIO(await file.download_as_bytearray())

    image_content.seek(0)
    
    bytes_data = image_content.read()
    labels = analyze_image(bytes_data)
    traduzidos = []
    for element in labels:
        blob = TextBlob(element)
        translation = element;
        try:
            translation = blob.translate(from_lang='en', to='pt')
        except:
            print("error")
        traduzidos.append(str(translation))
        
    await update.message.reply_text(f"Na imagem eu vejo: {', '.join(traduzidos)}")


async def iniciar(update, context):
    await update.message.reply_text("Seja bem-vindo!")
    
async def ajuda(update, context):
    await update.message.reply_text("""
                              /iniciar - Inicia a conversa
                    /ajuda - Mostra essa mensagem
                              """)

dp = ApplicationBuilder().token("6183521273:AAGeaXYjP5kbSry5sxp-GuhyzQR4V9jh-p4").build()

dp.add_handler(CommandHandler("iniciar", iniciar))
dp.add_handler(CommandHandler("ajuda", ajuda))
dp.add_handler(MessageHandler(filters.PHOTO, new_message_handler))

dp.run_polling()
