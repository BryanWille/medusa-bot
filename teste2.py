from textblob import TextBlob

blob = TextBlob("hello")
blob = blob.translate(from_lang='en', to='pt')
print(blob)