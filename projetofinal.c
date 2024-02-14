/*
 * File:  projetofinal.c
 * Author: Giovanna Amaral
 * Description: projeto de AED.
*/

#include<stdio.h> 
#include <string.h>

#define CMD_EXIT 'x'
#define CMD_ADD_PRODUCT 'a'
#define CMD_LIST_PRODUCT 'l'
#define CMD_ADD_STOCK 'q'
#define CMD_REMOVE_STOCK 'r'
#define CMD_CHANGE_PRICE 'p'
#define CMD_NEW_ORDER 'N'
#define CMD_LIST_ORDERS_ALL 'U'
#define CMD_LIST_ORDERS 'L'
#define CMD_ADD_PRODUCT_TO_ORDER 'A'
#define CMD_REMOVE_PRODUCT_FROM_ORDER 'R'
#define CMD_TOTAL_ORDER_PRICE 'C'
#define CMD_SEARCH_PRODUCT_IN_ORDER 'm'
#define CMD_LIST_PRODUCT_IN_ORDER 'E'

/* o maximo de carateres que uma descricao pode ter */
#define MAX_DESC_SIZE 64

/* o maximo de produtos que podem existir */
#define MAX_PRODUCTS 10000

/* o maximo de encomendas que podem ser feitas */
#define MAX_ORDERS 500

/* o peso maximo de uma encomenda */
#define MAX_ORDER_WEIGHT 200


/* define o tipo de data produto */
typedef struct product{  
    int id;
    char description[MAX_DESC_SIZE];                       
    int price;
    int weight;
    int stock;
} Product;


/* define este tipo de data para depois se poder ordenar */
typedef struct orderProduct{
    Product *product;
    int quantity;
} OrderProduct;


/* define o tipo de data encomenda */
typedef struct order{   
	int id;
	int weight;
    int numProducts;
	OrderProduct products[MAX_ORDER_WEIGHT];                                   
} Order;



/*Funcoes auxiliares com os algoritmos de ordenacao*/
void orderProductsMerge(Product products[], int left, int right, int middle)
{
    int size_of_ord1, size_of_ord2, i, j, k;
    Product ord1[MAX_PRODUCTS/2], ord2[MAX_PRODUCTS/2];
    /*dois arrays temporarios*/
    size_of_ord1 = (middle-left)+1;
    size_of_ord2 = (right-middle);

    for(i=0; i < size_of_ord1; i++)
    {
        ord1[i] = products[left+i];
    }

    for(i=0; i < size_of_ord2; i++)
    {
        ord2[i] = products[middle+1+i];
    }

    i=0;
    j=0;
    k=left;
    /*agora junta se os arrays, que ja estao ordenados. o array principal products
    vai ficar com o array que contem o elemento mais pequeno */
    while (i < size_of_ord1 && j < size_of_ord2)
    {
        if (ord1[i].price < ord2[j].price)
        {
            products[k] = ord1[i];
            i++;
        }
        else if (ord2[j].price < ord1[i].price)
        {
            products[k] = ord2[j];
            j++;
        }
        k++;
    }
    /* copiar os restantes elementos do ord1 para o array principal products*/
    while (i<size_of_ord1)
    {
        products[k] = ord1[i];
        k++;
        i++;
    }
    /* copiar os restantes elementos do ord2 para o array principal products*/
    while (j<size_of_ord2)
    {
        products[k] = ord2[j];
        k++;
        j++;
    }
}

void orderProductsMS(Product products[], int left, int right)
{
  if (left < right)
  {
    /* elemento do meio do array */
    int middle = (left+right)/2;

    orderProductsMS(products, left, middle);
    orderProductsMS(products, middle+1, right);

    /* chamamos a funcao que faz merge*/
    orderProductsMerge(products, left, right, middle);
  }
}


/* algoritmo de ordenacao baseado no quick sort */
void swap(OrderProduct* a, OrderProduct* b) 
{ 
    OrderProduct t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition(OrderProduct orderProducts[], int low, int high)
{
    OrderProduct pivot = orderProducts[high];    
    int i = (low - 1) ,j; 
  
    for (j = low; j <= high- 1; j++) 
    { 
        if (strcmp(orderProducts[j].product->description, pivot.product->description) < 0) 
        { 
            i++;   
            swap(&orderProducts[i], &orderProducts[j]); 
        } 
    } 
    swap(&orderProducts[i + 1], &orderProducts[high]); 
    return (i + 1); 
}

void orderOrderProductsMS(OrderProduct orderProducts[], int low, int high)
{
    if (low < high) 
    { 
        int pi = partition(orderProducts, low, high); 

        orderOrderProductsMS(orderProducts, low, pi - 1); 
        orderOrderProductsMS(orderProducts, pi + 1, high); 
    } 

}


/*funcao que vai ler a descricao do produto*/
void readDescription(char str[MAX_DESC_SIZE]){
    int i = 0;
    int ch;
    /*le o primeiro espaco*/
    getchar(); 
    while((ch = getchar()) != ':' && ch != EOF ){          
        if (i < MAX_DESC_SIZE) {
            str[i++] = ch;
        }
    }
    str[i] = '\0';
}


/* funcao que vai realizar o comando 'a', adiciona um novo produto ao sistema */
void addProduct(Product products[MAX_PRODUCTS], int id){   
    products[id].id = id;
    readDescription(products[id].description);

    scanf("%d:%d:%d", 
        &products[id].price,                                        
        &products[id].weight,
        &products[id].stock);

    printf("Novo produto %d.\n", id);
}


/* funcao que vai realizar o comando 'l', lista todos os produtos existentes no sistema por ordem crescente de preço */
void listProduct(Product products[MAX_PRODUCTS], int numProducts){
   int i;
   orderProductsMS(products, 0, numProducts - 1);
   printf("Produtos\n");
   for (i = 0; i < numProducts; i++)                                  
   {
       printf("* %s %d %d\n", 
            products[i].description,
            products[i].price,
            products[i].stock);
   }
}


/* funcao que vai realizar o comando 'q', adiciona stock a um produto existente no sistema */
void addStock(Product products[MAX_PRODUCTS], int numProducts){ 
    int id, qtd;

    scanf("%d:%d", &id, &qtd);
    
    if (id >= numProducts || id < 0)
    {
    	printf("Impossivel adicionar produto %d ao stock. Produto inexistente.\n", id);
    }
    else
    {
        /*adiciona o stock do produto identificado*/
    	products[id].stock = products[id].stock + qtd;
    }
}


/* funcao que vai realizar o comando 'r', remove stock a um produto existente no sistema */      
void removeStock(Product products[MAX_PRODUCTS], int numProducts){ 
	int id, qtd;

    scanf("%d:%d", &id, &qtd);
    /* o id nunca pode ser maior que o numero de produtos, isso h impossivel, daí a logica usada
       aqui e nas proximas funcoes tambem */
    if (id >= numProducts || id < 0)                       
    {
    	printf("Impossivel remover stock do produto %d. Produto inexistente.\n", id);
    }
    /* se a quantidade em stock for menor do que aquela que queremos remover, esta logica tambem
    sera usada em algumas das proximas funcoes */
    else if (products[id].stock < qtd)
    {
    	printf("Impossivel remover %d unidades do produto %d do stock. Quantidade insuficiente.\n", qtd, id);
    }
    else
    {
        /* remove o stock do produto identificado */
    	products[id].stock = products[id].stock - qtd;
    }
}


/* funcao que vai realizar o comando 'p', altera o preço de um produto existente no sistema */
void changePrice(Product products[MAX_PRODUCTS], int numProducts){ 
    int id, price;

	 scanf("%d:%d", &id, &price);

	 if (id >= numProducts || id < 0)
	 {
	 	printf("Impossivel alterar preco do produto %d. Produto inexistente.\n", id);
	 }
	 else 
	 {
        /* atualizacao do preco */
        products[id].price = price;
	 }
}


/* funcao que vai realizar o comando 'N', cria uma nova encomenda */
void newOrder(Order orders[MAX_ORDERS], int id){
    orders[id].id = id;
    orders[id].weight = 0;
    orders[id].numProducts = 0;
	printf("Nova encomenda %d.\n", id);
}


/* uma especie de funcao auxiliar, para se poder ver tudo */
void listOrderAll(Order orders[MAX_ORDERS], int numOrders){
    int i;
   printf("Ecomendas\n");
   for (i = 0; i < numOrders; i++)
   {
       printf("* %d %d\n", 
            orders[i].id,
            orders[i].weight);
   }
}


/* funcao que vai realizar o comando 'L', lista todos os produtos de uma encomenda por ordem alfabética da descrição */
void listOrder(Order orders[MAX_ORDERS], int numOrders){
    int id;
    scanf("%d", &id);
    if (id >= numOrders || id < 0){
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", id);
    }
    else{
        int i;
        orderOrderProductsMS(orders[id].products, 0, orders[id].numProducts - 1);
        printf("Encomenda %d\n", id);
        for (i = 0; i < orders[id].numProducts; i++)
        {
        printf("* %s %d %d\n", 
                orders[id].products[i].product->description,
                orders[id].products[i].product->price,
                orders[id].products[i].quantity);
        }
    }
}


/* funcao que vai realizar o comando 'A', adiciona um produto a uma encomenda. */
void addProductToOrder(Order orders[MAX_ORDERS], int numOrders, Product products[MAX_PRODUCTS], int numProducts){
    int idOrder, idProd, qtd, orderProdId = -1;

    scanf("%d:%d:%d",&idOrder, &idProd, &qtd);

    if (idOrder >= numOrders || idOrder < 0){
        printf("Impossivel adicionar produto %d a encomenda %d. Encomenda inexistente.\n", idProd, idOrder);
    }    
    else if (idProd >= numProducts || idProd < 0){
        printf("Impossivel adicionar produto %d a encomenda %d. Produto inexistente.\n", idProd, idOrder);
    }
    /* se a quantidade pretendida for maior do que aquilo que existe em stock */    
    else if (products[idProd].stock < qtd){
        printf("Impossivel adicionar produto %d a encomenda %d. Quantidade em stock insuficiente.\n", idProd, idOrder);
    }
    /* se a soma do peso da quantidade de produto que queremos adicionar com o peso da encomenda
       for superior ao limite de peso da encomenda */
    else if (orders[idOrder].weight + (products[idProd].weight*qtd) > MAX_ORDER_WEIGHT){
        printf("Impossivel adicionar produto %d a encomenda %d. Peso da encomenda excede o maximo de %d.\n", idProd, idOrder, MAX_ORDER_WEIGHT);
    }
    else {
        int i;
        for (i = 0; i < orders[idOrder].numProducts; i++){
            if (orders[idOrder].products[i].product->id == idProd){
                orderProdId = i;
                break;
            }
        }
        /* se eu encontrei produto igual*/
        if (orderProdId != -1){
            orders[idOrder].products[orderProdId].quantity += qtd;
            orders[idOrder].weight += products[idProd].weight * qtd;
        }
        else{   
            /* atualizacao dos dados da encomenda apos adicionarmos o produto */
            orders[idOrder].weight += products[idProd].weight * qtd;
            orderProdId = orders[idOrder].numProducts;
            orders[idOrder].numProducts++;
            orders[idOrder].products[orderProdId].quantity = qtd;
            orders[idOrder].products[orderProdId].product = &products[idProd];
        }
        products[idProd].stock -= qtd;
        
    }
    
}


/* funcao que vai realizar o comando 'R',  remove um produto de uma encomenda */
void removeProductFromOrder(Order orders[MAX_ORDERS], int numOrders, int numProducts){
    int idOrder, idProd, orderProdId = -1;
    Product *p;
    OrderProduct tmp;

    scanf("%d:%d",&idOrder, &idProd);

    if (idOrder >= numOrders || idOrder < 0){
        printf("Impossivel remover produto %d a encomenda %d. Encomenda inexistente.\n", idProd, idOrder);
    }
    else if (idProd >= numProducts || idProd < 0){
        printf("Impossivel remover produto %d a encomenda %d. Produto inexistente.\n", idProd, idOrder);
    }
    else{
        int i;
        for (i = 0; i < orders[idOrder].numProducts; i++){
            if (orders[idOrder].products[i].product->id == idProd){
                orderProdId = i;
                break;
            }
        }
        if (orderProdId != -1){
            p = orders[idOrder].products[orderProdId].product;
            p->stock += orders[idOrder].products[orderProdId].quantity;
            
            orders[idOrder].weight -= p->weight * orders[idOrder].products[orderProdId].quantity;

            orders[idOrder].numProducts--;

            tmp = orders[idOrder].products[orders[idOrder].numProducts];
            orders[idOrder].products[orders[idOrder].numProducts] = orders[idOrder].products[orderProdId];
            orders[idOrder].products[orderProdId] = tmp;
        }
    }

}


/* funcao que vai realizar o comando 'C',  calcula o custo de uma encomenda */
void totalOrderPrice(Order orders[MAX_ORDERS], int numOrders){
    int idOrder, i,orderPrice = 0;

    scanf("%d", &idOrder);

    if (idOrder >= numOrders || idOrder < 0){
        printf("Impossivel calcular custo da encomenda %d. Encomenda inexistente.\n", idOrder);
    }
    else{
        for (i = 0; i < orders[idOrder].numProducts; i++){
            orderPrice += orders[idOrder].products[i].product->price * orders[idOrder].products[i].quantity;
        }
        printf("Custo da encomenda %d %d.\n", idOrder, orderPrice);
    }
}


/* funcao que vai realizar o comando 'E', lista a descrição e a quantidade de um produto numa encomenda */
void listProductInOrder(Order orders[MAX_ORDERS], int numOrders, Product products[MAX_PRODUCTS], int numProducts){
    int idOrder, idProd, i;

    scanf("%d:%d", &idOrder, &idProd);

    if (idOrder >= numOrders || idOrder < 0){
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n",  idOrder);
    }
    else if (idProd >= numProducts || idProd < 0){
        printf("Impossivel listar produto %d. Produto inexistente.\n", idProd);
    }
    else{
        for (i = 0; i < orders[idOrder].numProducts; i++){
            if (orders[idOrder].products[i].product->id == idProd){
                printf("%s %d.\n", 
                    orders[idOrder].products[i].product->description, 
                    orders[idOrder].products[i].quantity);
                break;
            }
        }
        if (i >= orders[idOrder].numProducts){
            printf("%s %d.\n", 
                products[idProd].description, 
                0);
        }
    }   
}   


/* funcao que vai realizar o comando 'm', lista o identificador da encomenda em que o produto dado ocorre mais vezes.*/
void searchProductInOrder(Order orders[MAX_ORDERS], int numOrders, int numProducts){
    int idProd, idOrder, i;
    int bOrder = -1, bOrderAmount = -1;
    scanf("%d", &idProd);

    if (idProd >= numProducts || idProd < 0){
        printf("Impossivel listar maximo do produto %d. Produto inexistente.\n", idProd);
    }
    else{
        for (idOrder = 0; idOrder < numOrders; idOrder++){
            for (i = 0; i < orders[idOrder].numProducts; i++){
                if (orders[idOrder].products[i].product->id == idProd){
                    if (orders[idOrder].products[i].quantity > bOrderAmount){
                        bOrder = idOrder;
                        bOrderAmount = orders[idOrder].products[i].quantity;
                    }
                    break;
                }
            }
        }
        if (bOrder != -1){
            printf("Maximo produto %d %d %d.\n", idProd, bOrder, bOrderAmount);
        }
    }
}


int main(){
    char command;
    int numProd = 0; 
    int numOrder = 0; 
    Product products[MAX_PRODUCTS];
    Order orders[MAX_ORDERS];

    while ((command = getchar()))
    {
        switch (command)
        {
        case CMD_ADD_PRODUCT:
            addProduct(products, numProd++);
            break;
        case CMD_ADD_STOCK:
            addStock(products, numProd);
            break;
        case CMD_REMOVE_STOCK:
            removeStock(products, numProd);
            break;
        case CMD_LIST_PRODUCT:
            listProduct(products, numProd);
            break;
        case CMD_CHANGE_PRICE:
            changePrice(products, numProd);
            break;
        case CMD_NEW_ORDER:
            newOrder(orders, numOrder++);
            break;
        case CMD_LIST_ORDERS:
            listOrder(orders, numOrder);
            break;
        case CMD_LIST_ORDERS_ALL:
            listOrderAll(orders, numOrder);
            break;
        case CMD_ADD_PRODUCT_TO_ORDER:
            addProductToOrder(orders, numOrder, products, numProd);
            break;
        case CMD_LIST_PRODUCT_IN_ORDER:
            listProductInOrder(orders, numOrder, products, numProd);
            break;
        case CMD_SEARCH_PRODUCT_IN_ORDER:
            searchProductInOrder(orders, numOrder, numProd);
            break;
        case CMD_TOTAL_ORDER_PRICE:
            totalOrderPrice(orders, numOrder);
            break;
        case CMD_REMOVE_PRODUCT_FROM_ORDER:
            removeProductFromOrder(orders, numOrder, numProd);
            break;
        case CMD_EXIT:
            return 0;
        
        default:
            printf("I do not know the command '%c'.\n", command);
        }
        getchar();
    }
    
    return -1;
}

