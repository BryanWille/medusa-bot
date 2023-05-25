import requests
import json
import base64

# Defina o endpoint da API e a chave de API
endpoint = 'https://vision.googleapis.com/v1/images:annotate'
api_key = 'AIzaSyBgCIKA1opotVstmkqtAOagYu3eGlF5faM'

# Construa o corpo da solicitação em formato JSON
# Read the PNG image file
with open('sapo.png', 'rb') as image_file:
    image_data = image_file.read()


image_content =  base64.b64encode(image_data).decode('utf-8')

request_data = {
    "requests": [
        {
            "image": {
                "content": image_content
            },
            "features": [
                {
                    "type": "LABEL_DETECTION",
                    "maxResults": 10
                }
            ]
        }
    ]
}

# Faça a solicitação POST para a API do Google Cloud Vision
params = {
    'key': api_key
}

headers = {
    'Content-Type': 'application/json'
}

response = requests.post(endpoint, params=params, headers=headers, data=json.dumps(request_data))

text = ""
# Obtenha os resultados da detecção de rótulos
if response.status_code == 200:
    labels = response.json()['responses'][0]['labelAnnotations']
    print
    for label in labels:
        text += label['description'] + ", "
else:
    print('Erro na solicitação:', response.status_code, response.text)
