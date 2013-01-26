#include "Basic.hh"
using namespace std;

void printPonto(Vec3f p);

void printAmostras( vector<Vec3f>& amostras);

double distanciaEuclidiana(Vec3f amostra,Vec3f centroide);

int menorDistColuna(double **m,int coluna,int k);

void matrizCopy(double **m1,double **m2,int tam_amostra,int k);

int matrizInvariante(double **m1,double **m2,int tam_amostra,int k);

void initAmostras(vector<Vec3f>& amostras,Vec3f p);

double **mat(int lin,int col);

double ** kmeans_main(vector<Vec3f>& amostras,int k);

