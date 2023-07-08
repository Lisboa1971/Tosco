import speech_recognition as sr
import pyttsx3
import openai

# Inicializar o mecanismo de síntese de voz
engine = pyttsx3.init()

# Definir a língua para português brasileiro
engine.setProperty('voice', 'pt-br')

# Definir as credenciais da API do OpenAI
openai.api_key = "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"  # Substitua com sua chave de API do OpenAI

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
        print("Erro na solicitação ao serviço de reconhecimento de voz; {0}".format(e))

# Função para responder em voz
def responder_voz(texto):
    # Definir o texto a ser falado
    engine.say(texto)

    # Aguardar a finalização da fala
    engine.runAndWait()

# Função para enviar uma mensagem para o modelo ChatGPT e obter uma resposta
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

# Loop principal do programa
while True:
    # Realizar o reconhecimento de voz
    texto = reconhecer_voz()

    # Verificar se algum texto foi reconhecido
    if texto:
        print("Você:", texto)

        # Obter resposta do modelo ChatGPT
        resposta = obter_resposta(texto)

        # Exibir a resposta no console
        print("GPT:", resposta)

        # Responder em voz
        resposta_voz = ": " + resposta
        responder_voz(resposta_voz)
