#ifndef YA_ORDER_ITEM_H
#define YA_ORDER_ITEM_H

/**
 * 商品，提供序列化和反序列化
 */
class Item {
public:
    int id;
    int price;
    int amount;
public:
    Item(int id, int price, int amount);

    Item();
};

#endif //YA_ORDER_ITEM_H
