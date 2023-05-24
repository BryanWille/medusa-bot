import requests
import json

# Set your API key and endpoint URL
api_key = 'AIzaSyBgCIKA1opotVstmkqtAOagYu3eGlF5faM'
url = 'https://translation.googleapis.com/language/translate/v2'

# Set the text you want to translate
text = 'Hello, world!'
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

# Extract the translated text from the response
translated_text = data['data']['translations'][0]['translatedText']

# Print the translated text
print(f'Translated text: {translated_text}')