# FutureNetwork 📊💻

Sistema de monitoramento ambiental para espaços de trabalho remoto (Home Office), construído sobre **ESP32** e sensores de temperatura, umidade, distância e movimento. A solução busca apoiar empresas e colaboradores na criação de ambientes mais saudáveis, produtivos e sustentáveis, oferecendo um *dashboard* em tempo real para acompanhamento das condições do local.

---

## 👥 Integrantes do Grupo

- Kaio Victtor Santos Andrade Galvão – RM566536

---

## 🧩 Descrição do Desafio

Com a expansão do modelo de trabalho remoto, empresas perdem visibilidade sobre as condições físicas do ambiente onde o colaborador atua: conforto térmico, ergonomia e presença. O **FutureNetwork** entrega uma interface simples para acompanhar temperatura, umidade, distância (posição/base de ergonomia) e detecção de movimento. A coleta é feita por um sensor ultrassônico e um sensor DHT (11 ou 22, dependendo do ambiente/simulador), integrados ao ESP32 e exibidos no navegador. 

Objetivos principais:
- Aumentar segurança e bem-estar do colaborador remoto.
- Facilitar auditoria leve das condições do posto de trabalho.
- Centralizar sinais em um painel acessível e responsivo.

---

### Funcionalidades Implementadas

- Captura contínua de temperatura (°C) e umidade (%) via DHT11/DHT22.
- Leitura de distância para detectar posição ou aproximação (sensor ultrassônico).
- Detecção de movimento simples para inferir presença/atividade.
- Dashboard web estático (HTML + CSS) com atualização periódica.
- Suporte a simulação no Wokwi para validação rápida.
- Estrutura pronta para expansão com outros atributos (ex.: luminosidade, ruído).

---

## 💡 Solução Técnica

O sistema foi montado fisicamente em protoboard e também validado em ambiente de simulação usando **Wokwi**, permitindo testes rápidos sem dependência exclusiva do hardware local.

### 🔗 Simulação no Wokwi
[Acesse o projeto](https://wokwi.com/projects/448288332963268609)

### 🎬 Vídeo Explicativo
> [Clique aqui para assistir](https://youtu.be/0isYhB47jRg)

---

## 🔧 Componentes Utilizados

| Item | Descrição |
|------|-----------|
| 1 × ESP32 | Microcontrolador principal |
| 1 × Sensor Ultrassônico | Medição de distância/presença |
| 1 × Sensor DHT11/DHT22 | Temperatura e umidade |
| Protoboard + Jumpers | Conexões físicas |
| Fonte USB + Cabo | Alimentação do ESP32 |

---

## 🛠️ Montagem do Circuito (Resumo)

- Sensor Ultrassônico: pinos digitais 17 (trigger) e 35 (echo) conforme código utilizado.
- Sensor DHT (11/22): pino digital 16.
- Trilhos laterais: distribuição de GND e 3V3 para estabilidade.
- Recomenda-se resistores ou estabilização conforme datasheet (quando aplicável). 

---

## 💾 Execução do Projeto

1. Monte o circuito seguindo o esquema (físico ou simulado).
2. Abra o código na IDE apropriada (Arduino IDE / PlatformIO).
3. Ajuste pinos se necessário para o seu hardware.
4. Faça o upload para o ESP32.
5. Abra o *dashboard* (`index.html`) em seu navegador para visualizar os dados.
6. No Wokwi, ajuste controles virtuais (temperatura/umidade/distância) para validar mudanças.
7. Verifique atualização periódica dos valores e presença (movimento) no painel.
8. (Opcional) Estenda o código para novos sensores.

---

## ⚠️ Observações

Este repositório foca em demonstração educacional. Para uso corporativo mais amplo, recomenda-se adicionar criptografia de comunicação, autenticação e persistência segura dos dados.

