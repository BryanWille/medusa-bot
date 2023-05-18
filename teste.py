from google.cloud import vision
from google.oauth2.service_account import Credentials
import io

def analyze_image(image_path):
    # Carrega as credenciais a partir do arquivo JSON
    credentials = Credentials.from_service_account_file('credentials.json')

    # Cria o cliente da API do Cloud Vision usando as credenciais
    client = vision.ImageAnnotatorClient(credentials=credentials)

    with io.open(image_path, 'rb') as image_file:
        content = image_file.read()

    image = vision.Image(content=content)

    response = client.label_detection(image=image)
    labels = response.label_annotations

    print('Labels:')
    for label in labels:
        print(label.description)

# Specify the path to your image file
image_path = 'C:/Users/processos02/Desktop/sapo.png'

analyze_image(image_path)
