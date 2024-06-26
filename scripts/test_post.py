import requests

# URL del server destinazione
url = 'http://example.com/post_endpoint'

# Dati da inviare tramite POST
data = {
    'username': 'user123',
    'password': 'password123',
    'email': 'user123@example.com'
}

# Invio della richiesta POST
response = requests.post(url, data=data)

# Verifica dello stato della risposta
if response.status_code == 200:
    print('Dati inviati con successo!')
    print('Risposta dal server:')
    print(response.text)
else:
    print(f'Si è verificato un errore durante l\'invio dei dati: {response.status_code}')
    print('Risposta dal server:')
    print(response.text)
