#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "../me/me.h"

#define TAM 200

int64_t min = std::numeric_limits<int64_t>::min();
int64_t max = std::numeric_limits<int64_t>::max();

int main() {
  double a;
  double maior = 0;
  std::vector<double> v;
  for (int i = 0; i < TAM; i++) {
    std::cin >> a;
    v.push_back(a);
    maior = std::max(maior, a);
  }

  for (int i = 0; i < TAM; i++) {
    v[i] = v[i] / maior;
    std::cout << v[i] << " ";
  }
  std::uniform_int_distribution<int64_t> distrib(min, max);
  std::random_device rd;
  std::mt19937 gen(rd());

  std::set<int64_t> security_ids;
  std::set<int64_t> order_ids;

  MeMessage r[TAM];

  int64_t security_id;
  int64_t order_id;
  int64_t time = 0;
  short price;
  int qnt;

  // loop de ordens
  for (int i = 0; i < TAM; i++) {
    /*define side*/
    double ramdom_number = (distrib(gen) % (10000 + 1)) / 10000.0;
    double chance = ramdom_number * v[i] + 0.1;
    if (chance < 0.2) {
      r[i].message.order.side = ME_SIDE_SELL;
      std::cout << "Venda " << std::endl;
    } else if (chance < 0.5) {
      std::cout << "Nada" << std::endl;
      continue;
    } else {
      std::cout << "Compra" << std::endl;
      r[i].message.order.side = ME_SIDE_BUY;
    }
    /*making random ids*/
    do {
      security_id = distrib(gen);
    } while (security_ids.find(security_id) != security_ids.end());
    do {
      order_id = distrib(gen);
    } while (order_ids.find(order_id) != order_ids.end());

    r[i].security_id = security_id;
    r[i].message.order.order_id = order_id;

    /*define message type and time*/
    r[i].message.order.timestamp = time++;
    r[i].msg_type = ME_MESSAGE_NEW_ORDER;

    /*define price and quantity*/
    price = distrib(gen) % 200;
    r[i].message.order.price = price;

    qnt = distrib(gen) % (100000 / price);
    r[i].message.order.quantity = qnt;

    /*define order type*/
    short order_type = (distrib(gen) % 2);
    if (order_type == 0) {
      r[i].message.order.ord_type = ME_ORDER_LIMIT;
    } else {
      r[i].message.order.ord_type = ME_ORDER_MARKET;
    }
  }
}
