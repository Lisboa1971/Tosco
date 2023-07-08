import speech_recognition as sr
import pyttsx3

# Inicializar o mecanismo de síntese de voz
engine = pyttsx3.init()

# Definir a língua para português brasileiro
engine.setProperty('voice', 'pt-br')

# Função para realizar o reconhecimento de voz
def reconhecer_voz():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Diga algo...")
        audio = r.listen(source)

    try:
        print("Processando...")
        texto = r.recognize_google(audio, language='pt-BR')
        return texto
    except sr.UnknownValueError:
        print("Não foi possível reconhecer o áudio")
    except sr.RequestError as e:
        print("Erro na solicitação ao serviço de reconhecimento de voz; {0}".format(e))

# Função para responder em voz
def responder_voz(texto):
    # Definir o texto a ser falado
    engine.say(texto)

    # Aguardar a finalização da fala
    engine.runAndWait()

# Loop principal do programa
while True:
    # Realizar o reconhecimento de voz
    texto = reconhecer_voz()

    # Verificar se algum texto foi reconhecido
    if texto:
        print("Você disse:", texto)
        # Responder em voz
        resposta = "Você disse: " + texto
        responder_voz(resposta)
