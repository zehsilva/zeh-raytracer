#include "kmeans2.hh"

void printPonto(Vec3f p){
    cout << "[" << p.x << "," << p.y << "," << p.z << "]" << endl;
}

void printAmostras( vector<Vec3f>& amostras){
    int i=0;
    cout << "Total de primitivas: " << amostras.size() << endl;
    for(i=0;i<amostras.size();i++)
        printPonto(amostras[i]);
}

double distanciaEuclidiana(Vec3f amostra,Vec3f centroide){
    return sqrt((amostra.x - centroide.x)*(amostra.x - centroide.x) 
              + (amostra.y - centroide.y)*(amostra.y - centroide.y)
              + (amostra.z - centroide.z)*(amostra.z - centroide.z)
              );
}

int menorDistColuna(double **m,int coluna,int k){
    int i,rtn=0;
    double menor;
    menor = m[0][coluna];
    for(i=0;i<k;i++){

        if(menor > m[i][coluna]){
            menor = m[i][coluna];
            rtn=i;
        }
    }

    return rtn;
}

void matrizCopy(double **m1,double **m2,int tam_amostra,int k){
    int i,j;

    for(j=0;j<k;j++){
        for(i=0;i<tam_amostra;i++)
           m1[j][i]=m2[j][i];
    }
    return;
}

int matrizInvariante(double **m1,double **m2,int tam_amostra,int k){
    int i,j;

    for(j=0;j < k;j++){
        for(i=0;i < tam_amostra;i++)
            if(m1[j][i] != m2[j][i])
                return 0;
    }
    return 1;
}

void initAmostras(vector<Vec3f> &amostras,Vec3f p){
	amostras.push_back(p);
}

double **mat(int lin,int col){
    double **m = (double**)malloc(sizeof(double*)*col);
    for(int i = 0;i < col; i++){
	m[i] = (double*)malloc(sizeof(double)*lin);
    }

    return m;
}

double ** kmeans_main(vector<Vec3f> &amostras,int k){
    int *cent;
    int i,j;
    int temp;
    int soma=0;

    double menor_dist;

    Vec3f novo_cent[k];

    int tam_amostra = amostras.size();

    //printAmostras(amostras);

    double **m_dist = mat(amostras.size(),k);
    double **m_agrup = mat(amostras.size(),k);
    double **m_agrup_temp = mat(amostras.size(),k);

    for(i=0;i<k;i++)
        for(j=0;j<tam_amostra;j++)
            m_dist[i][j] = 0;

    for(i=0;i<k;i++)
        for(j=0;j<tam_amostra;j++)
            m_agrup[i][j] = 0;

    for(i=0;i<k;i++)
        for(j=0;j<tam_amostra;j++)
            m_agrup_temp[i][j] = 0;

    m_agrup_temp[k-1][tam_amostra-1]=1;

    //inicializando os centroides com osendo os primeiros da amostra
    for(i=0;i<k;i++){
        novo_cent[i] = amostras[i+9];
    }

    int c=0;
    while(!matrizInvariante(m_agrup_temp,m_agrup,tam_amostra,k)){
        c++;
        
        matrizCopy(m_agrup_temp,m_agrup,tam_amostra,k);
        //calculando matriz de distancia
        /*
        *   Retorna uma matriz do tipo
        *     A1 A2  A3   A4
        *   | 0  1  3.61  5    | grupo 0
        *   | 0  1  2.83  4.24 | grupo 1
        */
        for(j=0;j<k;j++){
            for(i=0;i<tam_amostra;i++){
                m_dist[j][i] = distanciaEuclidiana(amostras[i],novo_cent[j]); // os centroides sao os primeiros elementos da amostra
                //printf("====>%lf\n",m_dist[j][i]);
            }
        }

        //agrupacao
        /*
        *   Retorna uma matriz do tipo
        *   A1  A2  A3 A4
        *   | 1  0  0  0 | grupo 0
        *   | 0  1  1  1 | grupo 1
        */
        for(i=0;i<tam_amostra;i++){
            temp = menorDistColuna(m_dist,i,k);
            for(j=0;j<k;j++){
                if(j==temp)
                    m_agrup[j][i] = 1;
                else
                    m_agrup[j][i] = 0;
            }      
        }
        //printf("aqui\n");
        //for(j=0;j<k;j++){
        //    for(i=0;i<tam_amostra;i++)
        //        printf("%lf ",m_agrup[j][i]);
        //    printf("\n");
        //}
        //printf("---------------\n");
        //calcular os centroides de cada grupo
        for(i=0;i<k;i++){
            novo_cent[i].x = 0;
            novo_cent[i].y = 0;
            novo_cent[i].z = 0;
        }
        for(j=0;j<k;j++){
            soma=0;
            for(i=0;i<tam_amostra;i++){
                if(m_agrup[j][i]==1){
                    soma++;

                    //printPonto(novo_cent[j]);
                    //printPonto(amostras[j]);
                    novo_cent[j].x = novo_cent[j].x + amostras[i].x;
                    novo_cent[j].y = novo_cent[j].y + amostras[i].y;
                    novo_cent[j].z = novo_cent[j].z + amostras[i].z;
                }
            }
            if(soma==0)
                soma=1;
            novo_cent[j].x = (double)novo_cent[j].x/(soma);
            novo_cent[j].y = (double)novo_cent[j].y/(soma);
            novo_cent[j].z = (double)novo_cent[j].z/(soma);
        }

        /*printf("\n--------------------\n");
        for(j=0;j<k;j++){
            for(i=0;i<tam_amostra;i++)
                printf("%lf ",m_dist[j][i]);
            printf("\n");
        }
        printf("\n%lf,%lf,%d\n",novo_cent[1].x,novo_cent[1].y,soma);*/
    }
    

    /*for(i=0;i<tam_amostra;i++){
        for(j=0;j<k;j++)
            if(m_agrup[j][i]==1)
                printf("Amostra %d: %d\n",i,j);
    }*/

    return m_agrup;
}

