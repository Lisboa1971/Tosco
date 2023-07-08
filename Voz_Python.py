import pyttsx3

# Inicializar o mecanismo de síntese de voz
engine = pyttsx3.init()

# Definir a língua para português
engine.setProperty('voice', 'pt-br')

def responder_voz(texto):
    # Definir o texto a ser falado
    engine.say(texto)

    # Aguardar a finalização da fala
    engine.runAndWait()

# Exemplo de uso
responder_voz("Olá, Mano! Sou o GPT aqui. Como posso ajudar você hoje?")

