import speech_recognition as sr
import pyttsx3
import requests
import openai

# Inicializar o mecanismo de síntese de voz
engine = pyttsx3.init()

# Definir a língua para português brasileiro
engine.setProperty('voice', 'pt-br')

# Definir as credenciais da API do OpenAI
openai.api_key = "API"

# Função para realizar o reconhecimento de voz
def reconhecer_voz():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Fale")
        audio = r.listen(source)

    try:
        print("Recebi")
        texto = r.recognize_google(audio, language='pt-BR')
        return texto
    except sr.UnknownValueError:
        print("Não entendi")
    except sr.RequestError as e:
        print("Erro na solicitação ao serviço de reconhecimento de voz: {0}".format(e))

# Função para responder em voz
def responder_voz(texto):
    # Definir o texto a ser falado
    engine.say(texto)

    # Aguardar a finalização da fala
    engine.runAndWait()

# Função para enviar uma pergunta para o modelo GPT e obter uma resposta
def obter_resposta(texto):
    response = openai.Completion.create(
        engine="text-davinci-003",
        prompt=texto,
        max_tokens=50,
        temperature=0.8,
        frequency_penalty=0.2,
        presence_penalty=0.0
    )
    return response.choices[0].text.strip()

# URL do servidor web para controlar os relés
url_reles = "http://192.168.0.208/"

# Mapa dos comandos de voz para os relés
mapa_reles = {
    "quarto": 0,
    "sala": 1,
    "cozinha": 2,
    "deck": 3,
}

# Loop principal do programa
while True:
    # Realizar o reconhecimento de voz
    texto = reconhecer_voz()

    # Verificar se algum texto foi reconhecido
    if texto:
        print("Você:", texto)

        # Verificar se o texto corresponde a um comando de relé
        comando_rele = None
        for comando, rele in mapa_reles.items():
            if comando in texto.lower():  # Comparar em minúsculas para evitar problemas de correspondência de maiúsculas/minúsculas
                comando_rele = rele
                break

        if comando_rele is not None:
            # Enviar o comando para o servidor web
            url_comando = url_reles + f"toggle?relay={comando_rele}"
            response = requests.get(url_comando)

            # Exibir a resposta no console
            print("Casinha: Feito")

            # Responder em voz
            resposta_voz = "Beleza"
            responder_voz(resposta_voz)
        elif "liga tudo" in texto.lower():
            # Enviar comando para ligar todos os relés
            for rele in mapa_reles.values():
                url_comando = url_reles + f"toggle?relay={rele}&state=on"
                requests.get(url_comando)

            # Exibir a resposta no console
            print("Casinha: Todos os relés foram ligados")

            # Responder em voz
            resposta_voz = "Belezera"
            responder_voz(resposta_voz)
        elif "desliga tudo" in texto.lower():
            # Enviar comando para desligar todos os relés
            for rele in mapa_reles.values():
                url_comando = url_reles + f"toggle?relay={rele}&state=off"
                requests.get(url_comando)

            # Exibir a resposta no console
            print("Casinha: Beleza")

            # Responder em voz
            resposta_voz = "Tá feito"
            responder_voz(resposta_voz)
        else:
            # Obter resposta do modelo GPT
            resposta_gpt = obter_resposta(texto)

            # Exibir a resposta no console
            print("Casinha:", resposta_gpt)

            # Responder em voz
            responder_voz(resposta_gpt)
