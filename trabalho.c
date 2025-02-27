#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 500

struct info {
    char data[11];
    float tempmedia;
    float precipi;
    float umid;
    float vel_vento;
};

void exibirMenu() {
    printf("\n========== MENU =========="
           "\n1. Registrar dados climaticos"
           "\n2. Gerar relatorios mensais"
           "\n3. Gerar relatorios anuais"
           "\n4. Detectar comportamentos fora do padrao"
           "\n5. Encerrar programa"
           "\n=========================="
           "\nEscolha uma opcao: ");
}

struct info* registrardados(struct info *info_clima, int *quantidade, FILE *dados_c) {
    struct info *locar = realloc(info_clima, (*quantidade + 1) * sizeof(struct info));
    if (locar == NULL) {
        printf("Erro ao alocar memoria!\n");
        return info_clima;
    }
    info_clima = locar;
    printf("Data(dd/mm/aaaa): ");
    fgets(info_clima[*quantidade].data, 11, stdin);
    printf("Temperatura media: ");
    scanf("%f", &info_clima[*quantidade].tempmedia);
    getchar();
    printf("Precipitacao: ");
    scanf("%f", &info_clima[*quantidade].precipi);
    getchar();
    printf("Umidade: ");
    scanf("%f", &info_clima[*quantidade].umid);
    getchar();
    printf("Velocidade do vento(km/h): ");
    scanf("%f", &info_clima[*quantidade].vel_vento);
    getchar();
    fprintf(dados_c, "Data: %s\nTemperatura M\u00E9dia: %.2f°C\nPrecipitacao: %.1f mm\nUmidade: %.2f%%\nVelocidade do vento: %.2f km/h\n\n",
            info_clima[*quantidade].data, info_clima[*quantidade].tempmedia, info_clima[*quantidade].precipi,
            info_clima[*quantidade].umid, info_clima[*quantidade].vel_vento);
    (*quantidade)++;
    return info_clima;
}

void gerarrelatoriosmensais(struct info *info_clima, int *quantidade, FILE *relatorios_mensais) {
    if (*quantidade == 0) {
        printf("Nao ha nenhum dado para que o relatorio seja gerado.\n");
        return;
    }
    float soma_temp = 0, soma_precip = 0, soma_umid = 0, soma_vento = 0;
    int dias_no_mes = 0;
    char mes_atual[3] = "";
    char ano_atual[5] = "";
    printf("Gerando relatorio mensal...\n");
    fprintf(relatorios_mensais, "***********************\nRelatorio Mensal de Dados Climaticos\n***********************\n\n");
    for (int i = 0; i < *quantidade; i++) {
        char mes[3], ano[5];
        strncpy(mes, info_clima[i].data + 3, 2);
        mes[2] = '\0';
        strncpy(ano, info_clima[i].data + 6, 4);
        ano[4] = '\0';
        if (strcmp(mes_atual, mes) != 0 || strcmp(ano_atual, ano) != 0) {
            if (dias_no_mes > 0){
                fprintf(relatorios_mensais, "Mes: %s/%s\n", mes_atual, ano_atual);
                fprintf(relatorios_mensais, "Temperatura Media: %.2f°C\n", soma_temp / dias_no_mes);
                fprintf(relatorios_mensais, "Precipitacao Acumulada: %.1f mm\n", soma_precip);
                fprintf(relatorios_mensais, "Umidade Media: %.2f%%\n", soma_umid / dias_no_mes);
                fprintf(relatorios_mensais, "Velocidade Media do Vento: %.2f km/h\n\n", soma_vento / dias_no_mes);
            }
            strcpy(mes_atual, mes);
            strcpy(ano_atual, ano);
            soma_temp = soma_precip = soma_umid = soma_vento = 0;
            dias_no_mes = 0;
        }
        soma_temp += info_clima[i].tempmedia;
        soma_precip += info_clima[i].precipi;
        soma_umid += info_clima[i].umid;
        soma_vento += info_clima[i].vel_vento;
        dias_no_mes++;
    }
    if (dias_no_mes > 0){
        fprintf(relatorios_mensais, "Mes: %s/%s\n", mes_atual, ano_atual);
        fprintf(relatorios_mensais, "Temperatura Media: %.2f°C\n", soma_temp / dias_no_mes);
        fprintf(relatorios_mensais, "Precipitacao Acumulada: %.1f mm\n", soma_precip);
        fprintf(relatorios_mensais, "Umidade Media: %.2f%%\n", soma_umid / dias_no_mes);
        fprintf(relatorios_mensais, "Velocidade Media do Vento: %.2f km/h\n\n", soma_vento / dias_no_mes);
    }
    printf("Relatorio mensal gerado com sucesso e salvo em 'relatorios_mensais.txt'.\n");
}

void gerarrelatoriosanuais(struct info *info_clima, int *quantidade, FILE *relatorios_anuais) {
    if (*quantidade == 0) {
        printf("Nao ha nenhum dado para que o relatorio seja gerado.\n");
        return;
    }
    float soma_temp = 0, soma_precip = 0, soma_umid = 0, soma_vento = 0;
    int dias_no_ano = 0;
    char ano_atual[5] = "";
    float variacao_temp = 0, variacao_precip = 0, variacao_umid = 0;
    int anos_contados = 0;
    printf("Gerando relatorio anual...\n");
    fprintf(relatorios_anuais, "***********************\nRelatorio Anual de Dados Climaticos\n***********************\n\n");
    for (int i = 0; i < *quantidade; i++) {
        char ano[5];
        strncpy(ano, info_clima[i].data + 6, 4);
        ano[4] = '\0';
        if (strcmp(ano_atual, ano) != 0){
            if (dias_no_ano > 0) {
                fprintf(relatorios_anuais, "Ano: %s\n", ano_atual);
                fprintf(relatorios_anuais, "Temperatura Media: %.2f°C\n", soma_temp / dias_no_ano);
                fprintf(relatorios_anuais, "Precipitacao Acumulada: %.1f mm\n", soma_precip);
                fprintf(relatorios_anuais, "Umidade Media: %.2f%%\n", soma_umid / dias_no_ano);
                fprintf(relatorios_anuais, "Velocidade Media do Vento: %.2f km/h\n\n", soma_vento / dias_no_ano);

                if (anos_contados > 0) {
                    variacao_temp += (soma_temp / dias_no_ano) - variacao_temp;
                    variacao_precip += (soma_precip / dias_no_ano) - variacao_precip;
                    variacao_umid += (soma_umid / dias_no_ano) - variacao_umid;
                }
                anos_contados++;
            }
            strcpy(ano_atual, ano);
            soma_temp = soma_precip = soma_umid = soma_vento = 0;
            dias_no_ano = 0;
        }
        soma_temp += info_clima[i].tempmedia;
        soma_precip += info_clima[i].precipi;
        soma_umid += info_clima[i].umid;
        soma_vento += info_clima[i].vel_vento;
        dias_no_ano++;
    }
    if (dias_no_ano > 0){
        fprintf(relatorios_anuais, "Ano: %s\n", ano_atual);
        fprintf(relatorios_anuais, "Temperatura Media: %.2f°C\n", soma_temp / dias_no_ano);
        fprintf(relatorios_anuais, "Precipitacao Acumulada: %.1f mm\n", soma_precip);
        fprintf(relatorios_anuais, "Umidade Media: %.2f%%\n", soma_umid / dias_no_ano);
        fprintf(relatorios_anuais, "Velocidade Media do Vento: %.2f km/h\n\n", soma_vento / dias_no_ano);

        if (anos_contados > 0){
            variacao_temp += (soma_temp / dias_no_ano) - variacao_temp;
            variacao_precip += (soma_precip / dias_no_ano) - variacao_precip;
            variacao_umid += (soma_umid / dias_no_ano) - variacao_umid;
        }
        anos_contados++;
    }
    if (anos_contados > 1){
        fprintf(relatorios_anuais, "Tendencias Climaticas:\n");
        fprintf(relatorios_anuais, "Variacao Media da Temperatura: %.2f°C/ano\n", variacao_temp / (anos_contados - 1));
        fprintf(relatorios_anuais, "Variacao Media da Precipitacao: %.2f mm/ano\n", variacao_precip / (anos_contados - 1));
        fprintf(relatorios_anuais, "Variacao Media da Umidade: %.2f%%/ano\n\n", variacao_umid / (anos_contados - 1));
    }
    printf("Relatorio anual gerado com sucesso e salvo em 'relatorios_anuais.txt'.\n");
}

void detectar_comportamentos_fora_padrao(struct info *info_clima, int quantidade, char *localizacao) {
    if (quantidade == 0) {
        printf("Nao ha dados para detectar anomalias.\n");
        return;
    }
    int opcao;
    printf("Detectar anomalias por:\n1. Mes\n2. Ano\nEscolha uma opcao: ");
    scanf("%d", &opcao);
    getchar();
    FILE *anomalias = fopen("anomalias.txt", "w");
    if (anomalias == NULL) {
        printf("Erro ao abrir o arquivo anomalias.txt!\n");
        return;
    }
    fprintf(anomalias, "***********************\nEstado: %s\n***********************\n\n", localizacao);
    if (opcao == 1) {
        float soma_temp = 0, soma_precip = 0, soma_umid = 0, soma_vento = 0;
        int dias_no_mes = 0;
        char mes_atual[3] = "";
        char ano_atual[5] = "";
        int encontrou = 0;
        for (int i = 0; i < quantidade; i++){
            char mes[3], ano[5];
            strncpy(mes, info_clima[i].data + 3, 2);
            mes[2] = '\0';
            strncpy(ano, info_clima[i].data + 6, 4);
            ano[4] = '\0';   
            if (strcmp(mes_atual, mes) != 0 || strcmp(ano_atual, ano) != 0) {
                if (dias_no_mes > 0){
                    float media_temp = soma_temp / dias_no_mes;
                    float media_umid = soma_umid / dias_no_mes;
                    float media_vento = soma_vento / dias_no_mes;
                    int anomalia = 0;
                    char lista[200] = "";
                    if (media_temp > 30){
                        strcat(lista, "Temperatura alta; ");
                        anomalia = 1;
                    }
                    if (media_umid < 30){
                        strcat(lista, "Umidade baixa; ");
                        anomalia = 1;
                    }
                    if (soma_precip < 2.5){
                        strcat(lista, "Precipitacao baixa; ");
                        anomalia = 1;
                    }
                    if (media_vento > 51){
                        strcat(lista, "Vento elevado; ");
                        anomalia = 1;
                    }
                    if (anomalia){
                        fprintf(anomalias, "Mes: %s/%s\n", mes_atual, ano_atual);
                        fprintf(anomalias, "Temperatura Media: %.2f°C\n", media_temp);
                        fprintf(anomalias, "Precipitacao Acumulada: %.2f mm\n", soma_precip);
                        fprintf(anomalias, "Umidade Media: %.2f%%\n", media_umid);
                        fprintf(anomalias, "Velocidade Media do Vento: %.2f km/h\n", media_vento);
                        fprintf(anomalias, "Anomalias: %s\n\n", lista);
                        encontrou = 1;
                    }
                }
                strcpy(mes_atual, mes);
                strcpy(ano_atual, ano);
                soma_temp = soma_precip = soma_umid = soma_vento = 0;
                dias_no_mes = 0;
            }
            soma_temp += info_clima[i].tempmedia;
            soma_precip += info_clima[i].precipi;
            soma_umid += info_clima[i].umid;
            soma_vento += info_clima[i].vel_vento;
            dias_no_mes++;
        }
        if (dias_no_mes > 0){
            float media_temp = soma_temp / dias_no_mes;
            float media_umid = soma_umid / dias_no_mes;
            float media_vento = soma_vento / dias_no_mes;
            int anomalia = 0;
            char lista[200] = "";
            if (media_temp > 30){
                strcat(lista, "Temperatura alta; ");
                anomalia = 1;
            }
            if (media_umid < 30){
                strcat(lista, "Umidade baixa; ");
                anomalia = 1;
            }
            if (soma_precip < 2.5){
                strcat(lista, "Precipitacao baixa; ");
                anomalia = 1;
            }
            if (media_vento > 51){
                strcat(lista, "Vento elevado; ");
                anomalia = 1;
            }
            if (anomalia){
                fprintf(anomalias, "Mes: %s/%s\n", mes_atual, ano_atual);
                fprintf(anomalias, "Temperatura Media: %.2f°C\n", media_temp);
                fprintf(anomalias, "Precipitacao Acumulada: %.2f mm\n", soma_precip);
                fprintf(anomalias, "Umidade Media: %.2f%%\n", media_umid);
                fprintf(anomalias, "Velocidade Media do Vento: %.2f km/h\n", media_vento);
                fprintf(anomalias, "Anomalias: %s\n\n", lista);
                encontrou = 1;
            }
        }
        if (!encontrou)
            fprintf(anomalias, "Nenhum comportamento fora do padrao detectado para o agrupamento mensal.\n");
        printf("Deteccao mensal concluida. Resultados salvos em 'anomalias.txt'.\n");

    } else if (opcao == 2){
        float soma_temp = 0, soma_precip = 0, soma_umid = 0, soma_vento = 0;
        int dias_no_ano = 0;
        char ano_atual[5] = "";
        int encontrou = 0;
        for (int i = 0; i < quantidade; i++){
            char ano[5];
            strncpy(ano, info_clima[i].data + 6, 4);
            ano[4] = '\0';
            if (strcmp(ano_atual, ano) != 0){
                if (dias_no_ano > 0) {
                    float media_temp = soma_temp / dias_no_ano;
                    float media_umid = soma_umid / dias_no_ano;
                    float media_vento = soma_vento / dias_no_ano;
                    int anomalia = 0;
                    char lista[200] = "";
                    if (media_temp > 30) {
                        strcat(lista, "Temperatura alta; ");
                        anomalia = 1;
                    }
                    if (media_umid < 30) {
                        strcat(lista, "Umidade baixa; ");
                        anomalia = 1;
                    }
                    if (soma_precip < 2.5) {
                        strcat(lista, "Precipitacao baixa; ");
                        anomalia = 1;
                    }
                    if (media_vento > 51) {
                        strcat(lista, "Vento elevado; ");
                        anomalia = 1;
                    }
                    if (anomalia) {
                        fprintf(anomalias, "Ano: %s\n", ano_atual);
                        fprintf(anomalias, "Temperatura Media: %.2f°C\n", media_temp);
                        fprintf(anomalias, "Precipitacao Acumulada: %.2f mm\n", soma_precip);
                        fprintf(anomalias, "Umidade Media: %.2f%%\n", media_umid);
                        fprintf(anomalias, "Velocidade Media do Vento: %.2f km/h\n", media_vento);
                        fprintf(anomalias, "Anomalias: %s\n\n", lista);
                        encontrou = 1;
                    }
                }
                strcpy(ano_atual, ano);
                soma_temp = soma_precip = soma_umid = soma_vento = 0;
                dias_no_ano = 0;
            }
            soma_temp += info_clima[i].tempmedia;
            soma_precip += info_clima[i].precipi;
            soma_umid += info_clima[i].umid;
            soma_vento += info_clima[i].vel_vento;
            dias_no_ano++;
        }
        if (dias_no_ano > 0) {
            float media_temp = soma_temp / dias_no_ano;
            float media_umid = soma_umid / dias_no_ano;
            float media_vento = soma_vento / dias_no_ano;
            int anomalia = 0;
            char lista[200] = "";
            if (media_temp > 30) {
                strcat(lista, "Temperatura alta; ");
                anomalia = 1;
            }
            if (media_umid < 30) {
                strcat(lista, "Umidade baixa; ");
                anomalia = 1;
            }
            if (soma_precip < 2.5) {
                strcat(lista, "Precipitacao baixa; ");
                anomalia = 1;
            }
            if (media_vento > 51) {
                strcat(lista, "Vento elevado; ");
                anomalia = 1;
            }
            if (anomalia) {
                fprintf(anomalias, "Ano: %s\n", ano_atual);
                fprintf(anomalias, "Temperatura Media: %.2f°C\n", media_temp);
                fprintf(anomalias, "Precipitacao Acumulada: %.2f mm\n", soma_precip);
                fprintf(anomalias, "Umidade Media: %.2f%%\n", media_umid);
                fprintf(anomalias, "Velocidade Media do Vento: %.2f km/h\n", media_vento);
                fprintf(anomalias, "Anomalias: %s\n\n", lista);
                encontrou = 1;
            }
        }
        if(!encontrou)
            fprintf(anomalias, "Nenhum comportamento fora do padrao detectado para o agrupamento anual.\n");
        printf("Deteccao anual concluida. Resultados salvos em 'anomalias.txt'.\n");   
    } else{
        printf("Opcao invalida!\n");
    }
    fclose(anomalias);
}

void processaMenu(struct info **info_clima, int *quantidade, FILE *dados_c, FILE *relatorios_mensais, FILE *relatorios_anuais, char *localizacao) {
    int opcao;
    exibirMenu();
    scanf("%d", &opcao);
    getchar();
    switch (opcao){
        case 1:
            *info_clima = registrardados(*info_clima, quantidade, dados_c);
            break;
        case 2:
            gerarrelatoriosmensais(*info_clima, quantidade, relatorios_mensais);
            break;
        case 3:
            gerarrelatoriosanuais(*info_clima, quantidade, relatorios_anuais);
            break;
        case 4:
            detectar_comportamentos_fora_padrao(*info_clima, *quantidade, localizacao);
            break;
        case 5:
            printf("Encerrando programa...\n");
            fclose(dados_c);
            fclose(relatorios_mensais);
            fclose(relatorios_anuais);
            free(*info_clima);
            return;
        default:
            printf("Opcao invalida! Tente novamente.\n");
    }
    processaMenu(info_clima, quantidade, dados_c, relatorios_mensais, relatorios_anuais, localizacao);
}

int main(){
    struct info *info_clima = NULL;
    char localizacao[MAX + 1];
    int quantidade = 0;
    FILE *dados_c;
    FILE *relatorios_mensais;
    FILE *relatorios_anuais;
    
    dados_c = fopen("dados_climaticos.txt", "w");
    if (dados_c == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    relatorios_mensais = fopen("relatorios_mensais.txt", "w");
    if (relatorios_mensais == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    relatorios_anuais = fopen("relatorios_anuais.txt", "w");
    if (relatorios_anuais == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    printf("Digite o nome do estado para analise climatica: ");
    fgets(localizacao, MAX + 1, stdin);
    fprintf(dados_c, "***********************\nEstado: %s\n***********************\n\n", localizacao);
    fprintf(relatorios_mensais, "***********************\nEstado: %s\n***********************\n\n", localizacao);
    fprintf(relatorios_anuais, "***********************\nEstado: %s\n***********************\n\n", localizacao);
    processaMenu(&info_clima, &quantidade, dados_c, relatorios_mensais, relatorios_anuais, localizacao);
    return 0;
}