#include<iostream>
#include<random>

#define TAM  200

int main(){
    double a;
    double maior = 0;
    std::vector<double> v;
    for(int i = 0; i < TAM; i++){
        std::cin >> a;
        v.push_back(a);
        maior = std::max(maior,a);
    }

    for(int i = 0; i < TAM; i++){
        v[i] = v[i]/maior;
        std::cout << v[i] << " ";
    }

    //simulando o mercado
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<>  distrib(0,1000);
    
    // loop de ordens
    for(int i = 0; i < TAM; i++){    
        double ramdom_number = distrib(gen)/1000.000;
        double probability = ramdom_number*v[i] + 0.1;
        short  choice;
        if(probability < 0.2)
            std::cout << "Venda" << std::endl;
        else  if( probability < 0.4)
            std::cout << "Nada" << std::endl;
        else
            std::cout << "Compra" << std::endl;
    }
}
