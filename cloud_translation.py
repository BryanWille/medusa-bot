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
text =""
# Obtenha os resultados da detecção de rótulos
if response.status_code == 200:
    labels = response.json()['responses'][0]['labelAnnotations']
    for label in labels:
        text += label['description'] + ", "
else:
    print('Erro na solicitação:', response.status_code, response.text)


# Set your API key and endpoint URL
url = 'https://translation.googleapis.com/language/translate/v2'

# Set the text you want to translate
target_language = 'pt-br' 

# Create the request payload
payload = {
    'q': text,
    'target': target_language,
    'key': api_key
}

# Send the POST request to the API
response = requests.post(url, data=payload)

# Parse the response
data = json.loads(response.text)
print(response.text)
# Extract the translated text from the response
#translated_text = data['data']['translations'][0]['translatedText']

# Print the translated text
print(f'Translated text: {translated_text}')