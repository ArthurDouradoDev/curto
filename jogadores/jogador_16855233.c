#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogador_16855233.h" // Substitua 16855233 pelo seu número USP
#include "../baralho.h"

// Variáveis globais para armazenar o estado do jogador
static int id;
static int num_jogadores;
static Carta mao[6];
static int num_cartas;
static int aposta_atual = 0;
static Valor manilha_atual;
static int vitorias_atuais = 0;

// Função para comparar cartas (assumindo que existe externamente)
extern int comparar_cartas(Carta a, Carta b, Valor manilha);

const char* nome_jogador_16855233() {
    return "Campeão 16855233"; // Substitua pelo seu nome/número
}

void iniciar_16855233(int meu_id, int total_jogadores) {
    id = meu_id;
    num_jogadores = total_jogadores;
}

void nova_rodada_16855233(int rodada, Carta carta_virada, Valor manilha, int n_cartas, Carta* minha_mao) {
    num_cartas = n_cartas;
    manilha_atual = manilha;
    vitorias_atuais = 0;
    
    for (int i = 0; i < n_cartas; i++) {
        mao[i] = minha_mao[i];
    }
}

// Função para avaliar força de uma carta (0-100)
int avaliar_forca_carta(Carta carta) {
    if (carta.valor == manilha_atual) {
        // Manilhas: força base 80 + bônus por naipe
        return 80 + carta.naipe * 5; // Ouros=80, Espadas=85, Copas=90, Paus=95
    }
    
    // Cartas normais: força baseada no valor
    switch(carta.valor) {
        case TRES: return 75;  // Mais forte das normais
        case DOIS: return 70;
        case AS: return 65;
        case REI: return 60;
        case VALETE: return 55;
        case DAMA: return 50;
        case SETE: return 45;
        case SEIS: return 40;
        case CINCO: return 35;
        case QUATRO: return 30; // Mais fraca
        default: return 0;
    }
}

// Função para estimar probabilidade de vencer com uma carta
double probabilidade_vitoria(Carta carta, int cartas_ja_jogadas) {
    int forca = avaliar_forca_carta(carta);
    
    // Cartas muito fortes (manilhas altas) têm alta probabilidade
    if (forca >= 90) return 0.9;
    if (forca >= 85) return 0.8;
    if (forca >= 80) return 0.7; // Manilhas mais fracas
    
    // Cartas normais: probabilidade baseada na força e cartas restantes
    double prob_base = (double)forca / 100.0;
    
    // Ajusta baseado no número de jogadores e cartas já jogadas
    double fator_jogadores = 1.0 - (num_jogadores - 1) * 0.15;
    double fator_cartas_jogadas = 1.0 + cartas_ja_jogadas * 0.05;
    
    return prob_base * fator_jogadores * fator_cartas_jogadas;
}

int apostar_16855233() {
    int aposta = 0;
    
    // Analisa cada carta e sua probabilidade de vitória
    for (int i = 0; i < num_cartas; i++) {
        double prob = probabilidade_vitoria(mao[i], 0);
        
        // Se probabilidade > 60%, conta como vitória provável
        if (prob > 0.6) {
            aposta++;
        }
        // Se probabilidade entre 40-60%, conta metade do tempo
        else if (prob > 0.4 && aposta < num_cartas / 2) {
            aposta++;
        }
    }
    
    // Ajustes estratégicos
    // Em rodadas iniciais (poucas cartas), seja mais conservador
    if (num_cartas <= 3 && aposta > 0) {
        aposta = aposta * 0.8; // Reduz em 20%
    }
    
    // Em rodadas finais, pode ser mais agressivo se necessário
    if (num_cartas >= 5) {
        // Se tem muitas cartas médias, pode apostar um pouco mais
        int cartas_medias = 0;
        for (int i = 0; i < num_cartas; i++) {
            int forca = avaliar_forca_carta(mao[i]);
            if (forca >= 50 && forca < 80) cartas_medias++;
        }
        if (cartas_medias >= 3) aposta++;
    }
    
    // Garante que aposta não excede número de cartas
    if (aposta > num_cartas) aposta = num_cartas;
    
    aposta_atual = aposta;
    return aposta;
}

// Encontra a melhor carta para jogar baseado na situação da mesa
int encontrar_melhor_carta(Carta* mesa, int num_na_mesa) {
    int melhor_idx = -1;
    int melhor_score = -1000;
    
    for (int i = 0; i < num_cartas; i++) {
        if (carta_foi_usada(mao[i])) continue;
        
        int score = 0;
        int pode_ganhar = 1;
        
        // Verifica se pode ganhar da mesa atual
        for (int j = 0; j < num_na_mesa; j++) {
            if (comparar_cartas(mao[i], mesa[j], manilha_atual) <= 0) {
                pode_ganhar = 0;
                break;
            }
        }
        
        int forca = avaliar_forca_carta(mao[i]);
        
        // Estratégia baseada em se precisa ou não de mais vitórias
        if (vitorias_atuais < aposta_atual) {
            // Precisa vencer mais mãos
            if (pode_ganhar) {
                score = 1000 - forca; // Prefere carta mais fraca que ainda ganha
            } else {
                score = -forca; // Se não pode ganhar, joga carta mais fraca
            }
        } else {
            // Já tem vitórias suficientes, quer perder
            if (!pode_ganhar) {
                score = forca; // Prefere carta mais forte que perde (economiza fracas)
            } else {
                score = -1000 + forca; // Se vai ganhar, pelo menos usa carta forte
            }
        }
        
        // Bônus para jogar primeiro (se mesa vazia) com cartas médias
        if (num_na_mesa == 0 && forca >= 45 && forca <= 65) {
            score += 50;
        }
        
        if (score > melhor_score) {
            melhor_score = score;
            melhor_idx = i;
        }
    }
    
    return melhor_idx;
}

int jogar_16855233(Carta* mesa, int num_na_mesa) {
    int idx = encontrar_melhor_carta(mesa, num_na_mesa);
    
    // Se não encontrou carta (erro), joga primeira disponível
    if (idx == -1) {
        for (int i = 0; i < num_cartas; i++) {
            if (!carta_foi_usada(mao[i])) {
                idx = i;
                break;
            }
        }
    }
    
    // Marca carta como usada
    mao[idx] = USADA;
    
    return idx;
}